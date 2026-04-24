#ifndef MOJITO_ALPHABETA_HPP
#define MOJITO_ALPHABETA_HPP

#include <array>
#include <cmath>
#include <type_traits>

#include "angle_wrapped.hpp"
#include "type_traits.hpp"
#include "types.hpp"
#include "units.hpp"

namespace mojito {

/**
 * @brief Two-phase stationary (αβ) coordinate frame.
 */
template <typename T = real_t>
class alphabeta {
public:
    using fundamental_type = fundamental_type_t<T>;
    using value_type = T;
    using iterator = typename std::array<T, 3>::iterator;
    using const_iterator = typename std::array<T, 3>::const_iterator;

    constexpr alphabeta() = default;
    constexpr alphabeta(const std::array<T, 2>& values) : _values(values) {}
    constexpr alphabeta(T alpha, T beta) : _values{alpha, beta} {}

    constexpr alphabeta(const alphabeta<T>& other) : _values(other._values) {}
    constexpr alphabeta(alphabeta<T>&& other) : _values(std::move(other._values)) {}

    template <typename U>
    constexpr alphabeta(const alphabeta<U>& other)
        : _values{static_cast<T>(other.alpha()), static_cast<T>(other.beta())}
    {
    }

    constexpr alphabeta<T>& operator=(const alphabeta<T>& other)
    {
        _values = other._values;
        return *this;
    }

    constexpr alphabeta<T>& operator=(alphabeta<T>&& other)
    {
        _values = std::move(other._values);
        return *this;
    }

    constexpr T& operator[](size_t idx) { return _values[idx]; }
    constexpr const T& operator[](size_t idx) const { return _values[idx]; }

    constexpr operator std::array<T, 2>() const { return _values; }
    constexpr std::array<T, 2> array() const { return _values; }

    T alpha() const { return _values[0]; }
    T beta() const { return _values[1]; }

    T& alpha() { return _values[0]; }
    T& beta() { return _values[1]; }

    constexpr T magnitude() const
    {
        if constexpr (std::is_floating_point_v<T>) {
            return std::sqrt(_values[0] * _values[0] + _values[1] * _values[1]);
        }
        else {
            return mojito::sqrt(_values[0] * _values[0] + _values[1] * _values[1]);
        }
    }

    angle_wrapped phase() const
    {
        if constexpr (std::is_same_v<fundamental_type, float>) {
            return angle_wrapped{std::atan2(static_cast<float>(_values[1]), static_cast<float>(_values[0]))};
        }
        else {
            return angle_wrapped{std::atan2(_values[1], _values[0])};
        }
    }

    constexpr alphabeta<T> rotate_counter_clockwise(const angle_wrapped& theta) const
    {
        const real_t cos_angle = std::cos(theta.get_radians());
        const real_t sin_angle = std::sin(theta.get_radians());
        return alphabeta<T>{_values[0] * cos_angle - _values[1] * sin_angle,
                            _values[0] * sin_angle + _values[1] * cos_angle};
    }

    constexpr alphabeta<T> rotate_clockwise(const angle_wrapped& theta) const
    {
        return rotate_counter_clockwise(angle_wrapped(-theta.get_radians()));
    }

    constexpr alphabeta<T> rotate(const angle_wrapped& theta) const { return rotate_counter_clockwise(theta); }

    constexpr alphabeta<T> operator+(const alphabeta<T>& other) const
    {
        return alphabeta<T>{_values[0] + other._values[0], _values[1] + other._values[1]};
    }

    constexpr alphabeta<T> operator-(const alphabeta<T>& other) const
    {
        return alphabeta<T>{_values[0] - other._values[0], _values[1] - other._values[1]};
    }

    constexpr alphabeta<T>& operator+=(const alphabeta<T>& other)
    {
        _values[0] += other._values[0];
        _values[1] += other._values[1];
        return *this;
    }

    constexpr alphabeta<T>& operator-=(const alphabeta<T>& other)
    {
        _values[0] -= other._values[0];
        _values[1] -= other._values[1];
        return *this;
    }

    constexpr alphabeta<T> operator*(T scalar) const { return alphabeta<T>{_values[0] * scalar, _values[1] * scalar}; }

    constexpr alphabeta<T> operator/(T scalar) const { return alphabeta<T>{_values[0] / scalar, _values[1] / scalar}; }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator*(Scalar scalar) const -> alphabeta<decltype(std::declval<T>() * std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() * std::declval<Scalar>());
        return alphabeta<result_type>{_values[0] * scalar, _values[1] * scalar};
    }

    template <typename Scalar, typename = std::enable_if_t<!std::is_same_v<Scalar, T>>>
    constexpr auto operator/(Scalar scalar) const -> alphabeta<decltype(std::declval<T>() / std::declval<Scalar>())>
    {
        using result_type = decltype(std::declval<T>() / std::declval<Scalar>());
        return alphabeta<result_type>{_values[0] / scalar, _values[1] / scalar};
    }

    constexpr alphabeta<T>& operator*=(T scalar)
    {
        _values[0] *= scalar;
        _values[1] *= scalar;
        return *this;
    }

    constexpr alphabeta<T>& operator/=(T scalar)
    {
        _values[0] /= scalar;
        _values[1] /= scalar;
        return *this;
    }

    constexpr alphabeta<T> operator-() const { return alphabeta<T>{-_values[0], -_values[1]}; }

    auto begin() { return _values.begin(); }
    auto end() { return _values.end(); }
    auto begin() const { return _values.begin(); }
    auto end() const { return _values.end(); }
    std::size_t size() const { return _values.size(); }

    alphabeta<T> absolute() const { return alphabeta<T>{std::abs(_values[0]), std::abs(_values[1])}; }

private:
    std::array<T, 2> _values{};
};

template <typename T>
constexpr alphabeta<T> operator*(T scalar, const alphabeta<T>& coords)
{
    return coords * scalar;
}

template <typename Scalar, typename T>
constexpr auto operator*(Scalar scalar, const alphabeta<T>& coords) -> alphabeta<decltype(scalar * coords.alpha())>
{
    using result_type = decltype(scalar * coords.alpha());
    return alphabeta<result_type>{scalar * coords.alpha(), scalar * coords.beta()};
}

}  // namespace mojito

#endif  // MOJITO_ALPHABETA_HPP
