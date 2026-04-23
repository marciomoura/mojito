#ifndef mojito_COORDINATES_HPP
#define mojito_COORDINATES_HPP

#include <array>
#include <cmath>
#include <type_traits>

#include "angle_wrapped.hpp"
#include "constants.hpp"
#include "type_traits.hpp"
#include "types.hpp"
#include "units.hpp"

namespace mojito {

template <typename T>
class abc;
template <typename T>
class alphabeta;
template <typename T>
class dq;

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

    constexpr abc() = default;
    constexpr abc(const std::array<T, 3>& values) : _values(values) {}
    constexpr abc(std::array<T, 3>&& values) : _values(values) {}
    constexpr abc(T a, T b, T c) : _values{a, b, c} {}
    constexpr abc(const abc<T>& other) : _values(other._values) {}
    constexpr abc(abc<T>&& other) : _values(std::move(other._values)) {}

    template <typename U>
    constexpr abc(const abc<U>& other)
        : _values{static_cast<T>(other.a()), static_cast<T>(other.b()), static_cast<T>(other.c())}
    {
    }

    constexpr abc<T>& operator=(const abc<T>& other)
    {
        _values = other._values;
        return *this;
    }

    constexpr abc<T>& operator=(abc<T>&& other)
    {
        _values = std::move(other._values);
        return *this;
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

    constexpr alphabeta<T> to_alphabeta() const;
    constexpr dq<T> to_dq(const angle_wrapped& theta) const;

    T a() const { return _values[0]; }
    T b() const { return _values[1]; }
    T c() const { return _values[2]; }

    T& a() { return _values[0]; }
    T& b() { return _values[1]; }
    T& c() { return _values[2]; }

    constexpr abc<T> rotate(const angle_wrapped& angle) const
    {
        return to_alphabeta().rotate(angle).to_abc();
    }

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
        _values[2] /= scalar;
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

    constexpr abc<T> to_abc() const;
    constexpr dq<T> to_dq(const angle_wrapped& theta) const;

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

    constexpr dq(const dq<T>& other) : _values(other._values) {}
    constexpr dq(dq<T>&& other) : _values(std::move(other._values)) {}

    template <typename U>
    constexpr dq(const dq<U>& other) : _values{static_cast<T>(other.d()), static_cast<T>(other.q())}
    {
    }

    constexpr dq<T>& operator=(const dq<T>& other)
    {
        _values = other._values;
        return *this;
    }

    constexpr dq<T>& operator=(dq<T>&& other)
    {
        _values = std::move(other._values);
        return *this;
    }

    constexpr T& operator[](size_t idx) { return _values[idx]; }
    constexpr const T& operator[](size_t idx) const { return _values[idx]; }

    constexpr operator std::array<T, 2>() const { return _values; }
    constexpr std::array<T, 2> array() const { return _values; }

    constexpr alphabeta<T> to_alphabeta(const angle_wrapped& theta) const;
    constexpr abc<T> to_abc(const angle_wrapped& theta) const;

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

template <typename T>
constexpr alphabeta<T> abc<T>::to_alphabeta() const
{
    return {real_t{2.0 / 3.0} * (_values[0] - real_t{0.5} * _values[1] - real_t{0.5} * _values[2]),
            (sqrt_3 / real_t{3.0}) * (_values[1] - _values[2])};
}

template <typename T>
constexpr dq<T> abc<T>::to_dq(const angle_wrapped& theta) const
{
    return to_alphabeta().to_dq(theta);
}

template <typename T>
constexpr abc<T> alphabeta<T>::to_abc() const
{
    return abc<T>{_values[0], real_t{-0.5} * _values[0] + (sqrt_3 / real_t{2.0}) * _values[1],
                  real_t{-0.5} * _values[0] - (sqrt_3 / real_t{2.0}) * _values[1]};
}

template <typename T>
constexpr dq<T> alphabeta<T>::to_dq(const angle_wrapped& theta) const
{
    const real_t cos_theta = std::cos(theta.get_radians());
    const real_t sin_theta = std::sin(theta.get_radians());
    return dq<T>{cos_theta * _values[0] + sin_theta * _values[1], -sin_theta * _values[0] + cos_theta * _values[1]};
}

template <typename T>
constexpr alphabeta<T> dq<T>::to_alphabeta(const angle_wrapped& theta) const
{
    const real_t cos_theta = std::cos(theta.get_radians());
    const real_t sin_theta = std::sin(theta.get_radians());
    return alphabeta<T>{cos_theta * _values[0] - sin_theta * _values[1],
                        sin_theta * _values[0] + cos_theta * _values[1]};
}

template <typename T>
constexpr abc<T> dq<T>::to_abc(const angle_wrapped& theta) const
{
    return to_alphabeta(theta).to_abc();
}

template <typename T>
constexpr abc<T> make_abc(T magnitude, const angle_wrapped& phase)
{
    const real_t angle_rad = phase.get_radians();
    return abc<T>{magnitude * std::cos(angle_rad), magnitude * std::cos(angle_rad - two_pi_thirds),
                  magnitude * std::cos(angle_rad + two_pi_thirds)};
}

template <typename T>
constexpr alphabeta<T> make_alphabeta(T magnitude, const angle_wrapped& phase)
{
    const real_t angle_rad = phase.get_radians();
    return alphabeta<T>{magnitude * std::cos(angle_rad), magnitude * std::sin(angle_rad)};
}

template <typename T>
constexpr dq<T> make_dq(T d, T q)
{
    return dq<T>{d, q};
}

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

template <typename Dim>
constexpr auto to_si(const abc<quantity<Dim, per_unit>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.a(), base_quantity));
    return abc<result_quantity_t>(to_si(pu_frame.a(), base_quantity), to_si(pu_frame.b(), base_quantity),
                                  to_si(pu_frame.c(), base_quantity));
}

template <typename Dim>
constexpr auto to_pu(const abc<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu(si_frame.a(), base_quantity));
    return abc<result_quantity_t>(to_pu(si_frame.a(), base_quantity), to_pu(si_frame.b(), base_quantity),
                                  to_pu(si_frame.c(), base_quantity));
}

template <typename Dim>
constexpr auto to_si(const alphabeta<quantity<Dim, per_unit>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.alpha(), base_quantity));
    return alphabeta<result_quantity_t>(to_si(pu_frame.alpha(), base_quantity), to_si(pu_frame.beta(), base_quantity));
}

template <typename Dim>
constexpr auto to_pu(const alphabeta<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu(si_frame.alpha(), base_quantity));
    return alphabeta<result_quantity_t>(to_pu(si_frame.alpha(), base_quantity), to_pu(si_frame.beta(), base_quantity));
}

template <typename Dim>
constexpr auto to_si(const dq<quantity<Dim, per_unit>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.d(), base_quantity));
    return dq<result_quantity_t>(to_si(pu_frame.d(), base_quantity), to_si(pu_frame.q(), base_quantity));
}

template <typename Dim>
constexpr auto to_pu(const dq<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu(si_frame.d(), base_quantity));
    return dq<result_quantity_t>(to_pu(si_frame.d(), base_quantity), to_pu(si_frame.q(), base_quantity));
}

template <typename Dim>
constexpr auto to_pu(const abc<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu(si_frame.a(), divisor));
    return abc<result_quantity_t>(to_pu(si_frame.a(), divisor), to_pu(si_frame.b(), divisor),
                                  to_pu(si_frame.c(), divisor));
}

template <typename Dim>
constexpr auto to_pu(const alphabeta<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu(si_frame.alpha(), divisor));
    return alphabeta<result_quantity_t>(to_pu(si_frame.alpha(), divisor), to_pu(si_frame.beta(), divisor));
}

template <typename Dim>
constexpr auto to_pu(const dq<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu(si_frame.d(), divisor));
    return dq<result_quantity_t>(to_pu(si_frame.d(), divisor), to_pu(si_frame.q(), divisor));
}

template <typename QuantityType>
abc<QuantityType> to_dimension_abc(const abc<real_t>& scalar_abc)
{
    abc<QuantityType> quantity_abc{};
    std::transform(scalar_abc.begin(), scalar_abc.end(), quantity_abc.begin(),
                   [](real_t val) { return QuantityType{val}; });
    return quantity_abc;
}

template <typename QuantityType>
alphabeta<QuantityType> to_dimension_alphabeta(const alphabeta<real_t>& scalar_alphabeta)
{
    alphabeta<QuantityType> quantity_alphabeta{};
    std::transform(scalar_alphabeta.begin(), scalar_alphabeta.end(), quantity_alphabeta.begin(),
                   [](real_t val) { return QuantityType{val}; });
    return quantity_alphabeta;
}

template <typename QuantityType>
dq<QuantityType> to_dimension_dq(const dq<real_t>& scalar_dq)
{
    dq<QuantityType> quantity_dq{};
    std::transform(scalar_dq.begin(), scalar_dq.end(), quantity_dq.begin(),
                   [](real_t val) { return QuantityType{val}; });
    return quantity_dq;
}

template <typename QuantityType>
abc<real_t> to_dimensionless_abc(const abc<QuantityType>& quantity_abc)
{
    abc<real_t> scalar_abc{};
    std::transform(quantity_abc.begin(), quantity_abc.end(), scalar_abc.begin(),
                   [](const QuantityType& val) { return val.value(); });
    return scalar_abc;
}

template <typename QuantityType>
alphabeta<real_t> to_dimensionless_alphabeta(const alphabeta<QuantityType>& quantity_alphabeta)
{
    alphabeta<real_t> scalar_alphabeta{};
    std::transform(quantity_alphabeta.begin(), quantity_alphabeta.end(), scalar_alphabeta.begin(),
                   [](const QuantityType& val) { return val.value(); });
    return scalar_alphabeta;
}

template <typename QuantityType>
dq<real_t> to_dimensionless_dq(const dq<QuantityType>& quantity_dq)
{
    dq<real_t> scalar_dq{};
    std::transform(quantity_dq.begin(), quantity_dq.end(), scalar_dq.begin(),
                   [](const QuantityType& val) { return val.value(); });
    return scalar_dq;
}

/**
 * @brief Line-to-line representation of three-phase quantities.
 */
template <typename T = real_t>
struct line_voltage {
    using value_type = T;

    constexpr line_voltage() = default;
    constexpr line_voltage(T v_ab, T v_bc, T v_ca) : _ab(v_ab), _bc(v_bc), _ca(v_ca) {}

    constexpr explicit line_voltage(const abc<T>& phase)
        : _ab(phase.a() - phase.b()), _bc(phase.b() - phase.c()), _ca(phase.c() - phase.a())
    {
    }

    constexpr abc<T> to_phase_balanced() const
    {
        const T a = (real_t{2.0 / 3.0} * _ab) + (real_t{1.0 / 3.0} * _bc);
        const T b = -(real_t{1.0 / 3.0} * _ab) + (real_t{1.0 / 3.0} * _bc);
        const T c = -(real_t{1.0 / 3.0} * _ab) - (real_t{2.0 / 3.0} * _bc);
        return abc<T>{a, b, c};
    }

    T ab() const { return _ab; }
    T bc() const { return _bc; }
    T ca() const { return _ca; }

    T& ab() { return _ab; }
    T& bc() { return _bc; }
    T& ca() { return _ca; }

private:
    T _ab{};
    T _bc{};
    T _ca{};
};

template <typename T>
constexpr line_voltage<T> to_line_voltage(const abc<T>& phase)
{
    return line_voltage<T>{phase};
}

template <typename T>
abc<T> to_line(abc<T> phase)
{
    return abc<T>{phase.a() - phase.b(), phase.b() - phase.c(), phase.c() - phase.a()};
}

template <typename T>
constexpr abc<T> from_line(T v_ab, T v_bc)
{
    const T a = (real_t{2.0 / 3.0} * v_ab) + (real_t{1.0 / 3.0} * v_bc);
    const T b = -(real_t{1.0 / 3.0} * v_ab) + (real_t{1.0 / 3.0} * v_bc);
    const T c = -(real_t{1.0 / 3.0} * v_ab) - (real_t{2.0 / 3.0} * v_bc);
    return abc<T>{a, b, c};
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

#endif  // mojito_COORDINATES_HPP
