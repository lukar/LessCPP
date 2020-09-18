#include "const_globals.h"
#include "game.h"
#include "helpers.h"
#include "wall.h"

#include <array>
#include <stdexcept>
#include <algorithm>

Game::Game(std::array<WallConfig, 9> wallconfigs) {
    for (size_t y = 0; y < 3; ++y) {
        for (size_t x = 0; x < 3; ++x) {
            m_wall_matrix[y][x] = wallconfigs[y*3 + x];
        }
    }
}

bool Game::operator==(const Game & game) const {
    if ( piecesInLocations(m_whiteLocations, game.m_whiteLocations) and
         piecesInLocations(m_blackLocations, game.m_blackLocations) ) return true;
    return false;
}

// Getter methods
int Game::white_moves() const { return m_white_moves; }
int Game::black_moves() const { return m_black_moves; }
State Game::getState() const { return m_state; }
Player Game::active_player() const { return m_active_player; }
Player Game::winning_player() const {return m_winning_player;}
int Game::moves_left() const { return m_moves_left; }

std::array<Location, 4> & Game::active_pieces() {
    return m_active_player == Player::WHITE? m_whiteLocations : m_blackLocations;
}

bool Game::isEndOfTurn() const {
    return m_moves_left == 0 or (active_player() == Player::WHITE
                                 ? piecesInLocations(m_whiteLocations, blackStart)
                                 : piecesInLocations(m_blackLocations, whiteStart));
}


void Game::nextTurn() {
    m_active_player = (m_active_player == Player::WHITE ? Player::BLACK : Player::WHITE);

    if ( piecesInLocations(m_blackLocations, whiteStart) or m_state == State::LAST_TURN ) m_state = State::ENDED;
    else if ( piecesInLocations(m_whiteLocations, blackStart) ) { m_state = State::LAST_TURN; m_moves_left = 3 - m_moves_left; }
    else { m_moves_left = 3; }

    // Wait if black also reaches end in same turn
    if ( m_state == State::ENDED ) {
        if ( piecesInLocations(m_blackLocations, whiteStart) and piecesInLocations(m_whiteLocations, blackStart) ) m_winning_player = Player::NONE;
        else if (piecesInLocations(m_whiteLocations, blackStart)) m_winning_player = Player::WHITE;
        else m_winning_player = Player::BLACK;
    }
}

void Game::setGameOver(Player winner) { m_state = State::ENDED; m_winning_player = winner; }

// Return number of wall segments between start and end.
constexpr int Game::countInnerWalls(Location const start, Location const end) const {
    
    // differences
	int const dx = end.x - start.x;
	int const dy = end.y - start.y;

    // block coordinates
	uint const sx = uint(start.x / 2);
	uint const sy = uint(start.y / 2);

	uint const ex = uint(end.x / 2);
	uint const ey = uint(end.y / 2);
    WallConfig const & wstart = m_wall_matrix[sy][sx];
    WallConfig const & wend = m_wall_matrix[ey][ex];

    int const first = (dx ? 1 : -1);
    int const second = (sx == ex && sy == ey ? 0 : dx + dy);
	int const third = -1 + 2 * cabs(dx) * (start.y % 2) + 2 * cabs(dy) * (start.x % 2);

	int num = 0;
    if (second) {
        num += wall::hasWallSeg(wend, {first, -second, third});
    }
    num += wall::hasWallSeg(wstart, {first, second, third});

    return num;
}

// Is there a Piece on that Location?
constexpr bool Game::existsPieceAtLocation(Location const location) const {
    bool toReturn = false;
    for (const auto &piece : m_whiteLocations) {
        if (piece == location) toReturn = true;
    }
    for (const auto &piece : m_blackLocations) {
        if (piece == location) toReturn = true;
    }
    return toReturn;
}

// For AI
constexpr std::optional<int> Game::moveCost(Location old_location, Direction direction) const {
    if (std::optional<Location> new_location = old_location + direction) {
        return countInnerWalls(old_location, new_location.value());
    }
    return {};
}

// For human
constexpr std::optional<int> Game::moveCost(Location oldL, Location newL) const {

    if (oldL == newL) return 0;

    if (newL.x - oldL.x != 0 && newL.y - oldL.y != 0) return {};

    if (existsPieceAtLocation(newL)) return {};

	int wallCount = 0;
	int distance = abs(newL.x - oldL.x) + abs(newL.y - oldL.y);
    if (distance > 2) return {};
    if (distance == 2) {
        Location const connecting = {(newL.x + oldL.x) / 2, (newL.y + oldL.y) / 2};
        wallCount += countInnerWalls(oldL, connecting);
        wallCount += countInnerWalls(connecting, newL);

        if (!existsPieceAtLocation(connecting)) return {};
        if (wallCount != 0) return {};
        return 1;
    }
    wallCount += countInnerWalls(oldL, newL);
    return wallCount + 1;
}


std::optional<int> Game::getPieceNumber(Location location) {
    for (size_t i = 0; i != 4; ++i) {
        if( active_pieces()[i] == location ) return i;
    }
    return {};
}

Location Game::getPieceLocation(int piece_num) {
	return active_pieces()[static_cast<uint>(piece_num)];
}

bool Game::decrementMoves(int moves) {
    if (m_moves_left < moves ) return false;
    (m_active_player == Player::WHITE ? m_white_moves : m_black_moves) += moves;
    m_moves_left -= moves;
    return true;
}

// for AI
std::optional<Location> Game::movePiece(int piece_num, Direction direction) {
    Location old_location = getPieceLocation(piece_num);
	std::optional<int> cost;
    std::optional<Location> new_location = old_location + direction;

    if (new_location){
        if ( existsPieceAtLocation(new_location.value()) ) {
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
std::optional<Location> Game::movePiece(Location old_location, Location new_location){
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

void Game::setPieceLocation(int piece, Location location) {
	active_pieces()[static_cast<uint>(piece)] = location;
}

