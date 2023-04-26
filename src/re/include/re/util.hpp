#pragma once

#include "re_generated.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace re {

template <class Container>
class IndexIterator {
public:
    IndexIterator(const Container& container, uint32_t index)
        : _container(container)
        , _index(index)
    { }

    IndexIterator& operator++()
    {
        ++_index;
        return *this;
    }

    IndexIterator operator++(int)
    {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator*() const
    {
        return _container[_index];
    }

    friend bool operator==(const IndexIterator& lhs, const IndexIterator& rhs)
    {
        return lhs._index == rhs._index;
    }

    friend bool operator!=(const IndexIterator& lhs, const IndexIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    const Container& _container;
    uint32_t _index = 0;
};

class Strings {
public:
    Strings() = default;
    Strings(const fb::Strings* fbStrings);

    [[nodiscard]] IndexIterator<Strings> begin() const;
    [[nodiscard]] IndexIterator<Strings> end() const;

    [[nodiscard]] std::string_view operator[](uint32_t index) const;
    [[nodiscard]] uint32_t size() const;

private:
    const fb::Strings* _fbStrings = nullptr;
};

flatbuffers::Offset<fb::Strings> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::string>& strings);

class BinaryData {
public:
    BinaryData() = default;
    BinaryData(const fb::BinaryData* fbBinaryData);

    [[nodiscard]] IndexIterator<BinaryData> begin() const;
    [[nodiscard]] IndexIterator<BinaryData> end() const;

    [[nodiscard]] std::span<const std::byte> operator[](uint32_t index) const;
    [[nodiscard]] uint32_t size() const;

private:
    const fb::BinaryData* _fbBinaryData = nullptr;
};

flatbuffers::Offset<fb::BinaryData> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::vector<std::byte>>& blobs);

struct NamedBinaryData {
    std::string_view name;
    std::span<const std::byte> data;
};

class NamedBinaryDataStorage {
public:
    NamedBinaryDataStorage() = default;
    NamedBinaryDataStorage(
        const fb::NamedBinaryDataStorage* fbNamedBinaryDataStorage);

    [[nodiscard]] IndexIterator<NamedBinaryDataStorage> begin() const;
    [[nodiscard]] IndexIterator<NamedBinaryDataStorage> end() const;

    [[nodiscard]] NamedBinaryData operator[](uint32_t index) const;
    [[nodiscard]] NamedBinaryData operator[](std::string_view name) const;
    [[nodiscard]] uint32_t size() const;
    [[nodiscard]] std::span<const std::byte> data(uint32_t index) const;
    [[nodiscard]] std::span<const std::byte> data(std::string_view name) const;

private:
    Strings _names;
    BinaryData _binaryData;
    std::map<std::string, uint32_t> _indexByName;
};

flatbuffers::Offset<fb::NamedBinaryDataStorage> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::string>& names,
    const std::vector<std::vector<std::byte>>& blobs);

} // namespace re
