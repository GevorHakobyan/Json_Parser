#include "JsonFile.hpp"

namespace cjs {
    //GLOBAL
    std::ostream& operator<<(std::ostream& os, JsonFile& file) {
        os << file.m_value.str();
        return os;
    }

    JsonFile::JsonFile(const char* val)
    : m_value{JsonValue(val)} {};

    JsonFile::JsonFile(int val)
    : m_value{JsonValue(val)} {};

    JsonFile::JsonFile(bool val)
    : m_value{JsonValue(val)} {};

    JsonFile::JsonFile(double val)
    : m_value{val} {};

    JsonFile::JsonFile(const JsonValue& val)
    : m_value{val} {};

    JsonFile::JsonFile(std::initializer_list<JsonFile> list) 
    {
        Type type = determineType(list);
        if (Type::Literal == type) {
            setType(handleLiteralCase(list));
            return;
        }
        if (Type::Array == type) {
            setArray(list);
            setType(Type::Array);
            return;
        }
        setMap(list);
        setType(Type::Object);
    };

    JsonFile::Type JsonFile::determineType(std::initializer_list<JsonFile> list) const {
        if (are_allLiterals(list)) {
            return Type::Literal;
        }
        if (list.size() > 1) {
            return (are_allObjects(list)) ? Type::Object : Type::Array;
        }
        return (list.begin()->is_array()) ? Type::Array : Type::Object;
    }

    void JsonFile::setType(Type type) {
        m_type = type;
    }

    bool JsonFile::is_object() const {
        return (Type::Object == m_type);
    }

    bool JsonFile::is_array() const {
        return (Type::Array == m_type);
    }

    std::pair<bool, size_t> JsonFile::is_literal() const {
        std::pair<bool, size_t> answer;
        answer.first = (Type::Literal == m_type);
        answer.second = m_value.getIndex();
        return answer;
    }

    bool JsonFile::are_allObjects(InitList list )const {
        for (const auto& elem : list) {
            if (!elem.is_object()) {
                return false;
            }
        }
        return true;
    }

    bool JsonFile::are_allLiterals(InitList list) const {
        for (const auto& elem : list) {
            if (!elem.is_literal().first) {
                return false;
            }
        }
        return true;
    }

    JsonFile::Type JsonFile::handleLiteralCase(InitList list) {
        if (list.size() != 2) {
            m_value = JsonValue(list);
            return Type::Array;
        }

        const auto[isLiteral, index] = list.begin()->is_literal();
        if (index == 2) {
            const char* value = *list.begin();
            std::string tmp1{value};
            JsonFile tmp2{*(list.begin() + 1)};
            LiteralMap m;
            m[tmp1] = tmp2;

            m_value = JsonValue(std::move(m));
            return Type::Object;
        }

        m_value = JsonValue(list);
        return Type::Array;
    }

    void JsonFile::setArray(InitList list) {
        m_value = JsonValue(list);
    }

    void JsonFile::setMap(InitList list) {
        m_value = JsonValue(LiteralMap());
        for (auto& elem : list) {
            LiteralMap map = elem;
            auto[key, value] = *map.begin();
            m_value._Map.value()[key] = value;
        }
    }

    const JsonFile::JsonValue& JsonFile::getValue() const {
        return m_value;
    }

    const JsonFile& JsonFile::operator[](size_t index) const {
        if (!m_value._Vector.has_value()) {
            throw InvalidAccess("Invalid Access: Vector dosn't have value\n", std::source_location::current());
        }
        return m_value._Vector.value()[index];
    }

    const JsonFile& JsonFile::operator[](const String& key) const {
         if (!m_value._Map.has_value()) {
            throw InvalidAccess("Invalid Access: Map dosn't have value\n", std::source_location::current());
        }
        return (*m_value._Map.value().find(key)).second;
    }

    JsonFile& JsonFile::operator[](const String& key) {
        if (!m_value._Map.has_value()) {
            throw InvalidAccess("Invalid Access: Map dosn't have value\n", std::source_location::current());
        }
        return m_value._Map.value()[key];
    }

    JsonFile& JsonFile::operator[](size_t index) {
        if (!m_value._Vector.has_value()) {
            throw InvalidAccess("Invalid Access: Vector dosn't have value\n", std::source_location::current());
        }
        return m_value._Vector.value()[index];
    }
    JsonFile::JsonValue& JsonFile::getValue() {
        return m_value;
    }

    void JsonFile::push_back(const JsonValue& val) {
        if (Type::Object == m_type) {
            throw InvalidType("Invalid Type: Can not use push_back with Objects\n",std::source_location::current());
        }
        m_value._Vector.value().push_back(std::move(JsonFile(val)));
    }
    //JsonFileValue Part BEGIN
    JsonFile::JsonValue::JsonValue(int val)
    : _Int{val}, _index{0} {};

    JsonFile::JsonValue::JsonValue(double val)
    : _Double{val}, _index{1} {};

    JsonFile::JsonValue::JsonValue(const char* val)
    : _String{val}, _index{2} {};
    
    JsonFile::JsonValue::JsonValue(bool val)
    : _Bool{val}, _index{3} {};

    JsonFile::JsonValue::JsonValue(InitList list)
    : _Vector{LiteralVector{}}, _index{4} 
    {
        for (auto elem : list) {
            _Vector.value().push_back(elem);
        }
        
    };

    JsonFile::JsonValue::JsonValue(LiteralMap val)
    : _Map{std::move(val)}, _index{5} {};

    JsonFile::operator int() const {
        if (0 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Int\n", std::source_location::current());
        }
        return m_value._Int.value();
    }

    JsonFile::operator double() const {
       if (1 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Double\n", std::source_location::current());
        }
        return m_value._Double.value();
    }

    JsonFile::operator bool() const {
        if (3 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Bool\n", std::source_location::current());
        }
        return m_value._Bool.value(); 
    }

    JsonFile::operator const char *() const {
        if (2 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to const char*\n", std::source_location::current());
        }
        return m_value._String.value(); 
    }

    JsonFile::operator JsonFile::LiteralVector() const {
        if (4 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Vector\n", std::source_location::current());
        }
        return m_value._Vector.value(); 
    }

    JsonFile::operator JsonFile::LiteralMap() const {
        if (5 != m_value.getIndex()) {
            throw InvalidConversion("Invalid Conversion: Can not convert to Map\n", std::source_location::current());
        }
        return m_value._Map.value(); 
    }

    JsonFile::String JsonFile::JsonValue::str(size_t index) const {
        if (4 == _index) {
            return arr_toString(index);
        }

        if (5 == _index) {
            return map_toString(index);
        }

        if (_index == 0) { //INT
            return std::to_string(_Int.value());
        }

        if (_index == 1) { //DOUBLE
            return std::to_string(_Double.value());
        }

        if (_index == 2) {
            return _String.value();
        }

        return (_Bool.value()) ? "true" : "false";
    }

    JsonFile::String JsonFile::JsonValue::arr_toString(size_t index) const {
        std::string answer {'['};
        answer += ' ';

        for (const auto& elem : _Vector.value()) {
            answer += elem.getValue().str(1);
            answer += ',';
            answer += ' ';
        }
        answer += ']';
        answer += (0 == index) ? '\n' : ',';
        return answer;
    }

    JsonFile::String JsonFile::JsonValue::map_toString(size_t index) const {
        std::string answer{'{'};

        for (const auto& elem : _Map.value()) {
            answer += '{';
            answer += elem.first;
            answer += elem.second.getValue().str(1);
            answer += '}';
        }
        answer += '}';
        answer += (0 == index) ? '\n' : ' ';
        return answer;
    }

    size_t JsonFile::JsonValue::getIndex() const {
        return _index;
    }
}; //namesace cjs