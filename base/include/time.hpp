#pragma once

#include "unit_t.hpp"

namespace sakurajin{
    namespace unit_system{
        namespace base{
            class time : public unit_t<1>{
            public:
                time(long double val);
                time(long double val, long double mult);
                
            };
            
        }
    }
}
