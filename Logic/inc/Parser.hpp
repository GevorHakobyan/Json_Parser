#pragma once 
#include <istream>

namespace parse {
    class Parser {
        public:
        class Lexer {
            static bool analyze(const char*);
        };
        class Syntax_Analyzer {
            static bool analyze(std::istream&);
        };
    };
};//namespace parse