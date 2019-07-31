#include <utility>
#include <memory>

#include <gasp/common/string.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

torricelly_value_union::torricelly_value_union() : _integer(0) {}
torricelly_value_union::~torricelly_value_union() {}

torricelly_value::torricelly_value(torricelly_system_type_type system_type, const torricelly_value_union& value) 
   : _type(torricelly_type_type::SYSTEM), _system_type(system_type) {
   copy_value(value);
}

torricelly_value::torricelly_value(const torricelly_value& other)
   : _type(other._type), _system_type(other._system_type) {
   copy_value(other._value);
}

void torricelly_value::copy_value(const torricelly_value_union& value) {
   if(_type == torricelly_type_type::SYSTEM)
      switch(_system_type){
         case torricelly_system_type_type::UNDEFINED:
            throw torricelly_error("Cannot create a value out of undefined type");
         case torricelly_system_type_type::VOID:
            throw torricelly_error("Cannot create a value out of 'void' type");
         case torricelly_system_type_type::INTEGER:
         _value._integer = value._integer;
         break;
         case torricelly_system_type_type::FLOAT:
            _value._float = value._float;
            break;
         case torricelly_system_type_type::DOUBLE:
            _value._double = value._double;
            break;
         case torricelly_system_type_type::CHAR:
            _value._char = value._char;
            break;
         case torricelly_system_type_type::BOOLEAN:
            _value._boolean = value._boolean;
            break;
         case torricelly_system_type_type::STRING_LITERAL:
            _value._string_literal = value._string_literal;
            break;
         default:
            throw torricelly_error("Cannot create a value out of unknown type");
      }
}

torricelly_value& torricelly_value::operator=(const torricelly_value& other) {
   _type = other._type;
   _system_type = other._system_type;
   copy_value(other._value);
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
std::string torricelly_value::get_string_literal() const { throw_if_is_not(torricelly_system_type_type::STRING_LITERAL); return _value._string_literal; }

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

torricelly_value torricelly_value::make(int value) { 
   torricelly_value_union value_union;
   value_union._integer = value;
   return torricelly_value(torricelly_system_type_type::INTEGER, value_union); 
   }
torricelly_value torricelly_value::make(bool value){ 
    torricelly_value_union value_union;
   value_union._boolean = value;
   return torricelly_value(torricelly_system_type_type::BOOLEAN, value_union); 
}
torricelly_value torricelly_value::make(char value){ 
    torricelly_value_union value_union;
   value_union._char = value;
   return torricelly_value(torricelly_system_type_type::CHAR, value_union); 
}
torricelly_value torricelly_value::make(float value){ 
    torricelly_value_union value_union;
   value_union._float = value;
   return torricelly_value(torricelly_system_type_type::FLOAT, value_union); 
}
torricelly_value torricelly_value::make(double value){ 
    torricelly_value_union value_union;
   value_union._double = value;
   return torricelly_value(torricelly_system_type_type::DOUBLE, value_union); 
}
torricelly_value torricelly_value::make(std::string value){ 
    torricelly_value_union value_union;
   value_union._string_literal = value;
   return torricelly_value(torricelly_system_type_type::STRING_LITERAL, value_union); 
}
