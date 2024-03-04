#pragma once
#include "simple_enum/simple_enum.hpp"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wctad-maybe-unsupported"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wundef"
#include <boost/ut.hpp>
#pragma clang diagnostic pop
#include "enum_definitions.h"

namespace ut = boost::ut;
using ut::eq;
using ut::expect;
using ut::fatal;
using ut::literals::operator""_test;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wheader-hygiene"
using namespace std::string_view_literals;
#pragma clang diagnostic pop

std::ostream & operator<<(std::ostream & os, simple_enum::enum_concept auto const & value)
  {
  os << simple_enum::enum_name(value);
  return os;
  }
