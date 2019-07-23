#include <string>
#include <memory>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp::common;
using namespace gasp::torricelly;

gasp::torricelly::torricelly_module::torricelly_module(const std::string& module_name)
   : _module_name(module_name) { }

 std::string gasp::torricelly::torricelly_module::module_name() const { return _module_name; }

std::shared_ptr<torricelly_module> gasp::torricelly::make_torricelly_module(const std::string& module_name){
 return memory::gasp_make_shared<torricelly_module>(module_name);
}