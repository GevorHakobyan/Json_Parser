#include "Exception.hpp"

namespace cjs {
    class InvalidConversion : public Exception {
        public:
            InvalidConversion(const Messeage&, const Location& = std::source_location::current());
    };
}