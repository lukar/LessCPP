#include "game_context.h"
#include "ai.h"
#include <tuple>

GameContext::GameContext() : game(Game(wall_configs)), gui(Gui(wall_configs))
{
	texture.create(window_width, window_height);

	// create texture (necessary)

	sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// generate side text
	font = getFont("resources/Roboto_Medium.ttf");
	text = initializePlayerText(font, window_height + 10, 10, sf::Color::Green);
}

Result GameContext::process(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	// HUMAN
	if (game.getState() != GameState::ENDED and game.active_player() != Player::BLACK) {
		// GRAB PLAYER
		if (event.type == sf::Event::MouseButtonPressed) {
			for (auto& piece : gui.getPieces(game.active_player())) {
				if (euclideanDistance(mouse_pos, piece.getPosition()) <= piece_size) {
					sound_pickup.play();
					held_piece = &piece;
				}
			}
		}
		// DROP PLAYER
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
		auto path = recurseFindOptimal(game, Player::BLACK, 1, 0, 100, evaluation(game));
		for (auto elem : path) {
			if (game.active_player() != Player::BLACK) break;
			auto newlocation = game.movePiece(std::get<0>(elem), std::get<1>(elem));
			gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].setLocation(newlocation.value());
			gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].resetPosition();
		}
	}

	text.setString(get_side_text(game));


	// DRAWING
	if (held_piece != nullptr) held_piece->setPosition(mouse_pos);

	texture.clear();

	texture.draw(text);

	texture.draw(gui);
	if (held_piece != nullptr) texture.draw(*held_piece);

	texture.display();


	return texture.getTexture();
}
