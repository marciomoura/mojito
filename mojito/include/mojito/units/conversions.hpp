#pragma once

#include "systems.hpp"
#include "quantity.hpp"
#include "../divisor.hpp"

namespace mojito {

// SI <-> Per-Unit
template <typename Dim, typename Base>
constexpr auto to_si(const quantity<Dim, per_unit<Base>>& pu_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, si>(pu_quantity.value() * base_quantity.value());
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const quantity<Dim, si>& base_quantity)
{
    return quantity<Dim, per_unit<Base>>(si_quantity.value() / base_quantity.value());
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const quantity<Dim, si>& si_quantity, const divisor<quantity<Dim, si>>& divisor)
{
    return quantity<Dim, per_unit<Base>>(si_quantity.value() * divisor.reciprocal());
}

template <typename Dim, typename Base>
constexpr auto to_si(const quantity<Dim, per_unit<Base>>& pu_quantity, const divisor<quantity<Dim, si>>& divisor)
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
template <typename NewBase, typename OldBase, typename Dim>
constexpr auto to_different_pu(const quantity<Dim, per_unit<OldBase>>& pu_quantity,
                               const quantity<Dim, si>& old_base_quantity, const quantity<Dim, si>& new_base_quantity)
{
    return to_pu<NewBase>(to_si(pu_quantity, old_base_quantity), new_base_quantity);
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
