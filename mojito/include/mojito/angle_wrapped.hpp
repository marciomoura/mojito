#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#if MOJITO_HAS_IOSTREAM
#include <ostream>
#endif

#include "constants.hpp"
#include "types.hpp"
#include "units.hpp"

namespace mojito {

/**
 * @brief Represents an angle with automatic range wrapping to [0, 2π] using uint32_t overflow.
 *
 * This class leverages uint32_t arithmetic to achieve zero-cost wrapping.
 * The full range [0, 2^32-1] maps to [0, 2π).
 */
class angle_wrapped {
public:
    static constexpr angle_wrapped from_degrees(real_t degrees)
    {
        return angle_wrapped(degrees * (pi / real_t{180.0}));
    }

    static constexpr angle_wrapped from_radians(angle_t radians) { return angle_wrapped(radians); }

    constexpr angle_wrapped() = default;

    constexpr angle_wrapped(real_t value) : _value(radians_to_uint32(angle_t{value})) {}

    constexpr angle_wrapped(angle_t value) : _value(radians_to_uint32(value)) {}

    static constexpr angle_t deg_to_rad(real_t degrees) { return angle_t{degrees * pi / real_t{180.0}}; }

    static constexpr real_t rad_to_deg(angle_t radians) { return radians.value() * real_t{180.0} / pi; }

    constexpr void set_value(angle_t value) { _value = radians_to_uint32(value); }

    constexpr real_t get_degrees() const noexcept { return rad_to_deg(uint32_to_radians(_value)); }

    constexpr angle_t get_radians() const noexcept { return uint32_to_radians(_value); }

    constexpr angle_pu_t get_pu() const noexcept
    {
        return angle_pu_t{static_cast<real_t>(_value) / static_cast<real_t>(UINT32_MAX)};
    }

    constexpr operator angle_t() const noexcept { return uint32_to_radians(_value); }

    constexpr explicit operator real_t() const noexcept
    {
        return static_cast<real_t>(uint32_to_radians(_value).value());
    }

    constexpr angle_t operator()() const noexcept { return uint32_to_radians(_value); }

    constexpr angle_wrapped operator-() const
    {
        angle_wrapped result;
        result._value = static_cast<uint32_t>(-static_cast<int32_t>(_value));
        return result;
    }

    constexpr angle_wrapped operator+(const angle_wrapped& other) const
    {
        angle_wrapped result;
        result._value = _value + other._value;
        return result;
    }

    constexpr angle_wrapped operator-(const angle_wrapped& other) const
    {
        angle_wrapped result;
        result._value = _value - other._value;
        return result;
    }

    constexpr angle_wrapped& operator+=(const angle_wrapped& other)
    {
        _value += other._value;
        return *this;
    }

    constexpr angle_wrapped& operator-=(const angle_wrapped& other)
    {
        _value -= other._value;
        return *this;
    }

    constexpr angle_wrapped operator+(const angle_t& angle_value) const
    {
        return angle_wrapped(uint32_to_radians(_value) + angle_value);
    }

    constexpr angle_wrapped operator-(const angle_t& angle_value) const
    {
        return angle_wrapped(uint32_to_radians(_value) - angle_value);
    }

    constexpr angle_wrapped& operator+=(const angle_t& angle_value)
    {
        set_value(uint32_to_radians(_value) + angle_value);
        return *this;
    }

    constexpr angle_wrapped& operator-=(const angle_t& angle_value)
    {
        set_value(uint32_to_radians(_value) - angle_value);
        return *this;
    }

    constexpr angle_wrapped operator*(const angle_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) * scalar);
    }

    constexpr angle_wrapped operator/(const angle_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) / scalar);
    }

    constexpr angle_wrapped& operator*=(const angle_t& scalar)
    {
        set_value(uint32_to_radians(_value) * scalar);
        return *this;
    }

    constexpr angle_wrapped& operator/=(const angle_t& scalar)
    {
        set_value(uint32_to_radians(_value) / scalar);
        return *this;
    }

    constexpr angle_wrapped operator*(const real_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) * scalar);
    }

    constexpr angle_wrapped operator/(const real_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) / scalar);
    }

    constexpr angle_wrapped& operator*=(const real_t& scalar)
    {
        set_value(uint32_to_radians(_value) * scalar);
        return *this;
    }

    constexpr angle_wrapped& operator/=(const real_t& scalar)
    {
        set_value(uint32_to_radians(_value) / scalar);
        return *this;
    }

    constexpr bool operator==(const angle_wrapped& other) const noexcept
    {
        uint32_t diff = (_value > other._value) ? (_value - other._value) : (other._value - _value);
        // Epsilon of ~1e-10 radians corresponds to approximately 682 uint32 units
        constexpr uint32_t epsilon_units = 682;
        return diff <= epsilon_units;
    }

    constexpr bool operator!=(const angle_wrapped& other) const noexcept { return !(*this == other); }

    constexpr bool operator<(const angle_wrapped& other) const noexcept { return _value < other._value; }

    constexpr bool operator<=(const angle_wrapped& other) const noexcept { return *this < other || *this == other; }

    constexpr bool operator>(const angle_wrapped& other) const noexcept { return other < *this; }

    constexpr bool operator>=(const angle_wrapped& other) const noexcept { return *this > other || *this == other; }

private:
    static constexpr uint32_t radians_to_uint32(angle_t radians)
    {
        real_t normalized = radians.value();
        if (normalized < real_t{0.0} || normalized >= two_pi) {
            normalized = normalized - two_pi * std::floor(normalized / two_pi);
        }
        constexpr real_t scale = real_t{4294967296.0} / two_pi;
        return static_cast<uint32_t>(normalized * scale);
    }

    static constexpr angle_t uint32_to_radians(uint32_t value)
    {
        constexpr real_t scale = two_pi / real_t{4294967296.0};
        return angle_t{static_cast<real_t>(value) * scale};
    }

    uint32_t _value{0};
};

constexpr angle_wrapped operator*(const real_t& scalar, const angle_wrapped& angle) { return angle * scalar; }

constexpr angle_wrapped operator*(const angle_t& scalar, const angle_wrapped& angle) { return angle * scalar; }

constexpr angle_wrapped operator+(const angle_t& angle_value, const angle_wrapped& angle)
{
    return angle + angle_value;
}

#if MOJITO_HAS_IOSTREAM
inline std::ostream& operator<<(std::ostream& os, const angle_wrapped& angle)
{
    os << angle.get_radians();
    return os;
}
#endif

}  // namespace mojito
