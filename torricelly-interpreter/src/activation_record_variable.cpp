#include <memory>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_activation_record_variable_union::torricelly_activation_record_variable_union(){}
torricelly_activation_record_variable_union::~torricelly_activation_record_variable_union(){}

std::string gasp::torricelly::interpreter::to_string(torricelly_activation_record_variable_type type)
{
   switch (type)
   {
   case torricelly_activation_record_variable_type::UNDEFINED:
      return "undefined";
   case torricelly_activation_record_variable_type::VOID:
      return "void";
   case torricelly_activation_record_variable_type::INTEGER:
      return "integer";
   case torricelly_activation_record_variable_type::CHAR:
      return "char";
   case torricelly_activation_record_variable_type::BOOLEAN:
      return "boolean";
   case torricelly_activation_record_variable_type::FLOAT:
      return "float";
   case torricelly_activation_record_variable_type::DOUBLE:
      return "double";
   case torricelly_activation_record_variable_type::POINTER:
      return "pointer";
   default:
      throw torricelly_interpreter_error("Unexpected type. It cannot be converted into string.");
   }
}

torricelly_activation_record_variable::torricelly_activation_record_variable(int i) : _type(torricelly_activation_record_variable_type::INTEGER) { _value._integer = i; }
torricelly_activation_record_variable::torricelly_activation_record_variable(char c): _type(torricelly_activation_record_variable_type::CHAR) { _value._char = c; }
torricelly_activation_record_variable::torricelly_activation_record_variable(bool b): _type(torricelly_activation_record_variable_type::BOOLEAN) { _value._boolean = b; }
torricelly_activation_record_variable::torricelly_activation_record_variable(float f): _type(torricelly_activation_record_variable_type::FLOAT) { _value._float = f; }
torricelly_activation_record_variable::torricelly_activation_record_variable(double d): _type(torricelly_activation_record_variable_type::DOUBLE) { _value._double = d; }
torricelly_activation_record_variable::torricelly_activation_record_variable(std::shared_ptr<void> p): _type(torricelly_activation_record_variable_type::POINTER) { _value._pointer = p; }

torricelly_activation_record_variable_type torricelly_activation_record_variable::type() const { return _type; }
bool torricelly_activation_record_variable::match(std::shared_ptr<torricelly::torricelly_type> torricelly_type) const
{
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (system_type->system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         return false;
      case torricelly_system_type_type::VOID:
         return _type == torricelly_activation_record_variable_type::VOID;
      case torricelly_system_type_type::INTEGER:
         return _type == torricelly_activation_record_variable_type::INTEGER;
      case torricelly_system_type_type::FLOAT:
         return _type == torricelly_activation_record_variable_type::FLOAT;
      case torricelly_system_type_type::DOUBLE:
         return _type == torricelly_activation_record_variable_type::DOUBLE;
      case torricelly_system_type_type::CHAR:
        return _type == torricelly_activation_record_variable_type::CHAR;
      case torricelly_system_type_type::BOOLEAN:
         return _type == torricelly_activation_record_variable_type::BOOLEAN;
      case torricelly_system_type_type::STRING_LITERAL:
         return _type == torricelly_activation_record_variable_type::POINTER;
      default:
         throw torricelly_interpreter_error("Unknown or unsupported torricelly system type when matching with activation record type.");
      }
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_interpreter_error("Torricelly structured type not supported yet");
      break;
   default:
      throw torricelly_interpreter_error("Unexpected or unknown toricelly type when matching it with activation record variable.");
   }
}

int torricelly_activation_record_variable::get_integer() const
{
   if (_type != torricelly_activation_record_variable_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get integer type. variable is of type ", to_string(_type), "."));
   return _value._integer;
}
char torricelly_activation_record_variable::get_char() const
{
   if (_type != torricelly_activation_record_variable_type::CHAR)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get char type. variable is of type ", to_string(_type), "."));
   return _value._char;
}
bool torricelly_activation_record_variable::get_boolean() const
{
   if (_type != torricelly_activation_record_variable_type::BOOLEAN)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get boolean type. variable is of type ", to_string(_type), "."));
   return _value._boolean;
}
float torricelly_activation_record_variable::get_float() const
{
   if (_type != torricelly_activation_record_variable_type::FLOAT)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get float type. variable is of type ", to_string(_type), "."));
   return _value._float;
}
double torricelly_activation_record_variable::get_double() const
{
   if (_type != torricelly_activation_record_variable_type::DOUBLE)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get double type. variable is of type ", to_string(_type), "."));
   return _value._double;
}

std::shared_ptr<void> torricelly_activation_record_variable::get_pointer() const
{
   if (_type != torricelly_activation_record_variable_type::POINTER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get integer type. variable is of type ", to_string(_type), "."));
   return _value._pointer;
}

void torricelly_activation_record_variable::set_integer(int i)
{
   if (_type != torricelly_activation_record_variable_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set integer type. variable is of type ", to_string(_type), "."));
   _value._integer = i;
}

void torricelly_activation_record_variable::set_char(char c)
{
   if (_type != torricelly_activation_record_variable_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set char type. variable is of type ", to_string(_type), "."));
   _value._char = c;
}
void torricelly_activation_record_variable::set_boolean(bool b)
{
   if (_type != torricelly_activation_record_variable_type::BOOLEAN)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set boolean type. variable is of type ", to_string(_type), "."));
    _value._boolean = b;
}
void torricelly_activation_record_variable::set_float(float f)
{
   if (_type != torricelly_activation_record_variable_type::FLOAT)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set float type. variable is of type ", to_string(_type), "."));
   _value._float = f;
}
void torricelly_activation_record_variable::set_double(double d)
{
   if (_type != torricelly_activation_record_variable_type::DOUBLE)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set double type. variable is of type ", to_string(_type), "."));
   _value._double = d;
}
void torricelly_activation_record_variable::set_pointer(std::shared_ptr<void> p)
{
   if (_type != torricelly_activation_record_variable_type::POINTER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set pointer type. variable is of type ", to_string(_type), "."));
   _value._pointer = p;
}

torricelly_activation_record_variable torricelly_activation_record_variable::make(const torricelly_value &value)
{
   switch (value.type())
   {
   case torricelly_type_type::SYSTEM:
   {
      switch (value.system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         throw torricelly_interpreter_error("Cannot create activation record variable from UNDEFINED torricelly type");
      case torricelly_system_type_type::VOID:
         throw torricelly_interpreter_error("Cannot create activation record variable from VOID torricelly type");
      case torricelly_system_type_type::INTEGER:
         return torricelly_activation_record_variable(value.get_integer());
      case torricelly_system_type_type::FLOAT:
         return torricelly_activation_record_variable(value.get_float());
       case torricelly_system_type_type::DOUBLE:
        return torricelly_activation_record_variable(value.get_double());
      case torricelly_system_type_type::CHAR:
         return torricelly_activation_record_variable(value.get_char());
      case torricelly_system_type_type::BOOLEAN:
        return torricelly_activation_record_variable(value.get_boolean());
      case torricelly_system_type_type::STRING_LITERAL:
      {
         auto string_literal = value.get_string_literal();
         auto pointer = std::shared_ptr<std::string>(new std::string(string_literal));
         return torricelly_activation_record_variable(pointer);
      }
      default:
         throw torricelly_interpreter_error("Unknown or unsupported torricelly system type when creating a new activation record type.");
      }
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_interpreter_error("Torricelly structured type not supported yet. Cannot create a new activation record.");
      break;
   default:
      throw torricelly_interpreter_error("Unexpected or unknown toricelly type when creating a new activation record variable.");
   }
}
