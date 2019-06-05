#include <ostream>
#include <stdexcept>
#include <vector>

#include <gasp/blaise/ast.hpp>

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, gasp::blaise::ast::blaise_ast_type type)
{
   switch(type){
      case blaise_ast_type::VOID:
         return os << "void";
      case blaise_ast_type::INTEGER:
         return os << "integer";
      case blaise_ast_type::FLOAT:
         return os << "float";
      case blaise_ast_type::DOUBLE:
         return os << "double";
      case blaise_ast_type::CHAR:
         return os << "char";
      case blaise_ast_type::STRING:
         return os << "string";
      case blaise_ast_type::BOOLEAN:
         return os << "boolean";
   }
  throw std::runtime_error("Unexpected type when converting type into string representation.");
}

std::ostream &gasp::blaise::ast::operator<<(std::ostream &os, const std::vector<gasp::blaise::ast::blaise_ast_type>& list)
{
  for(int index = 0; index < list.size(); index++){
     os << list.at(index);
     if(index < list.size()-1) os << ", ";
  }
  return os;
}