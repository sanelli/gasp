#include <string>
#include <memory>

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
   return other->type_type() == type_type() &&
          std::dynamic_pointer_cast<torricelly_system_type>(other)->system_type() == system_type();
}

std::shared_ptr<torricelly_system_type> torricelly_type_utility::as_system_type(std::shared_ptr<torricelly_type> type)
{
   return std::static_pointer_cast<torricelly_system_type>(type);
}

bool torricelly_type_utility::is_void(std::shared_ptr<torricelly_type> type) { 
   if(type->type_type() != torricelly_type_type::SYSTEM) return false;
   auto system_type = torricelly_type_utility::as_system_type(type);
   return system_type->system_type() == torricelly_system_type_type::VOID;
}

std::shared_ptr<torricelly_system_type> torricelly::make_torricelly_system_type(torricelly_system_type_type system_type)
{
   return memory::make_shared<torricelly_system_type>(system_type);
}

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_type_type type)
{
   return os << to_string(type);
}

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_system_type_type type)
{
   return os << to_string(type);
}

std::ostream &torricelly::operator<<(std::ostream &os, const std::shared_ptr<const torricelly_type> type)
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
   case torricelly_system_type_type::STRING_LITERAL:
      return "string";
   default:
      throw torricelly_error("Unknown torricelly system type");
   }
}
std::string torricelly::to_string(const std::shared_ptr<const torricelly_type> type)
{
   switch (type->type_type())
   {
   case torricelly_type_type::UNDEFINED:
      return to_string(torricelly_system_type_type::UNDEFINED);
   case torricelly_type_type::SYSTEM:
   {
      auto system_type = std::dynamic_pointer_cast<const torricelly_system_type>(type);
      return to_string(torricelly_type_type::SYSTEM) + "::" + to_string(system_type->system_type());
   }
   case torricelly_type_type::STRUCTURED:
      return to_string(torricelly_type_type::STRUCTURED);
   default:
      throw torricelly_error("Unknown type");
   }
}