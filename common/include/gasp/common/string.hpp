#pragma once

#include <string>
#include <sstream>
#include <array>

#include <gasp/common/debug.hpp>

namespace gasp::common
{

int ltrim(std::string &s);

template <typename TArg>
inline void __make_string(std::stringstream &stream, const TArg &arg)
{
   GASP_DEBUG("__make_string_FINAL" << std::endl);
   stream << arg;
}

template <typename TArg, typename... TArgs>
inline void __make_string(std::stringstream &stream, const TArg &arg, const TArgs &... args)
{
   GASP_DEBUG("__make_string" << std::endl);

   stream << arg;
   __make_string(stream, args...);
}

template <typename... TArgs>
std::string make_string(const TArgs &... args)
{
   GASP_DEBUG("I WANNA MAKE A STRING" << std::endl);

   std::stringstream stream;
   __make_string(stream, args...);
   return stream.str();
}

} // namespace gasp::common