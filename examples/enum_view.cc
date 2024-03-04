#include <simple_enum/ranges_views.hpp>
#include <ranges>
#include <iostream>

namespace views = std::views;
namespace ranges = std::ranges;

enum struct lorem_ipsum
  {
  eu,
  occaecat,
  dolore,
  excepteur,
  mollit,
  adipiscing,
  sunt,
  ut,
  aliqua
  };

template<>
struct simple_enum::info<lorem_ipsum>
  {
  static constexpr auto first = lorem_ipsum::eu;
  static constexpr auto last = lorem_ipsum::aliqua;
  };

using simple_enum::enum_name;
using simple_enum::enum_view;

int main()
  {
  constexpr auto view_over_lorem_ipsum = enum_view<lorem_ipsum>{} | views::transform(enum_name);
  std::cout << "simple_enum " SIMPLE_ENUM_NAME_VERSION "\n";
  for(auto data: view_over_lorem_ipsum)
    std::cout << " " << data << '\n';
  }

/*
simple_enum 0.5.2
 eu
 occaecat
 dolore
 excepteur
 mollit
 adipiscing
 sunt
 ut
 aliqua
 */
