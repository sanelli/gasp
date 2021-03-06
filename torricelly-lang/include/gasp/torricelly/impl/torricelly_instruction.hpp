#pragma once

#include <memory>

#include <gasp/torricelly/impl/torricelly_instruction_code.hpp>

namespace gasp::torricelly {

/**
 * @brief 
 * 
 */
enum class torricelly_inst_ref_type {
   UNDEFINED,
   /**
    * @brief Refers to a module field
    * 
    */
   MODULE, 
   /**
    * @brief Refers to a subroutine function
    * 
    */
   SUBROUTINE,
   /**
    * @brief Refers to a label ID
    * 
    */
   LABEL
};

class torricelly_instruction {
   torricelly_inst_code _code;
   unsigned int _parameter_reference;
   torricelly_inst_ref_type _ref_type;
   unsigned int _label;
   torricelly_instruction(const torricelly_inst_code code, const unsigned int parameter_reference, const torricelly_inst_ref_type ref_type);
   torricelly_instruction(const torricelly_inst_code code);
public:
   torricelly_instruction(const torricelly_instruction& other);
   torricelly_instruction(torricelly_instruction&& other);
   torricelly_instruction& operator=(torricelly_instruction&& other);
   torricelly_instruction& operator=(const torricelly_instruction& other);

   ~torricelly_instruction();

   torricelly_inst_code code() const;
   bool has_parameter_reference() const;
   unsigned int parameter_reference() const;
   torricelly_inst_ref_type ref_type() const;
   void set_parameter_reference(unsigned int parameter_reference, torricelly_inst_ref_type ref_type);
   bool has_label() const;
   unsigned int label() const;
   void set_label(unsigned int label);

   void validate() const;

   static torricelly_instruction make(const torricelly_inst_code code, const unsigned int parameter_reference, const torricelly_inst_ref_type ref_type);
   static torricelly_instruction make(const torricelly_inst_code code);

};

std::string to_string(torricelly_inst_ref_type type);
std::string to_string(torricelly_instruction instruction);

}