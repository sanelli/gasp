#include <cstdlib>
#include <iostream>
#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/output.hpp>

using namespace std;
using namespace gasp::common;

int main(int argc, char *argv[])
{
   cout << "GASP - by Stefano Anelli." << endl;

   tokenizer<int> tokenizer;
   tokenizer.add(1, "program", false);
   tokenizer.add(3, ";", false);
   tokenizer.add(2, "[a-zA-Z_][a-zA-z0-9_]*", true);

   vector<token<int>> tokens;
   tokenizer.parse(1, "program hello;", tokens);

   cout << tokens << endl;

   return EXIT_SUCCESS;
}