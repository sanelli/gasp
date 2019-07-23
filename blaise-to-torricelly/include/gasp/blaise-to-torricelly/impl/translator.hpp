#pragma once

#include <memory>
#include <vector>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>


namespace gasp::blaise_to_torricelly {

class translator {
   std::shared_ptr<blaise::ast::blaise_ast_module> _blaise_module;

public:
   translator(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module);
   std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module() const;

   void execute(std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>>& torricelly_modules) const;

};

};