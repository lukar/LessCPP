#include "game_context.h"

GameContext::GameContext() : game(Game(wall_configs)), gui(Gui(wall_configs)) {

	// create texture (necessary)
	texture.create(window_width, window_height);

	sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// generate side text
	font = getFont("resources/Roboto_Medium.ttf");
	text = initializePlayerText(font, window_height + 10, 10, sf::Color::Green);
}

Result GameContext::process(sf::Event event, sf::Vector2f mouse_pos) {
	text.setString(get_side_text(game));

	// ON CLOSE EVENT
	//if (event.type == sf::Event::Closed) {
	//	window.close();
	//}
	//// CLOSE WHEN PRESSED Q
	//if (event.type == sf::Event::KeyPressed) {
	//	if (event.key.code == sf::Keyboard::Q) {
	//		window.close();
	//	}
	//}
	// HUMAN
	if (game.getState() != GameState::ENDED) {
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
	//else if (game.getState() != GameState::ENDED and game.active_player() == Player::BLACK) {
	//	auto path = recurseFindOptimal(game, Player::BLACK, 1, 0, 100, evaluation(game)).value();
	//	for (auto elem : path) {
	//		if (game.active_player() != Player::BLACK) break;
	//		auto newLocation = game.movePiece(std::get<0>(elem), std::get<1>(elem));
	//		gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].setLocation(newLocation.value());
	//		gui.getPieces(Player::BLACK)[static_cast<uint>(std::get<0>(elem))].resetPosition();
	//		window.clear();
	//		window.draw(text);
	//		window.draw(gui);
	//		window.display();
	//	}
	//}


	 // DRAWING
	 if (held_piece != nullptr) held_piece->setPosition(mouse_pos);

	 texture.clear();
	 
	 texture.draw(text);

	 texture.draw(gui);
	 if (held_piece != nullptr) texture.draw(*held_piece);

	 texture.display();


	 return texture.getTexture();
}