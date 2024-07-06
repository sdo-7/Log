#pragma once

#include <sstream>

template <typename T1, typename... Types>
static void pushArg (std::stringstream &ss, T1 &a1, Types&... args) {
  ss << a1;
  if constexpr (sizeof...(args))
    pushArg(ss, args...);
}

template <typename... Types>
static std::string toString (Types&... args) {
  std::stringstream ss;
  if constexpr (sizeof...(args))
    pushArg(ss, args...);
  return ss.str();
}
