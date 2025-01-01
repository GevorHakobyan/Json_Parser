#include "Exception.hpp"

namespace cjs {
    class InvalidType : public Exception {
        public:
            InvalidType(const Messeage&, const Location& = std::source_location::current());
    };
}