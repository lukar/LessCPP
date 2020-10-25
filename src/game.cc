#include "const_globals.h"
#include "game.h"
#include "helpers.h"
#include "wall.h"

#include <array>
#include <stdexcept>
#include <algorithm>

bool GameBase::isEndOfTurn() const {
	return m_moves_left == 0 or (active_player() == Player::WHITE
															 ? piecesInLocations(m_whiteLocations, blackStart)
															 : piecesInLocations(m_blackLocations, whiteStart));
}


void GameBase::nextTurn() {
	m_active_player = (m_active_player == Player::WHITE ? Player::BLACK : Player::WHITE);

	if (piecesInLocations(m_blackLocations, whiteStart) or m_state == GameState::LAST_TURN) {
		m_state = GameState::ENDED;
	} else if (piecesInLocations(m_whiteLocations, blackStart)) {
		m_state = GameState::LAST_TURN;
		m_moves_left = 3 - m_moves_left;
	} else {
		m_moves_left = 3;
	}

	// Wait if black also reaches end in same turn
	if (m_state == GameState::ENDED) {
		if (piecesInLocations(m_blackLocations, whiteStart) and piecesInLocations(m_whiteLocations, blackStart)) {
			m_winning_player = Player::NONE;
		}
		else if (piecesInLocations(m_whiteLocations, blackStart)) {
			m_winning_player = Player::WHITE;
		}
		else {
			m_winning_player = Player::BLACK;
		}
	}
}

// Return number of wall segments between start and end. Locations must be orthogonal and adjacent
constexpr int GameBase::countInnerWalls(Location const start, Location const end) const {
    
    // differences
	const int dx = end.x - start.x;
	const int dy = end.y - start.y;

    // block coordinates
	const uint sx = uint(start.x / 2);
	const uint sy = uint(start.y / 2);

	const uint ex = uint(end.x / 2);
	const uint ey = uint(end.y / 2);
	const WallConfig & wstart = m_wall_matrix[sy][sx];
	const WallConfig & wend = m_wall_matrix[ey][ex];

	const int first = (dx ? 1 : -1);
	const int second = (sx == ex and sy == ey ? 0 : dx + dy);
	const int third = 2*(dx ? start.y%2 : start.x%2) - 1; 

	int num = 0;
	if (second) {
			num += wall::hasWallSeg(wend, {first, -second, third});
	}
	num += wall::hasWallSeg(wstart, {first, second, third});

	return num;
}

// For AI
constexpr std::optional<int> GameBase::moveCost(Location old_location, Direction direction) const {
	if (std::optional<Location> new_location = old_location + direction) {
			return countInnerWalls(old_location, new_location.value());
	}
	return {};
}

// For human
constexpr std::optional<int> GameBase::moveCost(Location oldL, Location newL) const {

	if (oldL == newL) return 0;

	if (newL.x - oldL.x != 0 && newL.y - oldL.y != 0) return {};

	if (pieceAtLocation(newL)) return {};

	int wallCount = 0;
	int distance = abs(newL.x - oldL.x) + abs(newL.y - oldL.y);
	if (distance > 2) return {};
	if (distance == 2) {
		Location const connecting = {(newL.x + oldL.x) / 2, (newL.y + oldL.y) / 2};
		wallCount += countInnerWalls(oldL, connecting);
		wallCount += countInnerWalls(connecting, newL);

		if (!pieceAtLocation(connecting)) return {};
		if (wallCount != 0) return {};
		return 1;
	}
	wallCount += countInnerWalls(oldL, newL);
	return wallCount + 1;
}


std::optional<int> GameBase::getPieceNumber(Location location) {
	for (size_t i = 0; i != 4; ++i) {
			if( active_pieces()[i] == location ) return i;
	}
	return {};
}

Location GameBase::getPieceLocation(int piece_num) {
	return active_pieces()[static_cast<uint>(piece_num)];
}

bool GameBase::decrementMoves(int moves) {
	if (m_moves_left < moves ) return false;
	(m_active_player == Player::WHITE ? m_white_moves : m_black_moves) += moves;
	m_moves_left -= moves;
	return true;
}

// for AI
std::optional<Location> GameBase::movePiece(int piece_num, Direction direction) {
	Location old_location = getPieceLocation(piece_num);
	std::optional<int> cost;
	std::optional<Location> new_location = old_location + direction;

	if (new_location){
		if ( pieceAtLocation(new_location.value()) ) {
				if ((new_location = new_location.value() + direction)) {
						cost = moveCost(old_location, new_location.value());
				}
		} else {
				cost = moveCost(old_location, new_location.value());
		}
	}

	if ( cost && decrementMoves(cost.value())) {
		setPieceLocation(piece_num, new_location.value());
	} else {
		return {};
	}

	if ( isEndOfTurn() ) nextTurn();
	return (new_location && cost)? new_location: std::nullopt;
}

// For human
std::optional<Location> GameBase::movePiece(Location old_location, Location new_location){
	auto piece = getPieceNumber(old_location);
	if (!piece) return {};
	std::optional<int> cost = moveCost(old_location, new_location);

	if (cost && decrementMoves(cost.value())){
		// the moves have been 'paid' successfully, we now have to move to piece
		setPieceLocation(piece.value(), new_location);
	} else {
		return {};
	}

	if ( isEndOfTurn() ) nextTurn();

	return new_location;
}

void GameBase::setPieceLocation(int piece, Location location) {
	active_pieces()[static_cast<uint>(piece)] = location;
}

