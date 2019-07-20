#include <catch2/catch.hpp>
#include <fstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/common/output.hpp>
#include <gasp/common/debug.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;

// #define GENERATE_TEST_FOR(filename)                                                                           \
//    GIVEN(filename)                                                                                            \
//    {                                                                                                          \
//       ifstream stream(std::string("../blaise-lang/test/blaise_test_sources/") + filename, std::ifstream::in); \
//       if (stream.is_open())                                                                                   \
//       {                                                                                                       \
//          REQUIRE_NOTHROW(tokenizer.tokenize(stream, context));                                                \
//          REQUIRE_NOTHROW(parser.parse(context));                                                              \
//       }                                                                                                       \
//    }

SCENARIO("Parse testing programs", "[blaise-lang][parser][base]")
{

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;

   const char *filenames[] = {"0000_simple_empty_program",
                              "0001_variable_declaration",
                              "0002_operations_on_integers",
                              "0003_operations_on_booleans",
                              "0004_declarations_of_arrays",
                              "0005_operations_on_arrays"
                              };

   for (const auto filename : filenames)
      GIVEN(filename)
      {
         ifstream stream(std::string("../blaise-lang/test/blaise_test_sources/") + filename + ".blaise", std::ifstream::in);
         if (stream.is_open())
         {
            REQUIRE_NOTHROW(tokenizer.tokenize(stream, context));
            REQUIRE_NOTHROW(parser.parse(context));
         }
         else
         {
            throw std::runtime_error(std::string("cannot load file ") + filename);
         }
      }

   // GENERATE_TEST_FOR("0000_simple_empty_program.blaise");
   // GENERATE_TEST_FOR("0001_variable_declaration.blaise");
}
