#pragma once

#include <string>
#include <tuple>
#include <map>
#include <utility>

namespace gasp::common
{

template <typename TTokenType>
class token_provider_constructor
{
protected:
   std::map<TTokenType, std::tuple<std::string, std::string, bool, bool, bool>> _token_values;
   std::vector<TTokenType> _tokens;

   void add_token(TTokenType token, std::string rule, std::string name, bool keep_value = false, bool keep_token = true, bool is_punctuation = false)
   {
      _tokens.push_back(token);
      _token_values.insert(std::make_pair<>(token, std::make_tuple<>(rule, name, keep_value, keep_token, is_punctuation)));
   }

public:
   token_provider_constructor() {}

   typename std::vector<TTokenType>::const_iterator cbegin() const { return _tokens.cbegin(); }
   typename std::vector<TTokenType>::const_iterator cend() const { return _tokens.cend(); }

   std::string rule(TTokenType token) const { return std::get<0>(_token_values.at(token)); }
   std::string name(TTokenType token) const { return std::get<1>(_token_values.at(token)); }
   bool keep_value(TTokenType token) const { return std::get<2>(_token_values.at(token)); }
   bool keep_token(TTokenType token) const { return std::get<3>(_token_values.at(token)); }
   bool is_punctuation(TTokenType token) const { return std::get<4>(_token_values.at(token)); }
};

} // namespace gasp::common