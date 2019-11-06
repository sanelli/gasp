#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace sanelli;
using namespace gasp;
using namespace gasp::torricelly;

torricelly_type::torricelly_type(torricelly_type_type type_type) : _type_type(type_type) {}
torricelly_type::~torricelly_type() {}
torricelly_type_type torricelly_type::type_type() const { return _type_type; }

torricelly_system_type::torricelly_system_type(torricelly_system_type_type system_type)
    : torricelly_type(torricelly_type_type::SYSTEM),
      _system_type(system_type) {}
torricelly_system_type::~torricelly_system_type() {}
torricelly_system_type_type torricelly_system_type::system_type() const { return _system_type; }
inline bool torricelly_system_type::equals(std::shared_ptr<torricelly_type> other) const
{
   if (other->type_type() != type_type())
      return false;
   auto other_system_type = torricelly_type_utility::as_system_type(other);
   return system_type() == other_system_type->system_type();
}

torricelly_array_type::torricelly_array_type(std::shared_ptr<torricelly_type> underlying_type, const std::vector<unsigned int> &dimensions)
    : torricelly_type(torricelly_type_type::ARRAY), _underlying_type(underlying_type)
{
   std::copy(dimensions.begin(), dimensions.end(), std::back_inserter(_dimensions));
}

torricelly_array_type::~torricelly_array_type() {}
std::shared_ptr<torricelly_type> torricelly_array_type::underlying_type() const { return _underlying_type; }
unsigned int torricelly_array_type::dimensions() const { return _dimensions.size(); }
unsigned int torricelly_array_type::dimension(unsigned int dim) const { return _dimensions.at(dim); }
inline bool torricelly_array_type::equals(std::shared_ptr<torricelly_type> other) const
{
   if (other->type_type() != type_type())
      return false;
   auto other_array_type = torricelly_type_utility::as_array_type(other);
   if (!underlying_type()->equals(other_array_type->underlying_type()))
      return false;
   if (dimensions() != other_array_type->dimensions())
      return false;
   for (auto d = 0U; d < dimensions(); ++d)
   {
      auto dim = dimension(d);
      auto other_dimension = other_array_type->dimension(d);
      if (dim == torricelly_array_type::undefined_dimension() ||
          other_dimension == torricelly_array_type::undefined_dimension())
         continue;
      if (dim != other_dimension)
         return false;
   }
   return true;
}
unsigned int torricelly_array_type::undefined_dimension() { return std::numeric_limits<unsigned int>::max(); }

std::shared_ptr<torricelly_system_type> torricelly_type_utility::as_system_type(std::shared_ptr<torricelly_type> type)
{
   return std::static_pointer_cast<torricelly_system_type>(type);
}

std::shared_ptr<torricelly_array_type> torricelly_type_utility::as_array_type(std::shared_ptr<torricelly_type> type)
{
   return std::static_pointer_cast<torricelly_array_type>(type);
}

bool torricelly_type_utility::is_system_type(std::shared_ptr<torricelly_type> type)
{
   return type->type_type() == torricelly_type_type::SYSTEM;
}

bool torricelly_type_utility::is_void(std::shared_ptr<torricelly_type> type)
{
   if (!torricelly_type_utility::is_system_type(type)) return false;
   auto system_type = torricelly_type_utility::as_system_type(type);
   return system_type->system_type() == torricelly_system_type_type::VOID;
}

bool torricelly_type_utility::is_string_literal(std::shared_ptr<torricelly_type> type)
{
   if (torricelly_type_utility::is_system_type(type))
      return false;
   auto system_type = torricelly_type_utility::as_system_type(type);
   return system_type->system_type() == torricelly_system_type_type::LITERAL_STRING;
}

std::shared_ptr<torricelly_system_type> torricelly::make_torricelly_system_type(torricelly_system_type_type system_type)
{
   return memory::make_shared<torricelly_system_type>(system_type);
}

std::shared_ptr<torricelly_array_type> torricelly::make_torricelly_array_type(std::shared_ptr<torricelly_type> underlying_type, const std::vector<unsigned int> &dimensions)
{
   return memory::make_shared<torricelly_array_type>(underlying_type, dimensions);
}

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_type_type type)
{
   return os << to_string(type);
}

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_system_type_type type)
{
   return os << to_string(type);
}

std::ostream &torricelly::operator<<(std::ostream &os, const std::shared_ptr<torricelly_type> type)
{
   return os << to_string(type);
}

std::string torricelly::to_string(torricelly_type_type type)
{
   switch (type)
   {
   case torricelly_type_type::UNDEFINED:
      return "undefined";
   case torricelly_type_type::SYSTEM:
      return "system";
   case torricelly_type_type::ARRAY:
      return "array";
   case torricelly_type_type::STRUCTURED:
      return "structured";
   default:
      throw torricelly_error("Unknown torricelly type");
   }
}
std::string torricelly::to_string(torricelly_system_type_type type)
{
   switch (type)
   {
   case torricelly_system_type_type::UNDEFINED:
      return "undefined";
   case torricelly_system_type_type::VOID:
      return "void";
   case torricelly_system_type_type::INTEGER:
      return "integer";
   case torricelly_system_type_type::FLOAT:
      return "float";
   case torricelly_system_type_type::DOUBLE:
      return "double";
   case torricelly_system_type_type::CHAR:
      return "char";
   case torricelly_system_type_type::BOOLEAN:
      return "boolean";
   case torricelly_system_type_type::LITERAL_STRING:
      return "string_literal";
   default:
      throw torricelly_error("Unknown torricelly system type");
   }
}
std::string torricelly::to_string(const std::shared_ptr<torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      return to_string(torricelly_system_type_type::UNDEFINED);
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = torricelly_type_utility::as_system_type(type);
      return to_string(torricelly_type_type::SYSTEM) + "::" + to_string(system_type->system_type());
   }
   case torricelly_type_type::ARRAY:
   {
      auto array_type = torricelly_type_utility::as_array_type(type);
      auto representation = to_string(torricelly_type_type::ARRAY) + "<" + to_string(array_type->underlying_type()) + ">[";
      for (auto dimension = 0U; dimension < array_type->dimensions(); ++dimension)
      {
         if (dimension > 0)
            representation += ", ";
         auto d = array_type->dimension(dimension);
         representation += d != torricelly_array_type::undefined_dimension()
                               ? std::to_string(d)
                               : "...";
      }
      representation += "]";
      return representation;
   }
   case torricelly_type_type::STRUCTURED:
      return to_string(torricelly_type_type::STRUCTURED);
   default:
      throw torricelly_error("Unknown type");
   }
}