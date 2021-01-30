#ifndef MENU_TEXT_INPUT_H
#define MENU_TEXT_INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "const_globals.h"
#include "less.h"
#include <string>


// helper for menu creation

namespace menu
{
     

class TextInput : public sf::Drawable
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text m_nameText;
    const size_t m_charWidth;

    sf::Text m_insertText;
    std::string m_input_string;
    sf::RectangleShape m_frame;

    size_t m_cursor;
    sf::RectangleShape m_cursorLine;

    std::function<bool(std::string)> m_validator;

public:
    void validate();
    void backspace();
    void delete_front();
    void append(char c);
    std::string getText() {return m_input_string; }
    void setPosition(int xpos, int ypos);
    void cursorLeft();
    void cursorRight();

    TextInput(std::string nameText,
              uint charWidth,
              std::string defaultText = "",
              std::function<bool(std::string)> validator = [](...){return true;}
    );
    TextInput() = delete;

    bool contains(sf::Vector2f pos) { return m_frame.getGlobalBounds().contains(pos); }

};

}

#endif // MENU_TEXT_INPUT_H
