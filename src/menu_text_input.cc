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
    if (m_input_string.length() < m_charWidth) {
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

TextInput::TextInput(std::string nameText, uint charWidth, std::string defaultText, std::function<bool(std::string)> validator)
: m_nameText(nameText, mono_font, medium_font),
    m_charWidth(charWidth),
    m_insertText(defaultText, mono_font, medium_font),
    m_input_string(defaultText),
    m_frame({medium_font * (2.0f/3.0f) * m_charWidth, medium_font * 1.5f}),
    m_cursor(m_input_string.length()),
    m_cursorLine({medium_font * 0.04f, medium_font * 1.5f}),
    m_validator(validator)
{
    // set some default values for text input
    m_nameText.setFillColor(sf::Color::White);
    m_insertText.setFillColor(sf::Color::White);
    m_cursorLine.setFillColor(sf::Color::Green);


    m_frame.setFillColor(sf::Color::Black);
    m_frame.setOutlineColor(sf::Color(255,165,0));
    m_frame.setOutlineThickness(medium_font / 10.0);

    setPosition(0, 0);
    validate();
}

void TextInput::setPosition(int xpos, int ypos)
{
    m_nameText.setPosition(xpos, ypos);
    m_insertText.setPosition(xpos + (medium_font * (4.0f/5.0f)) * m_nameText.getString().getSize(), ypos);
    m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));

    const sf::FloatRect bounds = m_insertText.getGlobalBounds();
    m_frame.setPosition({ bounds.left - medium_font / 4, bounds.top - medium_font / 3 });
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // insides are not transparent, so m_frame needs to be drawn first
    target.draw(m_frame, states);
    target.draw(m_nameText, states);
    target.draw(m_insertText, states);
    target.draw(m_cursorLine, states);
}

}
