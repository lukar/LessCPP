#ifndef GAME_H
#define GAME_H

#include "const_globals.h"
#include <random>
#include <array>

#include "helpers.h"

class GameBase {
private:
	std::array<std::array<WallConfig, 3>, 3> & m_wall_matrix;

	int m_moves_left = 3;
	GameState m_state = GameState::ONGOING;
	Player m_active_player = Player::WHITE;
	Player m_winning_player = Player::NONE;
	int m_white_moves = 0;
	int m_black_moves = 0;

	Locations<4> m_whiteLocations = whiteStart;
	Locations<4> m_blackLocations = blackStart;

	void setPieceLocation(uint, const Location&);

public:

	// for Game class
	GameBase(std::array<std::array<WallConfig, 3>, 3> & wall_matrix) : m_wall_matrix(wall_matrix) {};

	// for GameRef class
	GameBase(const GameBase &) = default;

	bool operator==(const GameBase & ref) const {
		return piecesInLocations(m_whiteLocations, ref.m_whiteLocations) and
			piecesInLocations(m_blackLocations, ref.m_blackLocations);
	}

	// Getter methods
	constexpr int           white_moves() const { return m_white_moves; }
	constexpr int           black_moves() const { return m_black_moves; }
	constexpr GameState     getState() const { return m_state; }
	constexpr Player        active_player() const { return m_active_player; }
	constexpr Player        winning_player() const { return m_winning_player; }
	constexpr int           moves_left() const { return m_moves_left; }
	constexpr Locations<4>& active_pieces() {
		return m_active_player == Player::WHITE? m_whiteLocations : m_blackLocations;
	}


	std::optional<int> getPieceNumber(const Location&);

	constexpr Locations<4> getPieces(Player player) const {
		if (player == Player::WHITE) {
				return m_whiteLocations;
		} else {
				return m_blackLocations;
		}
	}

	constexpr std::optional<std::tuple<Player, int>> pieceAtLocation(const Location& location) const {
		for (int piece = 0; piece < 4; ++piece) {
				if (m_whiteLocations[piece] == location) return {{Player::WHITE, piece}};
		}
		for (int piece = 0; piece < 4; ++piece) {
				if (m_blackLocations[piece] == location) return {{Player::BLACK, piece}};
		}
		return {};
	}

	bool isEndOfTurn() const;

	void nextTurn();

	constexpr int countInnerWalls(const Location&, const Location&) const;

	constexpr bool existsPieceAtLocation(const Location&) const;

	constexpr std::optional<int> moveCost (const Location&, const Direction) const;

	constexpr std::optional<int> moveCost (const Location&, const Location&) const;

	Location getPieceLocation(uint);

	bool decrementMoves(int);

	std::optional<Location> movePiece(int, const Direction);

	std::optional<Location> movePiece(const Location&, const Location&);

};

class Game : public GameBase {
public:
	std::array<std::array<WallConfig, 3>, 3> m_wall_matrix;
	
	Game(std::array<WallConfig, 9> wallconfigs) : GameBase(m_wall_matrix) {
		for (size_t y = 0; y < 3; ++y) {
			for (size_t x = 0; x < 3; ++x) {
				m_wall_matrix[y][x] = wallconfigs[y*3 + x];
			}
		}
	}

	Game(const Game &) = delete;

};

class GameRef : public GameBase {
public:

	GameRef(const GameBase & ref) : GameBase(ref) {};

};

#endif // GAME_H

