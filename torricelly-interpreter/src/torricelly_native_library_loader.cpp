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
#define NATIVE_DL_OPEN(name) dlopen(name, RTLD_NOW | RTLD_LOCAL)
#define NATIVE_DL_CLOSE(handle) dlclose(handle)
#define NATIVE_DL_SYMBOL(handle, symbol) dlsym(handle, symbol)
#define PLATFORM_SPECIFIC_PREFIX "lib"
#define PLATFORM_SPECIFIC_POSTFIX ".dylib"
#else
#define NATIVE_DL_OPEN(name) nullptr
#define NATIVE_DL_CLOSE(handle)
#define NATIVE_DL_SYMBOL(handle, symbol) nullptr
#define PLATFORM_SPECIFIC_PREFIX ""
#define PLATFORM_SPECIFIC_POSTFIX ""
#endif

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_native_library::torricelly_native_library(std::string name)
    : _name(name), _native_handle(nullptr)
{
   _platform_name = sanelli::make_string(LIBRARIES_LOCATION, PLATFORM_SPECIFIC_PREFIX, name, PLATFORM_SPECIFIC_POSTFIX);
   _native_handle = NATIVE_DL_OPEN(_platform_name.c_str());
   if (_native_handle == nullptr)
      throw torricelly_interpreter_error(sanelli::make_string("Unable to open native library '", _name, "' ('", _platform_name, "')"));
}

torricelly_native_library::~torricelly_native_library()
{
   if (_native_handle != nullptr)
      NATIVE_DL_CLOSE(_native_handle);
}

std::function<void(std::vector<torricelly_activation_record_local> &)> torricelly_native_library::get_native(std::string name)
{
   auto native_symbol_pointer = NATIVE_DL_SYMBOL(_native_handle, name.c_str());
   if (native_symbol_pointer == nullptr)
      return nullptr;
   auto symbol_pointer = (void (*)(std::vector<torricelly_activation_record_local> &))native_symbol_pointer;
   return symbol_pointer;
}

torricelly_native_library_loader::~torricelly_native_library_loader()
{
   _libraries.clear();
}

bool torricelly_native_library_loader::has_library(std::string library)
{
   return _libraries.count(library) > 0;
}

void torricelly_native_library_loader::load_library(std::string library)
{
   if (has_library(library))
      return;
   _library_names.push_back(library);
   _libraries[library] = sanelli::memory::make_shared<torricelly_native_library>(library);
}

std::shared_ptr<torricelly_native_library> torricelly_native_library_loader::get_library(std::string library)
{
   if (!has_library(library))
      load_library(library);
   return _libraries[library];
}

typename std::vector<std::string>::const_iterator torricelly_native_library_loader::begin() const
{
   return _library_names.begin();
}

typename std::vector<std::string>::const_iterator torricelly_native_library_loader::end() const
{
   return _library_names.end();
}
