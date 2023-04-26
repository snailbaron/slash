#include <re/util.hpp>

#include <algorithm>
#include <concepts>
#include <tuple>

namespace re {

namespace {

template <class FbObject>
requires
    std::same_as<FbObject, fb::Strings> ||
    std::same_as<FbObject, fb::BinaryData>
std::tuple<uint32_t, uint32_t> calculateRange(
    const FbObject* fbObject, uint32_t index)
{
    return {
        fbObject->offsets()->Get(index),
        index + 1 < fbObject->offsets()->size() ?
            fbObject->offsets()->Get(index + 1) : fbObject->data()->size()
    };
}

} // namespace

Strings::Strings(const fb::Strings* fbStrings)
    : _fbStrings(fbStrings)
{ }

IndexIterator<Strings> Strings::begin() const
{
    return {*this, 0};
}

IndexIterator<Strings> Strings::end() const
{
    return {*this, _fbStrings->offsets()->size()};
}

std::string_view Strings::operator[](uint32_t index) const
{
    auto [begin, end] = calculateRange(_fbStrings, index);
    return _fbStrings->data()->string_view().substr(begin, end - begin);
}

uint32_t Strings::size() const
{
    return _fbStrings->offsets()->size();
}

flatbuffers::Offset<fb::Strings> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::string>& strings)
{
    auto data = std::string{};
    auto offsets = std::vector<uint32_t>{};
    for (const auto& string : strings) {
        offsets.push_back((uint32_t)data.length());
        data += string;
    }

    return fb::CreateStrings(
        builder, builder.CreateString(data), builder.CreateVector(offsets));
}

BinaryData::BinaryData(const fb::BinaryData* fbBinaryData)
    : _fbBinaryData(fbBinaryData)
{ }

IndexIterator<BinaryData> BinaryData::begin() const
{
    return {*this, 0};
}

IndexIterator<BinaryData> BinaryData::end() const
{
    return {*this, _fbBinaryData->offsets()->size()};
}

std::span<const std::byte> BinaryData::operator[](uint32_t index) const
{
    auto [begin, end] = calculateRange(_fbBinaryData, index);
    const auto* ptr =
        reinterpret_cast<const std::byte*>(_fbBinaryData->data()->data() + begin);
    return {ptr, end - begin};
}

uint32_t BinaryData::size() const
{
    return _fbBinaryData->offsets()->size();
}

flatbuffers::Offset<fb::BinaryData> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::vector<std::byte>>& blobs)
{
    auto data = std::vector<uint8_t>{};
    auto offsets = std::vector<uint32_t>{};
    for (const auto& blob : blobs) {
        offsets.push_back((uint32_t)data.size());
        for (std::byte b : blob) {
            data.push_back(static_cast<uint8_t>(b));
        }
    }

    return fb::CreateBinaryData(
        builder, builder.CreateVector(data), builder.CreateVector(offsets));
}

NamedBinaryDataStorage::NamedBinaryDataStorage(
    const fb::NamedBinaryDataStorage* fbNamedBinaryDataStorage)
    : _names(fbNamedBinaryDataStorage->names())
    , _binaryData(fbNamedBinaryDataStorage->binaryData())
{
    for (size_t i = 0; i < std::min(_names.size(), _binaryData.size()); i++) {
        _indexByName[std::string{_names[i]}] = i;
    }
}

IndexIterator<NamedBinaryDataStorage> NamedBinaryDataStorage::begin() const
{
    return {*this, 0};
}

IndexIterator<NamedBinaryDataStorage> NamedBinaryDataStorage::end() const
{
    return {*this, _names.size()};
}

NamedBinaryData NamedBinaryDataStorage::operator[](uint32_t index) const
{
    return {.name = _names[index], .data = _binaryData[index]};
}

NamedBinaryData NamedBinaryDataStorage::operator[](std::string_view name) const
{
    return (*this)[_indexByName.at(std::string{name})];
}

uint32_t NamedBinaryDataStorage::size() const
{
    return _names.size();
}

std::span<const std::byte> NamedBinaryDataStorage::data(uint32_t index) const
{
    return (*this)[index].data;
}

std::span<const std::byte> NamedBinaryDataStorage::data(std::string_view name) const
{
    return (*this)[name].data;
}

flatbuffers::Offset<fb::NamedBinaryDataStorage> pack(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<std::string>& names,
    const std::vector<std::vector<std::byte>>& blobs)
{
    return fb::CreateNamedBinaryDataStorage(
        builder,
        pack(builder, names),
        pack(builder, blobs));
}

} // namespace re
