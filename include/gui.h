#ifndef GUI_H
#define GUI_H

#include <array>
#include "const_globals.h"
#include "block.h"
#include "player.h"

#include <SFML/Graphics.hpp>

class Gui : public sf::Drawable
{
private:
    sf::Color WHITE{255, 222, 173};
    sf::Color BLACK{139, 69, 19};

    std::array<std::array<Block, 3>, 3> m_blocks;

    std::array<Player, 4> m_whitePlayers;
    std::array<Player, 4> m_blackPlayers;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Gui(std::array<WallConfig, 9>);
    std::array<Player, 4> & getPlayers(Side);
};

#endif // GUI_H
