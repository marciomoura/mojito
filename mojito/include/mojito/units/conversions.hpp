#pragma once

#include "systems.hpp"
#include "quantity.hpp"
#include "../divisor.hpp"

namespace mojito {

// SI <-> Per-Unit
template <typename Dim>
constexpr auto to_si(const quantity<Dim, per_unit>& pu_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, si>(pu_quantity.value() * base_quantity.value());
}

template <typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, per_unit>(si_quantity.value() / base_quantity.value());
}

template <typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const divisor<quantity<Dim, si>>& divisor)
{
    return quantity<Dim, per_unit>(si_quantity.value() * divisor.reciprocal());
}

template <typename Dim>
constexpr auto to_si(const quantity<Dim, per_unit>& pu_quantity, const divisor<quantity<Dim, si>>& divisor)
{
    return quantity<Dim, si>(pu_quantity.value() * divisor.value().value());
}

// SI <-> Percent
template <typename Dim>
constexpr auto to_si(const quantity<Dim, percent>& percent_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, si>((percent_quantity.value() / real_t{100.0}) * base_quantity.value());
}

template <typename Dim>
constexpr auto to_percent(const quantity<Dim, si>& si_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, percent>((si_quantity.value() / base_quantity.value()) * real_t{100.0});
}

// Per-Unit <-> Percent
template <typename Dim>
constexpr auto to_percent(const quantity<Dim, per_unit>& pu_quantity)
{
    return quantity<Dim, percent>(pu_quantity.value() * real_t{100.0});
}

template <typename Dim>
constexpr auto to_pu(const quantity<Dim, percent>& percent_quantity)
{
    return quantity<Dim, per_unit>(percent_quantity.value() / real_t{100.0});
}

// Casting
template <typename NewType, typename OldDim>
constexpr quantity<typename NewType::dimension, per_unit> per_unit_cast(const quantity<OldDim, per_unit>& q)
{
    static_assert(std::is_same_v<typename NewType::system, per_unit>, "NewType must be a per-unit quantity type.");
    return quantity<typename NewType::dimension, per_unit>(q.value());
}

template <typename NewType, typename OldDim>
constexpr quantity<typename NewType::dimension, percent> percent_cast(const quantity<OldDim, percent>& q)
{
    static_assert(std::is_same_v<typename NewType::system, percent>, "NewType must be a percent quantity type.");
    return quantity<typename NewType::dimension, percent>(q.value());
}

}  // namespace mojito
