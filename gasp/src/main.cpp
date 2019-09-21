#include <map>
#include <memory>
#include <stdexcept>

#include <sanelli/sanelli.hpp>

#include <gasp/common/gasp_error.hpp>

#include <gasp/module/gasp_module.hpp>
#include <gasp/module/gasp_module_tokenize.hpp>
#include <gasp/module/gasp_module_compile.hpp>
#include <gasp/module/gasp_module_execute.hpp>

// TODO: Add error codes when I throw errors

void print_available_modules(const std::map<std::string, std::unique_ptr<gasp::module::gasp_module>> &modules)
{
   for (auto it = modules.begin(); it != modules.end(); ++it)
   {
      std::cerr << "   * " << it->first << " : " << it->second->description() << std::endl;
   }
}

int main(int argc, char *argv[])
{
   SANELLI_INSTALL_DEBUGGER("blaise-parser");
   SANELLI_INSTALL_DEBUGGER("sanelli-parser");
   SANELLI_INSTALL_DEBUGGER("blaise-lang");
   SANELLI_INSTALL_DEBUGGER("sanelli-tokenizer");
   SANELLI_INSTALL_DEBUGGER("torricelly-lang");
   SANELLI_INSTALL_DEBUGGER("blaise-to-torricelly");
   SANELLI_INSTALL_DEBUGGER("torricelly-interpreter");

   std::map<std::string, std::unique_ptr<gasp::module::gasp_module>> modules;

   auto tokenize_module = std::make_unique<gasp::module::gasp_module_tokenize>();
   auto compile_module = std::make_unique<gasp::module::gasp_module_compile>();
   auto execute_module = std::make_unique<gasp::module::gasp_module_execute>();
   modules[tokenize_module->name()] = std::move(tokenize_module);
   modules[compile_module->name()] = std::move(compile_module);
   modules[execute_module->name()] = std::move(execute_module);

   if (argc < 2)
   {
      std::cerr << "No module selected. Available modules are: " << std::endl;
      print_available_modules(modules);
      return EXIT_FAILURE;
   }

   auto expected_module = std::string(argv[1]);
   std::unique_ptr<gasp::module::gasp_module> module = nullptr;
   for (auto it = modules.begin(); it != modules.end(); ++it)
   {
      if (it->first == expected_module)
      {
         module = std::move(it->second);
         break;
      }
   }

   // Either I found the module or not I do not need to waste
   // memory with something I will never user.
   modules.clear();

   if (module == nullptr)
   {
      std::cerr << "Cannot find module '" << expected_module << "'. Available modules are:" << std::endl;
      print_available_modules(modules);
      return EXIT_FAILURE;
   }

   try
   {
      auto success = module->run(argc, argv);
      return success ? EXIT_SUCCESS : EXIT_FAILURE;
   }
   catch (const gasp::common::gasp_error &e)
   {
      std::cerr << "Error while executing module '" << module->name() << "': " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch (const std::exception &e)
   {
      std::cerr << "Unexpected error while executing module '" << module->name() << "': " << e.what() << std::endl;
      return EXIT_FAILURE;
   }
   catch (...)
   {
      std::cerr << "An unexpected error occurred. No futher details available." << std::endl;
      return EXIT_FAILURE;
   }

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

   // program << R"___(
   //    program hello_world(p1: integer, p2: float);
   //    function duplicate(i: integer) : integer
   //    begin
   //       duplicate := 2 * i;
   //    end;
   //    function abs(i: integer) : integer
   //    begin
   //       abs := if i < 0 then -i else i;
   //    end;
   //    function abs(f: float) : float
   //    begin
   //       abs := if f < 0 then -f else f;
   //    end;
   //    function abs(d: double) : double
   //    begin
   //       abs := if d < 0 then -d else d;
   //    end;
   //    function average(a: float, b: float, c: float) : float
   //    begin
   //       average := (a + b + c) / 3;
   //    end;
   //    var
   //       i,j,k: integer;
   //       d: double;
   //       f1,f2: float;
   //       b: boolean;
   //    begin
   //       i := 0;
   //       j := 1;
   //       k := duplicate(j);
   //       b := true;
   //       while b begin
   //          b := false;
   //       end
   //       repeat begin
   //        b:= true;
   //       end until b;
   //       do begin
   //         b := false;
   //       end while b;
   //       if b then
   //       begin
   //         f1 := 2;
   //       end else begin
   //         f1 := 1;
   //       end
   //    end.
   // )___";
}