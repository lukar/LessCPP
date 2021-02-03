
#include "SFMLWidgets/button.h"

#include <iostream>

namespace widget
{

Button::Button(std::string text, Settings S)
    : m_text(text, S.font, S.chSize),
      m_chPixelWidth(S.chPixelWidth)
{
    m_text.setString(text);
    m_text.setFillColor(sf::Color::Red);


    const sf::FloatRect bounds = m_text.getGlobalBounds();
    m_frame = sf::RectangleShape({bounds.width + S.chPixelWidth, bounds.height + S.chPixelWidth});
    m_frame.setFillColor(sf::Color::Transparent);
    m_frame.setOutlineColor(sf::Color::Green);
    m_frame.setOutlineThickness(m_chPixelWidth/5);

    setPosition(0, 0);
}

void Button::setPosition(uint x, uint y)
{
    m_text.setPosition(x, y);
    const sf::FloatRect bounds = m_text.getGlobalBounds();

    m_frame.setPosition({bounds.left - m_chPixelWidth / 2, bounds.top - m_chPixelWidth / 2});
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_frame, states);
    target.draw(m_text, states);
}

}
