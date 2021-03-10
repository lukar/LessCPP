#include "contexts/game_context.h"
#include "ai.h"
#include "contexts/game_menu_context.h"
#include "helpers.h"

#include <iostream>

using namespace std::string_literals;

GameContext::GameContext(Context* previous, std::array<WallConfig, 9> wall_configs)
    : Context(previous),
      game(wall_configs),
      gui(wall_configs),
      m_game_mode(GameMode::LOCAL_PVP) {}

GameContext::GameContext(Context* previous, const nlohmann::json& game_json)
    : Context(previous),
      game(game_json),
      gui(game_json),
      m_game_mode(GameMode::LOCAL_PVP) {}

GameContext::GameContext(Context* previous, std::array<WallConfig, 9> wall_configs, uint AI_difficulty)
    : Context(previous),
      game(wall_configs),
      gui(wall_configs),
      m_AI_difficulty(AI_difficulty),
      m_game_mode(GameMode::SINGLEPLAYER) {}

GameContext::GameContext(Context* previous, const nlohmann::json& game_json, uint AI_difficulty)
    : Context(previous),
      game(game_json),
      gui(game_json),
      m_AI_difficulty(AI_difficulty),
      m_game_mode(GameMode::SINGLEPLAYER) {}

GameContext::GameContext(Context* previous, std::array<WallConfig, 9> wall_configs, std::unique_ptr<sf::TcpSocket> tcp_socket, std::string room_name)
    : Context(previous),
      game(wall_configs),
      gui(wall_configs),
      m_tcp_socket(std::move(tcp_socket)),
      m_game_mode(GameMode::MULTIPLAYER)
{
    sf::Packet packet;
    packet << "lobby"s << "post_game"s << room_name << game.getJsonRepresentation().dump();
    m_tcp_socket->send(packet);
}

GameContext::GameContext(Context* previous, const nlohmann::json& game_json, std::unique_ptr<sf::TcpSocket> tcp_socket)
    : Context(previous),
      game(game_json),
      gui(game_json),
      m_tcp_socket(std::move(tcp_socket)),
      m_game_mode(GameMode::MULTIPLAYER)
{
    m_tcp_socket->setBlocking(false);
    opponent_color = Player::WHITE;
}

void GameContext::processBackgroundTask()
{
    //// Multiplayer - opponent - Update Player 2 moves
    if (m_game_mode == GameMode::MULTIPLAYER and game.getState() != GameState::ENDED and game.active_player() == opponent_color) {
        if (game.getState() == GameState::PREVIEW) return; // TODO: check if causes problems ...

        if (game.active_player() == opponent_color) {
            if (auto optional_link = wait_move(*m_tcp_socket)) {
                auto link = optional_link.value();
                if (game.movePiece(link.first, link.second)) {
                    const auto [piece_idx, player] = gui.pieceAtLocation(link.first).value();
                    gui.getPieces(player)[piece_idx].setLocation(link.second);
                    sound_drop.play();
                }
            }
        }
    }
}

void GameContext::leave_room()
{
    sf::Packet packet;
    packet << "server" << "leave_room";
    m_tcp_socket->send(packet);
}

void GameContext::send_move(const Location& location_old, const Location& location_new) {
    sf::Packet packet;
    packet << "peer" << "move" << location_old.x << location_old.y << location_new.x << location_new.y;
    m_tcp_socket->send(packet);
}

Context* GameContext::processEvent(const sf::Event & event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q) {
            return new GameMenuContext(this, rentex.getTexture(), game);
        } else if (event.key.code == sf::Keyboard::Left) {
            const auto move = game.getReversedMove();
            if (move) {
                const auto [old_location, new_location] = move.value();
                const auto [piece_idx, player] = gui.pieceAtLocation(old_location).value();
                gui.getPieces(player)[piece_idx].setLocation(new_location);
            } else {
                std::cout << "Can't go back. At the first move\n";
            }
        }
        else if (event.key.code == sf::Keyboard::Right) {
            const auto move = game.getMove();
            if (move) {
                const auto [old_location, new_location] = move.value();
                const auto [piece_idx, player] = gui.pieceAtLocation(old_location).value();
                gui.getPieces(player)[piece_idx].setLocation(new_location);
            } else {
                std::cout << "Can't go forward. At the last move\n";
            }
        }
    }
    if(game.getState() == GameState::ENDED) return nullptr;

    // HUMAN - Playr 1
    if (game.active_player() != opponent_color) { // PvP -> player vs player
        // GRAB PIECE
        if (event.type == sf::Event::MouseButtonPressed) {
            for (auto& piece : gui.getPieces(game.active_player())) {
                if (euclideanDistance(m_mousepos, piece.getPosition()) <= piece_size) {
                    sound_pickup.play();
                    held_piece = &piece;
                }
            }
        }
        // DROP PIECE
        if (event.type == sf::Event::MouseButtonReleased) {
            if (held_piece != nullptr) {
                std::optional<Location> new_location = locationFromPosition(m_mousepos);

                if (new_location) {
                    if (game.movePiece(held_piece->getLocation(), new_location.value())) {
                        if (m_game_mode == GameMode::MULTIPLAYER) send_move(held_piece->getLocation(), new_location.value());
                        held_piece->setLocation(new_location.value());
                        sound_drop.play();
                    }
                    else {
                        sound_illegal.play();
                    }
                }
                else {
                    sound_illegal.play();
                }
                held_piece->resetPosition();
                held_piece = nullptr;
            }
        }
    }
    //// AI
    else if (game.active_player() == opponent_color and m_game_mode == GameMode::SINGLEPLAYER) {
        const auto & [path, eval] = recurseFindOptimal(game, Player::BLACK, 1, 0, 100);
        for (auto [piece, direction] : path) {
            if (game.active_player() != Player::BLACK) break;
            auto newlocation = game.movePiece(piece, direction);
            gui.getPieces(Player::BLACK)[piece].setLocation(newlocation.value());
            gui.getPieces(Player::BLACK)[piece].resetPosition();
        }
    }


    return nullptr;
}

sf::Texture GameContext::render()
{
    text.setString(get_side_text(game));

    if (held_piece != nullptr) held_piece->setPosition(m_mousepos);

    rentex.clear();

    rentex.draw(text);

    rentex.draw(gui);
    if (held_piece != nullptr) rentex.draw(*held_piece);

    rentex.display();


    return rentex.getTexture();
}
