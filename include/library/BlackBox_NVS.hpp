#pragma once

#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <variant>
#include <vector>

namespace BlackBox {

using Blob = std::vector<std::uint8_t>;

class NVS {
    friend std::ostream& operator<<(std::ostream& stream, const NVS& nvs);

private:
    mutable std::recursive_mutex m_mutex;
    const char* m_tag = "BlackBox_NVS";
    bool m_dirty = false;

    std::string m_name;
    std::string m_partition;

    std::unique_ptr<nvs::NVSHandle> m_handle;

public:
    using Key = std::string;
    using Value = std::variant<
        std::uint8_t,
        std::int8_t,
        std::uint16_t,
        std::int16_t,
        std::uint32_t,
        std::int32_t,
        std::uint64_t,
        std::int64_t,
        std::string,
        Blob>;

    NVS(const std::string& name, const std::string partition = "nvs");
    NVS(NVS&&);

    void initFlash();
    void openFlash();
    void eraseFlash();

    void init();
    void commit();

    static void erasePartition(const std::string& partition);
    void eraseAll();

    nvs::ItemType type(Key);
    Value get(Key);
    Value get(Key, Value fallback);
    void set(Key, Value);
    void erase(Key);

    std::map<Key, nvs::ItemType> types();

    static void dump(const std::string& part, const std::string& name, nvs_type_t type, std::ostream& out = std::cout);
    void dump(nvs_type_t type = NVS_TYPE_ANY, std::ostream& out = std::cout) const;
    void dump(std::ostream& out) const;

    bool contains(Key);

    ~NVS();
};

} // namespace BlackBox
