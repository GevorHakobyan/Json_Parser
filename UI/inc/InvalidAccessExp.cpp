#include "InvalidAccessExp.hpp"

namespace cjs {
    InvalidAccess::InvalidAccess(const Messeage& messeage, const Location& location)
    : Exception{messeage, location} {};
}