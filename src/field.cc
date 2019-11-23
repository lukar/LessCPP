#include "const_globals.h"
#include <random>
#include <array>
#include "block_shape.h"
#include "player.h"
#include "field.h"

#include "helpers.h"
#include <random>
#include <cassert>

#include <iostream>

Field::Field(sf::RenderWindow & window) : r_window(window) {

    auto eng = std::default_random_engine(std::random_device()());
    std::uniform_int_distribution<uint8_t> rand0to6(0, 6);
    std::uniform_int_distribution<int8_t> rand0to3(0, 3);

    // generate field blocks
    for (int y = 0; y < Y; ++y) {
        std::vector<Block> tmp;
        for (int x = 0; x < X; ++x) {
            Block tmp_block{block_size, block_border, wall::wall_configs[rand0to6(eng)], rand0to3(eng)};
            tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
            tmp.emplace_back(tmp_block);
        }
        field.emplace_back(tmp);
    }

    // generate players
    for( size_t i = 0; i < whitePlayers.size(); ++i ) {
        whitePlayers[i]= (Player(player_size, White, whiteStart[i]));
    }

    for( size_t i = 0; i < blackPlayers.size(); ++i ) {
        blackPlayers[i]= (Player(player_size, Black, blackStart[i]));
    }
}

// Getter methods
int Field::white_moves() const { return m_white_moves; }
int Field::black_moves() const { return m_black_moves; }
sf::Color Field::active_side() const { return m_active_side; }
std::array<Player, 4> & Field::active_players() { return m_active_side == White ? this->whitePlayers : this->blackPlayers; }
sf::Color Field::winning_side() const {return m_winning_side;}
int Field::moves_left() const { return m_moves_left; }
bool Field::game_over() const { return m_game_over; }

void Field::next_turn(int moves_left) {
    m_active_side = (m_active_side == White ? Black : White);
    m_moves_left = moves_left;
}

void Field::setGameOver(sf::Color winner) { m_game_over = true; m_winning_side = winner; }

int Field::countInnerWalls(Location const start, Location const end) const {

    // differences
    int const dx = end[0] - start[0];
    int const dy = end[1] - start[1];

    // block coordinates
    uint const sx = uint(start[0] / 2);
    uint const sy = uint(start[1] / 2);

    uint const ex = uint(end[0] / 2);
    uint const ey = uint(end[1] / 2);
    Block const & bstart = this->field[sy][sx];
    Block const & bend = this->field[ey][ex];

    // blocks are orthogonal and at most one space apart
    assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

    int const first = (dx ? 1 : -1);
    int const second = (sx == ex && sy == ey ? 0 : dx + dy);
    int const third = -1 + 2 * abs(dx) * (start[1] % 2) + 2 * abs(dy) * (start[0] % 2);

    int num = 0;
    if (second) {
        num += bend.hasWall({first, -second, third});
    }
    num += bstart.hasWall({first, second, third});

    return num;
}

bool Field::isPlayerAtLocation(Location const location) const {
    bool toReturn = false;
    for (const auto &player : whitePlayers) {
        if (player.getLocation() == location) toReturn = true;
    }
    for (const auto &player : blackPlayers) {
        if (player.getLocation() == location) toReturn = true;
    }
    return toReturn;
}

std::optional<int> Field::moveCost(Location oldL, Location newL) const {

    if (oldL == newL) return 0;

    if (newL[0] - oldL[0] != 0 && newL[1] - oldL[1] != 0) return {};

    if (isPlayerAtLocation(newL)) return {};

    int wallCount = 0;
    int distance = abs(newL[0] - oldL[0]) + abs(newL[1] - oldL[1]);
    if (distance > 2) return {};
    if (distance == 2) {
        Location const connecting = {(newL[0] + oldL[0]) / 2, (newL[1] + oldL[1]) / 2};
        wallCount += countInnerWalls(oldL, connecting);
        wallCount += countInnerWalls(connecting, newL);

        if (!isPlayerAtLocation(connecting)) return {};
        if (wallCount != 0) return {};
        return 1;
    }
    wallCount += countInnerWalls(oldL, newL);
    return wallCount + 1;
}

bool Field::playersInLocations(std::array<Player, 4> const &players,
                        std::array<Location, 4> const &locations) const {
    for ( auto & player: players) {
        if(std::find(locations.begin(), locations.end(), player.getLocation()) == locations.end()) {
            return false;
        }
    }
    return true;
}

void Field::selectPlayer(Player & player) {
    player.setSelected();
    this->selected_player = &player;
}

void Field::unselectPlayer() {
    if (existsPlayerSelected()) {
        this->selected_player->setPosition(this->selected_player->getLocation());
        this->selected_player->unsetSelected();
        this->selected_player = nullptr;
    }
}

bool Field::existsPlayerSelected() const {
    return this->selected_player==nullptr ? false : true;
}

bool Field::decrementMoves(int moves) {
    if (m_moves_left < moves ) return false;
    (active_side() == White ? m_white_moves : m_black_moves) += moves;
    if ( m_moves_left == moves) {
        next_turn(3);
    } else {
        m_moves_left -= moves;
    }
    return true;
}

Location Field::selectedPlayerLocation() {
    return this->selected_player->getLocation();
}

bool Field::moveSelectedPlayer(Location new_location){
    bool move_success = false; // to return
    Location old_location = selected_player->getLocation();
    auto cost = moveCost(old_location, new_location);

    if (cost){
        // at this point the move is legal, but there must still be enough moves left to complete it
        if (decrementMoves(cost.value())){
            // the moves have been 'paid' successfully, we now have to move to player
            selected_player->setLocation(new_location);
            move_success = true;
        }
    }
    unselectPlayer();

    if (!m_black_finished && playersInLocations(blackPlayers, whiteStart)) m_black_finished = true;
    if (!m_white_finished && playersInLocations(whitePlayers, blackStart) ) {
        m_white_finished = true;
        if ( m_moves_left < 3 ) next_turn(3 - m_moves_left);
    }

    if ( m_white_moves == m_black_moves || m_black_finished ) {
        if ( m_white_finished && m_black_finished ) {setGameOver(Noone); return move_success;}
        if ( m_white_finished ) {setGameOver(White); return move_success;}
        if ( m_black_finished ) {setGameOver(Black); return move_success;}
    }
    return move_success;
}

void Field::draw() {
    for (auto &row: field) {
        for (auto &block: row) {
            r_window.draw(block);
        }
    }

    for (auto &players: {whitePlayers, blackPlayers}){
        for(auto &player: players) {
            if(player.isSelected()) continue;
            r_window.draw(player);
        }
    }

    // Separately draw selected_player last so it will be drawn on top of others
    if (existsPlayerSelected()) {
        this->selected_player->setPosition(getMousePosition(r_window));
        r_window.draw(*this->selected_player);
    }
}

