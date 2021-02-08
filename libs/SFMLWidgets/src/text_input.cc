#include "SFMLWidgets/text_input.h"

#include <iostream>

namespace widget
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

TextInput::TextInput(std::string nameText, uint maxLength, const Settings& S, std::string defaultText, Validator V)
    :   m_nameText(nameText, S.font, S.chSize),
        m_maxLength(maxLength),
        m_insertText(defaultText, S.font, S.chSize),
        m_input_string(defaultText),
        m_cursor(m_input_string.length()),
        m_cursorLine({S.chPixelWidth * 0.2f, (3.0f/2.0f)*static_cast<float>(S.chSize)}),
        m_validator(V),
        m_chPixelWidth(S.chPixelWidth),
        m_chSize(S.chSize)
{
    // set some default values for text input
    m_cursorLine.setFillColor(sf::Color::Green);
    m_nameText.setFillColor(sf::Color::White);
    m_insertText.setFillColor(sf::Color::White);
    m_insertText.setOutlineColor(sf::Color(255,165,0));

    m_frame = sf::RectangleShape({static_cast<float>((m_chPixelWidth + 1) * m_maxLength),
                                  3 * m_chSize / 2.0f});
    m_frame.setFillColor(sf::Color::Transparent);
    m_frame.setOutlineColor(sf::Color(255,165,0));
    m_frame.setOutlineThickness(m_chSize / 5.0f);

    // To fix vertical displacement of the cursor
    m_cursorLine.setOrigin(0, m_chSize / 15);

    setPosition(0, 0);
    validate();
}

void TextInput::setPosition(uint xpos, uint ypos)
{
    m_nameText.setPosition(xpos, ypos);

    auto it_relXpos = (m_nameText.getString().getSize() + 1) * m_chPixelWidth;
    m_insertText.setPosition(xpos + it_relXpos, ypos);

    m_cursorLine.setPosition(m_insertText.findCharacterPos(m_cursor));

    m_frame.setPosition({xpos + it_relXpos - m_chPixelWidth / 5.0f,
                         ypos - 2.0f});
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_frame, states);
    target.draw(m_nameText, states);
    target.draw(m_insertText, states);
    target.draw(m_cursorLine, states);
}

}
