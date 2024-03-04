#pragma once

#include <simple_enum/simple_enum.hpp>

#include <numeric>
#include <algorithm>
#include <iterator>

#include "detail/static_call_operator_prolog.h"

#include <simple_enum/expected.h>

namespace simple_enum::inline v0_5
  {
using cxx23::bad_expected_access;
using cxx23::expected;
using cxx23::in_place;
using cxx23::in_place_t;
using cxx23::unexpect;
using cxx23::unexpect_t;
using cxx23::unexpected;

namespace detail
  {
  template<enum_concept enum_type>
  [[nodiscard]]
  constexpr auto sorted_indices() noexcept -> std::array<std::size_t, detail::enum_meta_info_t<enum_type>::size()>
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    std::array<std::size_t, enum_meta_info::size()> indices;
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(
      indices.begin(),
      indices.end(),
      [](std::size_t a, std::size_t b) noexcept -> bool
      { return enum_meta_info::meta_data[a] < enum_meta_info::meta_data[b]; }
    );
    return indices;
    }

  template<enum_concept enum_type>
  struct enum_meta_info_sorted_indices_t
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    using indicies_array_type = std::array<std::size_t, enum_meta_info::size()>;
    using iterator = typename indicies_array_type::const_iterator;
    static constexpr indicies_array_type indices{sorted_indices<enum_type>()};
    };

  // optimised for small ranges like up to ~256 elements
  template<std::random_access_iterator Iterator, typename ValueType, typename CompareType>
  constexpr auto bound_leaning_lower_bound(
    Iterator first, Iterator last, ValueType const & v, CompareType less
  ) noexcept(noexcept(less(*first, v))) -> Iterator
    {
    if(first == last)
      return last;

    auto size = last - first;
    auto middle = first + size / 2;

    if(less(*middle, v))
      {
      first = middle + 1;
      while(first < last && less(*(first + (last - first) / 2), v))
        first += (last - first) / 2 + 1;
      }
    else
      {
      last = middle;
      while(first < last && !less(*(first + (last - first) / 2), v))
        last = first + (last - first) / 2;
      }

    // Since ranges are small using linear search.
    for(; first < last; ++first)
      if(!less(*first, v))
        break;
    return first;
    }

  template<enum_concept EnumType>
  [[nodiscard]]
  constexpr auto lower_bound_search_indices(std::string_view target) noexcept ->
    typename detail::enum_meta_info_sorted_indices_t<EnumType>::iterator
    {
    using enum_meta_info = detail::enum_meta_info_t<EnumType>;
    using sorted_indices_type = detail::enum_meta_info_sorted_indices_t<EnumType>;

    auto comp = [&](std::size_t idx, std::string_view val) noexcept -> bool
    { return enum_meta_info::meta_data[idx].as_view() < val; };

    auto it = bound_leaning_lower_bound(
      sorted_indices_type::indices.begin(), sorted_indices_type::indices.end(), target, comp
    );

    if(it != sorted_indices_type::indices.end() && enum_meta_info::meta_data[*it].as_view() == target)
      return it;
    else
      return sorted_indices_type::indices.end();
    }

  }  // namespace detail

enum struct enum_cast_error
  {
  invalid_cast
  };

template<>
struct info<enum_cast_error>
  {
  static constexpr auto first = enum_cast_error::invalid_cast;
  static constexpr auto last = enum_cast_error::invalid_cast;
  };

template<enum_concept enum_type>
struct enum_cast_t
  {
  [[nodiscard]]
  static_call_operator constexpr auto
    operator()(std::string_view value) static_call_operator_const noexcept
    -> cxx23::expected<enum_type, enum_cast_error>
    {
    using enum_meta_info = detail::enum_meta_info_t<enum_type>;
    using sorted_indices_type = detail::enum_meta_info_sorted_indices_t<enum_type>;
    auto it{detail::lower_bound_search_indices<enum_type>(value)};
    if(it != sorted_indices_type::indices.end())
      return static_cast<enum_type>(*it + enum_meta_info::first_index);
    return cxx23::unexpected{enum_cast_error::invalid_cast};
    }
  };

template<enum_concept enum_type>
inline constexpr enum_cast_t<enum_type> enum_cast{};

  }  // namespace simple_enum::inline v0_5

#include "detail/static_call_operator_epilog.h"
