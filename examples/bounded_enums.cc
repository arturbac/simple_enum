#include <simple_enum/simple_enum.hpp>
#include <atomic>
#include <iostream>

// simpliest to use bounded enum declaring first and last

enum struct enum_bounded
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };

enum struct enum_upper_bounded
  {
  v0,
  v1,
  v2,
  v3,
  last = v3
  };

enum struct enum_externaly_bounded
  {
  v0,
  v1,
  v2,
  v3
  };

static void simple_in_enum_bounds()
  {
  // can be evaluated at compile time
  static_assert(simple_enum::enum_name(enum_bounded::v1) == "v1");

  // or at runtime
  auto x0{enum_bounded::v2};
  // enum_bounded has definitions for first and last so compile time is limited to processing meta info for declared
  // range only
  std::cout << simple_enum::enum_name(x0) << "\n";

  // enum_upper_bounded has definitions for last so compile time is limited to processing meta info for range
  // [0..last] range only for upper bounded enum may be sparse enum used with not present first elements including 0
  auto x1{enum_upper_bounded::v2};
  std::cout << simple_enum::enum_name(x1) << "\n";
  }

// lets see example for std::memory_order externally declaring boundary
template<>
struct simple_enum::info<std::memory_order>
  {
  /*
  enum class memory_order : int
    {
    relaxed,
    consume,
    acquire,
    release,
    acq_rel,
    seq_cst
    };
  */
  static constexpr auto first = std::memory_order::relaxed;
  static constexpr auto last = std::memory_order::seq_cst;
  };

static void memory_order_externaly_bounding()
  {
  auto x1{std::memory_order::release};
  std::cout << simple_enum::enum_name(x1) << "\n";
  }

int main()
  {
  simple_in_enum_bounds();
  memory_order_externaly_bounding();
  return EXIT_SUCCESS;
  }

/*
v2
v2
release
*/
