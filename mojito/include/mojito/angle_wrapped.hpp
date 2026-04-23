#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <ostream>

#include "types.hpp"
#include "units.hpp"

namespace mojito {

static constexpr real_t pi = real_t{3.1415926535897932385};
static constexpr real_t two_pi = real_t{2.0 * pi};

/**
 * @brief A class representing an angle with automatic range wrapping to [0, 2π] using uint32_t overflow
 *
 * This class provides a highly efficient, deterministic way to work with angles by leveraging
 * the natural overflow behavior of uint32_t arithmetic. The full range of uint32_t (0 to 2^32-1)
 * maps directly to the angle range [0, 2π), providing:
 *
 * - **Zero-cost wrapping**: No modulo operations or conditionals needed
 * - **Perfect determinism**: All operations have constant time complexity
 * - **High resolution**: ~1.46e-9 radians (~8.38e-8 degrees) per LSB
 * - **Exact binary angles**: Values like π/2, π, 3π/2 can be represented exactly
 *
 * The mapping is: uint32_value / 2^32 * 2π = angle_in_radians
 *
 * All angles are internally stored as uint32_t and automatically wrap via natural overflow.
 * This is particularly useful for embedded systems and real-time control applications.
 */
class angle_wrapped {
public:
    /**
     * @brief Creates an angle from degrees
     *
     * @param degrees The angle value in degrees
     * @return A new angle object with the specified degree value
     */
    static constexpr angle_wrapped from_degrees(real_t degrees)
    {
        return angle_wrapped(degrees * static_cast<real_t>(pi / 180.0));
    }

    /**
     * @brief Creates an angle from radians
     *
     * @param radians The angle value in radians
     * @return A new angle object with the specified radian value
     */
    static constexpr angle_wrapped from_radians(angle_t radians) { return angle_wrapped(radians); }

    constexpr angle_wrapped() = default;
    constexpr angle_wrapped(angle_wrapped const&) = default;
    constexpr angle_wrapped(angle_wrapped&&) = default;
    constexpr angle_wrapped& operator=(angle_wrapped const&) = default;
    constexpr angle_wrapped& operator=(angle_wrapped&&) = default;
    ~angle_wrapped() = default;

    /**
     * @brief Constructs an angle with the specified value
     *
     * The input value will be automatically wrapped to the [0, 2π] range.
     *
     * @param value The angle value in radians
     */
    constexpr angle_wrapped(real_t value) : _value(radians_to_uint32(angle_t{value})) {}

    /**
     * @brief Constructs an angle with the specified value
     *
     * The input value will be automatically wrapped to the [0, 2π] range.
     *
     * @param value The angle value in radians
     */
    constexpr angle_wrapped(angle_t value) : _value(radians_to_uint32(value)) {}

    static constexpr angle_t deg_to_rad(real_t degrees) { return angle_t{degrees * pi / real_t(180.0)}; }

    static constexpr real_t rad_to_deg(angle_t radians) { return radians.value() * real_t(180.0) / pi; }

    /**
     * @brief Sets the angle value with automatic range wrapping
     *
     * @param value The new angle value in radians
     */
    constexpr void set_value(angle_t value) { _value = radians_to_uint32(value); }

    /**
     * @brief Gets the angle value in degrees
     *
     * @return The angle in degrees, wrapped to the [0, 360] range
     */
    constexpr real_t get_degrees() const noexcept { return rad_to_deg(uint32_to_radians(_value)); }

    /**
     * @brief Gets the angle value in radians
     *
     * @return The angle in radians, wrapped to the [0, 2π] range
     */
    constexpr angle_t get_radians() const noexcept { return uint32_to_radians(_value); }

    /**
     * @brief Gets the angle value in per unit
     *
     * @return The angle in per unit, normalized to [0, 1] range where 0 corresponds to 0 and 1 to 2π
     */
    constexpr angle_pu_t get_pu() const noexcept
    {
        // Direct conversion: uint32 value naturally represents [0, 1) when scaled by 2^32
        return angle_pu_t{static_cast<real_t>(_value) / static_cast<real_t>(UINT32_MAX)};
    }

    /**
     * @brief Gets the angle value in radians
     *
     * This method allows the angle object to be used as a function object.
     *
     * @return The angle in radians, wrapped to the [0, 2π] range
     */
    constexpr operator angle_t() const noexcept { return uint32_to_radians(_value); }

    /**
     * @brief Converts the angle to float (radians)
     *
     * Enables static_cast<float>(angle_wrapped_value) for convenient float conversion.
     * The returned value represents the angle in radians, wrapped to [0, 2π].
     *
     * @return The angle in radians as a float, wrapped to the [0, 2π] range
     */
    constexpr explicit operator real_t() const noexcept
    {
        return static_cast<real_t>(uint32_to_radians(_value).value());
    }

    /**
     * @brief Gets the angle value in radians
     *
     * This method allows the angle object to be used as a function object.
     *
     * @return The angle in radians, wrapped to the [0, 2π] range
     */
    constexpr angle_t operator()() const noexcept { return uint32_to_radians(_value); }

    /**
     * @brief Negation operator for the angle value
     *
     * @return A new angle representing the negated value, wrapped to [0, 2π]
     */
    constexpr angle_wrapped operator-() const
    {
        // Negation leverages uint32 overflow: -x wraps naturally
        angle_wrapped result;
        result._value = static_cast<uint32_t>(-static_cast<int32_t>(_value));
        return result;
    }

    // ==================== Arithmetic Operations with angle_wrapped ====================

    /**
     * @brief Adds two angles
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param other The angle to add
     * @return A new angle representing the sum
     */
    constexpr angle_wrapped operator+(const angle_wrapped& other) const
    {
        angle_wrapped result;
        result._value = _value + other._value;  // Overflow wraps naturally
        return result;
    }

    /**
     * @brief Subtracts two angles
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param other The angle to subtract
     * @return A new angle representing the difference
     */
    constexpr angle_wrapped operator-(const angle_wrapped& other) const
    {
        angle_wrapped result;
        result._value = _value - other._value;  // Underflow wraps naturally
        return result;
    }

    /**
     * @brief Adds an angle to this one
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param other The angle to add
     * @return Reference to this angle after addition
     */
    constexpr angle_wrapped& operator+=(const angle_wrapped& other)
    {
        _value += other._value;  // Overflow wraps naturally
        return *this;
    }

    /**
     * @brief Subtracts an angle from this one
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param other The angle to subtract
     * @return Reference to this angle after subtraction
     */
    constexpr angle_wrapped& operator-=(const angle_wrapped& other)
    {
        _value -= other._value;  // Underflow wraps naturally
        return *this;
    }

    // ==================== Arithmetic Operations with angle_t ====================

    /**
     * @brief Adds an angle_t value to this angle
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param angle_value The angle_t value to add
     * @return A new angle representing the sum
     */
    constexpr angle_wrapped operator+(const angle_t& angle_value) const
    {
        return angle_wrapped(uint32_to_radians(_value) + angle_value);
    }

    /**
     * @brief Subtracts an angle_t value from this angle
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param angle_value The angle_t value to subtract
     * @return A new angle representing the difference
     */
    constexpr angle_wrapped operator-(const angle_t& angle_value) const
    {
        return angle_wrapped(uint32_to_radians(_value) - angle_value);
    }

    /**
     * @brief Adds an angle_t value to this angle
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param angle_value The angle_t value to add
     * @return Reference to this angle after addition
     */
    constexpr angle_wrapped& operator+=(const angle_t& angle_value)
    {
        set_value(uint32_to_radians(_value) + angle_value);
        return *this;
    }

    /**
     * @brief Subtracts an angle_t value from this angle
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param angle_value The angle_t value to subtract
     * @return Reference to this angle after subtraction
     */
    constexpr angle_wrapped& operator-=(const angle_t& angle_value)
    {
        set_value(uint32_to_radians(_value) - angle_value);
        return *this;
    }

    // ==================== Scalar Operations with angle_t ====================

    /**
     * @brief Multiplies the angle by an angle_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The angle_t scalar value to multiply by
     * @return A new angle representing the product
     */
    constexpr angle_wrapped operator*(const angle_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) * scalar);
    }

    /**
     * @brief Divides the angle by an angle_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The angle_t scalar value to divide by
     * @return A new angle representing the quotient
     */
    constexpr angle_wrapped operator/(const angle_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) / scalar);
    }

    /**
     * @brief Multiplies this angle by an angle_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The angle_t scalar value to multiply by
     * @return Reference to this angle after multiplication
     */
    constexpr angle_wrapped& operator*=(const angle_t& scalar)
    {
        set_value(uint32_to_radians(_value) * scalar);
        return *this;
    }

    /**
     * @brief Divides this angle by an angle_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The angle_t scalar value to divide by
     * @return Reference to this angle after division
     */
    constexpr angle_wrapped& operator/=(const angle_t& scalar)
    {
        set_value(uint32_to_radians(_value) / scalar);
        return *this;
    }

    // ==================== Scalar Operations with real_t ====================

    /**
     * @brief Multiplies the angle by a real_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The real_t scalar value to multiply by
     * @return A new angle representing the product
     */
    constexpr angle_wrapped operator*(const real_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) * scalar);
    }

    /**
     * @brief Divides the angle by a real_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The real_t scalar value to divide by
     * @return A new angle representing the quotient
     */
    constexpr angle_wrapped operator/(const real_t& scalar) const
    {
        return angle_wrapped(uint32_to_radians(_value) / scalar);
    }

    /**
     * @brief Multiplies this angle by a real_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The real_t scalar value to multiply by
     * @return Reference to this angle after multiplication
     */
    constexpr angle_wrapped& operator*=(const real_t& scalar)
    {
        set_value(uint32_to_radians(_value) * scalar);
        return *this;
    }

    /**
     * @brief Divides this angle by a real_t scalar
     *
     * The result is automatically wrapped to the [0, 2π] range.
     *
     * @param scalar The real_t scalar value to divide by
     * @return Reference to this angle after division
     */
    constexpr angle_wrapped& operator/=(const real_t& scalar)
    {
        set_value(uint32_to_radians(_value) / scalar);
        return *this;
    }

    // ==================== Comparison Operations ====================

    /**
     * @brief Compares two angles for equality
     *
     * Angles are considered equal if they differ by less than epsilon.
     *
     * @param other The angle to compare with
     * @return true if the angles are equal, false otherwise
     */
    constexpr bool operator==(const angle_wrapped& other) const noexcept
    {
        // For uint32-based angles, we need to check if the difference is small
        // Handle wrapping around zero
        uint32_t diff = (_value > other._value) ? (_value - other._value) : (other._value - _value);

        // Check if the difference is within epsilon
        // Epsilon of 1e-10 radians corresponds to approximately 682 uint32 units
        constexpr uint32_t epsilon_units = 682;  // ~1e-10 radians

        return diff <= epsilon_units;
    }

    /**
     * @brief Compares two angles for inequality
     *
     * @param other The angle to compare with
     * @return true if the angles are not equal, false otherwise
     */
    constexpr bool operator!=(const angle_wrapped& other) const noexcept { return !(*this == other); }

    /**
     * @brief Compares two angles for less than
     *
     * This comparison is based on the wrapped angle value in radians.
     *
     * @param other The angle to compare with
     * @return true if this angle is less than the other, false otherwise
     */
    constexpr bool operator<(const angle_wrapped& other) const noexcept { return _value < other._value; }

    /**
     * @brief Compares two angles for less than or equal to
     *
     * @param other The angle to compare with
     * @return true if this angle is less than or equal to the other, false otherwise
     */
    constexpr bool operator<=(const angle_wrapped& other) const noexcept { return *this < other || *this == other; }

    /**
     * @brief Compares two angles for greater than
     *
     * @param other The angle to compare with
     * @return true if this angle is greater than the other, false otherwise
     */
    constexpr bool operator>(const angle_wrapped& other) const noexcept { return other < *this; }

    /**
     * @brief Compares two angles for greater than or equal to
     *
     * @param other The angle to compare with
     * @return true if this angle is greater than or equal to the other, false otherwise
     */
    constexpr bool operator>=(const angle_wrapped& other) const noexcept { return *this > other || *this == other; }

private:
    /**
     * @brief Converts radians to uint32_t representation
     *
     * The mapping is: uint32_value = (angle_radians / 2π) * 2^32
     * This automatically wraps negative values and values > 2π
     *
     * @param radians The angle in radians
     * @return The uint32_t representation
     */
    static constexpr uint32_t radians_to_uint32(angle_t radians)
    {
        // First normalize to [0, 2π) range
        real_t normalized = radians.value();

        // Handle negative angles and angles >= 2π
        if (normalized < 0.0 || normalized >= two_pi) {
            normalized = normalized - two_pi * std::floor(normalized / two_pi);
        }

        // Convert to uint32: (angle / 2π) * 2^32
        // Using 2^32 / 2π as the scale factor
        constexpr real_t scale = static_cast<real_t>(4294967296.0) / two_pi;  // 2^32 / 2π

        return static_cast<uint32_t>(normalized * scale);
    }

    /**
     * @brief Converts uint32_t representation to radians
     *
     * The mapping is: angle_radians = (uint32_value / 2^32) * 2π
     *
     * @param value The uint32_t value
     * @return The angle in radians
     */
    static constexpr angle_t uint32_to_radians(uint32_t value)
    {
        // Convert from uint32: (value / 2^32) * 2π
        // Using 2π / 2^32 as the scale factor
        constexpr real_t scale = two_pi / static_cast<real_t>(4294967296.0);  // 2π / 2^32

        return angle_t{static_cast<real_t>(value) * scale};
    }

    uint32_t _value{0};  ///< The stored angle value as uint32, maps [0, 2^32-1] to [0, 2π)
};

// ==================== Free Function Operators for Commutative Operations ====================

/**
 * @brief Multiplies a real_t scalar by an angle_wrapped
 *
 * This enables expressions like: real_t{2.0} * angle_wrapped{pi}
 *
 * @param scalar The real_t scalar value
 * @param angle The angle to multiply
 * @return A new angle representing the product
 */
constexpr angle_wrapped operator*(const real_t& scalar, const angle_wrapped& angle) { return angle * scalar; }

/**
 * @brief Multiplies an angle_t scalar by an angle_wrapped
 *
 * This enables expressions like: angle_t{2.0} * angle_wrapped{pi}
 *
 * @param scalar The angle_t scalar value
 * @param angle The angle to multiply
 * @return A new angle representing the product
 */
constexpr angle_wrapped operator*(const angle_t& scalar, const angle_wrapped& angle) { return angle * scalar; }

/**
 * @brief Adds an angle_t value to an angle_wrapped
 *
 * This enables expressions like: angle_t{pi} + angle_wrapped{pi/2}
 *
 * @param angle_value The angle_t value
 * @param angle The angle_wrapped to add to
 * @return A new angle representing the sum
 */
constexpr angle_wrapped operator+(const angle_t& angle_value, const angle_wrapped& angle)
{
    return angle + angle_value;
}

/**
 * @brief Stream insertion operator for angle_wrapped
 *
 * Prints the angle in radians.
 */
inline std::ostream& operator<<(std::ostream& os, const angle_wrapped& angle)
{
    os << angle.get_radians();
    return os;
}

}  // namespace mojito
