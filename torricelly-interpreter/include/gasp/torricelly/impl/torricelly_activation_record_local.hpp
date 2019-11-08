#pragma once

#include <string>
#include <memory>
#include <vector>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter
{

union torricelly_activation_record_local_union {
   int8_t _byte;
   int16_t _short;
   int32_t _integer;
   int64_t _long;
   unsigned char _char;
   bool _boolean;
   float _float;
   double _double;

   torricelly_activation_record_local_union();
   ~torricelly_activation_record_local_union();
};

enum class torricelly_activation_record_local_type
{
   UNDEFINED,
   VOID,
   BYTE,
   SHORT,
   INTEGER,
   LONG,
   CHAR,
   BOOLEAN,
   FLOAT,
   DOUBLE,
   POINTER
};

enum class torricelly_activation_record_local_underlying_type
{
   UNDEFINED,
   LITERAL_STRING,
   ARRAY
};

enum class torricelly_activation_record_local_array_underlying_type
{
   UNDEFINED,
   BYTE,
   SHORT,
   INTEGER,
   LONG,
   CHAR,
   BOOLEAN,
   FLOAT,
   DOUBLE,
};

std::string to_string(torricelly_activation_record_local_type type);
std::string to_string(torricelly_activation_record_local_underlying_type type);
std::string to_string(torricelly_activation_record_local_array_underlying_type type);

torricelly_activation_record_local_array_underlying_type to_underlying_type(std::shared_ptr<torricelly_type> type);
torricelly_activation_record_local_array_underlying_type to_underlying_type(torricelly_activation_record_local_type type);

class torricelly_activation_record_local_multidimensional_array
{
   std::vector<unsigned int> _dimensions;
   std::vector<torricelly_activation_record_local_union> _values;
   torricelly_activation_record_local_array_underlying_type _underlying_type;

   void copy_from(const torricelly_activation_record_local_multidimensional_array &other);

public:
   torricelly_activation_record_local_multidimensional_array(const std::vector<unsigned int> &dimensions, torricelly_activation_record_local_array_underlying_type underlying_type);
   torricelly_activation_record_local_multidimensional_array(const torricelly_activation_record_local_multidimensional_array &other);
   torricelly_activation_record_local_multidimensional_array &operator=(const torricelly_activation_record_local_multidimensional_array &other);
   ~torricelly_activation_record_local_multidimensional_array();

   torricelly_activation_record_local_array_underlying_type underlying_type() const;

   unsigned int dimensions() const;
   unsigned int dimension(unsigned int dim) const;

   unsigned int index(const std::vector<unsigned int> &indexes) const;

   unsigned int size() const;
   void set(unsigned int index, torricelly_activation_record_local_union value);
   torricelly_activation_record_local_union get(unsigned int index) const;
};

class torricelly_activation_record_local
{
   torricelly_activation_record_local_type _type;
   torricelly_activation_record_local_union _value;
   std::shared_ptr<void> _pointer;
   torricelly_activation_record_local_underlying_type _pointer_unerlying_type;

   torricelly_activation_record_local(int8_t y);
   torricelly_activation_record_local(int16_t s);
   torricelly_activation_record_local(int32_t i);
   torricelly_activation_record_local(int64_t l);
   torricelly_activation_record_local(unsigned char c);
   torricelly_activation_record_local(bool b);
   torricelly_activation_record_local(float f);
   torricelly_activation_record_local(double d);
   torricelly_activation_record_local(std::shared_ptr<void> p, torricelly_activation_record_local_underlying_type underlying_type);

   void copy_value_from(const torricelly_activation_record_local &other);

public:
   torricelly_activation_record_local();
   ~torricelly_activation_record_local();
   torricelly_activation_record_local(const torricelly_activation_record_local &other);
   torricelly_activation_record_local(torricelly_activation_record_local &&other);
   torricelly_activation_record_local &operator=(const torricelly_activation_record_local &other);
   torricelly_activation_record_local &operator=(torricelly_activation_record_local &&other);

   torricelly_activation_record_local_type type() const;
   bool match(std::shared_ptr<torricelly::torricelly_type> torricelly_type) const;

   int8_t get_byte() const;
   int16_t get_short() const;
   int32_t get_integer() const;
   int64_t get_long() const;
   unsigned char get_char() const;
   bool get_boolean() const;
   float get_float() const;
   double get_double() const;
   std::shared_ptr<void> get_pointer() const;
   std::shared_ptr<std::string> get_string_pointer() const;
   std::shared_ptr<torricelly_activation_record_local_multidimensional_array> get_array_pointer() const;
   torricelly_activation_record_local_underlying_type get_pointer_underlying_type() const;

   void set_byte(int8_t y);
   void set_short(int16_t s);
   void set_integer(int32_t i);
   void set_long(int64_t l);
   void set_char(unsigned char c);
   void set_boolean(bool b);
   void set_float(float f);
   void set_double(double d);
   void set_pointer(std::shared_ptr<void> p);

   static torricelly_activation_record_local make(const torricelly_value &value);
   static torricelly_activation_record_local make(bool b);
   static torricelly_activation_record_local make(int8_t y);
   static torricelly_activation_record_local make(int16_t s);
   static torricelly_activation_record_local make(int32_t i);
   static torricelly_activation_record_local make(int64_t l);
   static torricelly_activation_record_local make(float f);
   static torricelly_activation_record_local make(double d);
   static torricelly_activation_record_local make(unsigned char c);
   static torricelly_activation_record_local make(const std::vector<unsigned int> &dimensions, torricelly_activation_record_local initial_value);

   friend sanelli::memory;
};

std::string to_string(const torricelly_activation_record_local &value);

} // namespace gasp::torricelly::interpreter