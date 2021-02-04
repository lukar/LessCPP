#ifndef MAIN_MENU_CONTEXT_LESSCPP
#define MAIN_MENU_CONTEXT_LESSCPP

#include "contexts/context.h"

#include "const_globals.h"
#include "less.h"

#include "SFMLWidgets/button.h"
#include <SFML/Network.hpp>
#include "helpers.h"

class MainMenuContext : public Context {
private:
    // side text
    sf::Text text = initializeText(window_width/3, window_height/3, Large, sf::Color::Green);

    widget::Button SPGameButton = widget::Button("Single player", Medium);
    widget::Button LPVPGameButton = widget::Button("Local PvP", Medium);
    widget::Button MPHGameButton = widget::Button("Multiplayer Host", Medium);
    widget::Button MPCGameButton = widget::Button("Multiplayer Client", Medium);
    widget::Button loadGameButton = widget::Button("Load Game", Medium);
    widget::Button quitButton = widget::Button("Quit", Medium);

    sf::IpAddress ip_player2 = "127.0.0.1";
    unsigned short tcp_port = 53012;

    Context* host_game();
    Context* load_game();

public:

    MainMenuContext(Context* previous);

    void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };
    Context* processEvent(const sf::Event &) override;
    Context* processBackgroundTask() override;
    sf::Texture render() override;
};


#endif // MAIN_MENU_CONTEXT_LESSCPP

