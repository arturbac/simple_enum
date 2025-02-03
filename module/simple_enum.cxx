module;
#include <simple_enum/simple_enum.hpp>
#include <simple_enum/enum_index.hpp>
#include <simple_enum/enum_cast.hpp>
#include <simple_enum/generic_error_category.hpp>
#include <simple_enum/generic_error_category_impl.hpp>
#include <simple_enum/ranges_views.hpp>
#include <simple_enum/indexed_access.hpp>
#include <simple_enum/std_format.hpp>
#include <simple_enum/basic_fixed_string.hpp>

export module simple_enum;

export namespace simple_enum
  {

using simple_enum::bounded_enum;
using simple_enum::enum_concept;

namespace concepts
  {
  using simple_enum::bounded_enum;
  using simple_enum::concepts::declared_error_code;
  using simple_enum::concepts::error_enum;
  using simple_enum::enum_concept;
  }  // namespace concepts

using simple_enum::adl_info;
using simple_enum::enum_name;
using simple_enum::enum_name_at_index;
using simple_enum::enum_name_t;
using simple_enum::enum_size;
using simple_enum::enum_value_at_index;
using simple_enum::enumeration_name_v;
using simple_enum::info;

namespace limits
  {
  using simple_enum::limits::max;
  using simple_enum::limits::min;
  }  // namespace limits

using simple_enum::consteval_enum_index;
using simple_enum::enum_index;
using simple_enum::enum_index_error;
using simple_enum::enum_index_t;

using simple_enum::enum_cast;
using simple_enum::enum_cast_error;
using simple_enum::enum_cast_t;

using simple_enum::expected;
using simple_enum::expected_ec;
using simple_enum::generic_error_category;
using simple_enum::make_error_code;
using simple_enum::make_unexpected_ec;
using simple_enum::unexpected;
using simple_enum::unexpected_ec;

using simple_enum::as_basic_fixed_string;
using simple_enum::basic_fixed_string;
using simple_enum::string_literal;
using simple_enum::to_camel_case;

using simple_enum::enum_enumerations;
using simple_enum::enum_view;

using simple_enum::begin;
using simple_enum::end;
using simple_enum::enum_names;
  }  // namespace simple_enum
