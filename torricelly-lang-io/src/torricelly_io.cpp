#include <ostream>
#include <string>
#include <sstream>
#include <istream>
#include <cmath>

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
   auto len = (int32_t)strlen(text);
   char byte_len[sizeof(int32_t)];
   sanelli::binary_converter::to_binary(len, byte_len);
   os._os.write(byte_len, sizeof(int32_t));
   os._os.write(text, len);
   return os;
}
torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const std::string &text)
{
   auto len = (int32_t)text.size();
   char byte_len[sizeof(int32_t)];
   sanelli::binary_converter::to_binary(len, byte_len);
   os._os.write(byte_len, sizeof(int32_t));
   auto bytes = text.c_str();
   os._os.write(bytes, len);
   return os;
}

template <typename TStream, typename TType>
inline void __write_binary(TStream &stream, TType value)
{
   char bytes[sizeof(TType)];
   sanelli::binary_converter::to_binary(value, bytes);
   stream.write(bytes, sizeof(TType));
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, int32_t number)
{
   __write_binary(os._os, number);
   return os;
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, int16_t number)
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

torricelly_binary_input::torricelly_binary_input(std::istream &is)
    : _buffer(nullptr), _index(0), _buffer_size(0)
{
   is.seekg(0, is.end);
   _buffer_size = is.tellg();
   is.seekg(0, is.beg);

   _buffer = new char[_buffer_size];
   is.read(_buffer, _buffer_size);
}

torricelly_binary_input::~torricelly_binary_input()
{
   if (_buffer != nullptr)
      delete[] _buffer;
}

int32_t torricelly_binary_input::version_major() const { return MAJOR_VERSION; }
int32_t torricelly_binary_input::version_minor() const { return MINOR_VERSION; }
int32_t torricelly_binary_input::version_build() const { return BUILD_VERSION; }

void torricelly_binary_input::debug() const
{
   std::cout << "------DEBUG----" << std::endl;
   std::cout << _index << std::endl;
   std::cout << _buffer_size << std::endl;
   for (int index = _index; index < std::min((int)_buffer_size, (int)(_index + 8)); ++index)
      std::cout << std::hex << (int)_buffer[index] << std::dec << std::endl;
   std::cout << "------END-DEBUG----" << std::endl;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, std::string &text)
{
   int32_t string_length;
   is >> string_length;

   std::stringstream stream;
   for (auto i = 0U; i < string_length; ++i)
   {
      char c;
      is >> c;
      stream << c;
   }
   text = stream.str();
   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, int32_t &number)
{
   number = sanelli::binary_converter::from_binary<int32_t>(is._buffer, &is._index);
   return is;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, int16_t &number)
{
   number = sanelli::binary_converter::from_binary<int16_t>(is._buffer, &is._index);
   return is;
}
torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, bool &boolean)
{
   boolean = sanelli::binary_converter::from_binary<bool>(is._buffer, &is._index);
   return is;
}
torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, char &character)
{
   character = sanelli::binary_converter::from_binary<char>(is._buffer, &is._index);
   return is;
}
torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, float &number)
{
   number = sanelli::binary_converter::from_binary<float>(is._buffer, &is._index);
   return is;
}
torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, double &number)
{
   number = sanelli::binary_converter::from_binary<double>(is._buffer, &is._index);
   return is;
}