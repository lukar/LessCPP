#ifndef GAME_CONTEXT_LESSCPP
#define GAME_CONTEXT_LESSCPP

#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include "less.h"
#include "ai.h"

#include "context.h"

#include <array>

class GameContext : public Context {
private:

	std::array<WallConfig, 9> wall_configs = wall::generateNwallconfigs<9>();
	Game game;
	Gui gui;

	Piece* held_piece = nullptr;

	// game sounds
	std::deque<sf::SoundBuffer> soundBuffers;
	sf::Sound sound_pickup;
	sf::Sound sound_drop;
	sf::Sound sound_illegal;

	// side text
	sf::Font font;
	sf::Text text;

public:

	// texture that will be passed out to render
	sf::RenderTexture texture;

	GameContext();

	Result process(sf::Event, sf::Vector2f) override;

};


#endif // GAME_CONTEXT_LESSCPP