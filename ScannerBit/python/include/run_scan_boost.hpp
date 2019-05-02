#ifndef __SCANNERBIT_PYTHON_RUN_SCAN_HPP__
#define __SCANNERBIT_PYTHON_RUN_SCAN_HPP__

#include <memory>
//#include "gambit/ScannerBit/factory_defs.hpp"
#include "gambit/ScannerBit/base_prior.hpp"
#include <yaml-cpp/yaml.h>
#include "interface.hpp"

namespace Gambit
{
    
    namespace Scanner
    {
        
        template<typename T>
        class Function_Base;
        
        template<typename ret, typename... args>
        class Function_Base <ret (args...)> : public std::enable_shared_from_this<Function_Base <ret (args...)>>
        {
        private:
            void *main_printer;
            Priors::BasePrior *prior;
            std::string purpose;
            int myRealRank; 
            double purpose_offset;
            bool use_alternate_min_LogL;
            bool _scanner_can_quit;

            virtual void deleter(Function_Base <ret (args...)> *in) const
            {
                delete in;
            }

            virtual const std::type_info & type() const {return typeid(ret (args...));}

        public:
            Function_Base(double offset = 0.) : myRealRank(0), purpose_offset(offset), use_alternate_min_LogL(false), _scanner_can_quit(false) {}
            virtual ret main(const args&...) = 0;
            virtual ~Function_Base(){}
        };
        
        class Factory_Base
        {
        public:
                virtual void * operator() (const std::string &purpose) const = 0;
                virtual ~Factory_Base() {};
        };
            
            
            
        namespace Python
        {
            namespace py = boost::python;
            
            
            class python_function_base : public Gambit::Scanner::Function_Base<double (std::unordered_map<std::string, double> &)>
            {
            private:
                typedef std::shared_ptr<printer_interface> print_ptr;
                py::object obj;
                print_ptr print;
                
            public:
                python_function_base(py::object obj, print_ptr print) : obj(obj), print(print) {}
                
                double main(std::unordered_map<std::string, double> &map)
                {
                    print->main_printer(map);
                    return pyconvert<double>(obj(boost::ref(map)));
                }
                
            };

            class python_factory : public Factory_Base
            {
            private:
                typedef std::shared_ptr<printer_interface> print_ptr;
                py::dict map;
                print_ptr print;
                
            public:
                python_factory(py::object obj, print_ptr print) : print(print)
                {
                    if (pytype(obj) == "dict")
                        map = py::dict(obj);
                    else
                        map["Likelihood"] = obj;
                }
                
                void *operator()(const std::string &purpose) const
                {
                    return new python_function_base(map[purpose], print);
                }
            };
            
            
            class python_prior : public Gambit::Priors::BasePrior
            {
            private:
                py::object obj;
                
            public:
                python_prior(py::object obj) : obj(obj)
                {
                    fake_vector vec;
                    std::unordered_map<std::string, double> map;
                    obj(boost::ref(vec), boost::ref(map));
                    this->setSize(vec.size());
                    
                    for (auto it = map.begin(), end = map.end(); it != end; ++it)
                    {
                        this->param_names.push_back(it->first);
                    }
                }
                
                void transform(const std::vector<double> &vec, std::unordered_map<std::string, double> &map) const
                {
                    obj(boost::ref(vec), boost::ref(map));
                }
            };

            //int run_scan(IniParser::Parser &iniFile, const Gambit::Scanner::Factory_Base *, Gambit::Priors::BasePrior *, bool resume);

        }
        
    }
    
}

#endif
