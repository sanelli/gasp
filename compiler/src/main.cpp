#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/common/output.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;

int main(int argc, char *argv[])
{
   cout << "GASP - by Stefano Anelli." << endl;

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   vector<token<blaise_token>> tokens;

   stringstream program;
   program << R"program(
      program Hello_World;  
      begin
      end.
   )program";

   try
   {
      tokenizer.parse(program, tokens);
      cout << tokens << endl;
      parser.parse(tokens);
      return EXIT_SUCCESS;
   }
   catch (gasp::common::tokenizer_error &error)
   {
      cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      cerr << tokens << endl;

      return EXIT_FAILURE;
   }
   catch (gasp::common::parser_error &error)
   {
      cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
}