#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>
#include <gasp/torricelly/torricelly_io.hpp>
#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;

std::shared_ptr<torricelly_module_loader> torricelly_module_loader::_instance = nullptr;
std::shared_ptr<torricelly_module> torricelly_module_loader::get_module(std::string module_name)
{
   if (_modules.count(module_name) == 0)
      load_module(module_name);
   return _modules[module_name];
}
void torricelly_module_loader::load_module(std::string module_name)
{
   auto local_path = std::filesystem::current_path() / (module_name + ".tb");
   auto library_path = std::filesystem::current_path() / "library" / (module_name + ".tb");

   std::filesystem::path path;
   if (std::filesystem::exists(local_path))
      path = local_path;
   else if (std::filesystem::exists(library_path))
      path = library_path;
   else
      throw torricelly_error(sanelli::make_string("Cannot load module '", module_name, "'. Module not found."));

   std::ifstream stream(path.string(), std::ios::binary);
   torricelly_binary_input input(stream);
   std::shared_ptr<torricelly_module> module;
   input >> module;
   stream.close();

   enrich_library(module);
}
torricelly_module_loader::torricelly_module_loader()
{
}
std::shared_ptr<torricelly_module_loader> torricelly_module_loader::get_instance()
{
   if (_instance == nullptr)
      _instance = sanelli::memory::make_shared<torricelly_module_loader>();
   return _instance;
}
void torricelly_module_loader::enrich_library(std::shared_ptr<torricelly_module> module)
{
   for (auto dependency_index = 0u; dependency_index < 0; ++dependency_index)
   {
      auto dependency = module->get_dependency(dependency_index);
      dependency = get_module(dependency->module_name());
      module->replace_dependency(dependency_index, dependency);
   }
}