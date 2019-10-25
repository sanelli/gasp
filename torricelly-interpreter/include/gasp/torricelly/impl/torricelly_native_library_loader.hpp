#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/impl/activation_record_local.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_native_library
{
   std::string _name;
   std::string _platform_name;
   void *_native_handle;
   torricelly_native_library(std::string name);

public:
   ~torricelly_native_library();
   std::function<void(std::vector<torricelly_activation_record_local> &)> get_native(std::string name);

   friend sanelli::memory;
};

class torricelly_native_library_loader
{
   std::vector<std::string> _library_names;
   std::map<std::string, std::shared_ptr<torricelly_native_library>> _libraries;

   bool has_library(std::string library);
   void load_library(std::string library);
public:
   ~torricelly_native_library_loader();
   std::shared_ptr<torricelly_native_library> get_library(std::string library);
   typename std::vector<std::string>::const_iterator begin() const;
   typename std::vector<std::string>::const_iterator end() const;
};

} // namespace gasp::torricelly::interpreter