#include "library/BlackBox_NVS.hpp"

#include "library/BlackBox_Exception.hpp"

#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <system_error>
#include <type_traits>
#include <variant>
#include <vector>

namespace BlackBox {

using Key = NVS::Key;
using Value = NVS::Value;

void NVS::dump(const std::string& part, const std::string& name, nvs_type_t type, std::ostream& out) {
    if (part.size() == 0)
        throw std::invalid_argument("Partition name must be non-empty");
    nvs_iterator_t it = nvs_entry_find(part.c_str(), name.size() ? name.c_str() : NULL, type);
    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        out << "Found item type: " << info.key << " of type " << static_cast<unsigned>(info.type);
        it = nvs_entry_next(it);
    }
}

std::ostream& operator<<(std::ostream& stream, const NVS& nvs) {
    nvs.dump(stream);
    return stream;
}

NVS::NVS(const std::string& name, const std::string partition)
    : m_name(name)
    , m_partition(partition) {}

void NVS::initFlash() {
    ESP_LOGI(m_tag, "Initializing NVS from partition %s", m_partition.c_str());
    esp_err_t err = nvs_flash_init_partition(m_partition.c_str());

    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

void NVS::openFlash() {
    ESP_LOGI(m_tag, "Opening namespace %s in partition %s", m_name.c_str(), m_partition.c_str());
    esp_err_t err;
    m_handle = nvs::open_nvs_handle_from_partition(m_partition.c_str(),
        m_name.c_str(),
        NVS_READWRITE,
        &err);

    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

void NVS::eraseFlash() {
    ESP_LOGI(m_tag, "Erasing partition %s", m_partition.c_str());
    esp_err_t err = nvs_flash_erase_partition(m_partition.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

void NVS::init() {
    ESP_LOGI(m_tag, "Initializing");
    try {
        initFlash();
    } catch (const IDFException& e) {
        if (e.code() == ESP_ERR_NVS_NO_FREE_PAGES
            || e.code() == ESP_ERR_NVS_NEW_VERSION_FOUND) {

            eraseFlash();
            initFlash();

        } else
            throw e;
    }

    openFlash();
}

void NVS::commit() {
    ESP_LOGI(m_tag, "Committing to namespace %s in partition %s", m_name.c_str(), m_partition.c_str());
    esp_err_t err = m_handle->commit();
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
    m_dirty = false;
}

void NVS::erasePartition(const std::string& partition) {
    esp_err_t err = nvs_flash_erase_partition(partition.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

void NVS::eraseAll() {
    esp_err_t err = m_handle->erase_all();
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

nvs::ItemType NVS::type(Key key) {
    if (key.size() >= NVS_KEY_NAME_MAX_SIZE)
        throw std::invalid_argument("Key is too long.");
    return types().at(key);
}

template <typename T>
static Value getItem(nvs::NVSHandle* handle, Key key) {
    T val;
    esp_err_t err = handle->get_item(key.c_str(), val);
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
    return Value(val);
}

Value NVS::get(Key key) {
    if (key.size() >= NVS_KEY_NAME_MAX_SIZE)
        throw std::invalid_argument("Key is too long.");

    auto type = this->type(key);
    ESP_LOGI(m_tag, "Getting key %s of type %u from namespace %s in partition %s", key.c_str(), static_cast<unsigned>(type), m_name.c_str(), m_partition.c_str());

    switch (type) {
    case nvs::ItemType::U8:
        return getItem<std::uint8_t>(m_handle.get(), key);

    case nvs::ItemType::I8:
        return getItem<std::int8_t>(m_handle.get(), key);

    case nvs::ItemType::U16:
        return getItem<std::uint16_t>(m_handle.get(), key);

    case nvs::ItemType::I16:
        return getItem<std::int16_t>(m_handle.get(), key);

    case nvs::ItemType::U32:
        return getItem<std::uint32_t>(m_handle.get(), key);

    case nvs::ItemType::I32:
        return getItem<std::int32_t>(m_handle.get(), key);

    case nvs::ItemType::U64:
        return getItem<std::uint64_t>(m_handle.get(), key);

    case nvs::ItemType::I64:
        return getItem<std::int64_t>(m_handle.get(), key);

    case nvs::ItemType::SZ: {
        std::size_t len;
        esp_err_t err = m_handle->get_item_size(type, key.c_str(), len);
        if (err != ESP_OK) {
            ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
            throw IDFException(err);
        }

        std::vector<char> res(len, '\0');
        err = m_handle->get_string(key.c_str(), res.data(), len);

        if (err != ESP_OK) {
            throw IDFException(err);
        }

        return Value(std::string(res.data()));
    }
    case nvs::ItemType::BLOB_DATA: {
        std::size_t len;
        esp_err_t err = m_handle->get_item_size(nvs::ItemType::BLOB, key.c_str(), len);
        if (err != ESP_OK) {
            ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
            throw IDFException(err);
        }

        std::vector<std::uint8_t> res(len, 0);
        err = m_handle->get_blob(key.c_str(), res.data(), len);

        if (err != ESP_OK) {
            ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
            throw IDFException(err);
        }

        return Value(res);
    }
    default:
        throw std::logic_error("Unknown item type");
    }
}

template <typename T>
static void setItem(nvs::NVSHandle* handle, Key key, T value) {
    esp_err_t err = handle->set_item(key.c_str(), value);
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

void NVS::set(Key key, Value value) {
    if (key.size() >= NVS_KEY_NAME_MAX_SIZE)
        throw std::invalid_argument("Key is too long.");

    std::visit([&](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::string>) {
            m_handle->set_string(key.c_str(), val.c_str());
        } else if constexpr (std::is_same_v<T, Blob>) {
            m_handle->set_blob(key.c_str(), val.data(), val.size());
        } else
            setItem(m_handle.get(), key, val);
    },
        value);
    m_dirty = true;
}

void NVS::erase(Key key) {
    esp_err_t err = m_handle->erase_item(key.c_str());
    if (err != ESP_OK) {
        ESP_LOGE(m_tag, "%s", esp_err_to_name(err));
        throw IDFException(err);
    }
}

std::map<Key, nvs::ItemType> NVS::types() {
    std::map<Key, nvs::ItemType> out;
    nvs_iterator_t it = nvs_entry_find(m_partition.c_str(), m_name.c_str(), NVS_TYPE_ANY);
    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        out.emplace(info.key, static_cast<nvs::ItemType>(info.type));
        it = nvs_entry_next(it);
    }
    return out;
}

void NVS::dump(nvs_type_t type, std::ostream& out) const {
    dump(m_partition, m_name, type, out);
};

void NVS::dump(std::ostream& out) const {
    dump(NVS_TYPE_ANY, out);
}

NVS::~NVS() {
    if (m_dirty)
        commit();
}

} // namespace BlackBox
