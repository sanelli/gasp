#pragma once

#include <iostream>
#include <set>

#undef GASP_DEBUG_ENABLED

#define GASP_DEBUG_GET_MACRO(_1, _2, NAME, ...) NAME
#define GASP_DEBUG(...)                                        \
   GASP_DEBUG_GET_MACRO(__VA_ARGS__, GASP_DEBUG2, GASP_DEBUG1) \
   (__VA_ARGS__)

#ifdef GASP_DEBUG_ENABLED

#define GASP_DEBUG1(x) \
   {                   \
      std::cerr << x;  \
   }
#define GASP_DEBUG2(debugger, x)                                               \
   {                                                                           \
      if (gasp::common::debug::gasp_debugger::__gasp_has_debug_code(debugger)) \
      {                                                                        \
         std::cerr << x;                                                       \
      }                                                                        \
   }
#define GASP_INSTALL_DEBUGGER(debugger)                                        \
   {                                                                           \
      gasp::common::debug::gasp_debugger::__gasp_install_debug_code(debugger); \
   }
#else
#define GASP_DEBUG1(x)
#define GASP_DEBUG2(x,y)
#define GASP_INSTALL_DEBUGGER(debugger)
#endif

namespace gasp::common::debug
{
class gasp_debugger
{
public:
   static void __gasp_install_debug_code(const char *debugger);
   static bool __gasp_has_debug_code(const char *debugger);
};
} // namespace gasp::common::debug
