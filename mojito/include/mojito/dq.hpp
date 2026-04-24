#ifndef MOJITO_DQ_HPP
#define MOJITO_DQ_HPP

#include <array>
#include <cmath>
#include <type_traits>

#include "angle_wrapped.hpp"
#include "type_traits.hpp"
#include "types.hpp"
#include "units.hpp"

namespace mojito {

/**
 * @brief Rotating (dq) coordinate frame.
 */
template <typename T = real_t>
class dq {
public:
    using fundamental_type = fundamental_type_t<T>;
    using value_type = T;
    using iterator = typename std::array<T, 3>::iterator;
    using const_iterator = typename std::array<T, 3>::const_iterator;

    constexpr dq() = default;
    constexpr dq(const std::array<T, 2>& values) : _values(values) {}
    constexpr dq(T d, T q) : _values{d, q} {}

    template <typename U>
    constexpr dq(const dq<U>& other) : _values{static_cast<T>(other.d()), static_cast<T>(other.q())}
    {
    }

    constexpr T& operator[](size_t idx) { return _values[idx]; }
    constexpr const T& operator[](size_t idx) const { return _values[idx]; }

    constexpr operator std::array<T, 2>() const { return _values; }
    constexpr std::array<T, 2> array() const { return _values; }

    T d() const { return _values[0]; }
    T q() const { return _values[1]; }

    T& d() { return _values[0]; }
    T& q() { return _values[1]; }

    constexpr T magnitude() const
    {
        if constexpr (std::is_floating_point_v<T>) {
            return std::sqrt(_values[0] * _values[0] + _values[1] * _values[1]);
        }
        else {
            return mojito::sqrt(_values[0] * _values[0] + _values[1] * _values[1]);
        }
    }

    angle_wrapped phase(const angle_wrapped& frame) const
    {
        if constexpr (std::is_same_v<fundamental_type, float>) {
            return frame + angle_wrapped(std::atan2(static_cast<float>(_values[1]), static_cast<float>(_values[0])));
        }
        else {
            return frame + angle_wrapped(std::atan2(_values[1], _values[0]));
        }
    }

    constexpr dq<T> rotate_counter_clockwise(const angle_wrapped& theta) const
    {
        const real_t cos_angle = std::cos(theta.get_radians());
        const real_t sin_angle = std::sin(theta.get_radians());
        return dq<T>{_values[0] * cos_angle - _values[1] * sin_angle, _values[0] * sin_angle + _values[1] * cos_angle};
    }

    constexpr dq<T> rotate_clockwise(const angle_wrapped& theta) const
    {
        return rotate_counter_clockwise(angle_wrapped(-theta.get_radians()));
    }

    constexpr dq<T> rotate(const angle_wrapped& theta) const { return rotate_counter_clockwise(theta); }

    constexpr dq<T> operator+(const dq<T>& other) const
    {
        return dq<T>{_values[0] + other._values[0], _values[1] + other._values[1]};
    }

    constexpr dq<T> operator-(const dq<T>& other) const
    {
        return dq<T>{_values[0] - other._values[0], _values[1] - other._values[1]};
    }

    constexpr dq<T>& operator+=(const dq<T>& other)
    {
        _values[0] += other._values[0];
        _values[1] += other._values[1];
        return *this;
    }

    constexpr dq<T>& operator-=(const dq<T>& other)
    {
        _values[0] -= other._values[0];
        _values[1] -= other._values[1];
        return *this;
    }

    constexpr dq<T> operator*(T scalar) const { return dq<T>{_values[0] * scalar, _values[1] * scalar}; }

    constexpr dq<T> operator/(T scalar) const { return dq<T>{_values[0] / scalar, _values[1] / scalar}; }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator*(Scalar scalar) const -> dq<decltype(std::declval<T>() * std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() * std::declval<Scalar>());
        return dq<result_type>{_values[0] * scalar, _values[1] * scalar};
    }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator/(Scalar scalar) const -> dq<decltype(std::declval<T>() / std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() / std::declval<Scalar>());
        return dq<result_type>{_values[0] / scalar, _values[1] / scalar};
    }

    constexpr dq<T>& operator*=(T scalar)
    {
        _values[0] *= scalar;
        _values[1] *= scalar;
        return *this;
    }

    constexpr dq<T>& operator/=(T scalar)
    {
        _values[0] /= scalar;
        _values[1] /= scalar;
        return *this;
    }

    constexpr dq<T> operator-() const { return dq<T>{-_values[0], -_values[1]}; }

    auto begin() { return _values.begin(); }
    auto end() { return _values.end(); }
    auto begin() const { return _values.begin(); }
    auto end() const { return _values.end(); }
    std::size_t size() const { return _values.size(); }

    dq<T> absolute() const { return dq<T>{std::abs(_values[0]), std::abs(_values[1])}; }

private:
    std::array<T, 2> _values{};
};

template <typename T>
constexpr dq<T> operator*(T scalar, const dq<T>& coords)
{
    return coords * scalar;
}

template <typename Scalar, typename T>
constexpr auto operator*(Scalar scalar, const dq<T>& coords) -> dq<decltype(scalar * coords.d())>
{
    using result_type = decltype(scalar * coords.d());
    return dq<result_type>{scalar * coords.d(), scalar * coords.q()};
}

}  // namespace mojito

#endif  // MOJITO_DQ_HPP
