#ifndef FIELD_H
#define FIELD_H

#include "const_globals.h"
#include <random>
#include <array>
#include "block_shape.h"
#include "player.h"

#include "helpers.h"
#include <random>
#include <cassert>

#include <iostream>

class Field: public sf::Drawable {
private:
    int X = 3;
    int Y = 3;

    std::vector<std::vector<Block>> field;

    std::array<Player, 4> whitePlayers;
    std::array<Player, 4> blackPlayers;

    Player * selected_player = nullptr;

    uint m_moves_left = 3;
    State m_state = State::ONGOING;
    sf::Color m_active_side = White;
    sf::Color m_winning_side;
    uint m_white_moves = 0;
    uint m_black_moves = 0;

public:
    Field();

    [[nodiscard]] uint white_moves() const;
    [[nodiscard]] uint black_moves() const;
    [[nodiscard]] State getState() const;
    [[nodiscard]] sf::Color active_side() const;
    [[nodiscard]] std::array<Player, 4> & active_players();
    [[nodiscard]] sf::Color winning_side() const;
    [[nodiscard]] uint moves_left() const;

    bool isEndOfTurn() const;

    void nextTurn();

    void setGameOver(sf::Color);

    [[nodiscard]] uint countInnerWalls(Location const, Location const) const;

    [[nodiscard]] bool isPlayerAtLocation(Location const) const;

    [[nodiscard]] std::optional<uint> moveCost (Location, Location) const;

    [[nodiscard]] std::optional<uint> moveCost (Location, Direction) const;

    [[nodiscard]] bool playersInLocations(std::array<Player, 4> const &,
                            std::array<Location, 4> const &) const;

    Player * selectPlayer(Player &);

    Player * selectPlayer(uint);

    std::nullptr_t unselectPlayer();

    [[nodiscard]] bool existsPlayerSelected() const;

    [[nodiscard]] bool decrementMoves(uint);

    [[nodiscard]] Location selectedPlayerLocation() const;

    std::optional<Location> moveSelectedPlayer(Direction);

    std::optional<Location> moveSelectedPlayer(Location);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

};

#endif // FIELD_H

