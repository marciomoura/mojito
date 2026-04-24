#ifndef MOJITO_TRANSFORM_HPP
#define MOJITO_TRANSFORM_HPP

#include <algorithm>
#include <cmath>

#include "ab_bc_ca.hpp"
#include "abc.hpp"
#include "alphabeta.hpp"
#include "angle_wrapped.hpp"
#include "constants.hpp"
#include "divisor.hpp"
#include "dq.hpp"

namespace mojito {

// --- Clarke Transforms (abc <-> alphabeta) ---

template <typename T>
constexpr alphabeta<T> to_alphabeta(const abc<T>& val)
{
    return {real_t{2.0 / 3.0} * (val.a() - real_t{0.5} * val.b() - real_t{0.5} * val.c()),
            (sqrt_3 / real_t{3.0}) * (val.b() - val.c())};
}

template <typename T>
constexpr abc<T> to_abc(const alphabeta<T>& val)
{
    return abc<T>{val.alpha(), real_t{-0.5} * val.alpha() + (sqrt_3 / real_t{2.0}) * val.beta(),
                  real_t{-0.5} * val.alpha() - (sqrt_3 / real_t{2.0}) * val.beta()};
}

// --- Park Transforms (alphabeta <-> dq) ---

template <typename T>
constexpr dq<T> to_dq(const alphabeta<T>& val, const angle_wrapped& theta)
{
    const real_t cos_theta = std::cos(theta.get_radians());
    const real_t sin_theta = std::sin(theta.get_radians());
    return dq<T>{cos_theta * val.alpha() + sin_theta * val.beta(), -sin_theta * val.alpha() + cos_theta * val.beta()};
}

template <typename T>
constexpr alphabeta<T> to_alphabeta(const dq<T>& val, const angle_wrapped& theta)
{
    const real_t cos_theta = std::cos(theta.get_radians());
    const real_t sin_theta = std::sin(theta.get_radians());
    return alphabeta<T>{cos_theta * val.d() - sin_theta * val.q(), sin_theta * val.d() + cos_theta * val.q()};
}

// --- Composite Transforms (abc <-> dq) ---

template <typename T>
constexpr dq<T> to_dq(const abc<T>& val, const angle_wrapped& theta)
{
    return to_dq(to_alphabeta(val), theta);
}

template <typename T>
constexpr abc<T> to_abc(const dq<T>& val, const angle_wrapped& theta)
{
    return to_abc(to_alphabeta(val, theta));
}

// --- Line-to-Line Transforms ---

template <typename T>
constexpr ab_bc_ca<T> to_ab_bc_ca(const abc<T>& phase)
{
    return ab_bc_ca<T>{phase.a() - phase.b(), phase.b() - phase.c(), phase.c() - phase.a()};
}

template <typename T>
constexpr abc<T> to_phase_balanced(const ab_bc_ca<T>& line)
{
    const T a = (real_t{2.0 / 3.0} * line.ab()) + (real_t{1.0 / 3.0} * line.bc());
    const T b = -(real_t{1.0 / 3.0} * line.ab()) + (real_t{1.0 / 3.0} * line.bc());
    const T c = -(real_t{1.0 / 3.0} * line.ab()) - (real_t{2.0 / 3.0} * line.bc());
    return abc<T>{a, b, c};
}

template <typename T>
constexpr abc<T> to_line(const abc<T>& phase)
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

// --- Rotation ---

template <typename T>
constexpr abc<T> rotate(const abc<T>& val, const angle_wrapped& angle)
{
    return to_abc(rotate(to_alphabeta(val), angle));
}

template <typename T>
constexpr alphabeta<T> rotate(const alphabeta<T>& val, const angle_wrapped& angle)
{
    return val.rotate(angle);
}

template <typename T>
constexpr dq<T> rotate(const dq<T>& val, const angle_wrapped& angle)
{
    return val.rotate(angle);
}

// --- Factory Functions ---

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

// --- Utilities ---

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

#endif  // MOJITO_TRANSFORM_HPP
