#ifndef FIELD_H
#define FIELD_H

#include "const_globals.h"
#include <random>
#include <array>

#include "helpers.h"
#include <random>

#include <iostream>

class Game {
private:

//    Player * selected_player = nullptr;

    uint m_moves_left = 3;
    State m_state = State::ONGOING;
    Side m_active_side = Side::WHITE;
    Side m_winning_side;
    uint m_white_moves = 0;
    uint m_black_moves = 0;

    std::array<Location, 4> m_whiteLocations = whiteStart;
    std::array<Location, 4> m_blackLocations = blackStart;

    std::array<std::array<WallConfig, 3>, 3> m_wall_matrix;

    void setPlayerLocation(uint, Location);

public:
    Game(std::array<WallConfig, 9>);
    Game(const Game &) = default;
    bool operator==(const Game &) const;

    [[nodiscard]] uint white_moves() const;
    [[nodiscard]] uint black_moves() const;
    [[nodiscard]] State getState() const;
    [[nodiscard]] Side active_side() const;
    [[nodiscard]] std::array<Location, 4> & active_players();
    [[nodiscard]] Side winning_side() const;
    [[nodiscard]] uint moves_left() const;

    std::optional<uint> getPlayerNumber(Location);

    constexpr std::array<Location, 4> getPlayers(Side side) const {
        if (side == Side::WHITE) {
            return m_whiteLocations;
        } else {
            return m_blackLocations;
        }
    }

    bool isEndOfTurn() const;

    void nextTurn();

    void setGameOver(Side);

	[[nodiscard]] constexpr uint countInnerWalls(Location const, Location const) const;

	[[nodiscard]] constexpr bool existsPlayerAtLocation(Location const) const;

	[[nodiscard]] constexpr std::optional<uint> moveCost (Location, Direction) const;

	[[nodiscard]] constexpr std::optional<uint> moveCost (Location, Location) const;

    Location getPlayerLocation(uint);

    [[nodiscard]] bool decrementMoves(uint);

    std::optional<Location> movePlayer(uint, Direction);

    std::optional<Location> movePlayer(Location, Location);

};

#endif // FIELD_H

