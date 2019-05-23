#pragma once

#include <iostream>

#undef GASP_DEBUG_ENABLED
#ifdef GASP_DEBUG_ENABLED
   #define GASP_DEBUG(x) std::cout << x;
#else
   #define GASP_DEBUG(x) 
#endif