#include "Exception.hpp"

cjs::Exception::Exception(const Messeage& messeage, const Location& location) 
: m_messeage{messeage}, m_ErrorLocation{location} {
    setMesseage();
};

const char* cjs::Exception::what() const noexcept {
    return m_messeage.c_str();
}

void cjs::Exception::setMesseage() {
    std::string fileName = m_ErrorLocation.file_name();
    int lineNumber = (m_ErrorLocation.line());
    
    m_messeage = m_messeage + " In: " + fileName + " On Line: " + std::to_string(lineNumber) + '\n';
}