#pragma once

#include <string>
#include <vector>
#include <map>

#include <gasp/torricelly/torricelly.hpp>
#include <sanelli/sanelli.hpp>

namespace gasp::torricelly::interpreter
{

class torricelly_module_loader
{
   static std::shared_ptr<torricelly_module_loader> _instance;
   std::map<std::string, std::shared_ptr<torricelly_module>> _modules;
   std::shared_ptr<torricelly_module> get_module(std::string module_name);
   void load_module(std::string module_name);

   torricelly_module_loader();

public:
   static std::shared_ptr<torricelly_module_loader> get_instance();
   void enrich_library(std::shared_ptr<torricelly_module> module);

   friend sanelli::memory;
};

} // namespace gasp::torricelly