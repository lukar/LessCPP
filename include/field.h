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

class Field {
private:
    int X = 3;
    int Y = 3;

    sf::RenderWindow & r_window;

    std::vector<std::vector<Block>> field;

    std::array<Player, 4> whitePlayers;
    std::array<Player, 4> blackPlayers;

    Player * selected_player = nullptr;

    int m_moves_left = 3;
    bool m_game_over = false;
    bool m_white_finished = false;
    bool m_black_finished = false;
    sf::Color m_active_side = White;
    sf::Color m_winning_side;
    int m_white_moves = 0;
    int m_black_moves = 0;

public:
    Field(sf::RenderWindow & window);

    [[nodiscard]] int white_moves() const;
    [[nodiscard]] int black_moves() const;
    [[nodiscard]] sf::Color active_side() const;
    [[nodiscard]] std::array<Player, 4> & active_players();
    [[nodiscard]] sf::Color winning_side() const;
    [[nodiscard]] int moves_left() const;
    [[nodiscard]] bool game_over() const;

    void next_turn(int);

    void setGameOver(sf::Color);

    [[nodiscard]] int countInnerWalls(Location const, Location const) const;

    [[nodiscard]] bool isPlayerAtLocation(Location const) const;

    [[nodiscard]] std::optional<int> moveCost[[nodiscard]] (Location, Location) const;

    [[nodiscard]] bool playersInLocations(std::array<Player, 4> const &,
                            std::array<Location, 4> const &) const;

    void selectPlayer(Player &);

    void unselectPlayer();

    [[nodiscard]] bool existsPlayerSelected() const;

    [[nodiscard]] bool decrementMoves(int);

    [[nodiscard]] Location selectedPlayerLocation();

    [[nodiscard]] bool moveSelectedPlayer(Location);

    void draw();

};

#endif // FIELD_H

