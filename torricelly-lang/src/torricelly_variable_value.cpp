#include <utility>
#include <memory>

#include <gasp/common/string.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_value::torricelly_value(torricelly_system_type_type system_type, torricelly_value_union value) 
   : _type(torricelly_type_type::SYSTEM), _system_type(system_type), _value(value) {}
torricelly_value::torricelly_value(const torricelly_value& other)
   : _type(other._type), _system_type(other._system_type), _value(other._value) {}
torricelly_value::torricelly_value(torricelly_value&& other) {
   *this = std::move(other);
}
torricelly_value& torricelly_value::operator=(const torricelly_value& other) {
   _type = other._type;
   _system_type = other._system_type;
   _value = other._value;
   return *this;
}
torricelly_value& torricelly_value::operator=(torricelly_value&& other){
   _type = std::move(other._type);
   _system_type = std::move(other._system_type);
   _value = std::move(other._value);
   return *this;
}

torricelly_type_type torricelly_value::type() const { return _type; }
torricelly_system_type_type torricelly_value::system_type() const { return _system_type; }

bool torricelly_value::is(torricelly_system_type_type system_type) const { 
   return _type == torricelly_type_type::SYSTEM 
      && _system_type == system_type;
} 

 void torricelly_value::throw_if_is_not(torricelly_system_type_type system_type) const {
    if(!is(system_type))
      throw torricelly_error(make_string("Expected type '", system_type, "' but found '", system_type,"'."));
 }

int torricelly_value::get_integer() const { throw_if_is_not(torricelly_system_type_type::INTEGER); return _value._integer; }
char torricelly_value::get_char() const { throw_if_is_not(torricelly_system_type_type::CHAR); return _value._char; }
bool torricelly_value::get_boolean() const { throw_if_is_not(torricelly_system_type_type::BOOLEAN); return _value._boolean; }
float torricelly_value::get_float() const { throw_if_is_not(torricelly_system_type_type::FLOAT); return _value._float; }
double torricelly_value::get_double() const { throw_if_is_not(torricelly_system_type_type::DOUBLE); return _value._double; }

bool torricelly_value::match(std::shared_ptr<torricelly_type> type) const {
   if(type->type_type() != _type) return false;
   switch(_type){
      case torricelly_type_type::UNDEFINED:
        { throw torricelly_error("Unexpected undefined type");}
      case torricelly_type_type::SYSTEM:
        { 
         auto system_typed_type = std::dynamic_pointer_cast<toricelly_system_type>(type);
         return system_typed_type->system_type() == _system_type;
        }
      case torricelly_type_type::STRUCTURED:
        { throw torricelly_error("Structured type not implemented yet");}
      default:
        { throw torricelly_error("Unexpected unknown torricelly type"); }
   }
   return false;
}

torricelly_value torricelly_value::make(int value) { return torricelly_value(torricelly_system_type_type::INTEGER, { ._integer = value}); }
torricelly_value torricelly_value::make(bool value){ return torricelly_value(torricelly_system_type_type::BOOLEAN, { ._boolean = value}); }
torricelly_value torricelly_value::make(char value){ return torricelly_value(torricelly_system_type_type::CHAR, { ._char = value}); }
torricelly_value torricelly_value::make(float value){ return torricelly_value(torricelly_system_type_type::FLOAT, { ._float = value}); }
torricelly_value torricelly_value::make(double value){ return torricelly_value(torricelly_system_type_type::DOUBLE, { ._double = value}); }
