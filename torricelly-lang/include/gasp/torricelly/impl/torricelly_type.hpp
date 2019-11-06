#pragma once

#include <string>
#include <memory>
#include <ostream>
#include <vector>

#include <sanelli/sanelli.hpp>

namespace gasp::torricelly
{

enum class torricelly_type_type : unsigned char
{
   UNDEFINED,
   SYSTEM,
   ARRAY,
   STRUCTURED
};

enum class torricelly_system_type_type : unsigned char
{
   UNDEFINED,
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   BOOLEAN,
   LITERAL_STRING // Used to define literal constants in the code (for instance for calling functions)
};

class torricelly_type
{
   torricelly_type_type _type_type;

protected:
   torricelly_type(torricelly_type_type type_type);

public:
   virtual ~torricelly_type();
   torricelly_type_type type_type() const;
   inline virtual bool equals(std::shared_ptr<torricelly_type> other) const = 0;
};

class torricelly_system_type : public torricelly_type
{
   torricelly_system_type_type _system_type;
   torricelly_system_type(torricelly_system_type_type system_type);

public:
   ~torricelly_system_type() override;
   torricelly_system_type_type system_type() const;
   inline bool equals(std::shared_ptr<torricelly_type> other) const override;

   friend sanelli::memory;
};

class torricelly_array_type : public torricelly_type {
   std::shared_ptr<torricelly_type> _underlying_type;
   std::vector<unsigned int> _dimensions;
   torricelly_array_type(std::shared_ptr<torricelly_type> underlying_type, const std::vector<unsigned int>& dimensions);

public:
   ~torricelly_array_type() override;
   std::shared_ptr<torricelly_type> underlying_type() const;
   unsigned int dimensions() const;
   unsigned int dimension(unsigned int dim) const;
   inline bool equals(std::shared_ptr<torricelly_type> other) const override;

   static unsigned int undefined_dimension();

   friend sanelli::memory;
};

class torricelly_type_utility
{
public:
   static std::shared_ptr<torricelly_system_type> as_system_type(std::shared_ptr<torricelly_type> type);
   static std::shared_ptr<torricelly_array_type> as_array_type(std::shared_ptr<torricelly_type> type);
   static bool is_system_type(std::shared_ptr<torricelly_type> type);
   static bool is_void(std::shared_ptr<torricelly_type> type);
   static bool is_string_literal(std::shared_ptr<torricelly_type> type);
};

std::shared_ptr<torricelly_system_type> make_torricelly_system_type(torricelly_system_type_type system_type);
std::shared_ptr<torricelly_array_type> make_torricelly_array_type(std::shared_ptr<torricelly_type> underlying_type, const std::vector<unsigned int>& dimensions);

std::ostream &operator<<(std::ostream &os, torricelly_type_type type);
std::ostream &operator<<(std::ostream &os, torricelly_system_type_type type);
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<torricelly_type> type);

std::string to_string(torricelly_type_type type);
std::string to_string(torricelly_system_type_type type);
std::string to_string(const std::shared_ptr<torricelly_type> type);

} // namespace gasp::torricelly