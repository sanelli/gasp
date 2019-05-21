#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

#include <gasp/common/string.hpp>

// https://stackoverflow.com/a/217605
int gasp::common::ltrim(std::string &s)
{
   auto itBegin = s.begin();
   auto itEnd = std::find_if(s.begin(), s.end(), [](int ch) {
      return !std::isspace(ch);
   });
   s.erase(itBegin, itEnd);
   return itEnd - itBegin;
}
