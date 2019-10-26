#include <ostream>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

const unsigned int MAJOR_VERSION = 0;
const unsigned int MINOR_VERSION = 0;
const unsigned int BUILD_VERSION = 1;

torricelly_text_output::torricelly_text_output(std::ostream &os) : _os(os) {}

unsigned int torricelly_text_output::version_major() const { return MAJOR_VERSION; }
unsigned int torricelly_text_output::version_minor() const { return MINOR_VERSION; }
unsigned int torricelly_text_output::version_build() const { return BUILD_VERSION; }

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const char *text)
{
   os._os << text;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const std::string &text)
{
   os._os << text;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, int number)
{
   os._os << number;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, unsigned int number)
{
   os._os << number;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, size_t number)
{
   os._os << number;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, bool boolean)
{
   os._os << boolean;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, char character)
{
   os._os << character;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, float number)
{
   os._os << number;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, double number)
{
   os._os << number;
   return os;
}
torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, std::ostream &(*manipualtor)(std::ostream &))
{
   manipualtor(os._os);
   return os;
}

torricelly_binary_output::torricelly_binary_output(std::ostream &os) : _os(os) {}

int32_t torricelly_binary_output::version_major() const { return MAJOR_VERSION; }
int32_t torricelly_binary_output::version_minor() const { return MINOR_VERSION; }
int32_t torricelly_binary_output::version_build() const { return BUILD_VERSION; }

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const char *text)
{
   auto len = (int32_t) strlen(text);
   char byte_len[sizeof(int32_t)];
   sanelli::binary_converter::to_binary(len, byte_len);
   os._os.write(byte_len, sizeof(int32_t));
   os._os.write(text, len);
   return os;
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const std::string &text)
{
   auto len = (int32_t) text.size();
   char byte_len[sizeof(int32_t)];
   sanelli::binary_converter::to_binary(len, byte_len);
   os._os.write(byte_len, sizeof(int32_t));
   os._os.write(text.c_str(), len);
   return os;
}

template<typename TStream, typename TType>
inline void __write_binary(TStream& stream, TType value) {
   char bytes[sizeof(TType)];
   sanelli::binary_converter::to_binary(value, bytes);
   stream.write(bytes, sizeof(TType));
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, int32_t number)
{
   __write_binary(os._os, number);
   return os;
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, bool boolean)
{
  __write_binary(os._os, boolean);
   return os;
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, char character)
{
   __write_binary(os._os, character);
   return os;
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, float number)
{
   __write_binary(os._os, number);
   return os;
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, double number)
{
   __write_binary(os._os, number);
   return os;
}