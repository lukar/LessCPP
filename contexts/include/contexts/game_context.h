#ifndef GAME_CONTEXT_LESSCPP
#define GAME_CONTEXT_LESSCPP

#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include "less.h"
#include "ai.h"

#include "contexts/context.h"

#include <array>
#include "nlohmann/json.hpp"
#include <stack>

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
    sf::Text text = initializeText(window_height + 10, 10, Tiny, sf::Color::Green);

    Player opponent_color = Player::BLACK;
    sf::IpAddress m_ip_player2 = "0.0.0.0";
    unsigned short m_tcp_port = 53012;
//    sf::TcpListener listener;
    std::unique_ptr<sf::TcpSocket> m_tcp_socket;
    uint m_synced_moves = 0;

    // AI
    uint m_AI_difficulty = 0;

    const GameMode m_game_mode;
    
public:

    // LOCAL PVP
    GameContext(Context*, std::array<WallConfig, 9>);
    GameContext(Context*, const nlohmann::json&);

    // SINGLEPLAYER
    GameContext(Context*, std::array<WallConfig, 9>, uint AI_difficulty);
    GameContext(Context*, const nlohmann::json&, uint AI_difficulty);

    // MULTIPLAYER
    GameContext(Context*, std::array<WallConfig, 9>, std::unique_ptr<sf::TcpSocket>, std::string room_name);
    GameContext(Context*, const nlohmann::json&, std::unique_ptr<sf::TcpSocket>);

    Context* processEvent(const sf::Event& event) override;
    void processBackgroundTask() override;
    
    sf::Texture render() override;

};


#endif // GAME_CONTEXT_LESSCPP
