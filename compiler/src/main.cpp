#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>

#include <gasp/blaise/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/output.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;

int main(int argc, char *argv[])
{
   cout << "GASP - by Stefano Anelli." << endl;

   blaise_tokenizer tokenizer;
   vector<token<blaise_token>> tokens;

   stringstream program;
   program << R"program(
      program Hello_World;
      begin
      end.
   )program";

   tokenizer.parse(program, tokens);

   cout << tokens << endl;

   return EXIT_SUCCESS;
}