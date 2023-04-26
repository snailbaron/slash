#include <re/sprites.hpp>

#include <algorithm>

namespace re {

int32_t Sprite::x() const
{
    return _fbRect->x();
}

int32_t Sprite::y() const
{
    return _fbRect->y();
}

int32_t Sprite::w() const
{
    return _fbRect->w();
}

int32_t Sprite::h() const
{
    return _fbRect->h();
}

std::string_view Sprite::name() const
{
    return _name;
}

Sprite::Sprite(const fb::Sprites* fbSprites, uint32_t index)
    : _fbRect(fbSprites->spriteFrames()->Get(index))
    , _name(Strings{fbSprites->spriteNames()}[index])
{ }

Sprites::Sprites(const fb::Sprites* fbSprites)
    : _fbSprites(fbSprites)
{
    auto names = Strings{_fbSprites->spriteNames()};
    for (size_t i = 0; i < size(); i++) {
        _indexByName[std::string{names[i]}] = i;
    }
}

IndexIterator<Sprites> Sprites::begin() const
{
    return {*this, 0};
}

IndexIterator<Sprites> Sprites::end() const
{
    return {*this, _fbSprites->spriteFrames()->size()};
}

Sprite Sprites::operator[](uint32_t index) const
{
    return {_fbSprites, index};
}

Sprite Sprites::operator[](std::string_view name) const
{
    return {_fbSprites, _indexByName.at(std::string{name})};
}

uint32_t Sprites::size() const
{
    return std::min(
        _fbSprites->spriteFrames()->size(),
        Strings{_fbSprites->spriteNames()}.size());
}

std::span<const std::byte> Sprites::sheet() const
{
    return {
        reinterpret_cast<const std::byte*>(_fbSprites->sheet()->data()),
        _fbSprites->sheet()->size()
    };
}

Data::Data(const fb::Data* fbData)
    : _fbData(fbData)
{ }

Sprites Data::sprites() const
{
    return Sprites{_fbData->sprites()};
}

} // namespace re
