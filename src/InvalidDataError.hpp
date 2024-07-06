#pragma once

#include <exception>
#include "toString.hpp"

class InvalidDataError
: public std::runtime_error {
public:
  template <typename... Types>
  inline InvalidDataError (const Types&... args)
  : std::runtime_error (toString(args...)) {
  }
} ;
