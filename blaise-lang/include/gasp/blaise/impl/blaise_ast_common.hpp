#pragma once

#include <gasp/blaise/tokenizer/tokens.hpp>


namespace gasp::blaise::ast {

   class blaise_ast {
      protected:
         unsigned int _line;
         unsigned int _column;

         blaise_ast(unsigned int line, unsigned int column);
         blaise_ast(const sanelli::token<gasp::blaise::blaise_token_type>& reference);
         blaise_ast(const sanelli::token<gasp::blaise::blaise_token_type>& reference, blaise_token_type expected_type);

      public:
         unsigned int line() const;
         unsigned int column() const;
   };

}