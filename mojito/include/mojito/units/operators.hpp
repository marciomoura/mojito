#pragma once

#include <cmath>
#include <type_traits>
#include "quantity.hpp"
#include "dimensions.hpp"

namespace mojito {

template <typename D, typename S>
constexpr quantity<D, S> operator+(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return quantity<D, S>(lhs.value() + rhs.value());
}

template <typename D, typename S>
constexpr quantity<D, S> operator-(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return quantity<D, S>(lhs.value() - rhs.value());
}

template <typename D, typename S>
constexpr quantity<D, S> operator-(const quantity<D, S>& q)
{
    return quantity<D, S>(-q.value());
}

template <typename D1, typename D2, typename S>
constexpr auto operator*(const quantity<D1, S>& lhs, const quantity<D2, S>& rhs)
{
    using result_dim = typename multiply_dimensions<D1, D2>::type;
    return quantity<result_dim, S>(lhs.value() * rhs.value());
}

template <typename D1, typename D2, typename S>
constexpr auto operator/(const quantity<D1, S>& lhs, const quantity<D2, S>& rhs)
{
    using result_dim = typename divide_dimensions<D1, D2>::type;
    return quantity<result_dim, S>(lhs.value() / rhs.value());
}

template <typename D, typename S>
constexpr auto sqrt(const quantity<D, S>& q)
{
    using result_dim = typename root_dimension<D, 2>::type;
    return quantity<result_dim, S>(std::sqrt(q.value()));
}

template <typename D, typename S>
constexpr auto abs(const quantity<D, S>& q)
{
    return quantity<D, S>(std::abs(q.value()));
}

template <typename D, typename S>
constexpr quantity<D, S> max(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return (lhs.value() > rhs.value()) ? lhs : rhs;
}

template <typename D, typename S>
constexpr quantity<D, S> min(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return (lhs.value() < rhs.value()) ? lhs : rhs;
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator*(const quantity<D, S>& q, const Scalar& s)
{
    return quantity<D, S>(q.value() * static_cast<real_t>(s));
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator*(const Scalar& s, const quantity<D, S>& q)
{
    return q * s;
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr quantity<D, S> operator/(const quantity<D, S>& q, const Scalar& s)
{
    return quantity<D, S>(q.value() / static_cast<real_t>(s));
}

template <typename D, typename S, typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0>
constexpr auto operator/(const Scalar& s, const quantity<D, S>& q)
{
    using inverted_dim = typename invert_dimensions<D>::type;
    return quantity<inverted_dim, S>(static_cast<real_t>(s) / q.value());
}

template <typename D, typename S>
constexpr bool operator==(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return lhs.value() == rhs.value();
}

template <typename D, typename S>
constexpr bool operator!=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(lhs == rhs);
}

template <typename D, typename S>
constexpr bool operator<(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return lhs.value() < rhs.value();
}

template <typename D, typename S>
constexpr bool operator>(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return rhs < lhs;
}

template <typename D, typename S>
constexpr bool operator<=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(rhs < lhs);
}

template <typename D, typename S>
constexpr bool operator>=(const quantity<D, S>& lhs, const quantity<D, S>& rhs)
{
    return !(lhs < rhs);
}

}  // namespace mojito
