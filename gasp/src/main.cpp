#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

#include <sanelli/sanelli.hpp>

#include <gasp/common/internal_error.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace std;

using namespace gasp::blaise;
using namespace gasp::torricelly;
using namespace gasp::blaise_to_torricelly;


// TODO: Add error codes when I throw errors

int main(int argc, char *argv[])
{
   SANELLI_INSTALL_DEBUGGER("blaise-parser"); 
   SANELLI_INSTALL_DEBUGGER("sanelli-parser");
   SANELLI_INSTALL_DEBUGGER("blaise-lang");
   SANELLI_INSTALL_DEBUGGER("sanelli-tokenizer");
   SANELLI_INSTALL_DEBUGGER("torricelly-lang");
   SANELLI_INSTALL_DEBUGGER("blaise-to-torricelly");
   
   cout << "GASP - by Stefano Anelli." << endl;

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;
   std::vector<std::shared_ptr<torricelly_module>> modules;
   torricelly_text_output torricelly_output(std::cout);

   stringstream program;

   // program << R"__(
   //    variable
   // )__";

   // program <<  R"__(program variable_program;
   // var
   //   true_: boolean;
   // begin    
   // end.)__";

//    program << R"___(
// program code0003;
// var
//    i, j, k: boolean;
// begin
//    i := true;
//    i := false;
//    i := true and false;
//    i := true or false;
//    i := not true;

//    j := i and true;
//    j := true and i;
//    j := i and i;

//    j := i or true;
//    j := true or i;
//    j := i or i;

//    j := not i;
//    k := (i and j) or (i and true) or (j and false);

//    k := true == true;
//    k := i == true;
//    k := true == i;
//    k := i == j;

//    k := true > true;
//    k := i > true;
//    k := true > i;
//    k := i > j;

//    k := true >= true;
//    k := i >= true;
//    k := true >= i;
//    k := i >= j;

//    k := true <= true;
//    k := i <= true;
//    k := true <= i;
//    k := i <= j;

//    k := true < true;
//    k := i < true;
//    k := true < i;
//    k := i < j;

//    k := true <> true;
//    k := i <> true;
//    k := true <> i;
//    k := i <> j;
// end.
//    )___";

   // program << R"___(
   //    program hello_world;
   //    function native read_integer() : integer;
   //    function native read_float() : float;
   //    procedure native write(input: string); 
   //    procedure native write(input: integer);
   //    procedure native write(input: float);
   //    procedure native write(input: array<integer>, input_size: integer);
   //    function duplicate(input : float) : float 
   //    begin
   //       duplicate := 2 * input;
   //    end;
   //    function abs(input : integer) : integer 
   //    begin
   //       abs := if input >= 0 then input else -input;
   //    end;
   //    var
   //       foo, another_foo: integer;
   //       bar: float;
   //       equals: boolean;
   //       a_value: boolean;
   //       myc: char;
   //       myi: integer;
   //       ctr1: array<string>[20];
   //       ctr2: array<integer>[2];
   //    begin
   //       foo := read_integer();
   //       another_foo := read_integer();
   //       bar := read_float();
   //       bar := foo + bar * 2 + -1 / duplicate(foo);
   //       equals := bar == foo;
   //       a_value := true and false;
   //       begin
   //          write("Results: ");
   //          write(foo);
   //       end
   //       myc := 'A';
   //       myi := cast<integer>(myc);
   //       myi := abs(if myi == 0 then 1 else (myi));
   //       if a_value then 
   //          a_value := foo == another_foo;
   //       if myc == 'b' then begin
   //          equals:= false;
   //          a_value := true;
   //       end
   //       else
   //          a_value := false;
   //       ctr1[0] := "Stefano";
   //       ctr2[0] := 11;
   //       ctr2[1] := 10 + 2 * ctr2[2];
   //       while equals begin
   //          equals := false;
   //       end
   //       do begin
   //          equals := equals and false;
   //       end while equals;
   //       repeat begin
   //          foo := foo + 1;
   //          equals := foo > 100;
   //       end until not equals;
   //       for myi from 0 to 10 step 2 begin
   //          foo := foo + 10;
   //       end
   //       bar := bar + foo;
   //    end.
   // )___";

   program << R"___(
      program hello_world;
      function duplicate(i: integer) : integer
      begin
         duplicate := 2 * i;
      end;
      function abs(i: integer) : integer
      begin
         abs := if i < 0 then -i else i;
      end;
      function abs(f: float) : float
      begin
         abs := if f < 0 then -f else f;
      end;
      function abs(d: double) : double
      begin
         abs := if d < 0 then -d else d;
      end;
      function average(a: float, b: float, c: float) : float
      begin
         average := (a + b + c) / 3;
      end;
      var
         i,j,k: integer;
         d: double;
         f1,f2: float;
      begin
         i := 0;
         j := 1;
         k := duplicate(j);
      end.
   )___";

   try
   {
      tokenizer.tokenize(program, context);
      cout << "-- TOKENS -------------------------------------------------------" << endl;
      cout << context << endl;
      parser.parse(context);
      gasp::blaise_to_torricelly::translator translator(context.module());
      translator.execute(modules);

      cout << "-- TORRICELLY -------------------------------------------------------" << endl;
      for(auto module : modules)
         torricelly_output << module;

      return EXIT_SUCCESS;
   }
   catch (sanelli::tokenizer_error &error)
   {
      cerr << "TOKENIZER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      cerr << context << endl;

      return EXIT_FAILURE;
   }
   catch (sanelli::parser_error &error)
   {
      cerr << "PARSER_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch (gasp::blaise::ast::blaise_ast_error &error)
   {
      cerr << "BLAISE_AST_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch (gasp::common::gasp_internal_error& error){
       cerr << "INTERNAL_ERROR: " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch (gasp::blaise_to_torricelly::blaise_to_torricelly_internal_error& error){
       cerr << "BLAISE_TO_TORRICELLI_INTERNAL_ERROR: " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch (gasp::blaise_to_torricelly::blaise_to_torricelly_error &error)
   {
      cerr << "BLAISE_TO_TORRICELLI_ERROR(" << error.line() << "," << error.column() << "): " << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch(std::exception &error) { 
      cerr << "GENERIC_ERROR: "  << error.what() << endl;
      return EXIT_FAILURE;
   }
   catch(...) { 
      cerr << "UNKNOWN ERROR -- No further information available at this stage"  << endl;
      return EXIT_FAILURE;
   }
}