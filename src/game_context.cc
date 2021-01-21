#include "game_context.h"
#include "ai.h"
#include "sub_menu_context.h"
#include "helpers.h"


#include <iostream>

GameContext::GameContext(std::array<WallConfig, 9> wall_configs) : game(wall_configs), gui(wall_configs) {}

GameContext::GameContext(const nlohmann::json & game_json) : game(game_json), gui(game_json) {}

Context* GameContext::processEvent(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::S) {
		// NETWORKING
		// temporary; to be moved to main menu - Join game
		// bind the socket to a port
		if (socket_recieve_game_context.bind(5555) != sf::Socket::Done)
		{
			// error...
		}
		std::cout << "Socket recieve - game context context bound to :" << socket_recieve_game_context.getLocalPort() << "\n";
		char data[100];
		socket_recieve_game_context.setBlocking(true);

		std::size_t received = 0;
		if (socket_recieve_game_context.receive(data, 100, received, sender, port) != sf::Socket::Done) {
			// error...
			std::cout << "Error recieve\n";
		}
		if (received > 0) {
			socket_recieve_game_context.unbind();
			std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
			if (strncmp("init", data, 4) == 0) {
				std::cout << "Init recieved\n";
				sleep(1000); // miliseconds
				std::cout << "sending ...\n";
				udpSendStr(game.getJsonRepresentation().dump(),"127.0.0.1",5554);
				std::cout << "Sent!\n";
			}
		}
		
		// END GAME?
		} else if (event.key.code == sf::Keyboard::Q) {
			// quit = true;
			return new SubMenuContext(quitLevel, rentex.getTexture(), game);
		} else if (event.key.code == sf::Keyboard::P) {
			// quit = true;
			PvP = !PvP; // toggle
			std::cout << "PvP = " << PvP <<"\n";
		} else if (event.key.code == sf::Keyboard::O) {
			// quit = true;
			if (Player::BLACK == multiplayer_opponent) {
				multiplayer_opponent = Player::WHITE;
				std::cout << "multiplayer_opponent = WHITE\n";
			}
			else {
				multiplayer_opponent = Player::BLACK;
				std::cout << "multiplayer_opponent = BLACK\n";
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
	// HUMAN
	if ((game.getState() != GameState::ENDED) and ((game.active_player() != Player::BLACK) or PvP)) { // PvP -> player vs player
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
						held_piece->setLocation(new_location.value());
						sound_drop.play();
						udpSendStr(game.getJsonRepresentation().dump());
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
	else if (game.getState() != GameState::ENDED and game.active_player() == Player::BLACK and PvP == false) {
		const auto & [path, eval] = recurseFindOptimal(game, Player::BLACK, 1, 0, 100);
		for (auto [piece, direction] : path) {
			if (game.active_player() != Player::BLACK) break;
			auto newlocation = game.movePiece(piece, direction);
			gui.getPieces(Player::BLACK)[piece].setLocation(newlocation.value());
			gui.getPieces(Player::BLACK)[piece].resetPosition();
		}
	}
	//// Multiplayer - opponent
	/*else if (game.getState() != GameState::ENDED and game.active_player() == multiplayer_opponent) {
		path = 
		for (auto [piece, direction] : path) {
			if (game.active_player() != Player::BLACK) break;
			auto newlocation = game.movePiece(piece, direction);
			gui.getPieces(Player::BLACK)[piece].setLocation(newlocation.value());
			gui.getPieces(Player::BLACK)[piece].resetPosition();
		}
	}*/
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
