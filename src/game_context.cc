#include "game_context.h"
#include "ai.h"
#include "sub_menu_context.h"
#include "helpers.h"


#include <iostream>

GameContext::GameContext(std::array<WallConfig, 9> wall_configs) : game(wall_configs), gui(wall_configs) {}

GameContext::GameContext(const nlohmann::json & game_json) : game(game_json), gui(game_json) {}
GameContext::GameContext(const nlohmann::json& game_json, sf::IpAddress ip_player2, unsigned short tcp_port = 53000) : game(game_json), gui(game_json) {
	ip_player2 = ip_player2;
	tcp_port = tcp_port;
	std::cout << "Initializing cp socket\n";
	sf::Socket::Status status = tcp_socket.connect(ip_player2, tcp_port);

	if (status != sf::Socket::Done)	{
		std::cout << "Error connecting tcp - constructor " << status <<"\n";
	}	else {
		std::cout << "Tcp socket connected " << status << "\n";
	}
	tcp_socket.setBlocking(false);
	ai_enable = false;
	opponent_color = Player::WHITE;
	multiplayer_game_ready = true;
}


void GameContext::processBackgroundEvents() { 
	//// Multiplayer - opponent - Update Player 2 moves
	if (game.getState() != GameState::ENDED and game.active_player() == opponent_color) {
		auto optional_link = wait_move(tcp_socket);
		if (!optional_link)
			return; // error reading socket
		auto link = optional_link.value();
		if (game.movePiece(link.first, link.second)) {
			const auto [piece_idx, player] = gui.pieceAtLocation(link.first).value();
			gui.getPieces(player)[piece_idx].setLocation(link.second);
			sound_drop.play();
		}
	}
	return;
}

Context* GameContext::processBackgroundTask() {
	processBackgroundEvents(); //(networking)
	return nullptr;
}
Context* GameContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::S) {
			ai_enable = false;
			// bind the listener to a port
			if (listener.listen(tcp_port) != sf::Socket::Done) { /*connection from main menu*/
				// error...
			}
			ip_player2 = host_game_tcp_packets(game.getJsonRepresentation().dump(), listener);
			if (listener.listen(tcp_port) != sf::Socket::Done) { /*connection from game context*/
				// error...
			}
			if (listener.accept(tcp_socket) != sf::Socket::Done) {
				// error...
			}
			std::cout << "Tcp socket connected\n";
			tcp_socket.setBlocking(false);
			multiplayer_game_ready = true;
		
		// END GAME?
		} else if (event.key.code == sf::Keyboard::Q) {
			// quit = true;
			return new SubMenuContext(quitLevel, rentex.getTexture(), game);
		} else if (event.key.code == sf::Keyboard::P) {
			if (ip_player2 != sf::IpAddress::Any) {
				ai_enable = false;
				std::cout << "You are in a multiplayer game; local_PvP = false\n ";
			} else {
				ai_enable = !ai_enable; // toggle
			std::cout << "local_PvP = " << ai_enable <<"\n";
			}
		} 
		else if (event.key.code == sf::Keyboard::Left) {
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

	// HUMAN - Playr 1
	if ((game.getState() != GameState::ENDED) and (game.active_player() != opponent_color)) { // PvP -> player vs player
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
						send_move(tcp_socket, held_piece->getLocation(), new_location.value());
						held_piece->setLocation(new_location.value());
						sound_drop.play();
						//udpSendStr(game.getJsonRepresentation().dump());
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
	else if (game.getState() != GameState::ENDED and game.active_player() == Player::BLACK and ai_enable == true) {
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
