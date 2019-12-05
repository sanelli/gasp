#include <memory>
#include <map>
#include <string>
#include <algorithm>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/ast.hpp>

using namespace sanelli;
using namespace gasp;
using namespace gasp::blaise::ast;
using namespace gasp::blaise;

blaise_ast_type::blaise_ast_type(blaise_ast_type_type type_type) : _type_type(type_type) {}
blaise_ast_type::~blaise_ast_type() {}
blaise_ast_type_type blaise_ast_type::type_type() const { return _type_type; }

blaise_ast_plain_type::blaise_ast_plain_type(blaise_ast_system_type system_type)
    : blaise_ast_type(blaise_ast_type_type::PLAIN), _system_type(system_type) {}
blaise_ast_plain_type::~blaise_ast_plain_type() {}
blaise_ast_system_type blaise_ast_plain_type::system_type() const { return _system_type; }

inline bool blaise_ast_plain_type::equals(std::shared_ptr<blaise_ast_type> other) const
{
   if (type_type() != other->type_type())
      return false;
   auto plain_other = ast::blaise_ast_utility::as_plain_type(other);
   return system_type() == plain_other->system_type();
};

std::shared_ptr<blaise_ast_type> gasp::blaise::ast::make_plain_type(blaise_ast_system_type system_type)
{
   static std::map<blaise_ast_system_type, std::shared_ptr<blaise_ast_type>> __types_cache;
   if (__types_cache.count(system_type) == 0)
      __types_cache[system_type] = memory::make_shared<blaise_ast_plain_type>(system_type);
   return __types_cache.at(system_type);
}

//
// TYPE
//
std::shared_ptr<blaise_ast_type> gasp::blaise::ast::get_type_from_token(const sanelli::token<gasp::blaise::blaise_token_type> &token)
{
   switch (token.type())
   {
   case blaise_token_type::TYPE_BYTE:
      return make_plain_type(blaise_ast_system_type::BYTE);
   case blaise_token_type::TYPE_INTEGER:
      return make_plain_type(blaise_ast_system_type::INTEGER);
   case blaise_token_type::TYPE_SHORT:
      return make_plain_type(blaise_ast_system_type::SHORT);
   case blaise_token_type::TYPE_LONG:
      return make_plain_type(blaise_ast_system_type::LONG);
   case blaise_token_type::TYPE_CHAR:
      return make_plain_type(blaise_ast_system_type::CHAR);
   case blaise_token_type::TYPE_FLOAT:
      return make_plain_type(blaise_ast_system_type::FLOAT);
   case blaise_token_type::TYPE_DOUBLE:
      return make_plain_type(blaise_ast_system_type::DOUBLE);
   case blaise_token_type::TYPE_BOOLEAN:
      return make_plain_type(blaise_ast_system_type::BOOLEAN);
   default:
      throw blaise_ast_error(token.line(), token.column(), sanelli::make_string("Cannot convert token '", token.type(), "' into a type."));
   }
}

// ARRAY
blaise_ast_array_type::blaise_ast_array_type(std::shared_ptr<blaise_ast_type> underlying_type, std::vector<unsigned int> dimensions)
    : blaise_ast_type(blaise_ast_type_type::ARRAY), _underlying_type(underlying_type), _dimensions(dimensions) {}
blaise_ast_array_type::~blaise_ast_array_type() {}
std::shared_ptr<blaise_ast_type> blaise_ast_array_type::underlying_type() const { return _underlying_type; }
unsigned int blaise_ast_array_type::dimensions() const { return _dimensions.size(); }
unsigned int blaise_ast_array_type::dimension(unsigned int dim) const { return _dimensions.at(dim); }
bool blaise_ast_array_type::is_unbounded() const
{
   return std::any_of(_dimensions.begin(), _dimensions.end(), [](unsigned int d) { return d == 0; });
}

inline bool blaise_ast_array_type::equals(std::shared_ptr<blaise_ast_type> other) const
{
   if (type_type() != other->type_type())
      return false;
   auto array_other = ast::blaise_ast_utility::as_array_type(other);
   if (underlying_type() != array_other->underlying_type())
      return false;
   if (dimensions() != array_other->dimensions())
      return false;
   for (auto d = 0u; d < dimensions(); ++d)
      if (dimension(d) != array_other->dimension(d) && array_other->dimension(d) > 0)
         return false;
   return true;
}

std::shared_ptr<blaise_ast_type> gasp::blaise::ast::get_array_type_from_token(
    const sanelli::token<gasp::blaise::blaise_token_type> &reference,
    std::shared_ptr<blaise_ast_type> underlying_type,
    const std::vector<unsigned int> dimensions,
    const bool accept_unbounded_array)
{
   if (dimensions.size() <= 0)
      throw blaise_ast_error(reference.line(), reference.column(), "Cannot create arrays with zero dimensions.");

   auto dimension_equal_to_zero = [](auto d) { return d == 0; };
   bool unbounded = std::any_of(dimensions.begin(), dimensions.end(), dimension_equal_to_zero);
   if (!accept_unbounded_array && unbounded)
      throw blaise_ast_error(reference.line(), reference.column(), "Unbounded array are not supported in this context.");

   if (std::any_of(dimensions.begin(), dimensions.end(), [](auto d) { return d < 0; }))
      throw blaise_ast_error(reference.line(), reference.column(), "Invalid array dimension.");

   // If unbounded all dimensions must be zero
   if (unbounded && !std::all_of(dimensions.begin(), dimensions.end(), dimension_equal_to_zero))
      throw blaise_ast_error(reference.line(), reference.column(), "Unsupported partially unbounded array.");

   return memory::make_shared<blaise_ast_array_type>(underlying_type, dimensions);
}

std::string gasp::blaise::ast::to_string(gasp::blaise::ast::blaise_ast_system_type type)
{
   switch (type)
   {
   case blaise_ast_system_type::BOOLEAN:
      return "boolean";
   case blaise_ast_system_type::CHAR:
      return "char";
   case blaise_ast_system_type::DOUBLE:
      return "double";
   case blaise_ast_system_type::FLOAT:
      return "float";
   case blaise_ast_system_type::BYTE:
      return "byte";
   case blaise_ast_system_type::SHORT:
      return "short";
   case blaise_ast_system_type::INTEGER:
      return "integer";
   case blaise_ast_system_type::LONG:
      return "long";
   case blaise_ast_system_type::STRING:
      return "string";
   case blaise_ast_system_type::VOID:
      return "void";
   default:
      throw blaise_ast_error(0, 0, "Cannot convert blaise system type to string");
   }
}

std::string gasp::blaise::ast::to_string(std::shared_ptr<blaise_ast_type> type)
{
   if (type == nullptr)
      return "nullptr";
   switch (type->type_type())
   {
   case blaise_ast_type_type::PLAIN:
   {
      auto plain_type = blaise_ast_utility::as_plain_type(type);
      return ast::to_string(plain_type->system_type());
   }
   break;
   case blaise_ast_type_type::ARRAY:
   {
      auto array_type = blaise_ast_utility::as_array_type(type);
      std::stringstream output;
      output << "array<" << to_string(array_type->underlying_type()) << ">";
      output << "[";
      for (auto d = 0u; d < array_type->dimensions(); ++d)
      {
         if (d > 0)
            output << ", ";
         auto dimension = array_type->dimension(d);
         if(dimension > 0)
            output << array_type->dimension(d);
      }
      output << "]";
      return output.str();
   }
   break;
   case blaise_ast_type_type::POINTER:
      throw blaise_ast_error(0, 0, "Cannot convert blaise pointer type to string");
   case blaise_ast_type_type::USER_DEFINED:
      throw blaise_ast_error(0, 0, "Cannot convert blaise user defined type to string");
   default:
      throw blaise_ast_error(0, 0, "Cannot convert blaise type to string");
   }
}