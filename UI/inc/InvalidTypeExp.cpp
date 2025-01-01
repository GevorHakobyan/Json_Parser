#include "InvalidTypeExp.hpp"

namespace cjs {
    InvalidType::InvalidType(const Messeage& messeage, const Location& location)
    : Exception{messeage, location} {};
}