#include <string>
#include <memory>
#include <algorithm>
#include <bitset>
#include <type_traits>
#include <sstream>

#include <gasp/blaise/ast.hpp>

#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace gasp::blaise::ast;
using namespace gasp::blaise;

using namespace std;

gasp::blaise::ast::blaise_ast_subroutine::blaise_ast_subroutine(weak_ptr<blaise_ast_module> module, const string &name)
    : _module(module), _name(name), _return_type(make_plain_type(blaise_ast_system_type::VOID)), _defined(false) {}

std::string gasp::blaise::ast::blaise_ast_subroutine::name() const { return _name; }
bool gasp::blaise::ast::blaise_ast_subroutine::defined() const { return _defined; }
void gasp::blaise::ast::blaise_ast_subroutine::define() { _defined = true; }
std::shared_ptr<blaise_ast_type> gasp::blaise::ast::blaise_ast_subroutine::return_type() const { return _return_type; }
void gasp::blaise::ast::blaise_ast_subroutine::return_type(std::shared_ptr<blaise_ast_type> type) { _return_type = type; }
std::weak_ptr<blaise_ast_module> gasp::blaise::ast::blaise_ast_subroutine::module() const { return _module; }

bool gasp::blaise::ast::blaise_ast_subroutine::signature_match_exactly(const std::string name, const std::vector<std::shared_ptr<blaise_ast_type>> &param_types) const
{
   if (_name != name)
      return false;
   if (_parameters.size() != param_types.size())
      return false;
   for (int index = 0; index < _parameters.size(); ++index)
      if (!_parameters.at(index)->type()->equals(param_types.at(index)))
         return false;
   return true;
}

bool gasp::blaise::ast::blaise_ast_subroutine::signature_match_with_cast(const std::string name, const std::vector<std::shared_ptr<blaise_ast_type>> &param_types) const
{
   if (_name != name)
      return false;
   if (_parameters.size() != param_types.size())
      return false;
   for (int index = 0; index < _parameters.size(); ++index)
   {
      auto param = param_types.at(index);
      auto _param = _parameters.at(index)->type();
      if (param->type_type() != _param->type_type())
         return false;
      if (param->type_type() == blaise_ast_type_type::ARRAY)
      {
         auto param_array = blaise_ast_utility::as_array_type(param);
         auto _param_array = blaise_ast_utility::as_array_type(_param);

         if(param_array->is_unbounded() && !_param_array->is_unbounded())
            return false;

         if ((param_array->is_unbounded() || _param_array->is_unbounded()) 
            && param_array->dimensions() == _param_array->dimensions() 
            && param_array->underlying_type()->equals(_param_array->underlying_type()))
            continue;

         if (!param_array->is_unbounded() && !_param_array->is_unbounded() 
            && param_array->dimensions() == _param_array->dimensions()
            && param_array->underlying_type()->equals(_param_array->underlying_type()))
         {
            auto matches = true;
            for (auto d = 0; matches && (d < param_array->dimensions()); ++d)
            {
               matches = param_array->dimension(d) == _param_array->dimension(d);
            }
            if (matches)
               continue;
         }

         return false;
      }
      else if (!ast::blaise_ast_utility::can_auto_cast(param, _parameters.at(index)->type()))
         return false;
   }
   return true;
}

std::string gasp::blaise::ast::blaise_ast_subroutine::signature_as_string() const
{
   std::stringstream stream;
   stream << _name << "(";
   for (int index = 0; index < _parameters.size(); ++index)
   {
      stream << _parameters.at(index)->type();
      if (index != (_parameters.size() - 1))
         stream << ",";
   }
   stream << ")";
   return stream.str();
}

unsigned long gasp::blaise::ast::blaise_ast_subroutine::get_arity() const
{
   return _parameters.size();
}
std::shared_ptr<blaise_ast_type> gasp::blaise::ast::blaise_ast_subroutine::get_parameter_type(unsigned long index) const
{
   if (index >= get_arity())
      throw std::logic_error(sanelli::make_string("Required type at index ", get_arity(), " for subroutine '", signature_as_string(), "'."));
   return _parameters.at(index)->type();
}

bool gasp::blaise::ast::blaise_ast_subroutine::is(blaise_ast_subroutine_flags flag) const
{
   return _flags.test(static_cast<typename std::underlying_type<blaise_ast_subroutine_flags>::type>(flag));
}
void gasp::blaise::ast::blaise_ast_subroutine::set(blaise_ast_subroutine_flags flag)
{
   _flags.set(static_cast<typename std::underlying_type<blaise_ast_subroutine_flags>::type>(flag));
}
void gasp::blaise::ast::blaise_ast_subroutine::reset(blaise_ast_subroutine_flags flag)
{
   _flags.reset(static_cast<typename std::underlying_type<blaise_ast_subroutine_flags>::type>(flag));
}

std::shared_ptr<blaise_ast_variable> gasp::blaise::ast::blaise_ast_subroutine::get_variable(const std::string &identifier) const
{
   return get_memory_from_vector(_variables, identifier);
}

std::shared_ptr<blaise_ast_constant> gasp::blaise::ast::blaise_ast_subroutine::get_constant(const std::string &identifier) const
{
   return get_memory_from_vector(_constants, identifier);
}

std::shared_ptr<blaise_ast_subroutine_parameter> gasp::blaise::ast::blaise_ast_subroutine::get_parameter(const std::string &identifier) const
{
   return get_memory_from_vector(_parameters, identifier);
}

unsigned int gasp::blaise::ast::blaise_ast_subroutine::count_parameters() const { return _parameters.size(); }
std::shared_ptr<blaise_ast_subroutine_parameter> gasp::blaise::ast::blaise_ast_subroutine::get_parameter(unsigned int index) const { return _parameters.at(index); }
unsigned int gasp::blaise::ast::blaise_ast_subroutine::count_constants() const { return _constants.size(); }
std::shared_ptr<blaise_ast_constant> gasp::blaise::ast::blaise_ast_subroutine::get_constant(unsigned int index) const { return _constants.at(index); }
unsigned int gasp::blaise::ast::blaise_ast_subroutine::count_variables() const { return _variables.size(); }
std::shared_ptr<blaise_ast_variable> gasp::blaise::ast::blaise_ast_subroutine::get_variable(unsigned int index) const { return _variables.at(index); }

std::shared_ptr<blaise_ast_generic_memory_location> gasp::blaise::ast::blaise_ast_subroutine::get_memory_location(const std::string &identifier) const
{
   auto variable = get_variable(identifier);
   if (variable != nullptr)
      return variable;
   auto constant = get_constant(identifier);
   if (constant != nullptr)
      return constant;
   auto parameter = get_parameter(identifier);
   if (parameter != nullptr)
      return parameter;
   return nullptr;
}

shared_ptr<blaise_ast_variable> gasp::blaise::ast::blaise_ast_subroutine::add_variable(const token<blaise_token_type> &reference,
                                                                                       const std::string &identifier,
                                                                                       std::shared_ptr<blaise_ast_type> type)
{
   SANELLI_DEBUG("blaise-lang", "[BEGIN] blaise_ast_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Variable '", identifier, "' already defined."));
   auto variable = ast::make_blaise_ast_variable(reference, identifier, type);
   _variables.push_back(variable);
   SANELLI_DEBUG("blaise-lang", "[END] blaise_ast_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   return variable;
}

shared_ptr<blaise_ast_constant> gasp::blaise::ast::blaise_ast_subroutine::add_constant(const token<blaise_token_type> &reference,
                                                                                       const std::string &identifier,
                                                                                       std::shared_ptr<blaise_ast_type> type)
{
   SANELLI_DEBUG("blaise-lang", "[BEGIN] blaise_ast_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Constant '", identifier, "' already defined."));
   auto constant = ast::make_blaise_ast_constant(reference, identifier, type);
   _constants.push_back(constant);
   SANELLI_DEBUG("blaise-lang", "[END] blaise_ast_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
   return constant;
}

shared_ptr<blaise_ast_subroutine_parameter> gasp::blaise::ast::blaise_ast_subroutine::add_parameter(const token<blaise_token_type> &reference,
                                                                                                    const std::string &identifier,
                                                                                                    std::shared_ptr<blaise_ast_type> type)
{
   SANELLI_DEBUG("blaise-lang", "[BEGIN] blaise_ast_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Parameter '", identifier, "' already defined."));
   auto parameter = ast::make_blaise_ast_subroutine_parameter(reference, identifier, type);
   _parameters.push_back(parameter);
   SANELLI_DEBUG("blaise-lang", "[END] blaise_ast_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
   return parameter;
}

void gasp::blaise::ast::blaise_ast_subroutine::push_back(std::shared_ptr<blaise_ast_statement> statement)
{
   _statements.push_back(statement);
}
std::shared_ptr<blaise_ast_statement> gasp::blaise::ast::blaise_ast_subroutine::get_statement(unsigned int index) const
{
   return _statements.at(index);
}
unsigned int gasp::blaise::ast::blaise_ast_subroutine::get_statements_count() const
{
   return _statements.size();
}

std::shared_ptr<blaise_ast_subroutine> gasp::blaise::ast::make_blaise_ast_subroutine(std::weak_ptr<blaise_ast_module> module, const std::string &name)
{
   return memory::make_shared<blaise_ast_subroutine>(module, name);
}