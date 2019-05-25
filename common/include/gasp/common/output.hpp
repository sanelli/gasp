#pragma once

#include <ostream>
#include <vector>
#include <set>

namespace gasp::common
{

template <typename TType>
std::ostream & operator<<(std::ostream &os, const std::vector<TType> &sequence)
{
   for (auto it = sequence.begin(); it != sequence.end(); ++it)
      os << *it << " ";
   return os;
}

template <typename TType>
std::ostream & operator<<(std::ostream &os, const std::set<TType> &sequence)
{
   for (auto it = sequence.begin(); it != sequence.end(); ++it)
      os << *it << " ";
   return os;
}

} // namespace gasp::common