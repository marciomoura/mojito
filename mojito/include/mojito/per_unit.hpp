#pragma once

#include <cassert>

namespace mojito {

template <typename T1, typename T2>
T1 convert_si_to_pu(const T1& si_value, const T2& base_value)
{
    assert(base_value != T2{0} && "Base value must not be zero for per-unit conversion");
    return si_value / base_value;
}

template <typename T1, typename T2>
T1 convert_pu_to_si(const T1& pu_value, const T2& base_value)
{
    assert(base_value != T2{0} && "Base value must not be zero for per-unit conversion");
    return pu_value * base_value;
}

}  // namespace mojito
