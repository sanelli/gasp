#include <ostream>
#include <stdexcept>
#include <vector>

#include <gasp/blaise/ast.hpp>

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, gasp::blaise::ast::blaise_ast_system_type type)
{
   switch(type){
      case blaise_ast_system_type::VOID:
         return os << "void";
      case blaise_ast_system_type::INTEGER:
         return os << "integer";
      case blaise_ast_system_type::FLOAT:
         return os << "float";
      case blaise_ast_system_type::DOUBLE:
         return os << "double";
      case blaise_ast_system_type::CHAR:
         return os << "char";
      case blaise_ast_system_type::STRING:
         return os << "string";
      case blaise_ast_system_type::BOOLEAN:
         return os << "boolean";
   }
  throw std::runtime_error("Unexpected type when converting type into string representation.");
}

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, std::shared_ptr<gasp::blaise::ast::blaise_ast_type> type){
   switch(type->type_type()){
      case gasp::blaise::ast::blaise_ast_type_type::PLAIN:
         return os << gasp::blaise::ast::blaise_ast_utility::as_plain_type(type)->system_type();
      case gasp::blaise::ast::blaise_ast_type_type::ARRAY:
        { auto array_type = gasp::blaise::ast::blaise_ast_utility::as_array_type(type);
           return os << "array<" << array_type->inner_type() <<  ">(" << array_type->size() << ")";
         }
      default:
         throw std::runtime_error("Unexpected type's type.");
   }
}

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, const std::vector<std::shared_ptr<gasp::blaise::ast::blaise_ast_type>>& list)
{
  for(int index = 0; index < list.size(); index++){
     os << list.at(index);
     if(index < list.size()-1) os << ", ";
  }
  return os;
}