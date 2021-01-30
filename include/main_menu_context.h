#ifndef MAIN_MENU_CONTEXT_LESSCPP
#define MAIN_MENU_CONTEXT_LESSCPP

#include "context.h"

#include "const_globals.h"
#include "less.h"

#include "menu_button.h"
#include <SFML/Network.hpp>
#include "helpers.h"

class MainMenuContext : public Context {
private:
    // side text
    sf::Text text = initializeText(font, large_font, window_width/3, window_height/3, sf::Color::Green);

    menu::Button SPGameButton = menu::Button("Single player", window_width/ 3 - 100, window_height/3 + 100);
    menu::Button LPVPGameButton = menu::Button("Local PvP", window_width / 3 - 100, window_height / 3 + 150);
    menu::Button MPHGameButton = menu::Button("Multiplayer Host", window_width / 3 + 200, window_height / 3 + 100);
    menu::Button MPCGameButton = menu::Button("Multiplayer Client", window_width / 3 + 200, window_height / 3 + 150);
    menu::Button loadGameButton = menu::Button("Load Game", window_width/3 + 50, window_height/3 + 250);
    menu::Button quitButton = menu::Button("Quit", window_width / 2 - 15, window_height - 50);

	sf::IpAddress ip_player2 = "127.0.0.1";
	unsigned short tcp_port = 53012;

	Context* host_game();
	Context* load_game();

public:

    MainMenuContext(Context* previous) : Context(previous) {};

	void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };
	Context* processEvent(const sf::Event &) override;
    Context* processBackgroundTask() override;
	sf::Texture render() override;
};


#endif // MAIN_MENU_CONTEXT_LESSCPP

