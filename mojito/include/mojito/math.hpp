#ifndef MOJITO_MATH_HPP
#define MOJITO_MATH_HPP

#include <algorithm>
#include <type_traits>

#include "abc.hpp"
#include "alphabeta.hpp"
#include "dq.hpp"

namespace mojito {

template <typename Frame, typename T = typename Frame::value_type>
constexpr T cross_product(const Frame& a, const Frame& b)
{
    if constexpr (std::is_same_v<Frame, alphabeta<T>>) {
        return a.alpha() * b.beta() - a.beta() * b.alpha();
    }
    else if constexpr (std::is_same_v<Frame, dq<T>>) {
        return a.d() * b.q() - a.q() * b.d();
    }
    else {
        static_assert(std::is_same_v<Frame, alphabeta<T>> || std::is_same_v<Frame, dq<T>>,
                      "cross_product only supports alphabeta and dq frames");
    }
}

template <typename T>
T sum(abc<T> const& value)
{
    return value.a() + value.b() + value.c();
}

template <typename T>
T max(abc<T> const& value)
{
    return std::max({value.a(), value.b(), value.c()});
}

template <typename T>
T max(alphabeta<T> const& value)
{
    return std::max({value.alpha(), value.beta()});
}

template <typename T>
T max(dq<T> const& value)
{
    return std::max({value.d(), value.q()});
}

template <typename T>
T min(abc<T> const& value)
{
    return std::min({value.a(), value.b(), value.c()});
}

template <typename T>
T min(alphabeta<T> const& value)
{
    return std::min({value.alpha(), value.beta()});
}

template <typename T>
T min(dq<T> const& value)
{
    return std::min({value.d(), value.q()});
}

}  // namespace mojito

#endif  // MOJITO_MATH_HPP
