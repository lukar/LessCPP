#include "gui.h"

Gui::Gui(std::array<WallConfig, 9> wallconfigs)
{
    // generate field blocks
    for (size_t y = 0; y < 3; ++y) {
        std::array<Block, 3> row;
        for (size_t x = 0; x < 3; ++x) {
            Block a_block(wallconfigs[y*3+x]);
            a_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
            row[x] = a_block;
        }
        m_blocks[y] = row;
    }

    // generate players
    for( size_t i = 0; i < m_whitePlayers.size(); ++i ) {
        m_whitePlayers[i]= Player(whiteStart[i], WHITE);
    }

    for( size_t i = 0; i < m_blackPlayers.size(); ++i ) {
        m_blackPlayers[i]= Player(blackStart[i], BLACK);
    }
}

std::array<Player, 4> & Gui::getPlayers(Side side){
    return side == Side::WHITE ? m_whitePlayers : m_blackPlayers;
}

void Gui::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto row: m_blocks)
        for (auto b: row)
            target.draw(b, states);

    for (auto &p: m_whitePlayers)
        if (&p != selected_player) target.draw(p, states);

    for (auto &p: m_blackPlayers)
		if (&p != selected_player) target.draw(p, states);

	if (selected_player != nullptr) target.draw(*selected_player, states);

}
