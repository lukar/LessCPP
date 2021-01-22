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
#include "nlohmann/json.hpp"

class GameContext : public Context {
private:
	Game game;
	Gui gui;

	Piece* held_piece = nullptr;

	// game sounds
	std::deque<sf::SoundBuffer> soundBuffers;
	sf::Sound sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sf::Sound sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sf::Sound sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(font, 10, window_height + 10, 10, sf::Color::Green);

	bool ai_enable = false;
	bool multiplayer_game_ready = false;
	Player opponent_color = Player::BLACK;
	sf::IpAddress ip_player2="0.0.0.0";
	unsigned short tcp_port = 53012;
	sf::TcpListener listener;
	sf::TcpSocket tcp_socket;
	
public:

	GameContext(std::array<WallConfig, 9>);
	GameContext(const nlohmann::json &);
	GameContext(const nlohmann::json& ,sf::IpAddress ip_player2, unsigned short tcp_port);
	GameContext(int quitLevelInc);

	void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };

	Context* processEvent(const sf::Event& event) override;
	Context* processBackgroundTask(void) override;
	
	sf::Texture render() override;

};


#endif // GAME_CONTEXT_LESSCPP
