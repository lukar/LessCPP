#include "menu_text_input.h"

#include <iostream>

namespace menu
{

void TextInput::validate() {
    if (m_validator(m_input_string)) {
        m_insertText.setFillColor(sf::Color::White);
    } else {
        m_insertText.setFillColor(sf::Color::Red);
    }
}

void TextInput::backspace()
{
    if (m_cursor > 0) {
        m_input_string.erase(m_cursor - 1, 1);
        m_cursor--;
        m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));
    }
    m_insertText.setString(m_input_string);
    validate();
}

void TextInput::delete_front() // should be called delete, but it's taken
{
    if (m_cursor < m_input_string.length()) {
        m_input_string.erase(m_cursor, 1);
    }
    m_insertText.setString(m_input_string);
    validate();
}

void TextInput::append(char c)
{
    if (m_input_string.length() < m_maxLength) {
        m_input_string.insert(m_cursor, std::string(1,c));
        m_cursor++;
        m_insertText.setString(m_input_string);
        m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));
    }
    validate();
}

void TextInput::cursorLeft() {
    if (m_cursor > 0) {
        m_cursor--;
        m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));
    }
}

void TextInput::cursorRight() {
    if (m_cursor < m_input_string.length()) {
        m_cursor++;
        m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));
    }
}

TextInput::TextInput(std::string nameText, uint maxLength, Settings S, std::string defaultText, Validator V)
    :   m_nameText(nameText, S.font, S.chSize),
        m_maxLength(maxLength),
        m_insertText(defaultText, S.font, S.chSize),
        m_input_string(defaultText),
        m_cursor(m_input_string.length()),
        m_cursorLine({S.chPixelWidth * 0.1f, static_cast<float>(S.chSize)}),
        m_validator(V),
        m_chPixelWidth(S.chPixelWidth)
{
    // set some default values for text input
    m_nameText.setFillColor(sf::Color::White);
    m_insertText.setFillColor(sf::Color::White);
    m_insertText.setOutlineColor(sf::Color(255,165,0));

    const sf::FloatRect it_bounds = m_insertText.getGlobalBounds();
    m_frame = sf::RectangleShape({static_cast<float>((m_chPixelWidth + 1) * m_maxLength),
                                  3 * it_bounds.height / 2.0f});
    m_frame.setFillColor(sf::Color::Transparent);
    m_frame.setOutlineColor(sf::Color(255,165,0));
    m_frame.setOutlineThickness(it_bounds.height / 5.0f);

    // To fix vertical displacement of the cursor
    m_cursorLine.setOrigin(0, -it_bounds.height / 5);

    setPosition(0, 0);
    validate();
}

void TextInput::setPosition(uint xpos, uint ypos)
{
    m_nameText.setPosition(xpos, ypos);

    m_insertText.setPosition(xpos + (m_nameText.getString().getSize() + 1) * m_chPixelWidth, ypos);
    const sf::FloatRect it_bounds = m_insertText.getGlobalBounds();

    m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));

    m_frame.setPosition({it_bounds.left - m_chPixelWidth / 2,
                         it_bounds.top - it_bounds.height / 4});
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_frame, states);
    target.draw(m_nameText, states);
    target.draw(m_insertText, states);
    target.draw(m_cursorLine, states);
}

}
