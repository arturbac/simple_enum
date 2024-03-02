#include "simple_enum/simple_enum.hpp"
#include <boost/ut.hpp>
enum struct enum_bounded
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };
static_assert(simple_enum::detail::bounds<enum_bounded>::first == enum_bounded::v1);
static_assert(simple_enum::detail::bounds<enum_bounded>::last == enum_bounded::last);

enum struct enum_upper_bounded
  {
  v1 = 1,
  v2,
  v3,
  last = v3
  };
static_assert(simple_enum::detail::bounds<enum_upper_bounded>::first == static_cast<enum_upper_bounded>(0));
static_assert(simple_enum::detail::bounds<enum_upper_bounded>::last == enum_upper_bounded::last);

enum struct enum_lower_bounded
  {
  v1 = 9,
  v2,
  first = v1
  };
static_assert(simple_enum::detail::bounds<enum_lower_bounded>::first == enum_lower_bounded::v1);
static_assert(
  simple_enum::detail::bounds<enum_lower_bounded>::last
  == static_cast<enum_lower_bounded>(simple_enum::default_unbounded_uuper_range)
);

enum struct enum_unbounded
  {
  v1,
  v2,
  v3,
  vx,
  v5,
  v6a
  };

enum struct enum_unbounded_sparse
  {
  v1 = 9,
  v2
  };
static_assert(simple_enum::detail::bounds<enum_unbounded_sparse>::first == static_cast<enum_unbounded_sparse>(0));
static_assert(
  simple_enum::detail::bounds<enum_unbounded_sparse>::last
  == static_cast<enum_unbounded_sparse>(simple_enum::default_unbounded_uuper_range)
);

enum weak_global_untyped_e
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };

// check for external declarations
enum struct global_untyped_externaly_e
  {
  v1 = 1,
  v2,
  v3,
  };

template<>
struct simple_enum::info<global_untyped_externaly_e>
  {
  static constexpr auto first = global_untyped_externaly_e::v1;
  static constexpr auto last = global_untyped_externaly_e::v3;
  };

static_assert(simple_enum::detail::bounds<global_untyped_externaly_e>::first_index == 1);
static_assert(simple_enum::detail::bounds<global_untyped_externaly_e>::last_index == 3);

namespace se
  {
template<auto enumeration>
constexpr auto diag_b(n & res) noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#else
  char const * const func{std::source_location::current().function_name()};
#endif
  printf("%s\n", func);
  return std::string_view{func}.size();
  }

template<auto enumeration>
constexpr void diag_e(n & res, s enum_beg) noexcept
  {
#if defined(__clang__) || defined(__GNUC__)
  char const * const func{__PRETTY_FUNCTION__};
#else
  char const * const func{std::source_location::current().function_name()};
#endif
  printf("%s\n", func);
  }
  }  // namespace se

namespace simple_enum
  {

namespace ut = boost::ut;
enum struct strong_typed : uint8_t
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };
enum struct strong_untyped
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };

enum weak_typed_e : uint8_t
  {
  v1 = 1,
  v2,
  v3,
  first = v1,
  last = v3
  };

namespace test
  {
  enum struct strong_typed_2_e : int16_t
    {
    v1 = 1,
    v2,
    v3,
    first = v1,
    last = v3
    };

  enum struct strong_untyped_2_e
    {
    v1 = 1,
    v2,
    v3,
    first = v1,
    last = v3
    };

  namespace subnamespace
    {
    enum struct example_3_e : uint8_t
      {
      v1 = 1,
      v2,
      v3,
      first = v1,
      last = v3
      };

    enum weak_untyped_3_e
      {
      v1 = 1,
      v2,
      v3,
      first = v1,
      last = v3
      };

    namespace detail
      {
      namespace
        {
        enum struct example_4_e : uint8_t
          {
          v1 = 1,
          v2,
          v3,
          first = v1,
          last = v3
          };
        }
      }  // namespace detail

    inline namespace v2_0
      {
      namespace
        {
        enum struct example_5_e : uint8_t
          {
          v1 = 1,
          v2,
          v3,
          first = v1,
          last = v3
          };

        enum weak_typed_5_e : uint8_t
          {
          v1 = 1,
          v2,
          v3,
          first = v1,
          last = v3
          };
        }  // namespace
      }    // namespace v2_0
    }      // namespace subnamespace
  }        // namespace test

enum struct one_elem_untyped
  {
  v1 = 1,
  first = v1,
  last = v1
  };
enum struct sparse_untyped
  {
  v1 = 1,
  v3 = 3,
  first = v1,
  last = v3
  };
enum struct sparse_offseted_untyped
  {
  unknown = -1,
  v1 = 1,
  v3 = 3,
  first = unknown,  // simulate counting below the range
  last = v3
  };

template<auto enumeration>
constexpr auto se_view() noexcept -> std::string_view
  {
  using enum_type = std::remove_cvref_t<decltype(enumeration)>;
  meta_name value{};
  size_t beg{se::b<enum_type::first>(value)};
  se::e<enumeration>(value, beg);
  return std::string_view{value.data, value.size};
  }

static ut::suite<"simple_enum"> _ = []
{
  // ut::expect(enum_name(enum_unbounded_sparse::v1) == "v1");
  {
  constexpr auto v{static_cast<enum_unbounded_sparse>(0)};
  meta_name value{};
  se::diag_b<v>(value);
  se::diag_e<v>(value, 0u);
  }
using namespace ut;
"test enum_unbounded_sparse offseting"_test = []
{
  constexpr auto v{static_cast<enum_unbounded_sparse>(0)};
  meta_name value{};
  auto offset{se::b<v>(value)};
  se::e<v>(value, offset);
  se::e<enum_unbounded_sparse::v1>(value, offset);
  se::e<enum_unbounded_sparse::v2>(value, offset);
};

"test unbounded"_test = []
{
  ut::expect(enum_name(enum_unbounded::v1) == "v1");
  ut::expect(enum_name(enum_unbounded::v2) == "v2");
  ut::expect(enum_name(enum_unbounded::v6a) == "v6a");

  ut::expect(enum_name(enum_unbounded_sparse::v1) == "v1");
  ut::expect(enum_name(enum_unbounded_sparse::v2) == "v2");
};
"test variations"_test = []
{
  ut::expect(enum_name(one_elem_untyped::v1) == "v1");
  ut::expect(enum_name(static_cast<one_elem_untyped>(2)) == "");
  ut::expect(se_view<static_cast<sparse_untyped>(2)>() == "2");
  ut::expect(enum_name(static_cast<sparse_untyped>(2)) == "2");
  ut::expect(enum_name(sparse_offseted_untyped::unknown) == "unknown");
  ut::expect(enum_name(static_cast<sparse_offseted_untyped>(0)) == "0");
};

"test se meta name cut"_test = []
{
  ut::expect(se_view<weak_global_untyped_e::v1>() == "v1");
  // it is out of range or on sparse enum just value when used directly
  ut::expect(se_view<static_cast<one_elem_untyped>(2)>() == "2");

  ut::expect(se_view<strong_typed::v1>() == "v1");
  ut::expect(se_view<strong_typed::v2>() == "v2");
  ut::expect(se_view<strong_typed::v3>() == "v3");
  ut::expect(se_view<strong_typed::last>() == "v3");

  ut::expect(se_view<strong_untyped::v1>() == "v1");
  ut::expect(se_view<strong_untyped::v2>() == "v2");
  ut::expect(se_view<strong_untyped::v3>() == "v3");

  ut::expect(se_view<weak_typed_e::v1>() == "v1");
  ut::expect(se_view<weak_typed_e::v2>() == "v2");
  ut::expect(se_view<weak_typed_e::v3>() == "v3");

  ut::expect(se_view<v1>() == "v1");
  ut::expect(se_view<v2>() == "v2");
  ut::expect(se_view<v3>() == "v3");

  ut::expect(se_view<test::strong_typed_2_e::v1>() == "v1");
  ut::expect(se_view<test::strong_typed_2_e::v2>() == "v2");
  ut::expect(se_view<test::strong_typed_2_e::v3>() == "v3");

  ut::expect(se_view<test::subnamespace::example_3_e::v1>() == "v1");
  ut::expect(se_view<test::subnamespace::example_3_e::v2>() == "v2");
  ut::expect(se_view<test::subnamespace::example_3_e::v3>() == "v3");

  ut::expect(se_view<test::subnamespace::detail::example_4_e::v1>() == "v1");
  ut::expect(se_view<test::subnamespace::detail::example_4_e::v2>() == "v2");
  ut::expect(se_view<test::subnamespace::detail::example_4_e::v3>() == "v3");

  ut::expect(se_view<test::subnamespace::example_5_e::v1>() == "v1");
  ut::expect(se_view<test::subnamespace::example_5_e::v2>() == "v2");
  ut::expect(se_view<test::subnamespace::example_5_e::v3>() == "v3");

  ut::expect(se_view<test::subnamespace::weak_typed_5_e::v1>() == "v1");
  ut::expect(se_view<test::subnamespace::weak_typed_5_e::v2>() == "v2");
  ut::expect(se_view<test::subnamespace::weak_typed_5_e::v3>() == "v3");
};
"test enum name"_test = []
{
  ut::expect(enum_name(weak_global_untyped_e::v1) == "v1");

  ut::expect(enum_name(strong_typed::v1) == "v1");
  ut::expect(enum_name(strong_typed::v2) == "v2");
  ut::expect(enum_name(strong_typed::v3) == "v3");

  ut::expect(enum_name(strong_untyped::v1) == "v1");
  ut::expect(enum_name(strong_untyped::v2) == "v2");
  ut::expect(enum_name(strong_untyped::v3) == "v3");

  ut::expect(enum_name(weak_typed_e::v1) == "v1");
  ut::expect(enum_name(weak_typed_e::v2) == "v2");
  ut::expect(enum_name(weak_typed_e::v3) == "v3");

  ut::expect(enum_name(v1) == "v1");
  ut::expect(enum_name(v2) == "v2");
  ut::expect(enum_name(v3) == "v3");

  ut::expect(enum_name(test::strong_typed_2_e::v1) == "v1");
  ut::expect(enum_name(test::strong_typed_2_e::v2) == "v2");
  ut::expect(enum_name(test::strong_typed_2_e::v3) == "v3");

  ut::expect(enum_name(test::subnamespace::example_3_e::v1) == "v1");
  ut::expect(enum_name(test::subnamespace::example_3_e::v2) == "v2");
  ut::expect(enum_name(test::subnamespace::example_3_e::v3) == "v3");

  ut::expect(enum_name(test::subnamespace::detail::example_4_e::v1) == "v1");
  ut::expect(enum_name(test::subnamespace::detail::example_4_e::v2) == "v2");
  ut::expect(enum_name(test::subnamespace::detail::example_4_e::v3) == "v3");

  ut::expect(enum_name(test::subnamespace::example_5_e::v1) == "v1");
  ut::expect(enum_name(test::subnamespace::example_5_e::v2) == "v2");
  ut::expect(enum_name(test::subnamespace::example_5_e::v3) == "v3");
  ut::expect(enum_name(test::subnamespace::v1) == "v1");
  ut::expect(enum_name(test::subnamespace::v2) == "v2");
  ut::expect(enum_name(test::subnamespace::v3) == "v3");
};
};
  }  // namespace simple_enum

int main() { se::verify_offset(); }
