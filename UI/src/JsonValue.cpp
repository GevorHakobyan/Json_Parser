#include "JsonValue.hpp"

namespace cjs {
     //GLOBAL
    std::ostream& operator<<(std::ostream& os, JsonValue& file) {
        os << file.str();
        return os;
    }
    
    JsonValue::JsonValue(int val)
    : _value{val}, _index{0} {};

    JsonValue::JsonValue(double val)
    : _value{val}, _index{1} {};

    JsonValue::JsonValue(const char* val)
    : _value{val}, _index{3} {};
    
    JsonValue::JsonValue(bool val)
    : _value{val}, _index{2} {};

    JsonValue::JsonValue(InitList list)
    : _value{LiteralVector{}}, _index{4} 
    {
        for (auto elem : list) {
            std::get<LiteralVector>(_value).push_back(elem);
        }
        
    };

    JsonValue::JsonValue(LiteralMap val)
    : _value{std::move(val)}, _index{5} {};

    const JsonValue::Variant& JsonValue::getValue() const{
        return _value;
    }

    JsonValue::Variant& JsonValue::getValue() {
        return _value;
    }

    void JsonValue::insert(const Variant& value) {
        _value = value;
    }

    JsonValue::String JsonValue::str(size_t index) const {
        if (4 == _index) {
            return arr_toString(index);
        }

        if (5 == _index) {
            return map_toString(index);
        }

        if (_index == 0) { //INT
            return std::to_string(std::get<int>(_value));
        }

        if (_index == 1) { //DOUBLE
            return std::to_string(std::get<double>(_value));
        }

        if (_index == 3) {
            return std::get<String>(_value);
        }

        return (std::get<bool>(_value)) ? "true" : "false";
    }

    JsonValue::String JsonValue::arr_toString(size_t index) const {
        std::string answer {'['};
        answer += ' ';

        for (const auto& elem : std::get<LiteralVector>(_value)) {
            answer += elem.str(1);
            answer += ',';
            answer += ' ';
        }
        answer += ']';
        answer += (0 == index) ? '\n' : ',';
        return answer;
    }

    JsonValue::String JsonValue::map_toString(size_t index) const {
        std::string answer{'{'};

        for (const auto& elem : std::get<LiteralMap>(_value)) {
            answer += '{';
            answer += elem.first;
            answer += elem.second.str(1);
            answer += '}';
        }
        answer += '}';
        answer += (0 == index) ? '\n' : ' ';
        return answer;
    }

    size_t JsonValue::getIndex() const {
        return _index;
    }
    JsonValue::operator int() const {
        if (0 != _index) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Int\n", std::source_location::current());
        }
        return std::get<int>(_value);
    }

    JsonValue::operator double() const {
       if (1 != _index) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Double\n", std::source_location::current());
        }
        return std::get<double>(_value);
    }

    JsonValue::operator bool() const {
        if (2 != _index) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Bool\n", std::source_location::current());
        }
        return std::get<bool>(_value);
    }

    JsonValue::operator String() const {
        if (3 != _index){
            throw InvalidConversion("Invalid Conversion: Can not convert to const char*\n", std::source_location::current());
        }
        return std::get<String>(_value);
    }

    JsonValue::operator JsonValue::LiteralVector() const {
        if (4 != _index) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Vector\n", std::source_location::current());
        }
        return std::get<LiteralVector>(_value);
    }

    JsonValue::operator JsonValue::LiteralMap() const {
        if (5 != _index) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Map\n", std::source_location::current());
        }
        return std::get<LiteralMap>(_value);
    }
}