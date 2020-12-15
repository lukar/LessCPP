#include "game_context.h"
#include "ai.h"

#include "sub_menu_context.h"


#include <iostream>

void GameContext::init()
{
	// create texture (necessary)
	rentex.create(window_width, window_height);

	sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// generate side text
	font = getFont("resources/Roboto_Medium.ttf");
	text = initializeText(font, 10, window_height + 10, 10, sf::Color::Green);
}

GameContext::GameContext(std::array<WallConfig, 9> wall_configs) : game(wall_configs), gui(wall_configs)
{
	init();
}

GameContext::GameContext(const nlohmann::json & game_json) : game(game_json), gui(game_json)
{
	init();
}

Context* GameContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	// END GAME?
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Q) {
			// quit = true;
			return new SubMenuContext(quitLevel, rentex.getTexture(), game);
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
	if ((game.getState() != GameState::ENDED) and game.active_player() != Player::BLACK) {
		// GRAB PIECE
		if (event.type == sf::Event::MouseButtonPressed) {
			for (auto& piece : gui.getPieces(game.active_player())) {
				if (euclideanDistance(mouse_pos, piece.getPosition()) <= piece_size) {
					sound_pickup.play();
					held_piece = &piece;
				}
			}
		}
		// DROP PIECE
		if (event.type == sf::Event::MouseButtonReleased) {
			if (held_piece != nullptr) {
				std::optional<Location> new_location = locationFromPosition(mouse_pos);

				if (new_location) {
					if (game.movePiece(held_piece->getLocation(), new_location.value())) {
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
	else if (game.getState() != GameState::ENDED and game.active_player() == Player::BLACK) {
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

sf::Texture GameContext::render(const sf::Vector2f & mouse_pos)
{
	text.setString(get_side_text(game));

	if (held_piece != nullptr) held_piece->setPosition(mouse_pos);

	rentex.clear();

	rentex.draw(text);

	rentex.draw(gui);
	if (held_piece != nullptr) rentex.draw(*held_piece);

	rentex.display();


	return rentex.getTexture();
}
