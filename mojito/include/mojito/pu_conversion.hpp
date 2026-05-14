#ifndef MOJITO_PU_CONVERSION_HPP
#define MOJITO_PU_CONVERSION_HPP

#include "abc.hpp"
#include "alphabeta.hpp"
#include "dq.hpp"
#include "divisor.hpp"
#include "units.hpp"

namespace mojito {

template <typename Dim, typename Base>
constexpr auto to_si(const abc<quantity<Dim, per_unit<Base>>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.a(), base_quantity));
    return abc<result_quantity_t>(to_si(pu_frame.a(), base_quantity), to_si(pu_frame.b(), base_quantity),
                                  to_si(pu_frame.c(), base_quantity));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const abc<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.a(), base_quantity));
    return abc<result_quantity_t>(to_pu<Base>(si_frame.a(), base_quantity), to_pu<Base>(si_frame.b(), base_quantity),
                                  to_pu<Base>(si_frame.c(), base_quantity));
}

template <typename Dim, typename Base>
constexpr auto to_si(const alphabeta<quantity<Dim, per_unit<Base>>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.alpha(), base_quantity));
    return alphabeta<result_quantity_t>(to_si(pu_frame.alpha(), base_quantity), to_si(pu_frame.beta(), base_quantity));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const alphabeta<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.alpha(), base_quantity));
    return alphabeta<result_quantity_t>(to_pu<Base>(si_frame.alpha(), base_quantity), to_pu<Base>(si_frame.beta(), base_quantity));
}

template <typename Dim, typename Base>
constexpr auto to_si(const dq<quantity<Dim, per_unit<Base>>>& pu_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_si(pu_frame.d(), base_quantity));
    return dq<result_quantity_t>(to_si(pu_frame.d(), base_quantity), to_si(pu_frame.q(), base_quantity));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const dq<quantity<Dim, si>>& si_frame, const quantity<Dim, si>& base_quantity)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.d(), base_quantity));
    return dq<result_quantity_t>(to_pu<Base>(si_frame.d(), base_quantity), to_pu<Base>(si_frame.q(), base_quantity));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const abc<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.a(), divisor));
    return abc<result_quantity_t>(to_pu<Base>(si_frame.a(), divisor), to_pu<Base>(si_frame.b(), divisor),
                                  to_pu<Base>(si_frame.c(), divisor));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const alphabeta<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.alpha(), divisor));
    return alphabeta<result_quantity_t>(to_pu<Base>(si_frame.alpha(), divisor), to_pu<Base>(si_frame.beta(), divisor));
}

template <typename Base = default_base, typename Dim>
constexpr auto to_pu(const dq<quantity<Dim, si>>& si_frame, const divisor<quantity<Dim, si>>& divisor)
{
    using result_quantity_t = decltype(to_pu<Base>(si_frame.d(), divisor));
    return dq<result_quantity_t>(to_pu<Base>(si_frame.d(), divisor), to_pu<Base>(si_frame.q(), divisor));
}

template <typename NewBase, typename OldBase, typename Dim, template <typename> class Frame>
constexpr auto to_different_pu(const Frame<quantity<Dim, per_unit<OldBase>>>& pu_frame,
                               const quantity<Dim, si>& old_base_quantity, const quantity<Dim, si>& new_base_quantity)
{
    return to_pu<NewBase>(to_si(pu_frame, old_base_quantity), new_base_quantity);
}

}  // namespace mojito

#endif  // MOJITO_PU_CONVERSION_HPP
