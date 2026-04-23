#pragma once

#include <iostream>
#include <type_traits>

namespace mojito {

namespace internal {
// Helper to deduce the type of the reciprocal (1.0 / T)
template <typename T>
struct reciprocal_type_helper {
    using type = decltype(1.0 / std::declval<T>());
};
}  // namespace internal

/**
 * @brief A class that pre-calculates the reciprocal of a value for fast division.
 *
 * This class stores a value and its reciprocal (1/value). When used as the
 * right-hand side of a division, it performs a faster multiplication instead.
 * It can wrap either a dimensioned quantity or a raw scalar type.
 *
 * @tparam T The type of the value to be stored (e.g., voltage_t, real_t).
 */
template <typename T>
class divisor {
public:
    using value_type = T;
    using reciprocal_type = typename internal::reciprocal_type_helper<T>::type;

    /**
     * @brief Default constructor. Value is default-initialized (e.g., 0).
     * Reciprocal will be infinity if the value is 0.
     */
    constexpr divisor() : _value{}, _reciprocal{1.0 / _value} {}

    /**
     * @brief Constructs a divisor from a value.
     * @param val The value to use as the divisor.
     */
    constexpr explicit divisor(const T& val) : _value(val), _reciprocal(1.0 / val) {}

    /**
     * @brief Returns the original, non-reciprocal value.
     */
    constexpr const T& value() const { return _value; }

    /**
     * @brief Returns the pre-calculated reciprocal (1/value).
     */
    constexpr const reciprocal_type& reciprocal() const { return _reciprocal; }

private:
    T _value;
    reciprocal_type _reciprocal;
};

//==============================================================================
// Core Division Operator (The main purpose of this class)
//==============================================================================

/**
 * @brief Divides a value by a divisor using fast multiplication.
 */
template <typename LhsType, typename T>
constexpr auto operator/(const LhsType& lhs, const divisor<T>& rhs)
{
    // The optimization: division is replaced by multiplication.
    return lhs * rhs.reciprocal();
}

/**
 * @brief Divides a value by a divisor using fast multiplication.
 */
template <typename T>
constexpr auto operator/(const divisor<T>& lhs, const divisor<T>& rhs)
{
    // The optimization: division is replaced by multiplication.
    return lhs.value() * rhs.reciprocal();
}

//==============================================================================
// Arithmetic Operators for divisor
// (These operate on the original values and return a new divisor)
//==============================================================================

//--- Addition ---
template <typename T1, typename T2>
constexpr auto operator+(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() + rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

//--- Subtraction ---
template <typename T1, typename T2>
constexpr auto operator-(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() - rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

//--- Multiplication ---
template <typename T1, typename T2>
constexpr auto operator*(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() * rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

//--- Division ---
template <typename T1, typename T2>
constexpr auto operator/(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() / rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

//==============================================================================
// Output Formatting
//==============================================================================

template <typename T>
std::ostream& operator<<(std::ostream& os, const divisor<T>& d)
{
    // Print the original value, as it's more human-readable.
    os << d.value();
    return os;
}

}  // namespace mojito