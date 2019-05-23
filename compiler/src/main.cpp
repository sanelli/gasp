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
   blaise_parser_context context;

   stringstream program;
   program << R"GASP(
      program Hello_World;
      var
         foo, another_foo: integer;
         bar: boolean;
         foobar: string;
      begin
      end.
   )GASP";

   try
   {
      tokenizer.tokenize(program, context);
      cout << context << endl;
      parser.parse(context);
      return EXIT_SUCCESS;
   }
   catch (gasp::common::tokenizer_error &error)
   {
      cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      cerr << context << endl;

      return EXIT_FAILURE;
   }
   catch (gasp::common::parser_error &error)
   {
      cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
}