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
   stream << arg;
}

template <typename TArg, typename... TArgs>
inline void __make_string(std::stringstream &stream, const TArg &arg, const TArgs &... args)
{
   stream << arg;
   __make_string(stream, args...);
}

template <typename... TArgs>
std::string make_string(const TArgs &... args)
{
   std::stringstream stream;
   __make_string(stream, args...);
   return stream.str();
}

} // namespace gasp::common