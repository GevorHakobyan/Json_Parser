#pragma once
#include <optional>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include "JsonValue.hpp"
#include "InvalidAccessExp.hpp"
#include "InvalidConversionExp.hpp"
#include "InvalidTypeExp.hpp"

namespace cjs {
    class JsonValue {
        public:
            using String = std::string;
            using LiteralVector = std::vector<JsonValue>;
            using LiteralMap = std::unordered_map<String,JsonValue>;
            using InitList = std::initializer_list<JsonValue>;
            using Variant = std::variant<int, double, bool, String, LiteralVector, LiteralMap>;
        public: //constructors
            JsonValue() = default;
            JsonValue(int);
            JsonValue(double);
            JsonValue(bool);
            JsonValue(const char*);
            JsonValue(InitList);
            JsonValue(LiteralMap);
        public: //Implicit Conversion operators
            operator int() const;
            operator double() const;
            operator bool() const;
            operator String() const;
            operator LiteralVector() const;
            operator LiteralMap() const;
            friend std::ostream& operator<<(std::ostream&, JsonValue&);
        public:
            void insert(const Variant&); 
            const Variant& getValue() const;
            Variant& getValue();
            size_t getIndex() const;
            String str(size_t index = 0) const;
        private:
            String arr_toString(size_t) const;
            String map_toString(size_t) const;
        private:
            //Types
            Variant _value{};
            size_t _index{};
        };

}; //namespace cjs