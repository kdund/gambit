#ifndef __SCANNERBIT_PYTHON_INTERFACE_HPP__
#define __SCANNERBIT_PYTHON_INTERFACE_HPP__

#include <string>
#include <unordered_set>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace Gambit
{
    
    namespace Priors
    {
        
        class BasePrior;
        
    }
    
    namespace Scanner
    {
        
        class Factory_Base;
        
        namespace Python
        {

            class diagnostics
            {
            public:
                virtual void operator()(const std::vector<std::string> &args) = 0;
                virtual void operator()(const std::string &command) = 0;
                virtual ~diagnostics(){}
            };
            
            class printer_interface
            {
            public:
                virtual void main_printer(std::unordered_map<std::string, double> &) = 0;
                virtual void main_printer(const std::string &, const double &) = 0;
                virtual void aux_printer(const std::string &, const double &) = 0;
                virtual ~printer_interface(){}
            };

            class scan_interface
            {
            public:
                virtual std::shared_ptr<printer_interface> get_printer() = 0;
                virtual int run_scan_node(YAML::Node *, const Gambit::Scanner::Factory_Base *, Gambit::Priors::BasePrior *, bool) = 0;
                virtual int run_scan_str(std::string *, const Gambit::Scanner::Factory_Base *, Gambit::Priors::BasePrior *, bool resume) = 0;
                virtual ~scan_interface(){}
            };
            
        }
        
    }
    
}

#endif
