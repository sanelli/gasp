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

class torricelly_native_library : public sanelli::shared_library
{
   torricelly_native_library(std::string name, std::string path);

public:
   std::function<void(std::vector<torricelly_activation_record_local> &)> get_native(std::string symbol);

   static std::string get_library_path(std::string library);

   friend sanelli::memory;
};

} // namespace gasp::torricelly::interpreter