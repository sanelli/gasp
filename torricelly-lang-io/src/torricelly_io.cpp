#include <ostream>
#include <string>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output::torricelly_text_output(std::ostream& os) : _os(os) { }

unsigned int torricelly_text_output::version_major() const { return 0; }
unsigned int torricelly_text_output::version_minor() const { return 0; }
unsigned int torricelly_text_output::version_build() const { return 1; }

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, const char* text){
   os._os << text;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, const std::string& text) {
   os._os << text;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, int number){
   os._os << number;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, unsigned int number) {
   os._os << number;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, size_t number){
   os._os << number;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, bool boolean){
   os._os << boolean;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, char character){
   os._os << character;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, float number){
   os._os << number;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, double number){
   os._os << number;
   return os;
}
torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, std::ostream&(*manipualtor)(std::ostream&)){
   manipualtor(os._os);
   return os;
}
