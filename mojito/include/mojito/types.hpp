#pragma once

namespace mojito {

#if defined(mojito_PRECISION_DOUBLE)
using real_t = double;
#else
using real_t = float;
#endif

}  // namespace mojito