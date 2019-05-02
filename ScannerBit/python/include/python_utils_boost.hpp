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
         
            namespace py = boost::python;
            
            template<class T>
            inline py::list std_vector_to_py_list(const std::vector<T>& v)
            {
                py::object get_iter = py::iterator<std::vector<T> >();
                py::object iter = get_iter(v);
                return py::list(iter);
            }
            
            inline std::string pytype(py::object o) 
            {
                return std::string(py::extract<std::string>(o.attr("__class__").attr("__name__")));
            }
            
            template<typename T>
            T pyconvert(py::object o)
            {
                return T(py::extract<T>(o));
            }
            
            inline YAML::Node pyyamlconvert(py::object o)
            {
                YAML::Node node;
                std::string type = pytype(o);
                
                if (type == "dict")
                {
                    py::dict d = py::dict(o);
                    
                    for (auto it = py::stl_input_iterator< py::tuple >(d.items()), 
                             end = py::stl_input_iterator< py::tuple >(); it != end; it++)
                    {
                        node[pyyamlconvert((*it)[0])] = pyyamlconvert((*it)[1]);
                    }
                }
                else if(type == "list")
                {
                    py::list l = py::list(o);
                    
                    for (auto it = py::stl_input_iterator< py::object >(l), 
                             end = py::stl_input_iterator< py::object >(); it != end; it++)
                    {
                        node.push_back(pyyamlconvert(*it));
                    }
                    
                }
                else if(type == "tuple")
                {
                    py::tuple l = py::tuple(o);
                    
                    for (auto it = py::stl_input_iterator< py::object >(l), 
                             end = py::stl_input_iterator< py::object >(); it != end; it++)
                    {
                        node.push_back(pyyamlconvert(*it));
                    }
                    
                }
                else if (type == "float")
                {
                    node = pyconvert<double>(o);
                }
                else if (type == "int")
                {
                    node = pyconvert<int>(o);
                }
                else if (type == "str")
                {
                    node = pyconvert<std::string>(o);
                }
                else if (type == "bool")
                {
                    node = pyconvert<bool>(o);
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
