#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "JsonValue.hpp"

namespace cjs{
    class Json {
        private:
            class Iterator;
            class Const_Iterator;
        public: //types
            enum class Type{Literal, Array, Object};
            using String = std::string;
            using LiteralVector = std::vector<JsonValue>;
            using LiteralMap = std::unordered_map<String,JsonValue>;
            using InitList = std::initializer_list<JsonValue>;
        public://ctors
            Json() = default;
            Json(const char* s); 
            Json(bool);
            Json(int);
            Json(double);
            explicit Json(const JsonValue&);
            Json(InitList list);
        public://utilities
            void push_back(const JsonValue&);
            bool is_object() const;
            bool is_array() const;
            const JsonValue& operator[](size_t) const;
            const JsonValue& operator[](const String&) const;
            JsonValue& operator[](size_t);
            JsonValue& operator[](const String&);
        private:
            bool has_array(const JsonValue&) const;
            bool has_map(const JsonValue&) const;
            bool is_literal(const JsonValue&) const;
            Type handleLiteralCase(InitList);
            void setArray(InitList);
            void setMap(InitList);
            bool are_allLiterals(InitList) const;
            bool are_allObjects(InitList) const;
            void setType(Type);
            Type determineType(InitList) const;
        private:
            JsonValue m_value;
            Type m_type{Type::Literal};
    };

} //namespace cjs


