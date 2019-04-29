#ifndef __SCANNERBIT_PYTHON_DIAGOSTICS_HPP__
#define __SCANNERBIT_PYTHON_DIAGOSTICS_HPP__

#include <string>
#include <unordered_set>
#include <vector>

namespace Gambit
{
    
    namespace Scanner
    {
        
        namespace Python
        {

            class diagnostics
            {
            protected:
                std::unordered_set<std::string> valid_commands;
                
            public:

                virtual void operator()(const std::vector<std::string> &args)=0;
                
                virtual void operator()(const std::string &command)=0;
            };

        }
        
    }
    
}

#endif
