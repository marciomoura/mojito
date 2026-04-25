#ifndef MOJITO_ABC_HPP
#define MOJITO_ABC_HPP

#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include "type_traits.hpp"
#include "types.hpp"
#include "units.hpp"

namespace mojito {

/**
 * @brief Three-phase (abc) coordinate frame.
 */
template <typename T = real_t>
class abc {
public:
    using fundamental_type = fundamental_type_t<T>;
    using value_type = T;
    using iterator = typename std::array<T, 3>::iterator;
    using const_iterator = typename std::array<T, 3>::const_iterator;

    static constexpr abc<T> from_real(real_t a, real_t b, real_t c)
    {
        return abc<T>{T{a}, T{b}, T{c}};
    }

    constexpr abc() = default;
    constexpr abc(const std::array<T, 3>& values) : _values(values) {}
    constexpr abc(std::array<T, 3>&& values) : _values(std::move(values)) {}
    constexpr abc(T a, T b, T c) : _values{a, b, c} {}

    template <typename U>
    constexpr abc(const abc<U>& other)
        : _values{static_cast<T>(other.a()), static_cast<T>(other.b()), static_cast<T>(other.c())}
    {
    }

    constexpr T& operator[](size_t idx) { return _values[idx]; }
    constexpr const T& operator[](size_t idx) const { return _values[idx]; }

    constexpr operator std::array<T, 3>() const { return _values; }
    constexpr std::array<T, 3> array() const { return _values; }

    constexpr auto to_raw_frame() const
    {
        if constexpr (std::is_floating_point_v<fundamental_type>) {
            return _values;
        }
        else {
            using RawType = decltype(a().value());
            return abc<RawType>{a().value(), b().value(), c().value()};
        }
    }

    T a() const { return _values[0]; }
    T b() const { return _values[1]; }
    T c() const { return _values[2]; }

    T& a() { return _values[0]; }
    T& b() { return _values[1]; }
    T& c() { return _values[2]; }

    constexpr abc<T> operator+(const abc<T>& other) const
    {
        return abc<T>{_values[0] + other._values[0], _values[1] + other._values[1], _values[2] + other._values[2]};
    }

    constexpr abc<T> operator-(const abc<T>& other) const
    {
        return abc<T>{_values[0] - other._values[0], _values[1] - other._values[1], _values[2] - other._values[2]};
    }

    constexpr abc<T>& operator+=(const abc<T>& other)
    {
        _values[0] += other._values[0];
        _values[1] += other._values[1];
        _values[2] += other._values[2];
        return *this;
    }

    constexpr abc<T>& operator-=(const abc<T>& other)
    {
        _values[0] -= other._values[0];
        _values[1] -= other._values[1];
        _values[2] -= other._values[2];
        return *this;
    }

    constexpr abc<T> operator*(T scalar) const
    {
        return abc<T>{_values[0] * scalar, _values[1] * scalar, _values[2] * scalar};
    }

    constexpr abc<T> operator/(T scalar) const
    {
        return abc<T>{_values[0] / scalar, _values[1] / scalar, _values[2] / scalar};
    }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator*(Scalar scalar) const -> abc<decltype(std::declval<T>() * std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() * std::declval<Scalar>());
        return abc<result_type>{_values[0] * scalar, _values[1] * scalar, _values[2] * scalar};
    }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator/(Scalar scalar) const -> abc<decltype(std::declval<T>() / std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() / std::declval<Scalar>());
        return abc<result_type>{_values[0] / scalar, _values[1] / scalar, _values[2] / scalar};
    }

    constexpr abc<T>& operator*=(T scalar)
    {
        _values[0] *= scalar;
        _values[1] *= scalar;
        _values[2] *= scalar;
        return *this;
    }

    constexpr abc<T>& operator/=(T scalar)
    {
        _values[0] /= scalar;
        _values[1] /= scalar;
        return *this;
    }

    constexpr abc<T> operator-() const { return abc<T>{-_values[0], -_values[1], -_values[2]}; }

    auto begin() { return _values.begin(); }
    auto end() { return _values.end(); }
    auto begin() const { return _values.begin(); }
    auto end() const { return _values.end(); }
    std::size_t size() const { return _values.size(); }

    abc<T> absolute() const { return abc<T>{abs(_values[0]), abs(_values[1]), abs(_values[2])}; }

private:
    std::array<T, 3> _values{};
};

template <typename T>
constexpr abc<T> operator*(T scalar, const abc<T>& coords)
{
    return coords * scalar;
}

template <typename Scalar, typename T>
constexpr auto operator*(Scalar scalar, const abc<T>& coords) -> abc<decltype(scalar * coords.a())>
{
    using result_type = decltype(scalar * coords.a());
    return abc<result_type>{scalar * coords.a(), scalar * coords.b(), scalar * coords.c()};
}

}  // namespace mojito

#endif  // MOJITO_ABC_HPP
