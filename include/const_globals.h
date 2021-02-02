#ifndef LESSCPP_CONST_GLOBALS_H
#define LESSCPP_CONST_GLOBALS_H

#include <array>
#include <optional>

#include <ciso646>
#include <utility>

#include "location.h"

#include <SFML/Graphics.hpp>
#include "menu_defines.h"


// individual color pieces starting positions
constexpr Locations<4> whiteStart = {{{0, 0},{1, 0},{0, 1},{1, 1}}};
constexpr Locations<4> blackStart = {{{4, 4},{5, 4},{4, 5},{5, 5}}};

constexpr float block_size = 200;
constexpr float border_width = 2;
constexpr float line_width = 5;
constexpr float wall_width = 20;
constexpr float piece_size = 30;

constexpr int window_width = 800;
constexpr int window_height = 600;

// fonts
inline sf::Font getFont(std::string const& filepath) {
    sf::Font font;
    if (!font.loadFromFile(filepath)) {
        throw std::runtime_error("Cannot find the font file " + filepath);
    }
    return font;
}

inline const sf::Font mono_font = getFont("resources/Roboto_Mono_Medium_Nerd_Font_Complete.ttf");
inline const sf::Font font = getFont("resources/Roboto_Medium.ttf");

constexpr uint small_font = 15;
constexpr uint medium_font = 30;
constexpr uint large_font = 60;

inline const menu::Settings Medium(mono_font, medium_font);
inline const menu::Settings Large(mono_font, large_font);


#endif //LESSCPP_CONST_GLOBALS_H
