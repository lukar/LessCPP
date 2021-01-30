#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "const_globals.h"
#include "less.h"
#include <string>


// helper for menu creation

namespace menu
{

class Button : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape m_frame;
    sf::RectangleShape m_leftBanner;
    sf::RectangleShape m_rightBanner;

        sf::Text m_text;


public:
        Button(std::string text, int xpos, int ypos);
    Button() = delete;

    bool contains(sf::Vector2f pos) { return m_frame.getGlobalBounds().contains(pos); }

};

}

#endif // MENU_BUTTON_H
