#pragma once
#include "Exception.hpp"

namespace cjs {
 class InvalidAccess : public Exception {
    public:
        InvalidAccess(const Messeage&, const Location& = std::source_location::current());
 };   
}