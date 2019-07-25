#include <string>
#include <memory>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_module::torricelly_module(const std::string& module_name)
   : _module_name(module_name) { }

std::string torricelly_module::module_name() const { return _module_name; }
int torricelly_module::add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine) {
   _subroutines.push_back(subroutine);
   return _subroutines.size() - 1;
}

std::shared_ptr<torricelly_module> torricelly::make_torricelly_module(const std::string& module_name){
 return memory::gasp_make_shared<torricelly_module>(module_name);
}