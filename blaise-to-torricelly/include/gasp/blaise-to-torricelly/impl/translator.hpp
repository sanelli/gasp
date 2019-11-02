#pragma once

#include <memory>
#include <vector>
#include <functional>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>

namespace gasp::blaise_to_torricelly
{

class translator
{
   std::shared_ptr<blaise::ast::blaise_ast_module> _blaise_module;

   bool has_translated(const std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules, const std::string &module_name) const;

   std::string get_mangled_module_name(const std::string &name) const;
   std::string get_mangled_subroutine_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_subroutine> subroutine) const;
   std::string get_mangled_type_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> type) const;

   std::shared_ptr<gasp::torricelly::torricelly_module> translate_module(std::shared_ptr<blaise::ast::blaise_ast_module> module) const;
   std::shared_ptr<gasp::torricelly::torricelly_subroutine> translate_subroutine(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,
                                                                                 std::map<std::string, unsigned int> &module_variables_mapping, std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine) const;
   std::shared_ptr<gasp::torricelly::torricelly_type> translate_type(std::shared_ptr<blaise::ast::blaise_ast_type> type) const;

   void translate_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement> statement, unsigned int &max_stack_size) const;
   void translate_empty_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_empty> statement, unsigned int &max_stack_size) const;
   void translate_compound_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_compund> statement, unsigned int &max_stack_size) const;
   void translate_assignment_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_assignment> statement, unsigned int &max_stack_size) const;
   void translate_if_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_if> statement, unsigned int &max_stack_size) const;
   void translate_subroutine_call_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_subroutine_call> statement, unsigned int &max_stack_size) const;
   void translate_while_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_while_loop> statement, unsigned int &max_stack_size) const;
   void translate_do_while_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_dowhile_loop> statement, unsigned int &max_stack_size) const;
   void translate_repeat_until_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_repeatuntil_loop> statement, unsigned int &max_stack_size) const;
   void translate_for_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_for_loop> statement, unsigned int &max_stack_size) const;
   void translate_delete_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_delete> statement, unsigned int &max_stack_size) const;

   void translate_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                             std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression, unsigned int &max_stack_size) const;
   void translate_literal_integer_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_integer_value> expression, unsigned int &max_stack_size) const;
   void translate_literal_float_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_float_value> expression, unsigned int &max_stack_size) const;
   void translate_literal_double_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_double_value> expression, unsigned int &max_stack_size) const;
   void translate_literal_char_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_char_value> expression, unsigned int &max_stack_size) const;
   void translate_literal_boolean_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_boolean_value> expression, unsigned int &max_stack_size) const;
   void translate_memory_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_generic_memory_access> expression, unsigned int &max_stack_size) const;
   void translate_variable_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_memory_access> expression, unsigned int &max_stack_size) const;
   void translate_array_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_array_access> expression, unsigned int &max_stack_size) const;
   void translate_unary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_unary> expression, unsigned int &max_stack_size) const;
   void translate_binary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_binary> expression, unsigned int &max_stack_size) const;
   void translate_ternary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_ternary> expression, unsigned int &max_stack_size) const;
   void translate_subroutine_call_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                                             std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_subroutine_call> expression, unsigned int &max_stack_size) const;
   void translate_cast_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                                  std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_cast> expression, unsigned int &max_stack_size) const;
   void translate_new_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,
         std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, 
         std::map<std::string, unsigned int> &module_variables_mapping, 
         std::map<std::string, unsigned int> &variables_mapping, 
         std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_new> expression,
         unsigned int &max_stack_size) const;

   torricelly::torricelly_inst_code compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type, torricelly::torricelly_inst_code integer_op, torricelly::torricelly_inst_code float_op, torricelly::torricelly_inst_code double_op) const;
   torricelly::torricelly_inst_code compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type, torricelly::torricelly_inst_code integer_op, torricelly::torricelly_inst_code float_op, torricelly::torricelly_inst_code double_op,
                                                             torricelly::torricelly_inst_code char_op, torricelly::torricelly_inst_code boolean_op) const;
   torricelly::torricelly_inst_code get_jump_for_comparison_token(gasp::blaise::blaise_token_type token_type) const;
   gasp::torricelly::torricelly_value get_type_initial_value(std::shared_ptr<blaise::ast::blaise_ast_type> type) const;
   unsigned int add_temporary(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, gasp::torricelly::torricelly_value initial_value) const;
   unsigned int add_module_subroutine_reference_local(
       std::shared_ptr<gasp::torricelly::torricelly_module> module,
       std::string subroutine_mangled_name,
       std::map<std::string, unsigned int> &module_variables_mapping
       ) const;

   void translate_condition(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                            std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression, unsigned int &max_stack_size) const;
   void translate_subroutine_call(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                  std::map<std::string, unsigned int> &module_variables_mapping,
                                  std::map<std::string, unsigned int> &variables_mapping,
                                  std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine,
                                  std::function<std::shared_ptr<gasp::blaise::ast::blaise_ast_expression>(unsigned int)> get_parameter,
                                  unsigned int &max_stack_size) const;

public:
   translator(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module);
   std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module() const;

   void execute(std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules) const;
};

}; // namespace gasp::blaise_to_torricelly