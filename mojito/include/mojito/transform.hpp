#ifndef MOJITO_TRANSFORM_HPP
#define MOJITO_TRANSFORM_HPP

#include <cmath>

#include "ab_bc_ca.hpp"
#include "abc.hpp"
#include "alphabeta.hpp"
#include "angle_wrapped.hpp"
#include "constants.hpp"
#include "dq.hpp"
#include "types.hpp"

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
constexpr abc<T> rotate_counter_clockwise(const abc<T>& val, const angle_wrapped& angle)
{
    return to_abc(rotate_counter_clockwise(to_alphabeta(val), angle));
}

template <typename T>
constexpr abc<T> rotate_clockwise(const abc<T>& val, const angle_wrapped& angle)
{
    return to_abc(rotate_clockwise(to_alphabeta(val), angle));
}

template <typename T>
constexpr alphabeta<T> rotate_counter_clockwise(const alphabeta<T>& val, const angle_wrapped& angle)
{
    return val.rotate_counter_clockwise(angle);
}

template <typename T>
constexpr alphabeta<T> rotate_clockwise(const alphabeta<T>& val, const angle_wrapped& angle)
{
    return val.rotate_clockwise(angle);
}

template <typename T>
constexpr dq<T> rotate_counter_clockwise(const dq<T>& val, const angle_wrapped& angle)
{
    return val.rotate_counter_clockwise(angle);
}

template <typename T>
constexpr dq<T> rotate_clockwise(const dq<T>& val, const angle_wrapped& angle)
{
    return val.rotate_clockwise(angle);
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

}  // namespace mojito

#endif  // MOJITO_TRANSFORM_HPP
