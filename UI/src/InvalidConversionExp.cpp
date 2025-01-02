#include "InvalidConversionExp.hpp"

namespace cjs {
    InvalidConversion::InvalidConversion(const Messeage& messeage, const Location& location)
    : Exception{messeage, location} {};
}