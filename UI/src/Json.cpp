#include "Json.hpp"

namespace cjs {
    Json::Json(const char* val)
    : m_value{JsonValue(val)} {};

    Json::Json(int val)
    : m_value{JsonValue(val)} {};

    Json::Json(bool val)
    : m_value{JsonValue(val)} {};

    Json::Json(double val)
    : m_value{val} {};

    Json::Json(const JsonValue& val)
    : m_value{val} {};

    Json::Json(InitList list) 
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

    Json::Type Json::determineType(InitList list) const {
        if (are_allLiterals(list)) {
            return Type::Literal;
        }
        if (list.size() > 1) {
            return (are_allObjects(list)) ? Type::Object : Type::Array;
        }
        return (has_array(*(list.begin()))) ? Type::Array : Type::Object;
    }

    void Json::setType(Type type) {
        m_type = type;
    }

    bool Json::is_object() const {
        return (Type::Object == m_type);
    }

    bool Json::is_array() const {
        return (Type::Array == m_type);
    }

    bool Json::has_array(const JsonValue& elem) const {
        return (elem.getIndex() == 4) ? true : false;
    }

    bool Json::has_map(const JsonValue& elem) const {
        return (elem.getIndex() == 5) ? true : false;
    }

    bool Json::is_literal(const JsonValue& elem) const {
        return (elem.getIndex() != 4 && elem.getIndex() != 5) ? true : false;
    }

    bool Json::are_allObjects(InitList list )const {
        for (const auto& elem : list) {
            if (!has_map(elem)) {
                return false;
            }
        }
        return true;
    }

    bool Json::are_allLiterals(InitList list) const {
        for (const auto& elem : list) {
            if (!is_literal(elem)) {
                return false;
            }
        }
        return true;
    }

    Json::Type Json::handleLiteralCase(InitList list) {
        if (list.size() != 2) {
            m_value = JsonValue(list);
            return Type::Array;
        }

        size_t index = list.begin()->getIndex();
        if (index == 2) {
            String key = std::get<String>(list.begin()->getValue());
            JsonValue value{*(list.begin() + 1)};
            LiteralMap m;
            m[key] = value;

            m_value = JsonValue(std::move(m));
            return Type::Object;
        }

        m_value = JsonValue(list);
        return Type::Array;
    }

    void Json::setArray(InitList list) {
        m_value = JsonValue(list);
    }

    void Json::setMap(InitList list) {
        m_value = JsonValue(LiteralMap());
        for (auto& elem : list) {
            m_value.insert(elem.getValue());
        }
    }

    const JsonValue& Json::operator[](size_t index) const {
        if (m_value.getIndex() != 4) {
            throw InvalidAccess("Invalid Access: Vector dosn't have value\n", std::source_location::current());
        }
        return std::get<LiteralVector>(m_value.getValue())[index];
    }

    const JsonValue& Json::operator[](const String& key) const {
         if (m_value.getIndex() != 5) {
            throw InvalidAccess("Invalid Access: Map dosn't have value\n", std::source_location::current());
        }
        return (*std::get<LiteralMap>(m_value.getValue()).find(key)).second;
    }

    JsonValue& Json::operator[](const String& key) {
        if (m_value.getIndex() != 5) {
            throw InvalidAccess("Invalid Access: Map dosn't have value\n", std::source_location::current());
        }
        return std::get<LiteralMap>(m_value.getValue())[key];
    }

    JsonValue& Json::operator[](size_t index) {
        if (m_value.getIndex() != 4) {
            throw InvalidAccess("Invalid Access: Vector dosn't have value\n", std::source_location::current());
        }
        return std::get<LiteralVector>(m_value.getValue())[index];
    }

    void Json::push_back(const JsonValue& val) {
        if (Type::Object == m_type) {
            throw InvalidType("Invalid Type: Can not use push_back with Objects\n",std::source_location::current());
        }
        std::get<LiteralVector>(m_value.getValue()).push_back(val);
    }
        //JsonValue Part BEGIN
    
}; //namesace cjs