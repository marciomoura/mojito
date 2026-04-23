#pragma once

#include <ostream>
#include <type_traits>

#include "types.hpp"

namespace mojito {

namespace internal {
template <typename T>
struct reciprocal_type_helper {
    using type = decltype(real_t{1.0} / std::declval<T>());
};
}  // namespace internal

/**
 * @brief Pre-calculates the reciprocal of a value for fast division.
 *
 * When used as the right-hand side of a division, it performs a multiplication instead.
 */
template <typename T>
class divisor {
public:
    using value_type = T;
    using reciprocal_type = typename internal::reciprocal_type_helper<T>::type;

    constexpr divisor() : _value{}, _reciprocal{real_t{1.0} / _value} {}

    constexpr explicit divisor(const T& val) : _value(val), _reciprocal(real_t{1.0} / val) {}

    constexpr const T& value() const { return _value; }

    constexpr const reciprocal_type& reciprocal() const { return _reciprocal; }

private:
    T _value;
    reciprocal_type _reciprocal;
};

template <typename LhsType, typename T>
constexpr auto operator/(const LhsType& lhs, const divisor<T>& rhs)
{
    return lhs * rhs.reciprocal();
}

template <typename T>
constexpr auto operator/(const divisor<T>& lhs, const divisor<T>& rhs)
{
    return lhs.value() * rhs.reciprocal();
}

template <typename T1, typename T2>
constexpr auto operator+(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() + rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

template <typename T1, typename T2>
constexpr auto operator-(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() - rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

template <typename T1, typename T2>
constexpr auto operator*(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() * rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

template <typename T1, typename T2>
constexpr auto operator/(const divisor<T1>& lhs, const divisor<T2>& rhs)
{
    auto new_val = lhs.value() / rhs.value();
    return divisor<decltype(new_val)>(new_val);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const divisor<T>& d)
{
    os << d.value();
    return os;
}

}  // namespace mojito
