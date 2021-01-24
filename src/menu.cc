#include "menu.h"

namespace menu
{

Button::Button(std::string text, int xpos, int ypos, int fontSize)
{
	m_font = getFont("resources/Roboto_Medium.ttf");
	m_text = initializeText(m_font, fontSize, xpos, ypos, sf::Color::Red);
	m_text.setString(text);

	const sf::FloatRect bounds = m_text.getGlobalBounds();

	m_frame = sf::RectangleShape({bounds.width + fontSize/2, bounds.height + fontSize/2});
	m_frame.setFillColor(sf::Color::Black);
	m_frame.setOutlineColor(sf::Color::Green);
	m_frame.setOutlineThickness(fontSize/10);
	m_frame.setPosition({bounds.left - fontSize/4, bounds.top - fontSize/4});
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_frame, states);
    target.draw(m_text, states);
}

void TextInput::backspace() //
{
	if(label_string.length()>0 and cursor>0) {
		label_string.erase(cursor, 1);
		cursor--;
	}
		
	m_text.setString(label_string);
}

void TextInput::append(char c)
{
	label_string.insert(cursor,std::string(1,c));
	cursor++;
	m_text.setString(label_string);
}

TextInput::TextInput(std::string text, int xpos, int ypos, int fontSize) {
	m_font = getFont("resources/Roboto_Medium.ttf");
	m_text = initializeText(m_font, fontSize, xpos, ypos, sf::Color::Red);
	label_string = text;
	cursor = text.length();
	m_text.setString(label_string);

	const sf::FloatRect bounds = m_text.getGlobalBounds();

	m_frame = sf::RectangleShape({ bounds.width + fontSize / 2, bounds.height + fontSize / 2 });
	m_frame.setFillColor(sf::Color::Black);
	m_frame.setOutlineColor(sf::Color::Green);
	m_frame.setOutlineThickness(fontSize / 10);
	m_frame.setPosition({ bounds.left - fontSize / 4, bounds.top - fontSize / 4 });
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_frame, states);
	target.draw(m_text, states);
}

}
