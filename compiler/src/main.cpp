#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/ast.hpp>
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

      function native read_integer() : integer;
      function native read_float() : float;
      procedure native write(input: string); 
      procedure native write(input: integer);
      procedure native write(input: float);
      procedure native write(input: array<integer>, input_size: integer);
      function duplicate(input : float) : float 
      begin
         duplicate := 2 * input;
      end;
      function abs(input : integer) : integer 
      begin
         abs := if input >= 0 then input else -input;
      end;
      var
         foo, another_foo: integer;
         bar: float;
         equals: boolean;
         a_value: boolean;
         myc: char;
         myi: integer;
         ctr1: array<string>[20];
         ctr2: array<integer>[2];
      begin
         foo := read_integer();
         another_foo := read_integer();
         bar := read_float();
         bar := foo + bar * 2 + -1 / duplicate(foo);
         equals := bar == foo;
         a_value := true and false;
         begin
            write("Results: ");
            write(foo);
         end
         myc := 'A';
         myi := cast<integer>(myc);
         myi := abs(if myi == 0 then 1 else (myi));
         if a_value then 
            a_value := foo == another_foo;
         if myc == 'b' then begin
            equals:= false;
            a_value := true;
         end
         else
            a_value := false;
         ctr1[0] := "Stefano";
         ctr2[0] := 11;
         ctr2[1] := 10 + 2 * ctr2[2];
         for myi from (0) to (10) step 2 begin
            foo := foo + 10;
         end
         while equals begin
            equals := false;
         end
         do begin
            equals := equals and false;
         end while equals;
         releat
            foo := foo + 1;
            equals := foo > 100;
         until not equals;
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
   catch (gasp::blaise::ast::blaise_ast_error &error)
   {
      cerr << "BLAISE_ast_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
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