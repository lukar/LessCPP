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

enum class GameMode {
    SINGLEPLAYER,
    LOCAL_PVP,
    MULTIPLAYER
};

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

    Player opponent_color = Player::BLACK;
    sf::IpAddress m_ip_player2="0.0.0.0";
    unsigned short m_tcp_port = 53012;
    sf::TcpListener listener;
    sf::TcpSocket tcp_socket;

    const GameMode m_game_mode;
    
public:

    GameContext(Context*, std::array<WallConfig, 9>, GameMode game_mode);
    GameContext(Context*, std::array<WallConfig, 9> wall_configs, unsigned short tcp_port /*53012*/);
    GameContext(Context*, const nlohmann::json&, GameMode game_mode);
    GameContext(Context*, const nlohmann::json& ,sf::IpAddress ip_player2, unsigned short tcp_port);

    void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };

    Context* processEvent(const sf::Event& event) override;
    Context* processBackgroundTask() override;
    
    sf::Texture render() override;

};


#endif // GAME_CONTEXT_LESSCPP
