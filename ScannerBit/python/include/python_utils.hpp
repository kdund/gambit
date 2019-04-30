#ifndef __SCANNERBIT_PYTHON_UTILS_HPP__
#define __SCANNERBIT_PYTHON_UTILS_HPP__

#include<iterator>
#include<boost/python.hpp>
#include<yaml-cpp/yaml.h>

namespace Gambit
{
    
    namespace Scanner
    {
        
        namespace Python
        {
            
            template<class T>
            inline boost::python::list std_vector_to_py_list(const std::vector<T>& v)
            {
                boost::python::object get_iter = boost::python::iterator<std::vector<T> >();
                boost::python::object iter = get_iter(v);
                return boost::python::list(iter);
            }
            
            inline YAML::Node pyyconvert(boost::python::object o)
            {
                YAML::Node node;
                std::string type = boost::python::extract<std::string>(o.attr("__class__").attr("__name__"));
                
                if (type == "dict")
                {
                    boost::python::dict d = boost::python::dict(o);
                    
                    for (auto it = boost::python::stl_input_iterator< boost::python::tuple >(d.items()), 
                             end = boost::python::stl_input_iterator< boost::python::tuple >(); it != end; it++)
                    {
                        node[pyyconvert((*it)[0])] = pyyconvert((*it)[1]);
                    }
                }
                else if(type == "list")
                {
                    boost::python::list l = boost::python::list(o);
                    
                    for (auto it = boost::python::stl_input_iterator< boost::python::object >(l), 
                             end = boost::python::stl_input_iterator< boost::python::object >(); it != end; it++)
                    {
                        node.push_back(pyyconvert(*it));
                    }
                    
                }
                else if(type == "tuple")
                {
                    boost::python::tuple l = boost::python::tuple(o);
                    
                    for (auto it = boost::python::stl_input_iterator< boost::python::object >(l), 
                             end = boost::python::stl_input_iterator< boost::python::object >(); it != end; it++)
                    {
                        node.push_back(pyyconvert(*it));
                    }
                    
                }
                else if (type == "float")
                {
                    double x = boost::python::extract<double>(o);
                    node = x;
                }
                else if (type == "int")
                {
                    int x = boost::python::extract<int>(o);
                    node = x;
                }
                else if (type == "str")
                {
                    std::string x = boost::python::extract<std::string>(o);
                    node = x;
                }
                else if (type == "bool")
                {
                    bool x = boost::python::extract<bool>(o);
                    node = x;
                }
                else if (type == "NoneType")
                {
                    node = YAML::Node();
                }
                else
                {
                    throw std::runtime_error("Error converting python dictionary to YAML node:  " + type + " type not recognized.");
                }
                
                return node;
            }
            
            class fake_vector : public std::vector<double>
            {
            private:
                typedef std::vector<double> vec_type;
                
            public:
                using vec_type::vec_type;
            };
            
        }
        
    }
    
}

#endif
