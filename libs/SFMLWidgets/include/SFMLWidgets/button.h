#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "SFMLWidgets/settings.h"
#include <string>


// helper for menu creation

namespace widget
{

class Button : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape m_frame;
    sf::Text m_text;
    uint m_chPixelWidth;


public:
    Button(std::string text, Settings S);
    Button() = delete;

    void setPosition(uint x, uint y);

    bool contains(sf::Vector2f pos) { return m_frame.getGlobalBounds().contains(pos); }

};

}

#endif // MENU_BUTTON_H
