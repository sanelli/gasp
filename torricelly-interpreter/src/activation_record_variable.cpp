#include <memory>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <functional>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

torricelly_activation_record_variable_union::torricelly_activation_record_variable_union() {}
torricelly_activation_record_variable_union::~torricelly_activation_record_variable_union() {}

std::string gasp::torricelly::interpreter::to_string(torricelly_activation_record_local_type type)
{
   switch (type)
   {
   case torricelly_activation_record_local_type::UNDEFINED:
      return "undefined";
   case torricelly_activation_record_local_type::VOID:
      return "void";
   case torricelly_activation_record_local_type::INTEGER:
      return "integer";
   case torricelly_activation_record_local_type::CHAR:
      return "char";
   case torricelly_activation_record_local_type::BOOLEAN:
      return "boolean";
   case torricelly_activation_record_local_type::FLOAT:
      return "float";
   case torricelly_activation_record_local_type::DOUBLE:
      return "double";
   case torricelly_activation_record_local_type::POINTER:
      return "pointer";
   default:
      throw torricelly_interpreter_error("Unexpected type. It cannot be converted into string.");
   }
}

std::string gasp::torricelly::interpreter::to_string(torricelly_activation_record_variable_underlying_type type)
{
   switch (type)
   {
   case torricelly_activation_record_variable_underlying_type::UNDEFINED:
      return "undefined";
   case torricelly_activation_record_variable_underlying_type::STRING_LITERAL:
      return "string_literal";
   case torricelly_activation_record_variable_underlying_type::ARRAY:
      return "array";
   default:
      throw torricelly_interpreter_error("Unexpected underlying type. It cannot be converted into string.");
   }
}

std::string gasp::torricelly::interpreter::to_string(torricelly_activation_record_variable_array_underlying_type type)
{
   switch (type)
   {
   case torricelly_activation_record_variable_array_underlying_type::UNDEFINED:
      return "undefined";
   case torricelly_activation_record_variable_array_underlying_type::INTEGER:
      return "integer";
   case torricelly_activation_record_variable_array_underlying_type::CHAR:
      return "char";
   case torricelly_activation_record_variable_array_underlying_type::BOOLEAN:
      return "boolean";
   case torricelly_activation_record_variable_array_underlying_type::FLOAT:
      return "float";
   case torricelly_activation_record_variable_array_underlying_type::DOUBLE:
      return "double";
   default:
      throw torricelly_interpreter_error("Unexpected type. It cannot be converted into string.");
   }
}

torricelly_activation_record_variable_array_underlying_type gasp::torricelly::interpreter::to_underlying_type(std::shared_ptr<torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      throw torricelly_interpreter_error("Cannot generate an undefined type for underlying type.");
      break;
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(type);
      switch (system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::DOUBLE:
         return torricelly_activation_record_variable_array_underlying_type::DOUBLE;
      case torricelly::torricelly_system_type_type::FLOAT:
         return torricelly_activation_record_variable_array_underlying_type::FLOAT;
      case torricelly::torricelly_system_type_type::INTEGER:
         return torricelly_activation_record_variable_array_underlying_type::INTEGER;
      case torricelly::torricelly_system_type_type::BOOLEAN:
         return torricelly_activation_record_variable_array_underlying_type::BOOLEAN;
      case torricelly::torricelly_system_type_type::CHAR:
         return torricelly_activation_record_variable_array_underlying_type::CHAR;
      case torricelly::torricelly_system_type_type::STRING_LITERAL:
         throw torricelly_interpreter_error("STRING_LITERAL torricelly type unsupported for underlying type.");
      case torricelly::torricelly_system_type_type::VOID:
         throw torricelly_interpreter_error("VOID torricelly type unsupported for underlying type.");
      case torricelly::torricelly_system_type_type::UNDEFINED:
         throw torricelly_interpreter_error("UNDEFINED torricelly type unsupported for underlying type.");
      default:
         throw torricelly_interpreter_error("Unexpected or unknown torricelly for underlying type.");
      }
   }
   break;
   case torricelly_type_type::ARRAY:
      throw torricelly_interpreter_error("Cannot generate an array type for underlying type.");
      break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_interpreter_error("Cannot generate a structured type for underlying type.");
      break;
   }
}

torricelly_activation_record_variable_multidimensional_array::torricelly_activation_record_variable_multidimensional_array(
    const std::vector<unsigned int> &dimensions, torricelly_activation_record_variable_array_underlying_type underlying_type)
    : _underlying_type(underlying_type)
{
   std::copy(dimensions.begin(), dimensions.end(), std::back_inserter(_dimensions));
   auto size = std::accumulate(dimensions.begin(), dimensions.end(), 1, std::multiplies<unsigned int>());
   _values.resize(size);
   for (auto index = 0; index < size; ++index)
      memset(&_values[index], 0, sizeof(torricelly_activation_record_variable_union));
}

torricelly_activation_record_variable_multidimensional_array::torricelly_activation_record_variable_multidimensional_array(const torricelly_activation_record_variable_multidimensional_array &other)
    : _underlying_type(other._underlying_type)
{
   copy_from(other);
}
torricelly_activation_record_variable_multidimensional_array &torricelly_activation_record_variable_multidimensional_array::operator=(const torricelly_activation_record_variable_multidimensional_array &other)
{
   if (this == &other)
      return *this;
   copy_from(other);
   return *this;
}

torricelly_activation_record_variable_multidimensional_array::~torricelly_activation_record_variable_multidimensional_array() {}

void torricelly_activation_record_variable_multidimensional_array::copy_from(const torricelly_activation_record_variable_multidimensional_array &other)
{
   std::copy(other._dimensions.begin(), other._dimensions.end(), std::back_inserter(_dimensions));
   std::copy(other._values.begin(), other._values.end(), std::back_inserter(_values));
}

torricelly_activation_record_variable_array_underlying_type torricelly_activation_record_variable_multidimensional_array::underlying_type() const
{
   return _underlying_type;
}

unsigned int torricelly_activation_record_variable_multidimensional_array::dimensions() const { return _dimensions.size(); }
unsigned int torricelly_activation_record_variable_multidimensional_array::dimension(unsigned int dim) const { return _dimensions.at(dim); }

unsigned int torricelly_activation_record_variable_multidimensional_array::index(const std::vector<unsigned int> &indexes) const
{
   auto idx = indexes.at(0);
   for (auto i = 1; i < indexes.size(); ++idx)
      idx += _dimensions.at(i - 1) * indexes.at(i);
   return idx;
}

unsigned int torricelly_activation_record_variable_multidimensional_array::size() const
{
   return _values.size();
}

void torricelly_activation_record_variable_multidimensional_array::set(unsigned int index, torricelly_activation_record_variable_union value)
{
   _values.at(index) = value;
}

torricelly_activation_record_variable_union torricelly_activation_record_variable_multidimensional_array::get(unsigned int index) const
{
   return _values.at(index);
}

torricelly_activation_record_variable::torricelly_activation_record_variable()
    : _type(torricelly_activation_record_local_type::UNDEFINED), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
}
torricelly_activation_record_variable::torricelly_activation_record_variable(int i)
    : _type(torricelly_activation_record_local_type::INTEGER), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
   _value._integer = i;
}
torricelly_activation_record_variable::torricelly_activation_record_variable(char c)
    : _type(torricelly_activation_record_local_type::CHAR), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
   _value._char = c;
}
torricelly_activation_record_variable::torricelly_activation_record_variable(bool b) : _type(torricelly_activation_record_local_type::BOOLEAN), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
   _value._boolean = b;
}
torricelly_activation_record_variable::torricelly_activation_record_variable(float f) : _type(torricelly_activation_record_local_type::FLOAT), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
   _value._float = f;
}
torricelly_activation_record_variable::torricelly_activation_record_variable(double d) : _type(torricelly_activation_record_local_type::DOUBLE), _pointer_unerlying_type(torricelly_activation_record_variable_underlying_type::UNDEFINED)
{
   _value._double = d;
}
torricelly_activation_record_variable::torricelly_activation_record_variable(std::shared_ptr<void> p, torricelly_activation_record_variable_underlying_type underlying_type)
    : _type(torricelly_activation_record_local_type::POINTER), _pointer_unerlying_type(underlying_type)
{
   _pointer = p;
}

torricelly_activation_record_variable::torricelly_activation_record_variable(const torricelly_activation_record_variable &other)
    : _type(other._type), _pointer_unerlying_type(other._pointer_unerlying_type)
{
   copy_value_from(other);
}
torricelly_activation_record_variable::torricelly_activation_record_variable(torricelly_activation_record_variable &&other)
    : _type(std::move(other._type)), _pointer_unerlying_type(std::move(other._pointer_unerlying_type))
{
   switch (_type)
   {
   case torricelly_activation_record_local_type::UNDEFINED:
   case torricelly_activation_record_local_type::VOID:
      break;
   case torricelly_activation_record_local_type::INTEGER:
      _value._integer = std::move(other._value._integer);
      break;
   case torricelly_activation_record_local_type::CHAR:
      _value._char = std::move(other._value._char);
      break;
   case torricelly_activation_record_local_type::BOOLEAN:
      _value._boolean = std::move(other._value._boolean);
      break;
   case torricelly_activation_record_local_type::FLOAT:
      _value._float = std::move(other._value._float);
      break;
   case torricelly_activation_record_local_type::DOUBLE:
      _value._double = std::move(other._value._double);
      break;
   case torricelly_activation_record_local_type::POINTER:
      _pointer = std::move(other._pointer);
      break;
   default:
      throw torricelly_interpreter_error("Unexpected type. It cannot be moved inside constructor.");
   }
}
torricelly_activation_record_variable &torricelly_activation_record_variable::operator=(const torricelly_activation_record_variable &other)
{
   if (this == &other)
      return *this;
   _type = other._type;
   _pointer_unerlying_type = other._pointer_unerlying_type;
   copy_value_from(other);
   return *this;
}
torricelly_activation_record_variable &torricelly_activation_record_variable::operator=(torricelly_activation_record_variable &&other)
{
   _type = std::move(other._type);
   _pointer_unerlying_type = std::move(other._pointer_unerlying_type);
   switch (_type)
   {
   case torricelly_activation_record_local_type::UNDEFINED:
   case torricelly_activation_record_local_type::VOID:
      break;
   case torricelly_activation_record_local_type::INTEGER:
      _value._integer = std::move(other._value._integer);
      break;
   case torricelly_activation_record_local_type::CHAR:
      _value._char = std::move(other._value._char);
      break;
   case torricelly_activation_record_local_type::BOOLEAN:
      _value._boolean = std::move(other._value._boolean);
      break;
   case torricelly_activation_record_local_type::FLOAT:
      _value._float = std::move(other._value._float);
      break;
   case torricelly_activation_record_local_type::DOUBLE:
      _value._double = std::move(other._value._double);
      break;
   case torricelly_activation_record_local_type::POINTER:
      _pointer = std::move(other._pointer);
      break;
   default:
      throw torricelly_interpreter_error("Unexpected type. It cannot be assigned.");
   }
   return *this;
}

torricelly_activation_record_variable::~torricelly_activation_record_variable() {}

void torricelly_activation_record_variable::copy_value_from(const torricelly_activation_record_variable &other)
{
   switch (_type)
   {
   case torricelly_activation_record_local_type::UNDEFINED:
   case torricelly_activation_record_local_type::VOID:
      break;
   case torricelly_activation_record_local_type::INTEGER:
      _value._integer = other._value._integer;
      break;
   case torricelly_activation_record_local_type::CHAR:
      _value._char = other._value._char;
      break;
   case torricelly_activation_record_local_type::BOOLEAN:
      _value._boolean = other._value._boolean;
      break;
   case torricelly_activation_record_local_type::FLOAT:
      _value._float = other._value._float;
      break;
   case torricelly_activation_record_local_type::DOUBLE:
      _value._double = other._value._double;
      break;
   case torricelly_activation_record_local_type::POINTER:
      _pointer = other._pointer;
      break;
   default:
      throw torricelly_interpreter_error(sanelli::make_string("Unexpected type. It cannot be copied ( id=", (int)_type, ") ."));
   }
}

torricelly_activation_record_local_type torricelly_activation_record_variable::type() const { return _type; }
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
         return _type == torricelly_activation_record_local_type::VOID;
      case torricelly_system_type_type::INTEGER:
         return _type == torricelly_activation_record_local_type::INTEGER;
      case torricelly_system_type_type::FLOAT:
         return _type == torricelly_activation_record_local_type::FLOAT;
      case torricelly_system_type_type::DOUBLE:
         return _type == torricelly_activation_record_local_type::DOUBLE;
      case torricelly_system_type_type::CHAR:
         return _type == torricelly_activation_record_local_type::CHAR;
      case torricelly_system_type_type::BOOLEAN:
         return _type == torricelly_activation_record_local_type::BOOLEAN;
      case torricelly_system_type_type::STRING_LITERAL:
         return _type == torricelly_activation_record_local_type::POINTER;
      default:
         throw torricelly_interpreter_error("Unknown or unsupported torricelly system type when matching with activation record type.");
      }
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_interpreter_error("Torricelly structured type not supported yet");
      break;
   default:
      throw torricelly_interpreter_error("Unexpected or unknown torricelly type when matching it with activation record variable.");
   }
}

int torricelly_activation_record_variable::get_integer() const
{
   if (_type != torricelly_activation_record_local_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get integer type. variable is of type ", to_string(_type), "."));
   return _value._integer;
}
char torricelly_activation_record_variable::get_char() const
{
   if (_type != torricelly_activation_record_local_type::CHAR)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get char type. variable is of type ", to_string(_type), "."));
   return _value._char;
}
bool torricelly_activation_record_variable::get_boolean() const
{
   if (_type != torricelly_activation_record_local_type::BOOLEAN)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get boolean type. variable is of type ", to_string(_type), "."));
   return _value._boolean;
}
float torricelly_activation_record_variable::get_float() const
{
   if (_type != torricelly_activation_record_local_type::FLOAT)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get float type. variable is of type ", to_string(_type), "."));
   return _value._float;
}
double torricelly_activation_record_variable::get_double() const
{
   if (_type != torricelly_activation_record_local_type::DOUBLE)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get double type. variable is of type ", to_string(_type), "."));
   return _value._double;
}

std::shared_ptr<void> torricelly_activation_record_variable::get_pointer() const
{
   if (_type != torricelly_activation_record_local_type::POINTER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get pointer type. variable is of type ", to_string(_type), "."));
   return _pointer;
}

std::shared_ptr<std::string> torricelly_activation_record_variable::get_string_pointer() const
{
   return std::static_pointer_cast<std::string>(get_pointer());
}

std::shared_ptr<torricelly_activation_record_variable_multidimensional_array> torricelly_activation_record_variable::get_array_pointer() const
{
   return std::static_pointer_cast<torricelly_activation_record_variable_multidimensional_array>(get_pointer());
}

torricelly_activation_record_variable_underlying_type torricelly_activation_record_variable::get_pointer_underlying_type() const
{
   return _pointer_unerlying_type;
}

void torricelly_activation_record_variable::set_integer(int i)
{
   if (_type != torricelly_activation_record_local_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set integer type. variable is of type ", to_string(_type), "."));
   _value._integer = i;
}

void torricelly_activation_record_variable::set_char(char c)
{
   if (_type != torricelly_activation_record_local_type::INTEGER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set char type. variable is of type ", to_string(_type), "."));
   _value._char = c;
}
void torricelly_activation_record_variable::set_boolean(bool b)
{
   if (_type != torricelly_activation_record_local_type::BOOLEAN)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set boolean type. variable is of type ", to_string(_type), "."));
   _value._boolean = b;
}
void torricelly_activation_record_variable::set_float(float f)
{
   if (_type != torricelly_activation_record_local_type::FLOAT)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set float type. variable is of type ", to_string(_type), "."));
   _value._float = f;
}
void torricelly_activation_record_variable::set_double(double d)
{
   if (_type != torricelly_activation_record_local_type::DOUBLE)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set double type. variable is of type ", to_string(_type), "."));
   _value._double = d;
}
void torricelly_activation_record_variable::set_pointer(std::shared_ptr<void> p)
{
   if (_type != torricelly_activation_record_local_type::POINTER)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot set pointer type. variable is of type ", to_string(_type), "."));
   _pointer = p;
}

torricelly_activation_record_variable torricelly_activation_record_variable::make(const torricelly_value &value)
{
   switch (value.type()->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(value.type());
      switch (system_type->system_type())
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
         return torricelly_activation_record_variable(pointer, torricelly_activation_record_variable_underlying_type::STRING_LITERAL);
      }
      default:
         throw torricelly_interpreter_error("Unknown or unsupported torricelly system type when creating a new activation record type.");
      }
   }
   break;
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(value.type());
      auto torricelly_underlying_type = array_type->underlying_type();
      auto underlying_type = to_underlying_type(torricelly_underlying_type);
      std::vector<unsigned int> dimensions;
      auto size = 1;
      bool isundefined = false;
      for (auto dimension = 0; dimension < array_type->dimensions(); ++dimension)
      {
         dimensions.push_back(array_type->dimension(dimension));
         size *= array_type->dimension(dimension);
         isundefined = isundefined || (array_type->dimension(dimension) == torricelly_array_type::undefined_dimension());
      }
      if (isundefined)
         return torricelly_activation_record_variable(nullptr, torricelly_activation_record_variable_underlying_type::ARRAY);
      auto pointer = std::shared_ptr<torricelly_activation_record_variable_multidimensional_array>(
          new torricelly_activation_record_variable_multidimensional_array(dimensions, underlying_type));
      for (auto index = 0; index < size; ++index)
      {
         auto array_value = value.get_array();
         auto array_item = array_value->at(index);
         torricelly_activation_record_variable_union u;
         switch (underlying_type)
         {
         case torricelly_activation_record_variable_array_underlying_type::UNDEFINED:
            throw torricelly_interpreter_error("Cannot copy undefined underlying type");
         case torricelly_activation_record_variable_array_underlying_type::INTEGER:
            u._integer = array_value->at(index).get_integer();
            break;
         case torricelly_activation_record_variable_array_underlying_type::CHAR:
            u._char = array_value->at(index).get_char();
            break;
         case torricelly_activation_record_variable_array_underlying_type::BOOLEAN:
            u._boolean = array_value->at(index).get_boolean();
            break;
         case torricelly_activation_record_variable_array_underlying_type::FLOAT:
            u._float = array_value->at(index).get_float();
            break;
         case torricelly_activation_record_variable_array_underlying_type::DOUBLE:
            u._double = array_value->at(index).get_double();
            break;
         default:
            throw torricelly_interpreter_error("Unexpected type. Cannot convert value into string.");
         }
         pointer->set(index, u);
      }
      return torricelly_activation_record_variable(pointer, torricelly_activation_record_variable_underlying_type::ARRAY);
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw torricelly_interpreter_error("Torricelly structured type not supported yet. Cannot create a new activation record.");
      break;
   default:
      throw torricelly_interpreter_error("Unexpected or unknown torricelly type when creating a new activation record variable.");
   }
}

torricelly_activation_record_variable torricelly_activation_record_variable::make(bool b) { return torricelly_activation_record_variable(b); }
torricelly_activation_record_variable torricelly_activation_record_variable::make(int i) { return torricelly_activation_record_variable(i); }
torricelly_activation_record_variable torricelly_activation_record_variable::make(float f) { return torricelly_activation_record_variable(f); }
torricelly_activation_record_variable torricelly_activation_record_variable::make(double d) { return torricelly_activation_record_variable(d); }
torricelly_activation_record_variable torricelly_activation_record_variable::make(char c) { return torricelly_activation_record_variable(c); }

std::string gasp::torricelly::interpreter::to_string(const torricelly_activation_record_variable &value)
{
   switch (value.type())
   {
   case torricelly_activation_record_local_type::UNDEFINED:
      return "undefined";
   case torricelly_activation_record_local_type::VOID:
      return "void";
   case torricelly_activation_record_local_type::INTEGER:
      return std::to_string(value.get_integer());
   case torricelly_activation_record_local_type::CHAR:
   {
      auto cvalue = value.get_char();
      if (std::isprint(cvalue))
         return std::string(1, cvalue);
      return std::string("\\") + std::to_string((int)cvalue);
   }
   case torricelly_activation_record_local_type::BOOLEAN:
      return value.get_boolean() ? "true" : "false";
   case torricelly_activation_record_local_type::FLOAT:
      return std::to_string(value.get_float());
   case torricelly_activation_record_local_type::DOUBLE:
      return std::to_string(value.get_double());
   case torricelly_activation_record_local_type::POINTER:
   {
      std::stringstream sstream;
      switch (value.get_pointer_underlying_type())
      {
      case torricelly_activation_record_variable_underlying_type::STRING_LITERAL:
      {
         auto string_pointer = value.get_string_pointer();
         sstream << "'" << *string_pointer << "'"
                 << " (" << value.get_pointer() << ")"
                 << " [" << to_string(torricelly_activation_record_variable_underlying_type::STRING_LITERAL) << "]";
      }
      case torricelly_activation_record_variable_underlying_type::ARRAY:
      {
         auto array_pointer = value.get_array_pointer();
         if (array_pointer == nullptr)
         {
            sstream << "{} (" << value.get_pointer() << ") [" << to_string(torricelly_activation_record_variable_underlying_type::ARRAY) << "<>[]";
         }
         else
         {
            sstream << "{";
            for (auto index = 0; index < array_pointer->size(); ++index)
            {
               if (index > 0)
                  sstream << ",";
               switch (array_pointer->underlying_type())
               {
               case torricelly_activation_record_variable_array_underlying_type::UNDEFINED:
                  sstream << "undefined";
                  break;
               case torricelly_activation_record_variable_array_underlying_type::INTEGER:
                  sstream << array_pointer->get(index)._integer;
                  break;
               case torricelly_activation_record_variable_array_underlying_type::CHAR:
                  sstream << array_pointer->get(index)._char;
                  break;
               case torricelly_activation_record_variable_array_underlying_type::BOOLEAN:
                  sstream << array_pointer->get(index)._boolean;
                  break;
               case torricelly_activation_record_variable_array_underlying_type::FLOAT:
                  sstream << array_pointer->get(index)._float;
                  break;
               case torricelly_activation_record_variable_array_underlying_type::DOUBLE:
                  sstream << array_pointer->get(index)._double;
                  break;
               default:
                  throw torricelly_interpreter_error(sanelli::make_string("Unexpected type. Cannot convert value into string (type id is ", (int)array_pointer->underlying_type(), ")."));
               }
            }
            sstream << "} (" << value.get_pointer() << ")"
                    << " [" << to_string(torricelly_activation_record_variable_underlying_type::ARRAY) << "<"
                    << to_string(array_pointer->underlying_type()) << ">[";
            for (auto dimension = 0; dimension < array_pointer->dimensions(); ++dimension)
            {
               if (dimension > 0)
                  sstream << ",";
               sstream << array_pointer->dimension(dimension);
            }
            sstream << "]]";
         }
      }
      break;
      default:
         sstream << value.get_pointer();
         break;
      }
      return sstream.str();
   }
   default:
      throw torricelly_interpreter_error("Unexpected type for given value. It cannot be converted into string.");
   }
}