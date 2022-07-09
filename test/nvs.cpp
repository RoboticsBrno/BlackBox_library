#include "library/BlackBox_NVS.hpp"

#include "unity.h"

#include <iostream>

using namespace BlackBox;

void test_init() {
    try {
        NVS nvs("test");
        nvs.init();
    } catch (const std::exception& e) {
        TEST_FAIL_MESSAGE(e.what());
    }
}

void test_load_store() {
    try {
        NVS nvs("test");
        nvs.init();

        static const Blob blob = { 0x13, 0x14, 0x15 };

        nvs.set("U8", static_cast<std::uint8_t>(64));
        nvs.set("I8", static_cast<std::int8_t>(-64));
        nvs.set("U16", static_cast<std::uint16_t>(512));
        nvs.set("I16", static_cast<std::int16_t>(-512));
        nvs.set("U32", static_cast<std::uint32_t>(1 << 20));
        nvs.set("I32", static_cast<std::int32_t>(0 - (1 << 20)));
        nvs.set("U64", static_cast<std::uint64_t>(1ULL << 40));
        nvs.set("I64", static_cast<std::int64_t>(0LL - (1LL << 40)));
        nvs.set("STR", "hello");
        nvs.set("BLOB", blob);

        nvs.commit();

        auto res_U8 = nvs.get("U8");
        auto res_I8 = nvs.get("I8");
        auto res_U16 = nvs.get("U16");
        auto res_I16 = nvs.get("I16");
        auto res_U32 = nvs.get("U32");
        auto res_I32 = nvs.get("I32");
        auto res_U64 = nvs.get("U64");
        auto res_I64 = nvs.get("I64");
        auto res_STR = nvs.get("STR");
        auto res_BLOB = nvs.get("BLOB");

        TEST_ASSERT(std::holds_alternative<std::uint8_t>(res_U8));
        TEST_ASSERT(std::holds_alternative<std::int8_t>(res_I8));
        TEST_ASSERT(std::holds_alternative<std::uint16_t>(res_U16));
        TEST_ASSERT(std::holds_alternative<std::int16_t>(res_I16));
        TEST_ASSERT(std::holds_alternative<std::uint32_t>(res_U32));
        TEST_ASSERT(std::holds_alternative<std::int32_t>(res_I32));
        TEST_ASSERT(std::holds_alternative<std::uint64_t>(res_U64));
        TEST_ASSERT(std::holds_alternative<std::int64_t>(res_I64));
        TEST_ASSERT(std::holds_alternative<std::string>(res_STR));
        TEST_ASSERT(std::holds_alternative<Blob>(res_BLOB));

        TEST_ASSERT_EQUAL(64, std::get<std::uint8_t>(res_U8));
        TEST_ASSERT_EQUAL(-64, std::get<std::int8_t>(res_I8));
        TEST_ASSERT_EQUAL(512, std::get<std::uint16_t>(res_U16));
        TEST_ASSERT_EQUAL(-512, std::get<std::int16_t>(res_I16));
        TEST_ASSERT_EQUAL(1 << 20, std::get<std::uint32_t>(res_U32));
        TEST_ASSERT_EQUAL(0 - (1 << 20), std::get<std::int32_t>(res_I32));
        TEST_ASSERT_EQUAL(1ULL << 40, std::get<std::uint64_t>(res_U64));
        TEST_ASSERT_EQUAL(0LL - (1LL << 40), std::get<std::int64_t>(res_I64));
        TEST_ASSERT_EQUAL_STRING("hello", std::get<std::string>(res_STR).c_str());

        TEST_ASSERT_EQUAL(blob.size(), std::get<Blob>(res_BLOB).size());
        TEST_ASSERT_EQUAL_MEMORY(blob.data(), std::get<Blob>(res_BLOB).data(), blob.size());

    } catch (const std::exception& e) {
        TEST_FAIL_MESSAGE(e.what());
    }
}

void test_fallback() {

    NVS nvs("test");
    nvs.init();

    nvs.eraseAll();
    try {
        nvs.get("test");
        TEST_ABORT();
    } catch (const std::exception& e) {
    }

    try {
        auto res = nvs.get("test", 64);
        TEST_ASSERT_EQUAL(64, std::get<int>(res));
    } catch (const std::exception& e) {
        TEST_ABORT();
    }
}

extern "C" {
void app_main() {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_load_store);
    RUN_TEST(test_fallback);
    UNITY_END();
    // test_task();
}
}