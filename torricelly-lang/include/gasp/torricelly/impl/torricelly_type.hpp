#pragma once

#include <string>
#include <memory>
#include <ostream>

#include <sanelli/sanelli.hpp>

namespace gasp::torricelly
{

enum class torricelly_type_type : unsigned char
{
   UNDEFINED,
   SYSTEM,
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
   STRING_LITERAL // Used to define literal constants in the code (for instance for calling functions)
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

class torricelly_type_utility
{
public:
   static std::shared_ptr<torricelly_system_type> as_system_type(std::shared_ptr<torricelly_type> type);
   static bool is_void(std::shared_ptr<torricelly_type> type);
};

std::shared_ptr<torricelly_system_type> make_torricelly_system_type(torricelly_system_type_type system_type);

std::ostream &operator<<(std::ostream &os, torricelly_type_type type);
std::ostream &operator<<(std::ostream &os, torricelly_system_type_type type);
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<const torricelly_type> type);

std::string to_string(torricelly_type_type type);
std::string to_string(torricelly_system_type_type type);
std::string to_string(const std::shared_ptr<const torricelly_type> type);

} // namespace gasp::torricelly