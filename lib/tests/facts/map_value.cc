#include <gmock/gmock.h>
#include <facter/facts/map_value.hpp>
#include <facter/facts/array_value.hpp>
#include <facter/facts/scalar_value.hpp>
#include <rapidjson/document.h>
#include <yaml-cpp/yaml.h>
#include <sstream>

using namespace std;
using namespace facter::facts;
using namespace rapidjson;
using namespace YAML;

TEST(facter_facts_map_value, default_constructor) {
    map_value value;
    ASSERT_EQ(0u, value.elements().size());
}

TEST(facter_facts_map_value, map_constructor) {
    map<string, unique_ptr<value>> elements;
    elements["string"] = make_value<string_value>("hello");
    elements["integer"] = make_value<integer_value>(5);

    vector<unique_ptr<value>> array_elements;
    array_elements.emplace_back(make_value<string_value>("1"));
    array_elements.emplace_back(make_value<integer_value>(2));
    elements["array"] = make_value<array_value>(move(array_elements));

    map<string, unique_ptr<value>> submap;
    submap["foo"] = make_value<string_value>("bar");
    elements["map"] = make_value<map_value>(move(submap));

    map_value value(move(elements));
    ASSERT_EQ(4u, value.elements().size());

    auto str = value.get<string_value>("string");
    ASSERT_NE(nullptr, str);
    ASSERT_EQ("hello", str->value());

    auto integer = value.get<integer_value>("integer");
    ASSERT_NE(nullptr, integer);
    ASSERT_EQ(5, integer->value());

    auto array = value.get<array_value>("array");
    ASSERT_NE(nullptr, array);
    ASSERT_EQ(2u, array->elements().size());
    str = array->get<string_value>(0);
    ASSERT_EQ("1", str->value());
    integer = array->get<integer_value>(1);
    ASSERT_EQ(2u, integer->value());

    auto mapval = value.get<map_value>("map");
    ASSERT_NE(nullptr, mapval);
    ASSERT_EQ(1u, mapval->elements().size());
    str = mapval->get<string_value>("foo");
    ASSERT_NE(nullptr, str);
    ASSERT_EQ("bar", str->value());
}

TEST(facter_facts_map_value, to_json) {
    map<string, unique_ptr<value>> elements;
    elements["string"] = make_value<string_value>("hello");
    elements["integer"] = make_value<integer_value>(5);

    vector<unique_ptr<value>> array_elements;
    array_elements.emplace_back(make_value<string_value>("1"));
    array_elements.emplace_back(make_value<integer_value>(2));
    elements["array"] = make_value<array_value>(move(array_elements));

    map<string, unique_ptr<value>> submap;
    submap["foo"] = make_value<string_value>("bar");
    elements["map"] = make_value<map_value>(move(submap));

    map_value value(move(elements));

    rapidjson::Value json_value;
    MemoryPoolAllocator<> allocator;
    value.to_json(allocator, json_value);
    ASSERT_TRUE(json_value.IsObject());

    ASSERT_TRUE(json_value["string"].IsString());
    ASSERT_EQ("hello", string(json_value["string"].GetString()));

    ASSERT_TRUE(json_value["integer"].IsNumber());
    ASSERT_EQ(5ll, json_value["integer"].GetInt64());

    ASSERT_TRUE(json_value["array"].IsArray());
    ASSERT_EQ(2u, json_value["array"].Size());
    ASSERT_TRUE(json_value["array"][0u].IsString());
    ASSERT_EQ("1", string(json_value["array"][0u].GetString()));
    ASSERT_TRUE(json_value["array"][1u].IsNumber());
    ASSERT_EQ(2ll, json_value["array"][1u].GetInt64());

    ASSERT_TRUE(json_value["map"].IsObject());
    ASSERT_TRUE(json_value["map"]["foo"].IsString());
    ASSERT_EQ("bar", string(json_value["map"]["foo"].GetString()));
}

TEST(facter_facts_map_value, insertion_operator) {
    map<string, unique_ptr<value>> elements;
    elements["string"] = make_value<string_value>("hello");
    elements["integer"] = make_value<integer_value>(5);

    vector<unique_ptr<value>> array_elements;
    array_elements.emplace_back(make_value<string_value>("1"));
    array_elements.emplace_back(make_value<integer_value>(2));
    elements["array"] = make_value<array_value>(move(array_elements));

    map<string, unique_ptr<value>> submap;
    submap["foo"] = make_value<string_value>("bar");
    elements["map"] = make_value<map_value>(move(submap));

    map_value value(move(elements));

    ostringstream stream;
    stream << value;
    ASSERT_EQ("{ array => [ 1, 2 ], integer => 5, map => { foo => bar }, string => hello }", stream.str());
}

TEST(facter_facts_map_value, yaml_insertion_operator) {
    map<string, unique_ptr<value>> elements;
    elements["string"] = make_value<string_value>("hello");
    elements["integer"] = make_value<integer_value>(5);

    vector<unique_ptr<value>> array_elements;
    array_elements.emplace_back(make_value<string_value>("1"));
    array_elements.emplace_back(make_value<integer_value>(2));
    elements["array"] = make_value<array_value>(move(array_elements));

    map<string, unique_ptr<value>> submap;
    submap["foo"] = make_value<string_value>("bar");
    elements["map"] = make_value<map_value>(move(submap));

    map_value value(move(elements));

    Emitter emitter;
    emitter << value;
    ASSERT_EQ("array:\n  - \"1\"\n  - 2\ninteger: 5\nmap:\n  foo: \"bar\"\nstring: \"hello\"", string(emitter.c_str()));
}