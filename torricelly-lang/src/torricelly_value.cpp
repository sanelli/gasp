#include <utility>
#include <memory>
#include <vector>

#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_value_union::torricelly_value_union() : _integer(0) {}
torricelly_value_union::~torricelly_value_union() {}

torricelly_value::torricelly_value(torricelly_system_type_type system_type, const torricelly_value_union &value)
    : _type(make_torricelly_system_type(system_type)), _array(nullptr)
{
   copy_value(value);
}

torricelly_value::torricelly_value(std::shared_ptr<torricelly_array_type> type, torricelly_value initial_value)
    : _type(type)
{
   memset(&_value, 0, sizeof(_value));
   _array = std::make_shared<std::vector<torricelly_value>>();
   auto array_len = 1;
   bool is_fully_defined = true;
   for (auto dim = 0; dim < type->dimensions(); ++dim)
   {
      auto dimension = type->dimension(dim);
      if (dimension == torricelly_array_type::undefined_dimension())
      {
         is_fully_defined = false;
         array_len = 1;
         break;
      }
      else
      {
         array_len *= dimension;
      }
   }
   if (is_fully_defined)
   {
      for (auto index = 0; index < array_len; ++index)
         _array->push_back(initial_value);
   }
}

torricelly_value::torricelly_value(const torricelly_value &other)
    : _type(other._type)
{
   copy_value(other._value);
   copy_array(other._array);
}

torricelly_value &torricelly_value::operator=(const torricelly_value &other)
{
   _type = other._type;
   copy_value(other._value);
   copy_array(other._array);
   return *this;
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
      case torricelly_system_type_type::LITERAL_STRING:
         _value._string_literal = value._string_literal;
         break;
      default:
         throw torricelly_error("Cannot create a value out of unknown system type");
      }
   }
   break;
   case torricelly_type_type::ARRAY:
      memset(&_value, 0, sizeof(_value));
      break;
   default:
      throw torricelly_error(sanelli::make_string("Unsupported torricelly type"));
   }
}

void torricelly_value::copy_array(std::shared_ptr<std::vector<torricelly_value>> array)
{
   switch (type()->type_type())
   {
   case torricelly_type_type::SYSTEM:
      _array = nullptr;
      break;
   case torricelly_type_type::ARRAY:
      _array = std::make_shared<std::vector<torricelly_value>>();
      for(auto index = 0; index < array->size(); ++index)
          _array->push_back(array->at(index));
      break;
   default:
      throw torricelly_error(sanelli::make_string("Unsupported torricelly type"));
   }
}

std::shared_ptr<torricelly_type> torricelly_value::type() const
{
   return _type;
}

void torricelly_value::throw_if_is_not(std::shared_ptr<torricelly_type> check_type) const
{
   if (!type()->equals(check_type))
      throw torricelly_error(sanelli::make_string("Expected type '", to_string(check_type), "' but found '", to_string(type()), "'."));
}

int32_t torricelly_value::get_integer() const
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
   throw_if_is_not(make_torricelly_system_type(torricelly_system_type_type::LITERAL_STRING));
   return _value._string_literal;
}
std::shared_ptr<std::vector<torricelly_value>> torricelly_value::get_array() const
{
   if (type()->type_type() != torricelly_type_type::ARRAY)
      throw torricelly_error(sanelli::make_string("Expected type '", to_string(torricelly_type_type::ARRAY), "' but found '", to_string(type()), "'."));
   return _array;
}

torricelly_value torricelly_value::make(int32_t value)
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
   return torricelly_value(torricelly_system_type_type::LITERAL_STRING, value_union);
}
torricelly_value torricelly_value::make(std::shared_ptr<torricelly_array_type> type, torricelly_value initial_value)
{
   return torricelly_value(type, initial_value);
}

torricelly_value torricelly_value::get_default_value(std::shared_ptr<torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      throw torricelly_error("Cannot get default value for undefined type.");
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(type);
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
      case torricelly_system_type_type::LITERAL_STRING:
         return torricelly_value::make("");
      default:
         throw torricelly_error("Cannot get default value for unknwon system type.");
      }
   }
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(type);
      auto underlying_type = array_type->underlying_type();
      auto default_value = torricelly_value::get_default_value(underlying_type);
      return torricelly_value::make(array_type, default_value);
   }
   break;
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
      auto system_type = torricelly_type_utility::as_system_type(type);
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
      case torricelly_system_type_type::LITERAL_STRING:
         return torricelly_value::make(value);
      default:
         throw torricelly_error("Cannot get default value for unknwon system type.");
      }
   }
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(type);
      if (array_type->dimensions() != 1)
         throw torricelly_error("Multi dimensional array cannot be converted from a string.");
      if (array_type->dimension(0) == torricelly_array_type::undefined_dimension())
         throw torricelly_error("Cannot convert array with undefined dimensions.");
      auto return_value = torricelly_value::make(array_type, get_default_value(array_type->underlying_type()));
      auto array = return_value.get_array();
      std::vector<std::string> items;
      sanelli::split(value, ',', items);
      if (items.size() != array_type->dimensions())
         throw torricelly_error(sanelli::make_string("Input array has size ", items.size(), " but expected ", array_type->dimension(0)));
      return return_value;
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_error("Cannot get default value for structured type.");
   default:
      throw torricelly_error("Cannot get default value for unknown type.");
   }
}
