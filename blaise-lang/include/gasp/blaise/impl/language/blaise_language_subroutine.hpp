#pragma once

#include <string>
#include <memory>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_variable.hpp>
#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>

namespace gasp::blaise::language {

class blaise_subroutine {
   std::string _name;
   blaise_expression_type _return_type;
   //TODO: Add parameters
   std::vector<std::shared_ptr<blaise_variable>> _variables;
public:
   blaise_subroutine(const std::string& name);
   blaise_subroutine(const std::string& name, blaise_expression_type return_type);

   std::string name() const;
   blaise_expression_type return_type() const;
   
   // Variables management;
   void add_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                     const gasp::common::token<gasp::blaise::blaise_token_type>& type);
   std::shared_ptr<blaise_variable> get_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier) const;
};
}