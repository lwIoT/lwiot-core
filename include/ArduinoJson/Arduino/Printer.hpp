// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Internals/JsonFloat.hpp"
#include "../Internals/JsonInteger.hpp"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER <= 1800
// snprintf has been added in Visual Studio 2015
#define ARDUINOJSON_SNPRINTF _snprintf
#else
#define ARDUINOJSON_SNPRINTF snprintf
#endif

// This class reproduces Arduino's Print class
namespace lwiot
{
class Printer {
 public:
  explicit Printer();
  virtual ~Printer() = default;

  virtual size_t write(uint8_t) = 0;

  size_t print(const char* s);
  size_t print(ArduinoJson::Internals::JsonFloat value, int digits = 2);
  size_t print(ArduinoJson::Internals::JsonInteger value);
  size_t println();
};
}

typedef lwiot::Printer Print;
