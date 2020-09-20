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

//    Piece * selected_piece = nullptr;

	int m_moves_left = 3;
    GameState m_state = GameState::ONGOING;
    Player m_active_player = Player::WHITE;
    Player m_winning_player = Player::NONE;
	int m_white_moves = 0;
	int m_black_moves = 0;

    std::array<Location, 4> m_whiteLocations = whiteStart;
    std::array<Location, 4> m_blackLocations = blackStart;

    std::array<std::array<WallConfig, 3>, 3> m_wall_matrix;

	void setPieceLocation(int, Location);

public:
    Game(std::array<WallConfig, 9>);
    Game(const Game &) = default;
    bool operator==(const Game &) const;

	[[nodiscard]] int white_moves() const;
	[[nodiscard]] int black_moves() const;
    [[nodiscard]] GameState getState() const;
    [[nodiscard]] Player active_player() const;
    [[nodiscard]] std::array<Location, 4> & active_pieces();
    [[nodiscard]] Player winning_player() const;
	[[nodiscard]] int moves_left() const;

	std::optional<int> getPieceNumber(Location);

    constexpr std::array<Location, 4> getPieces(Player player) const {
        if (player == Player::WHITE) {
            return m_whiteLocations;
        } else {
            return m_blackLocations;
        }
    }

    bool isEndOfTurn() const;

    void nextTurn();

    void setGameOver(Player);

	[[nodiscard]] constexpr int countInnerWalls(Location const, Location const) const;

	[[nodiscard]] constexpr bool existsPieceAtLocation(Location const) const;

	[[nodiscard]] constexpr std::optional<int> moveCost (Location, Direction) const;

	[[nodiscard]] constexpr std::optional<int> moveCost (Location, Location) const;

	Location getPieceLocation(int);

	[[nodiscard]] bool decrementMoves(int);

	std::optional<Location> movePiece(int, Direction);

    std::optional<Location> movePiece(Location, Location);

};

#endif // FIELD_H

