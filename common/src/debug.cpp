#include <set>
#include <iostream>
#include <cstring>
#include <gasp/common/debug.hpp>
#include <gasp/common/output.hpp>

static std::set<const char*> __gasp_enables_debuggers = { };
void gasp::common::debug::gasp_debugger::__gasp_install_debug_code(const char* debugger)
{
   __gasp_enables_debuggers.insert(debugger);
}

bool gasp::common::debug::gasp_debugger::__gasp_has_debug_code(const char* debugger)
{
   for(auto it = __gasp_enables_debuggers.begin(); it != __gasp_enables_debuggers.end(); ++it)
   {
      const char* value = *it;
      if(strcmp(debugger, value)==0)
         return true;
   }
   return false;
}