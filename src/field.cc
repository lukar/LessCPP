#include "const_globals.h"
#include "block_shape.h"
#include "player.h"
#include "field.h"
#include "helpers.h"

#include <array>
#include <random>
#include <cassert>
#include <stdexcept>
#include <algorithm>

Field::Field() {

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
uint Field::white_moves() const { return m_white_moves; }
uint Field::black_moves() const { return m_black_moves; }
State Field::getState() const { return m_state; }
sf::Color Field::active_side() const { return m_active_side; }
std::array<Player, 4> & Field::active_players() { return m_active_side == White ? this->whitePlayers : this->blackPlayers; }
sf::Color Field::winning_side() const {return m_winning_side;}
uint Field::moves_left() const { return m_moves_left; }

bool Field::isEndOfTurn() const {
    return m_moves_left == 0 or (active_side() == White
                                 ? playersInLocations(whitePlayers, blackStart)
                                 : playersInLocations(blackPlayers, whiteStart));
}


void Field::nextTurn() {
    m_active_side = (m_active_side == White ? Black : White);
    unselectPlayer();

    if ( playersInLocations(blackPlayers, whiteStart) or m_state == State::LAST_TURN ) m_state = State::ENDED;
    else if ( playersInLocations(whitePlayers, blackStart) ) { m_state = State::LAST_TURN; m_moves_left = 3 - m_moves_left; }
    else { m_moves_left = 3; }

    if ( m_state == State::ENDED ) {
        if ( playersInLocations(blackPlayers, whiteStart) and playersInLocations(whitePlayers, blackStart) ) m_winning_side = Noone;
        else if (playersInLocations(whitePlayers, blackStart)) m_winning_side = White;
        else m_winning_side = Black;
    }
}

void Field::setGameOver(sf::Color winner) { m_state = State::ENDED; m_winning_side = winner; }

uint Field::countInnerWalls(Location const start, Location const end) const {

    // differences
    int const dx = static_cast<int>(end.x - start.x);
    int const dy = static_cast<int>(end.y - start.y);

    // block coordinates
    uint const sx = uint(start.x / 2);
    uint const sy = uint(start.y / 2);

    uint const ex = uint(end.x / 2);
    uint const ey = uint(end.y / 2);
    Block const & bstart = this->field[sy][sx];
    Block const & bend = this->field[ey][ex];

    // blocks are orthogonal and at most one space apart
    assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

    int const first = (dx ? 1 : -1);
    int const second = (sx == ex && sy == ey ? 0 : dx + dy);
    int const third = -1 + 2 * abs(dx) * static_cast<int>(start.y % 2) + 2 * abs(dy) * static_cast<int>(start.x % 2);

    uint num = 0;
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

std::optional<uint> Field::moveCost(Location old_location, Direction direction) const {
    if (std::optional<Location> new_location = old_location + direction) {
        return countInnerWalls(old_location, new_location.value());
    }
    return {};
}

std::optional<uint> Field::moveCost(Location oldL, Location newL) const {

    if (oldL == newL) return 0;

    if (newL.x - oldL.x != 0 && newL.y - oldL.y != 0) return {};

    if (isPlayerAtLocation(newL)) return {};

    int wallCount = 0;
    int distance = abs(static_cast<int>(newL.x - oldL.x)) + abs(static_cast<int>(newL.y - oldL.y));
    if (distance > 2) return {};
    if (distance == 2) {
        Location const connecting = {(newL.x + oldL.x) / 2, (newL.y + oldL.y) / 2};
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
    return std::all_of(players.begin(), players.end(), [=](const Player & player) {
        return std::find(locations.begin(), locations.end(), player.getLocation()) != locations.end();
    });
}

Player * Field::selectPlayer(Player & player) {
    this->selected_player = &player;
    return &player;
}

Player * Field::selectPlayer(uint player_num) {
    if (player_num > 3) throw std::domain_error("There is no player with index: " + std::to_string(player_num));
    Player & player = this->active_players()[player_num];
    this->selected_player = &player;
    return &player;
}

std::nullptr_t Field::unselectPlayer() {
    if (existsPlayerSelected()) {
        this->selected_player->setPosition(this->selected_player->getLocation());
        this->selected_player->unsetSelected();
        this->selected_player = nullptr;
    }
    return nullptr;
}

bool Field::existsPlayerSelected() const {
    return this->selected_player==nullptr ? false : true;
}

bool Field::decrementMoves(uint moves) {
    if (m_moves_left < moves ) return false;
    (active_side() == White ? m_white_moves : m_black_moves) += moves;
    m_moves_left -= moves;
    return true;
}

Location Field::selectedPlayerLocation() const {
    return this->selected_player->getLocation();
}

std::optional<Location> Field::moveSelectedPlayer(Direction direction) {
    Location old_location = selected_player->getLocation();
    std::optional<uint> cost;
    std::optional<Location> new_location = old_location + direction;

    if (new_location){
        if ( isPlayerAtLocation(new_location.value()) ) {
            if ((new_location = new_location.value() + direction)) {
                cost = moveCost(old_location, new_location.value());
            }
        } else {
            cost = moveCost(old_location, new_location.value());
        }
    }

    if ( cost && decrementMoves(cost.value())) {
        selected_player->setLocation(new_location.value());
    }

    if ( isEndOfTurn() ) nextTurn();
    return (new_location && cost)? new_location: std::nullopt;
}

std::optional<Location> Field::moveSelectedPlayer(Location new_location){
    Location old_location = selected_player->getLocation();
    std::optional<uint> cost = moveCost(old_location, new_location);

    if (cost && decrementMoves(cost.value())){
        // the moves have been 'paid' successfully, we now have to move to player
        selected_player->setLocation(new_location);
    }
    unselectPlayer();

    if ( isEndOfTurn() ) nextTurn();

    return new_location;
}

void Field::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &row: field) {
        for (auto &block: row) {
            target.draw(block, states);
        }
    }

    for (auto &players: {whitePlayers, blackPlayers}){
        for(auto &player: players) {
            if(&player == this->selected_player) continue;
            target.draw(player, states);
        }
    }

    // Separately draw selected_player last so it will be drawn on top of others
    if (existsPlayerSelected()) {
        target.draw(*this->selected_player, states);
    }
}

