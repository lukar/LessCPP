#ifndef MENU_DEFINES_H
#define MENU_DEFINES_H

#include <SFML/Graphics.hpp>
#include <functional>

namespace menu
{

inline uint getChPixelWidth(const sf::Font& font, uint chSize)
{
    const sf::Text text("A", font, chSize);
    const sf::FloatRect text_bounds = text.getGlobalBounds();
    return text_bounds.width;
}


struct Settings {
    const sf::Font& font;
    const uint chSize;
    const uint chPixelWidth;

    Settings(const sf::Font& _font, const uint _chSize)
        : font(_font),
          chSize(_chSize),
          chPixelWidth(getChPixelWidth(font, _chSize))
    {}
};

using Validator = std::function<bool(std::string)>;

}

#endif // MENU_DEFINES_H
