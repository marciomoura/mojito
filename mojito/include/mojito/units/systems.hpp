#pragma once

#include <type_traits>

namespace mojito {

struct si {};

struct default_base {};

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
