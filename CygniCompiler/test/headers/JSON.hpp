#include <utility>

#ifndef CYGNICOMPILER_JSON_HPP
#define CYGNICOMPILER_JSON_HPP

#include "String.hpp"
#include "Predef.hpp"
#include <iostream>
#include <fstream>

template<typename T>
using Ptr = std::shared_ptr<T>;

enum class JsonType
{
    JsonMapType,
    JsonValueType,
    JsonArrayType
};

class JsonObject
{
public:
    explicit JsonObject(JsonType jsonType)
            : jsonType{jsonType}
    {

    }

    JsonType jsonType;
};

class JsonMap : public JsonObject
{
public:
    JsonMap() : JsonObject(JsonType::JsonMapType)
    {

    }

    HashMap<String, Ptr<JsonObject>> map;

    void Add(const String &key, const Ptr<JsonObject> &value)
    {
        map.insert({key, value});
    }
};

class JsonArray : public JsonObject
{
public:
    JsonArray() : JsonObject(JsonType::JsonArrayType)
    {

    }

    Vector<Ptr<JsonObject>> items;

    void Add(const Ptr<JsonObject> &value)
    {
        items.push_back(value);
    }
};

class JsonValue : public JsonObject
{
public:
    explicit JsonValue(String value)
            : JsonObject(JsonType::JsonValueType), value{std::move(value)}
    {
    }

    String value;
};

void JsonToFile(const Ptr<JsonObject> &obj, std::ofstream &stream);


#endif //CYGNIconst COMPILER_JSON_H&PP