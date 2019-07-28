#pragma once

#include <memory>
#include <ostream>
#include <gasp/common/memory.hpp>

namespace gasp::torricelly {

enum class torricelly_type_type : unsigned char {
   UNDEFINED,
   SYSTEM,
   STRUCTURED
};

enum class torricelly_system_type_type : unsigned char {
   UNDEFINED,
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   BOOLEAN,
   STRING_LITERAL // Used to define literal constants in the code (for instance for calling functions)
};

class torricelly_type {
   torricelly_type_type _type_type;
protected:
   torricelly_type(torricelly_type_type type_type);
public:
   virtual ~torricelly_type();
   torricelly_type_type type_type() const;
   inline virtual bool equals(std::shared_ptr<torricelly_type> other) const = 0;
};

class toricelly_system_type : public torricelly_type {
   torricelly_system_type_type _system_type;
   toricelly_system_type(torricelly_system_type_type system_type);
   public:
   ~toricelly_system_type() override;
   torricelly_system_type_type system_type() const;
   inline bool equals(std::shared_ptr<torricelly_type> other) const override;

   friend gasp::common::memory;
};

std::shared_ptr<toricelly_system_type> make_torricelly_system_type(torricelly_system_type_type system_type);
   
}