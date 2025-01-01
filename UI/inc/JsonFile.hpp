#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <iostream>
#include "InvalidAccessExp.hpp"
#include "InvalidConversionExp.hpp"
#include "InvalidTypeExp.hpp"

namespace cjs{
    class JsonFile {
        private:
            class Iterator;
            class Const_Iterator;
        public:
            struct JsonValue;
        public: //types
            enum class Type{Literal, Array, Object};
            using String = std::string;
            using LiteralVector = std::vector<JsonFile>;
            using LiteralMap = std::unordered_map<String,JsonFile>;
            using InitList = std::initializer_list<JsonFile>;
        public:
            struct JsonValue {
                public: //constructors
                    JsonValue() = default;
                    JsonValue(int);
                    JsonValue(double);
                    JsonValue(bool);
                    JsonValue(const char*);
                    JsonValue(InitList);
                    JsonValue(LiteralMap);
                public: 
                  size_t getIndex() const;
                  String str(size_t index = 0) const;
                private:
                    String arr_toString(size_t) const;
                    String map_toString(size_t) const;
                public:
                    //Types
                    std::optional<int> _Int;
                    std::optional<double> _Double;
                    std::optional<const char*> _String;
                    std::optional<bool> _Bool;
                    std::optional<LiteralVector> _Vector;
                    std::optional<LiteralMap> _Map;        
                    size_t _index;
            };
        public: //Implicit Conversion operators
            operator int() const;
            operator double() const;
            operator bool() const;
            operator const char*() const;
            operator LiteralVector() const;
            operator LiteralMap() const;
        public://ctors
            JsonFile() = default;
            JsonFile(const char* s); 
            JsonFile(bool);
            JsonFile(int);
            JsonFile(double);
            explicit JsonFile(const JsonValue&);
            JsonFile(std::initializer_list<JsonFile> list);
            const JsonValue& getValue() const;
            JsonValue& getValue();
            void push_back(const JsonValue&);
        public://utilities
            bool is_object() const;
            bool is_array() const;
            std::pair<bool, size_t> is_literal() const;
            const JsonFile& operator[](size_t) const;
            const JsonFile& operator[](const String&) const;
            JsonFile& operator[](size_t);
            JsonFile& operator[](const String&);
            friend std::ostream& operator<<(std::ostream&, JsonFile&);
        private:
            Type handleLiteralCase(InitList);
            void setArray(InitList);
            void setMap(InitList);
            bool are_allLiterals(InitList) const;
            bool are_allObjects(InitList) const;
            void setType(Type);
            Type determineType(std::initializer_list<JsonFile>) const;
        private:
            JsonValue m_value;
            Type m_type{Type::Literal};
    };

} //namespace cjs


