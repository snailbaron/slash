#pragma once

#include "re_generated.h"

#include <re/util.hpp>

#include <cstdint>
#include <map>
#include <string_view>

namespace re {

class Sprite {
public:
    int32_t x() const;
    int32_t y() const;
    int32_t w() const;
    int32_t h() const;
    std::string_view name() const;

private:
    Sprite(const fb::Sprites* fbSprites, uint32_t index);

    const fb::Rect* _fbRect = nullptr;
    std::string_view _name;

    friend class Sprites;
};

class Sprites {
public:
    Sprites() = default;
    Sprites(const fb::Sprites* fbSprites);

    IndexIterator<Sprites> begin() const;
    IndexIterator<Sprites> end() const;

    Sprite operator[](uint32_t index) const;
    Sprite operator[](std::string_view name) const;

    uint32_t size() const;

    std::span<const std::byte> sheet() const;

private:
    const fb::Sprites* _fbSprites = nullptr;
    std::map<std::string, uint32_t> _indexByName;
};

class Data {
public:
    Data() = default;
    Data(const fb::Data* fbData);

    Sprites sprites() const;

private:
    const fb::Data* _fbData = nullptr;
};

} // namespace re
