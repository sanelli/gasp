#pragma once

#include <string>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>

namespace gasp::blaise::language {

class blaise_variable {
   std::string _name;
   blaise_expression_type _type;
   public:
      blaise_variable(const gasp::common::token<gasp::blaise::blaise_token_type>& identifier, 
                      const gasp::common::token<gasp::blaise::blaise_token_type>& type);

      std::string name() const;
      blaise_expression_type type() const;
};

class blaise_function_parameters : public blaise_variable{
   // TODO: Add modifiers like IN/OUT/INPUT
};

}