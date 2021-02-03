#ifndef LESS_H
#define LESS_H

#include <game.h>
#include <SFML/Audio.hpp>
#include <deque>

#include "SFMLWidgets/settings.h"

std::string get_side_text(Game const&);

sf::Sound getSound(std::string const&, std::deque<sf::SoundBuffer>&);

sf::Font getFont(std::string const&);

sf::Text initializeText(int, int, const widget::Settings&, sf::Color);




#endif // LESS_H
