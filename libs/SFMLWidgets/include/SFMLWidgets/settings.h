#ifndef MENU_DEFINES_H
#define MENU_DEFINES_H

#include <SFML/Graphics.hpp>
#include <functional>

using uint = unsigned int;
using Validator = std::function<bool(std::string)>;

namespace widget
{

inline uint getChPixelWidth(const sf::Font& font, uint chSize)
{
    const sf::Text text("A", font, chSize);
    const sf::FloatRect text_bounds = text.getGlobalBounds();
    return static_cast<uint>(text_bounds.width);
}


struct Settings {
    sf::Font font;
    uint chSize;
    uint chPixelWidth;

    Settings(sf::Font& _font, uint _chSize)
        : font(_font),
          chSize(_chSize),
          chPixelWidth(getChPixelWidth(font, _chSize))
    {}

    Settings(const Settings&) = delete;

    Settings() = delete;
};


}

#endif // MENU_DEFINES_H
