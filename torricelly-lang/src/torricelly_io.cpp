#include <ostream>
#include <string>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output::torricelly_text_output(std::ostream& os) : _os(os) { }

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