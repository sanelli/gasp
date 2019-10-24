#pragma once

#include <string>
#include <memory>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter
{

union torricelly_activation_record_variable_union {
   int _integer;
   char _char;
   bool _boolean;
   float _float;
   double _double;

   torricelly_activation_record_variable_union();
   ~torricelly_activation_record_variable_union();
};

enum class torricelly_activation_record_local_type
{
   UNDEFINED,
   VOID,
   INTEGER,
   CHAR,
   BOOLEAN,
   FLOAT,
   DOUBLE,
   POINTER
};

enum class torricelly_activation_record_variable_underlying_type
{
   UNDEFINED,
   STRING_LITERAL,
   ARRAY
};

enum class torricelly_activation_record_variable_array_underlying_type
{
   UNDEFINED,
   INTEGER,
   CHAR,
   BOOLEAN,
   FLOAT,
   DOUBLE,
};

std::string to_string(torricelly_activation_record_local_type type);
std::string to_string(torricelly_activation_record_variable_underlying_type type);
std::string to_string(torricelly_activation_record_variable_array_underlying_type type);

torricelly_activation_record_variable_array_underlying_type to_underlying_type(std::shared_ptr<torricelly_type> type);

class torricelly_activation_record_variable_multidimensional_array
{
   std::vector<unsigned int> _dimensions;
   std::vector<torricelly_activation_record_variable_union> _values;
   torricelly_activation_record_variable_array_underlying_type _underlying_type;

   void copy_from(const torricelly_activation_record_variable_multidimensional_array &other);

public:
   torricelly_activation_record_variable_multidimensional_array(const std::vector<unsigned int>& dimensions, torricelly_activation_record_variable_array_underlying_type underlying_type);
   torricelly_activation_record_variable_multidimensional_array(const torricelly_activation_record_variable_multidimensional_array &other);
   torricelly_activation_record_variable_multidimensional_array &operator=(const torricelly_activation_record_variable_multidimensional_array &other);
   ~torricelly_activation_record_variable_multidimensional_array();

   torricelly_activation_record_variable_array_underlying_type underlying_type() const;

   unsigned int dimensions() const;
   unsigned int dimension(unsigned int dim) const;

   unsigned int index(const std::vector<unsigned int> &indexes) const;

   unsigned int size() const;
   void set(unsigned int index, torricelly_activation_record_variable_union value);
   torricelly_activation_record_variable_union get(unsigned int index) const;
};

class torricelly_activation_record_variable
{
   torricelly_activation_record_local_type _type;
   torricelly_activation_record_variable_union _value;
   std::shared_ptr<void> _pointer;
   torricelly_activation_record_variable_underlying_type _pointer_unerlying_type;

   torricelly_activation_record_variable(int i);
   torricelly_activation_record_variable(char c);
   torricelly_activation_record_variable(bool b);
   torricelly_activation_record_variable(float f);
   torricelly_activation_record_variable(double d);
   torricelly_activation_record_variable(std::shared_ptr<void> p, torricelly_activation_record_variable_underlying_type underlying_type);

   void copy_value_from(const torricelly_activation_record_variable &other);

public:
   torricelly_activation_record_variable();
   ~torricelly_activation_record_variable();
   torricelly_activation_record_variable(const torricelly_activation_record_variable &other);
   torricelly_activation_record_variable(torricelly_activation_record_variable &&other);
   torricelly_activation_record_variable &operator=(const torricelly_activation_record_variable &other);
   torricelly_activation_record_variable &operator=(torricelly_activation_record_variable &&other);

   torricelly_activation_record_local_type type() const;
   bool match(std::shared_ptr<torricelly::torricelly_type> torricelly_type) const;

   int get_integer() const;
   char get_char() const;
   bool get_boolean() const;
   float get_float() const;
   double get_double() const;
   std::shared_ptr<void> get_pointer() const;
   std::shared_ptr<std::string> get_string_pointer() const;
   std::shared_ptr<torricelly_activation_record_variable_multidimensional_array> get_array_pointer() const;
   torricelly_activation_record_variable_underlying_type get_pointer_underlying_type() const;

   void set_integer(int i);
   void set_char(char c);
   void set_boolean(bool b);
   void set_float(float f);
   void set_double(double d);
   void set_pointer(std::shared_ptr<void> p);

   static torricelly_activation_record_variable make(const torricelly_value &value);
   static torricelly_activation_record_variable make(bool b);
   static torricelly_activation_record_variable make(int i);
   static torricelly_activation_record_variable make(float f);
   static torricelly_activation_record_variable make(double d);
   static torricelly_activation_record_variable make(char c);

   friend sanelli::memory;
};

std::string to_string(const torricelly_activation_record_variable &value);

} // namespace gasp::torricelly::interpreter