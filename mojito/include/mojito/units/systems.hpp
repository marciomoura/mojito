#pragma once

#include <type_traits>

namespace mojito {

struct si {};

// Time Unit Systems
struct microseconds {};
struct milliseconds {};
using seconds = si;
struct minutes {};
struct hours {};

struct default_base {};

// Time Base Tags
struct microseconds_base {};
struct milliseconds_base {};
struct seconds_base {};
struct minutes_base {};
struct hours_base {};


template <typename Base = default_base>
struct per_unit {
    using base = Base;
};

struct percent {};

template <typename T>
struct is_per_unit : std::false_type {};

template <typename Base>
struct is_per_unit<per_unit<Base>> : std::true_type {};

template <typename T>
inline constexpr bool is_per_unit_v = is_per_unit<T>::value;

}  // namespace mojito
