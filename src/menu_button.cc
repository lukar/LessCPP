
#include "menu_button.h"

#include <iostream>

namespace menu
{

Button::Button(std::string text, int xpos, int ypos)
{
    m_text = initializeText(font, medium_font, xpos, ypos, sf::Color::Red);
	m_text.setString(text);

	const sf::FloatRect bounds = m_text.getGlobalBounds();

    m_frame = sf::RectangleShape({bounds.width + medium_font/2, bounds.height + medium_font/2});
	m_frame.setFillColor(sf::Color::Black);
	m_frame.setOutlineColor(sf::Color::Green);
    m_frame.setOutlineThickness(medium_font/10);
    m_frame.setPosition({bounds.left - medium_font/4, bounds.top - medium_font/4});
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_frame, states);
    target.draw(m_text, states);
}

}
