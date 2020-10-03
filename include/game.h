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

	// Getter methods
	constexpr int white_moves() const { return m_white_moves; }
	constexpr int black_moves() const { return m_black_moves; }
	constexpr GameState getState() const { return m_state; }
	constexpr Player active_player() const { return m_active_player; }
	constexpr Player winning_player() const { return m_winning_player; }
	constexpr int moves_left() const { return m_moves_left; }
	constexpr std::array<Location, 4> & active_pieces() {
		return m_active_player == Player::WHITE? m_whiteLocations : m_blackLocations;
	}


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

