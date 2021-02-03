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
    const sf::Font& font;
    const uint chSize;
    const uint chPixelWidth;

    Settings(sf::Font& _font, uint _chSize)
        : font(_font),
          chSize(_chSize),
          chPixelWidth(getChPixelWidth(font, _chSize))
    {}

    // Please forgive me (this replaces the current object instance with the one being passed in)
    Settings& operator=(const Settings& second)
    {
        if (this == &second)
            return *this;
        this->~Settings();
        new(this) Settings(second);
        return *this;
    }

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wextra"
    Settings()
        : font(sf::Font()),
          chSize(0),
          chPixelWidth(0)
    {}
    #pragma GCC diagnostic pop
};


}

#endif // MENU_DEFINES_H
