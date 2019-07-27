#include <string>
#include <memory>

#include <gasp/common/memory.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::common;
using namespace gasp::torricelly;

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_type_type type){
   switch(type){
      case torricelly_type_type::UNDEFINED:
         return os << "undefined";
      case torricelly_type_type::SYSTEM:
         return os << "system";
      case torricelly_type_type::STRUCTURED:
         return os << "structured";
      default:
         throw torricelly_error("Unknown torricelly type");
   }
   return os;
}
std::ostream &torricelly::operator<<(std::ostream &os, torricelly_system_type_type type) {
 switch(type){
   case torricelly_system_type_type::UNDEFINED:
      return os << "undefined";
   case torricelly_system_type_type::VOID:
      return os << "void";
   case torricelly_system_type_type::INTEGER:
      return os << "integer";
   case torricelly_system_type_type::FLOAT:
      return os << "float";
   case torricelly_system_type_type::DOUBLE:
      return os << "double";
   case torricelly_system_type_type::CHAR:
      return os << "char";
   case torricelly_system_type_type::BOOLEAN:
      return os << "boolean";
   default:
      throw torricelly_error("Unknown torricelly system type");
   }
   return os;
}

torricelly_type::torricelly_type(torricelly_type_type type_type) : _type_type(type_type) {}
torricelly_type::~torricelly_type() {}
torricelly_type_type torricelly_type::type_type() const { return _type_type; }

toricelly_system_type::toricelly_system_type(torricelly_system_type_type system_type) 
   : torricelly_type(torricelly_type_type::SYSTEM),
      _system_type(system_type) {}
toricelly_system_type::~toricelly_system_type() { }
torricelly_system_type_type toricelly_system_type::system_type() const { return _system_type; }
inline bool toricelly_system_type::equals(std::shared_ptr<torricelly_type> other) const { 
   return other->type_type() == type_type() && 
          std::dynamic_pointer_cast<toricelly_system_type>(other)->system_type() == system_type();
}

std::shared_ptr<toricelly_system_type> torricelly::make_torricelly_system_type(torricelly_system_type_type system_type){
   return memory::gasp_make_shared<toricelly_system_type>(system_type);
}
