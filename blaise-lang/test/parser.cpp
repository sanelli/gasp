#include <catch2/catch.hpp>
#include <fstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/language.hpp>
#include <gasp/common/output.hpp>
#include <gasp/common/debug.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;

SCENARIO("Parse testing programs", "[blaise-lang][parser][base]") {

   blaise_tokenizer tokenizer;
   blaise_parser parser;
   blaise_parser_context context;

    GIVEN("simple_empty_program.blaise") {
       ifstream stream("simple_empty_program.blaise", std::ifstream::in);
       if(stream.is_open()){
         REQUIRE_NOTHROW(tokenizer.tokenize(stream, context));
         REQUIRE_NOTHROW(parser.parse(context));
       }
    }
}
