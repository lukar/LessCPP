#ifndef GUI_H
#define GUI_H

#include <array>
#include "const_globals.h"
#include "block.h"
#include "piece.h"

#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

class Gui : public sf::Drawable
{
private:
    sf::Color WHITE{255, 222, 173};
    sf::Color BLACK{139, 69, 19};

    std::array<std::array<Block, 3>, 3> m_blocks;

    std::array<Piece, 4> m_whitePieces;
    std::array<Piece, 4> m_blackPieces;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Gui(std::array<WallConfig, 9>);
    Gui(const nlohmann::json &);
    std::array<Piece, 4> & getPieces(Player);
};

#endif // GUI_H
