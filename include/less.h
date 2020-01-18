#ifndef LESS_H
#define LESS_H

#include <game.h>
#include <SFML/Audio.hpp>

std::string get_side_text(Game const&);

sf::Sound getSound(std::string const&);

sf::Font getFont(std::string const&);

sf::Text initializeSideText(sf::Font const&, int, int, sf::Color);




#endif // LESS_H
