#include "simple_enum/simple_enum.hpp"
#include <boost/ut.hpp>
#include <atomic>

using boost::ut::operator""_test;
namespace ut = boost::ut;

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

static ut::suite<"simple in enum bounds"> s0 = []
{
  "simple in enum bounds"_test = []
  {
    // can be evaluated at compile time
    static_assert(simple_enum::enum_name(enum_bounded::v2) == "v2");

    // or at runtime
    auto x0{enum_bounded::v2};
    // enum_bounded has definitions for first and last so compile time is limited to processing meta info for declared
    // range only
    ut::expect(simple_enum::enum_name(x0) == "v2");

    // enum_upper_bounded has definitions for last so compile time is limited to processing meta info for range
    // [0..last] range only for upper bounded enum may be sparse enum used with not present first elements including 0
    auto x1{enum_upper_bounded::v2};
    ut::expect(simple_enum::enum_name(x1) == "v2");
  };
};

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

static ut::suite<"std::memory_order externaly bounding"> s1 = []
{
  auto x1{std::memory_order::release};
  ut::expect(simple_enum::enum_name(x1) == "release");
};

int main() { return EXIT_SUCCESS; }
