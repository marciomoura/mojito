#ifndef MOJITO_QUANTITY_CAST_HPP
#define MOJITO_QUANTITY_CAST_HPP

#include <algorithm>

#include "abc.hpp"
#include "alphabeta.hpp"
#include "dq.hpp"
#include "types.hpp"

namespace mojito {

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

}  // namespace mojito

#endif  // MOJITO_QUANTITY_CAST_HPP
