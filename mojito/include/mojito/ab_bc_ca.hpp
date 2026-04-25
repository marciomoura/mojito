#ifndef MOJITO_AB_BC_CA_HPP
#define MOJITO_AB_BC_CA_HPP

#include "types.hpp"

namespace mojito {

/**
 * @brief Line-to-line representation of three-phase quantities.
 */
template <typename T = real_t>
struct ab_bc_ca {
    using value_type = T;

    static constexpr ab_bc_ca<T> from_real(real_t ab, real_t bc, real_t ca)
    {
        return ab_bc_ca<T>{T{ab}, T{bc}, T{ca}};
    }

    constexpr ab_bc_ca() = default;
    constexpr ab_bc_ca(T v_ab, T v_bc, T v_ca) : _ab(v_ab), _bc(v_bc), _ca(v_ca) {}

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

}  // namespace mojito

#endif  // MOJITO_AB_BC_CA_HPP
