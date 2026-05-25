#pragma once

#include "systems.hpp"
#include "quantity.hpp"
#include "../divisor.hpp"

namespace mojito {

// SI/Physical <-> Per-Unit
template <typename Dim, typename System, typename Base>
constexpr auto to_si(const quantity<Dim, per_unit<Base>>& pu_quantity, const quantity<Dim, System>& base_quantity)
{
    return quantity<Dim, System>(pu_quantity.value() * base_quantity.value());
}

template <typename Base = default_base, typename Dim, typename System>
constexpr auto to_pu(const quantity<Dim, System>& si_quantity, const quantity<Dim, System>& base_quantity)
{
    return quantity<Dim, per_unit<Base>>(si_quantity.value() / base_quantity.value());
}

template <typename Base = default_base, typename Dim, typename System>
constexpr auto to_pu(const quantity<Dim, System>& si_quantity, const divisor<quantity<Dim, System>>& divisor)
{
    return quantity<Dim, per_unit<Base>>(si_quantity.value() * divisor.reciprocal());
}

template <typename Dim, typename System, typename Base>
constexpr auto to_si(const quantity<Dim, per_unit<Base>>& pu_quantity, const divisor<quantity<Dim, System>>& divisor)
{
    return quantity<Dim, System>(pu_quantity.value() * divisor.value().value());
}

// SI/Physical <-> Percent
template <typename Dim, typename System>
constexpr auto to_si(const quantity<Dim, percent>& percent_quantity, const quantity<Dim, System>& base_quantity)
{
    return quantity<Dim, System>((percent_quantity.value() / real_t{100.0}) * base_quantity.value());
}

template <typename Dim, typename System>
constexpr auto to_percent(const quantity<Dim, System>& si_quantity, const quantity<Dim, System>& base_quantity)
{
    return quantity<Dim, percent>((si_quantity.value() / base_quantity.value()) * real_t{100.0});
}

// Per-Unit <-> Percent
template <typename Dim, typename Base>
constexpr auto to_percent(const quantity<Dim, per_unit<Base>>& pu_quantity)
{
    return quantity<Dim, percent>(pu_quantity.value() * real_t{100.0});
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const quantity<Dim, percent>& percent_quantity)
{
    return quantity<Dim, per_unit<Base>>(percent_quantity.value() / real_t{100.0});
}

// Per-Unit <-> Per-Unit
template <typename NewBase, typename OldBase, typename Dim, typename System>
constexpr auto to_different_pu(const quantity<Dim, per_unit<OldBase>>& pu_quantity,
                               const quantity<Dim, System>& old_base_quantity, const quantity<Dim, System>& new_base_quantity)
{
    return to_pu<NewBase>(to_si(pu_quantity, old_base_quantity), new_base_quantity);
}

// Time conversions
template <typename Unit>
struct time_conversion_factor;

template <>
struct time_conversion_factor<si> {
    static constexpr real_t value = real_t{1.0};
};

template <>
struct time_conversion_factor<microseconds> {
    static constexpr real_t value = real_t{1e-6};
};

template <>
struct time_conversion_factor<milliseconds> {
    static constexpr real_t value = real_t{1e-3};
};

template <>
struct time_conversion_factor<minutes> {
    static constexpr real_t value = real_t{60.0};
};

template <>
struct time_conversion_factor<hours> {
    static constexpr real_t value = real_t{3600.0};
};

namespace internal {
template <typename T, typename = void>
struct extract_system {
    using type = T;
};

template <typename T>
struct extract_system<T, std::void_t<typename T::system>> {
    using type = typename T::system;
};

template <typename T>
using extract_system_t = typename extract_system<T>::type;
} // namespace internal

template <typename To, typename FromSystem>
constexpr auto time_cast(const quantity<time_dim, FromSystem>& q)
{
    using ToSystem = internal::extract_system_t<To>;
    constexpr real_t factor_from = time_conversion_factor<FromSystem>::value;
    constexpr real_t factor_to = time_conversion_factor<ToSystem>::value;
    return quantity<time_dim, ToSystem>(q.value() * (factor_from / factor_to));
}

// Convenience converters
template <typename FromSystem>
constexpr auto to_us(const quantity<time_dim, FromSystem>& q)
{
    return time_cast<microseconds>(q);
}

template <typename FromSystem>
constexpr auto to_ms(const quantity<time_dim, FromSystem>& q)
{
    return time_cast<milliseconds>(q);
}

template <typename FromSystem>
constexpr auto to_seconds(const quantity<time_dim, FromSystem>& q)
{
    return time_cast<seconds>(q);
}

template <typename FromSystem>
constexpr auto to_minutes(const quantity<time_dim, FromSystem>& q)
{
    return time_cast<minutes>(q);
}

template <typename FromSystem>
constexpr auto to_hours(const quantity<time_dim, FromSystem>& q)
{
    return time_cast<hours>(q);
}

// Casting
template <typename NewType, typename OldDim, typename OldBase>
constexpr auto per_unit_cast(const quantity<OldDim, per_unit<OldBase>>& q)
{
    static_assert(is_per_unit_v<typename NewType::system>, "NewType must be a per-unit quantity type.");
    return quantity<typename NewType::dimension, typename NewType::system>(q.value());
}

template <typename NewType, typename OldDim>
constexpr quantity<typename NewType::dimension, percent> percent_cast(const quantity<OldDim, percent>& q)
{
    static_assert(std::is_same_v<typename NewType::system, percent>, "NewType must be a percent quantity type.");
    return quantity<typename NewType::dimension, percent>(q.value());
}

}  // namespace mojito
