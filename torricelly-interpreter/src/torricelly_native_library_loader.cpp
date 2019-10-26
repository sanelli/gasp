#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/interpreter.hpp>

#define LIBRARIES_LOCATION "library/"

#if defined(__APPLE__)
#include <dlfcn.h>
#define PLATFORM_SPECIFIC_PREFIX "lib"
#define PLATFORM_SPECIFIC_POSTFIX ".dylib"
#else
#define PLATFORM_SPECIFIC_PREFIX ""
#define PLATFORM_SPECIFIC_POSTFIX ""
#endif

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_native_library::torricelly_native_library(std::string name, std::string path)
    : sanelli::shared_library(name, path)
{
}

std::function<void(std::vector<torricelly_activation_record_local> &)> torricelly_native_library::get_native(std::string symbol)
{
   auto native_symbol_pointer = get_native_handler(symbol.c_str());
   if (native_symbol_pointer == nullptr)
      return nullptr;
   auto symbol_pointer = (void (*)(std::vector<torricelly_activation_record_local> &))native_symbol_pointer;
   return symbol_pointer;
}

std::string torricelly_native_library::get_library_path(std::string library)
{
   return sanelli::make_string(LIBRARIES_LOCATION, PLATFORM_SPECIFIC_PREFIX, library, PLATFORM_SPECIFIC_POSTFIX);
}
