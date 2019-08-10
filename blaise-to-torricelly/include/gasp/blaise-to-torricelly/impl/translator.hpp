#pragma once

#include <memory>
#include <vector>

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
public:
   translator(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module);
   std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module() const;

   void execute(std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules) const;
};

}; // namespace gasp::blaise_to_torricelly