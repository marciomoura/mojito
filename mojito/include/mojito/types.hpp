#pragma once

namespace mojito {

#if defined(mojito_PRECISION_DOUBLE)
using real_t = double;
#else
using real_t = float;
#endif

#if defined(MOJITO_NO_IOSTREAM)
#define MOJITO_HAS_IOSTREAM 0
#else
#define MOJITO_HAS_IOSTREAM 1
#endif

}  // namespace mojito