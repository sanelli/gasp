#include <cstdlib>
#include <iostream>

#include <vector>
#include <gasp/tokenizer.hpp>

int main(int argc, char *argv[])
{
   std::cout << "GASP - by Stefano Anelli." << std::endl;

   gasp::common::tokenizer<int> tokenizer;
   tokenizer.add(1, "program", false);
   tokenizer.add(2, "[a-zA-Z_][a-zA-z0-9_]*", true);

   std::vector<gasp::common::token<int>> tokens;
   tokenizer.parse("program hello;", tokens);

   return EXIT_SUCCESS;
}