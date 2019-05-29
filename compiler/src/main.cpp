#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/language.hpp>
#include <gasp/common/output.hpp>
#include <gasp/common/debug.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;

// TODO: Add error codes when I throw errors

int main(int argc, char *argv[])
{
   GASP_INSTALL_DEBUGGER("blaise-parser"); 
   GASP_INSTALL_DEBUGGER("common-parser");
   GASP_INSTALL_DEBUGGER("blaise-lang");
   GASP_INSTALL_DEBUGGER("common-tokenizer");

   cout << "GASP - by Stefano Anelli." << endl;

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;

   stringstream program;
   program << R"___(
      program Hello_World;
      var
         foo, another_foo: integer;
         bar: boolean;
         foobar: string;
      begin
         readline(foo, bar);
         another_foo := foo + bar * 2 + -1 / foo;
         begin
            writeline(foo);
            writeline(bar);
         end;
      end.
   )___";

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
   catch (gasp::blaise::language::blaise_language_error &error)
   {
      cerr << "BLAISE_LANGUAGE_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch(std::exception &error) { 
      cerr << "GENERIC_ERROR:"  << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch(...) { 
      cerr << "UNKNOWN ERROR -- TERMINATING"  << endl;
      return EXIT_FAILURE;
   }
}