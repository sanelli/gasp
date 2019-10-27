#pragma once

#include <string>
#include <ostream>
#include <istream>
#include <vector>

namespace gasp::torricelly
{

class torricelly_text_output
{
   std::ostream &_os;

public:
   torricelly_text_output(std::ostream &os);

   unsigned int version_major() const;
   unsigned int version_minor() const;
   unsigned int version_build() const;

   friend torricelly_text_output &operator<<(torricelly_text_output &os, const char *text);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, const std::string &text);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, int number);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, unsigned int number);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, size_t number);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, bool boolean);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, char character);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, float number);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, double number);
   friend torricelly_text_output &operator<<(torricelly_text_output &os, std::ostream &(*manipualtor)(std::ostream &));
};

torricelly_text_output &operator<<(torricelly_text_output &os, const char *text);
torricelly_text_output &operator<<(torricelly_text_output &os, const std::string &text);
torricelly_text_output &operator<<(torricelly_text_output &os, int number);
torricelly_text_output &operator<<(torricelly_text_output &os, unsigned int number);
torricelly_text_output &operator<<(torricelly_text_output &os, size_t number);
torricelly_text_output &operator<<(torricelly_text_output &os, bool boolean);
torricelly_text_output &operator<<(torricelly_text_output &os, char character);
torricelly_text_output &operator<<(torricelly_text_output &os, float number);
torricelly_text_output &operator<<(torricelly_text_output &os, double number);
torricelly_text_output &operator<<(torricelly_text_output &os, std::ostream &(*manipualtor)(std::ostream &));

class torricelly_binary_output
{
   std::ostream &_os;

public:
   torricelly_binary_output(std::ostream &os);

   int32_t version_major() const;
   int32_t version_minor() const;
   int32_t version_build() const;

   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, const char *text);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, const std::string &text);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, int32_t number);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, int16_t number);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, bool boolean);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, char character);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, float number);
   friend torricelly_binary_output &operator<<(torricelly_binary_output &os, double number);
};

torricelly_binary_output &operator<<(torricelly_binary_output &os, const char *text);
torricelly_binary_output &operator<<(torricelly_binary_output &os, const std::string &text);
torricelly_binary_output &operator<<(torricelly_binary_output &os, int32_t number);
torricelly_binary_output &operator<<(torricelly_binary_output &os, int16_t number);
torricelly_binary_output &operator<<(torricelly_binary_output &os, bool boolean);
torricelly_binary_output &operator<<(torricelly_binary_output &os, char character);
torricelly_binary_output &operator<<(torricelly_binary_output &os, float number);
torricelly_binary_output &operator<<(torricelly_binary_output &os, double number);

class torricelly_binary_input
{
   char *_buffer;
   unsigned int _buffer_size;
   size_t _index;

public:
   torricelly_binary_input(std::istream &is);
   ~torricelly_binary_input();

   int32_t version_major() const;
   int32_t version_minor() const;
   int32_t version_build() const;

   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, std::string &text);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, int32_t &number);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, int16_t &number);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, bool &boolean);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, char &character);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, float &number);
   friend torricelly_binary_input &operator>>(torricelly_binary_input &is, double &number);
};

torricelly_binary_input &operator>>(torricelly_binary_input &is, std::string &text);
torricelly_binary_input &operator>>(torricelly_binary_input &is, int32_t &number);
torricelly_binary_input &operator>>(torricelly_binary_input &is, int16_t &number);
torricelly_binary_input &operator>>(torricelly_binary_input &is, bool &boolean);
torricelly_binary_input &operator>>(torricelly_binary_input &is, char &character);
torricelly_binary_input &operator>>(torricelly_binary_input &is, float &number);
torricelly_binary_input &operator>>(torricelly_binary_input &is, double &number);

} // namespace gasp::torricelly