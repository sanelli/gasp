#include <memory>
#include <string>
#include <vector>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_subroutine::torricelly_subroutine(const std::string& name, 
   std::shared_ptr<torricelly::torricelly_type> return_type)
   : _name(name), _return_type(return_type) { }
std::string torricelly_subroutine::name() const { return _name; };
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::return_type() const { return _return_type; }
unsigned int torricelly_subroutine::add_parameter(std::shared_ptr<torricelly::torricelly_type> type){
   _parameters.push_back(type);
   return _parameters.size();
}
unsigned int torricelly_subroutine::add_variable(std::shared_ptr<torricelly::torricelly_type> type) {
   _variables.push_back(type);
   return _variables.size();
}

std::shared_ptr<torricelly::torricelly_subroutine> torricelly::make_torricelly_subroutine(const std::string& name, std::shared_ptr<torricelly::torricelly_type> return_type){
   return memory::gasp_make_shared<torricelly_subroutine>(name, return_type);
}
