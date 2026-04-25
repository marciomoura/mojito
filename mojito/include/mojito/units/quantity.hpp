#pragma once

#include <type_traits>
#include "../types.hpp"

namespace mojito {

namespace internal {
template <typename T, typename = void>
struct is_quantity : std::false_type {};

template <typename T>
struct is_quantity<T, std::void_t<decltype(std::declval<T>().value())>> : std::true_type {};
}  // namespace internal

template <typename T>
inline constexpr bool is_quantity_v = internal::is_quantity<T>::value;

template <typename T>
inline constexpr bool use_epsilon_comparison_v = std::is_floating_point_v<T> || is_quantity_v<T>;

/**
 * @brief Represents a physical quantity with dimension and unit system.
 */
template <typename Dimension, typename UnitSystem>
class quantity {
public:
    using dimension = Dimension;
    using system = UnitSystem;

    constexpr quantity() : _value(real_t{0.0}) {}

    constexpr explicit quantity(real_t val) : _value(val) {}

    constexpr real_t value() const { return _value; }

    constexpr quantity& operator+=(const quantity& rhs)
    {
        _value += rhs._value;
        return *this;
    }

    constexpr quantity& operator-=(const quantity& rhs)
    {
        _value -= rhs._value;
        return *this;
    }

    template <typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
    constexpr quantity& operator*=(const Scalar& s)
    {
        _value *= static_cast<real_t>(s);
        return *this;
    }

    template <typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
    constexpr quantity& operator/=(const Scalar& s)
    {
        _value /= static_cast<real_t>(s);
        return *this;
    }

    operator real_t() const { return _value; }

private:
    real_t _value;
};

}  // namespace mojito
