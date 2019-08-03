#pragma once

#include <string>
#include <ostream>

namespace gasp::torricelly {

class torricelly_text_output {
   std::ostream& _os;
public:
   torricelly_text_output(std::ostream& os);

   unsigned int version_major() const;
   unsigned int version_minor() const;
   unsigned int version_build() const;

   friend torricelly_text_output& operator<<(torricelly_text_output& os, const char* text);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, const std::string& text);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, int number);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, unsigned int number);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, size_t number);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, bool boolean);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, char character);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, float number);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, double number);
   friend torricelly_text_output& operator<<(torricelly_text_output& os, std::ostream&(*manipualtor)(std::ostream&));
};

torricelly_text_output& operator<<(torricelly_text_output& os, const char* text);
torricelly_text_output& operator<<(torricelly_text_output& os, const std::string& text);
torricelly_text_output& operator<<(torricelly_text_output& os, int number);
torricelly_text_output& operator<<(torricelly_text_output& os, unsigned int number);
torricelly_text_output& operator<<(torricelly_text_output& os, size_t number);
torricelly_text_output& operator<<(torricelly_text_output& os, bool boolean);
torricelly_text_output& operator<<(torricelly_text_output& os, char character);
torricelly_text_output& operator<<(torricelly_text_output& os, float number);
torricelly_text_output& operator<<(torricelly_text_output& os, double number);
torricelly_text_output& operator<<(torricelly_text_output& os, std::ostream&(*manipualtor)(std::ostream&));
}