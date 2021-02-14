#ifndef GAME_H
#define GAME_H

#include "const_globals.h"
#include <random>
#include <array>
#include <utility>

#include "helpers.h"

#include <nlohmann/json.hpp>

class GameBase
{
private:
    std::array<std::array<WallConfig, 3>, 3> & m_wall_matrix;

    int m_moves_left = 3;
    Player m_active_player = Player::WHITE;
    Player m_winning_player = Player::NONE;
    int m_white_moves = 0;
    int m_black_moves = 0;

    Locations<4> m_whiteLocations = whiteStart;
    Locations<4> m_blackLocations = blackStart;

    GameState m_state = GameState::ONGOING;
    GameState m_prev_state = GameState::ONGOING;

    void setPieceLocation(uint, const Location&);


public:

    // for Game class
    GameBase(std::array<std::array<WallConfig, 3>, 3> & wall_matrix) : m_wall_matrix(wall_matrix) {};
    GameBase(const nlohmann::json & game_json, std::array<std::array<WallConfig, 3>, 3> & wall_matrix) : m_wall_matrix(wall_matrix) {
        m_moves_left = game_json["moves_left"];
        m_state = game_json["state"];
        m_active_player = game_json["active_player"];
        m_winning_player = game_json["winning_player"];
        m_white_moves = game_json["white_moves"];
        m_black_moves = game_json["black_moves"];
        m_whiteLocations = game_json["whiteLocations"];
        m_blackLocations = game_json["blackLocations"];
    };

    // for GameRef class
    GameBase(const GameBase &) = default;
    GameBase()=default;
    bool operator==(const GameBase & ref) const {
        return piecesInLocations(m_whiteLocations, ref.m_whiteLocations) and
            piecesInLocations(m_blackLocations, ref.m_blackLocations);
    }

    // Getter methods
    int           white_moves() const { return m_white_moves; }
    int           black_moves() const { return m_black_moves; }
    GameState     getState() const { return m_state; }
    Player        active_player() const { return m_active_player; }
    Player        winning_player() const { return m_winning_player; }
    int           moves_left() const { return m_moves_left; }
    Locations<4>& active_pieces() {
        return m_active_player == Player::WHITE? m_whiteLocations : m_blackLocations;
    }


    std::optional<int> getPieceNumber(const Location&);

    Locations<4> getPieces(Player player) const {
        if (player == Player::WHITE) {
                return m_whiteLocations;
        } else {
                return m_blackLocations;
        }
    }

    std::optional<std::tuple<Player, int>> pieceAtLocation(const Location& location) const {
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

    int countInnerWalls(const Location&, const Location&) const;

    bool existsPieceAtLocation(const Location&) const;

    std::optional<int> moveCost (const Location&, const Direction) const;

    std::optional<int> moveCost (const Location&, const Location&) const;

    Location getPieceLocation(uint);

    bool decrementMoves(int);

    std::optional<Location> movePiece(int, const Direction);

    std::optional<Location> movePiece(const Location&, const Location&);

protected:

    nlohmann::json getPrivateFields() const;
    void setState(GameState state) {
        m_prev_state = m_state;
        m_state = state;
    }

    void Preview() { if (m_state != GameState::PREVIEW) setState(GameState::PREVIEW); };
    void Resume() { if (m_state == GameState::PREVIEW) setState(m_prev_state); };
};

class Game : public GameBase
{
public:
    std::array<std::array<WallConfig, 3>, 3> m_wall_matrix;

    History m_history; // All moves that were made so far
    uint m_history_index = 0;

    Game(std::array<WallConfig, 9> wallconfigs) : GameBase(m_wall_matrix) {
        for (size_t y = 0; y < 3; ++y) {
            for (size_t x = 0; x < 3; ++x) {
                m_wall_matrix[y][x] = wallconfigs[y*3 + x];
            }
        }
    }

    Game(const nlohmann::json & game_json) : GameBase(game_json, m_wall_matrix) {
        m_wall_matrix = game_json["wall_matrix"];
        m_history = game_json["history"].get<History>();
    }

    nlohmann::json getJsonRepresentation() const {
        nlohmann::json fields = getPrivateFields();
        fields["wall_matrix"] = m_wall_matrix;
        fields["history"] = m_history;

        return fields;
    }

    // overload movePiece functions to produce side-effect of logging moves
    std::optional<Location> movePiece(uint piece, Direction direction) {
        if (getState() == GameState::PREVIEW) return {};
        const auto old_location = active_pieces()[piece];
        const auto new_location = GameBase::movePiece(piece, direction);
        if (new_location) m_history.push_back({old_location, new_location.value()});
        return new_location;
    }

    std::optional<Location> movePiece(Location old_location, Location new_location) {
        if (getState() == GameState::PREVIEW) return {};
        auto result = GameBase::movePiece(old_location, new_location);
        if (result and old_location != new_location) {
            m_history.push_back({old_location, new_location});
        }
        return result;
    }

    const std::optional<Link> getReversedMove() {
        if (m_history_index == m_history.size()) return {};
        Preview();
        auto [start, end] = m_history.rbegin()[m_history_index++]; 
        return {{end, start}};
    }

    const std::optional<Link> getMove() {
        if (m_history_index == 0) { Resume(); return {}; }
        return m_history.rbegin()[--m_history_index]; 
    }

    Game() = default;
    Game(const Game &) = delete;

};

class GameRef : public GameBase
{
public:

    GameRef(const GameBase & ref) : GameBase(ref) {};

};

#endif // GAME_H

