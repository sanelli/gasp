#include <utility>
#include <memory>

#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_value_union::torricelly_value_union() : _integer(0) {}
torricelly_value_union::~torricelly_value_union() {}

torricelly_value::torricelly_value(torricelly_system_type_type system_type, const torricelly_value_union &value)
    : _type(make_torricelly_system_type(system_type))
{
   copy_value(value);
}

torricelly_value::torricelly_value(const torricelly_value &other)
    : _type(other._type)
{
   copy_value(other._value);
}

void torricelly_value::copy_value(const torricelly_value_union &value)
{
   switch (type()->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(type());
      switch (system_type->system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         throw torricelly_error("Cannot create a value out of undefined system type");
      case torricelly_system_type_type::VOID:
         throw torricelly_error("Cannot create a value out of 'void' system type");
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
         throw torricelly_error("Cannot create a value out of unknown system type");
      }
   }
   break;
   default:
      throw torricelly_error(sanelli::make_string("Unsupported torricelly type"));
   }
}

torricelly_value &torricelly_value::operator=(const torricelly_value &other)
{
   _type = other._type;
   copy_value(other._value);
   return *this;
}

std::shared_ptr<torricelly_type> torricelly_value::type() const { return _type; }

void torricelly_value::throw_if_is_not(std::shared_ptr<torricelly_type> check_type) const
{
   if (!type()->equals(check_type))
      throw torricelly_error(sanelli::make_string("Expected type '", to_string(check_type), "' but found '", to_string(type()), "'."));
}

int torricelly_value::get_integer() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::INTEGER));
   return _value._integer;
}
char torricelly_value::get_char() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::CHAR));
   return _value._char;
}
bool torricelly_value::get_boolean() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::BOOLEAN));
   return _value._boolean;
}
float torricelly_value::get_float() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::FLOAT));
   return _value._float;
}
double torricelly_value::get_double() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::DOUBLE));
   return _value._double;
}
std::string torricelly_value::get_string_literal() const
{
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL));
   return _value._string_literal;
}

torricelly_value torricelly_value::make(int value)
{
   torricelly_value_union value_union;
   value_union._integer = value;
   return torricelly_value(torricelly_system_type_type::INTEGER, value_union);
}
torricelly_value torricelly_value::make(bool value)
{
   torricelly_value_union value_union;
   value_union._boolean = value;
   return torricelly_value(torricelly_system_type_type::BOOLEAN, value_union);
}
torricelly_value torricelly_value::make(char value)
{
   torricelly_value_union value_union;
   value_union._char = value;
   return torricelly_value(torricelly_system_type_type::CHAR, value_union);
}
torricelly_value torricelly_value::make(float value)
{
   torricelly_value_union value_union;
   value_union._float = value;
   return torricelly_value(torricelly_system_type_type::FLOAT, value_union);
}
torricelly_value torricelly_value::make(double value)
{
   torricelly_value_union value_union;
   value_union._double = value;
   return torricelly_value(torricelly_system_type_type::DOUBLE, value_union);
}
torricelly_value torricelly_value::make(std::string value)
{
   torricelly_value_union value_union;
   value_union._string_literal = value;
   return torricelly_value(torricelly_system_type_type::STRING_LITERAL, value_union);
}

torricelly_value torricelly_value::get_default_value(std::shared_ptr<torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      throw torricelly_error("Cannot get default value for undefined type.");
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = std::dynamic_pointer_cast<const torricelly_system_type>(type);
      switch (system_type->system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         throw torricelly_error("Cannot get default value for undefined system type.");
      case torricelly_system_type_type::VOID:
         throw torricelly_error("Cannot get default value for void system type.");
      case torricelly_system_type_type::INTEGER:
         return torricelly_value::make(0);
      case torricelly_system_type_type::FLOAT:
         return torricelly_value::make(0.00f);
      case torricelly_system_type_type::DOUBLE:
         return torricelly_value::make(0.00);
      case torricelly_system_type_type::CHAR:
         return torricelly_value::make('\0');
      case torricelly_system_type_type::BOOLEAN:
         return torricelly_value::make(false);
      case torricelly_system_type_type::STRING_LITERAL:
         return torricelly_value::make("");
      default:
         throw torricelly_error("Cannot get default value for unknwon system type.");
      }
   }
   case torricelly_type_type::STRUCTURED:
      throw torricelly_error("Cannot get default value for structured type.");
   default:
      throw torricelly_error("Cannot get default value for unknown type.");
   }
}

torricelly_value torricelly_value::get_value_from_string(const std::string &value, std::shared_ptr<torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      throw torricelly_error("Cannot get default value for undefined type.");
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = std::dynamic_pointer_cast<const torricelly_system_type>(type);
      switch (system_type->system_type())
      {
      case torricelly_system_type_type::UNDEFINED:
         throw torricelly_error("Cannot get default value for undefined system type.");
      case torricelly_system_type_type::VOID:
         throw torricelly_error("Cannot get default value for void system type.");
      case torricelly_system_type_type::INTEGER:
         return torricelly_value::make(stoi(value));
      case torricelly_system_type_type::FLOAT:
         return torricelly_value::make(stof(value));
      case torricelly_system_type_type::DOUBLE:
         return torricelly_value::make(stod(value));
      case torricelly_system_type_type::CHAR:
         return torricelly_value::make(value.length() > 0 ? value[0] : '\0');
      case torricelly_system_type_type::BOOLEAN:
         return torricelly_value::make(value == "true");
      case torricelly_system_type_type::STRING_LITERAL:
         return torricelly_value::make(value);
      default:
         throw torricelly_error("Cannot get default value for unknwon system type.");
      }
   }
   case torricelly_type_type::STRUCTURED:
      throw torricelly_error("Cannot get default value for structured type.");
   default:
      throw torricelly_error("Cannot get default value for unknown type.");
   }
}
