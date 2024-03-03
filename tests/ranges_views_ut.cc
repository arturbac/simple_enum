#include <simple_enum/ranges_views.hpp>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wctad-maybe-unsupported"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wundef"
#include <boost/ut.hpp>
#pragma clang diagnostic pop
#include <ranges>
#include <string_view>
#include <array>

namespace ut = boost::ut;
namespace views = std::views;
namespace ranges = std::ranges;
using ut::eq;
using ut::expect;
using ut::literals::operator""_test;

std::ostream & operator<<(std::ostream & os, simple_enum::enum_concept auto const & value)
  {
  os << simple_enum::enum_name(value);
  return os;
  }

enum struct strong_typed : uint8_t
  {
  v1 = 1,
  v2,
  v3
  };

template<>
struct simple_enum::info<strong_typed>
  {
  static constexpr auto first = strong_typed::v1;
  static constexpr auto last = strong_typed::v3;
  };

enum struct strong_untyped
  {
  v1,
  v2,
  v3
  };

template<>
struct simple_enum::info<strong_untyped>
  {
  static constexpr auto first = strong_untyped::v1;
  static constexpr auto last = strong_untyped::v3;
  };

enum weak_typed_e : uint8_t
  {
  v1,
  v2,
  v3
  };

template<>
struct simple_enum::info<weak_typed_e>
  {
  static constexpr auto first = weak_typed_e::v1;
  static constexpr auto last = weak_typed_e::v3;
  };

namespace subnamespace
  {

enum weak_untyped_3_e
  {
  v1 = 1,
  v2,
  v3
  };
  }

template<>
struct simple_enum::info<subnamespace::weak_untyped_3_e>
  {
  static constexpr auto first = subnamespace::weak_untyped_3_e::v1;
  static constexpr auto last = subnamespace::weak_untyped_3_e::v3;
  };
// Example enumeration for demonstration
enum class my_enum
  {
  a,
  b,
  c
  };

template<>
struct simple_enum::info<my_enum>
  {
  static constexpr auto first = weak_typed_e::v1;
  static constexpr auto last = weak_typed_e::v3;
  };

// Check if the end() is compatible with ranges::end and its iterator satisfies sentinel_for with its own iterator type
static_assert(
  std::sentinel_for<
    decltype(ranges::end(std::declval<simple_enum::enum_view<my_enum>>())),
    decltype(ranges::begin(std::declval<simple_enum::enum_view<my_enum>>()))>,
  "enum_view::end() must return a type that satisfies std::sentinel_for with its iterator type."
);

// Optionally, directly check the sentinel_for between enum_view::iterator and the type returned by enum_view::end()
static_assert(
  std::sentinel_for<
    ranges::sentinel_t<simple_enum::enum_view<my_enum>>,
    ranges::iterator_t<simple_enum::enum_view<my_enum>>>,
  "The type returned by enum_view::end() must be a valid sentinel for its iterator."
);

// Static asserts to check enum_view requirements
static_assert(
  std::input_or_output_iterator<ranges::iterator_t<simple_enum::enum_view<my_enum>>>,
  "enum_view::iterator must satisfy input or output iterator requirements"
);

// If you've specialized ranges::enable_borrowed_range for enum_view, check that as well
static_assert(
  ranges::borrowed_range<simple_enum::enum_view<my_enum>>,
  "enum_view should be a borrowed range if intended for temporary objects usage"
);
static_assert(ranges::range<simple_enum::enum_view<my_enum>>, "enum_view must satisfy ranges::range");

static_assert(ranges::range<simple_enum::enum_view<weak_typed_e>>);

static void view_test()
  {
  using namespace simple_enum;

  "strong_typed enumeration"_test = []
  {
    constexpr auto view = enum_view<strong_typed>{strong_typed::v1, strong_typed::v3};
    auto it = view.begin();

    expect(eq(*it, strong_typed::v1));
    ++it;
    expect(eq(*it, strong_typed::v2));
    ++it;
    expect(eq(*it, strong_typed::v3));
    ++it;
    expect(it == view.end());
  };

  "strong_untyped enumeration"_test = []
  {
    constexpr auto view = enum_view<strong_untyped>{strong_untyped::v1, strong_untyped::v3};
    auto it = view.begin();

    expect(eq(*it, strong_untyped::v1));
    ++it;
    expect(eq(*it, strong_untyped::v2));
    ++it;
    expect(eq(*it, strong_untyped::v3));
    ++it;
    expect(it == view.end());
  };

  "weak_typed_e enumeration"_test = []
  {
    constexpr auto view = enum_view<weak_typed_e>{weak_typed_e::v1, weak_typed_e::v3};
    auto it = view.begin();

    expect(eq(*it, weak_typed_e::v1));
    ++it;
    expect(eq(*it, weak_typed_e::v2));
    ++it;
    expect(eq(*it, weak_typed_e::v3));
    ++it;
    expect(it == view.end());
  };

  "subnamespace::weak_untyped_3_e enumeration"_test = []
  {
    constexpr auto view = enum_view<subnamespace::weak_untyped_3_e>{
      subnamespace::weak_untyped_3_e::v1, subnamespace::weak_untyped_3_e::v3
    };
    auto it = view.begin();

    expect(eq(*it, subnamespace::weak_untyped_3_e::v1));
    ++it;
    expect(eq(*it, subnamespace::weak_untyped_3_e::v2));
    ++it;
    expect(eq(*it, subnamespace::weak_untyped_3_e::v3));
    ++it;
    expect(it == view.end());
  };
  }

static void ranges_transform_test()
  {
  using simple_enum::enum_name;
  using simple_enum::enum_view;
  using std::array;
  using std::back_inserter;
  using std::string_view;
  using std::vector;
  using namespace std::string_view_literals;

  "strong_typed enumeration transform"_test = []
  {
    {
    constexpr auto view = enum_view<strong_typed>{strong_typed::v1, strong_typed::v2} | views::transform(enum_name);
    static constexpr array expected{"v1"sv, "v2"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }
    {
    constexpr auto view = enum_view<strong_typed>{} | views::transform(enum_name);
    static constexpr array expected{"v1"sv, "v2"sv, "v3"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }
  };

  "strong_untyped enumeration transform"_test = []
  {
    {
    constexpr auto view
      = enum_view<strong_untyped>{strong_untyped::v2, strong_untyped::v3} | views::transform(enum_name);
    static constexpr array expected{"v2"sv, "v3"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }

    {
    constexpr auto view = enum_view<strong_untyped>{} | views::transform(enum_name);
    static constexpr array expected{"v1"sv, "v2"sv, "v3"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }
  };

  "weak_typed_e enumeration transform"_test = []
  {
    {
    constexpr auto view = enum_view<weak_typed_e>{weak_typed_e::v1, weak_typed_e::v1} | views::transform(enum_name);
    static constexpr array expected{"v1"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }

    {
    constexpr auto view = enum_view<weak_typed_e>{} | views::transform(enum_name);
    static constexpr array expected{"v1"sv, "v2"sv, "v3"sv};
    expect(ranges::equal(view, expected));
    vector<string_view> results;
    ranges::copy(view, back_inserter(results));
    expect(ranges::equal(results, expected));
    }
  };

  "subnamespace::weak_untyped_3_e enumeration transform"_test = []
  {
    using subnamespace::weak_untyped_3_e;
      {
      constexpr auto view
        = enum_view<weak_untyped_3_e>{weak_untyped_3_e::v2, weak_untyped_3_e::v2} | views::transform(enum_name);
      static constexpr array expected{"v2"sv};
      expect(ranges::equal(view, expected));
      vector<string_view> results;
      ranges::copy(view, back_inserter(results));
      expect(ranges::equal(results, expected));
      }

      {
      constexpr auto view = enum_view<weak_untyped_3_e>{} | views::transform(enum_name);
      static constexpr array expected{"v1"sv, "v2"sv, "v3"sv};
      expect(ranges::equal(view, expected));
      vector<string_view> results;
      ranges::copy(view, back_inserter(results));
      expect(ranges::equal(results, expected));
      }
  };
  }

int main()
  {
  view_test();
  ranges_transform_test();
  }
