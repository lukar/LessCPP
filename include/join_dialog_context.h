#ifndef JOIN_DIALOG_CONTEXT_LESSCPP
#define JOIN_DIALOG_CONTEXT_LESSCPP

#include "context.h"

#include "const_globals.h"
#include "less.h"

#include "menu_button.h"
#include "menu_text_input.h"
#include "helpers.h"

#include <regex>

class JoinDialogContext : public Context {
private:
    // side text
    sf::Font font = getFont("resources/Roboto_Medium.ttf");
    sf::Text text = initializeText(font, 60, window_width / 3, window_height / 3, sf::Color::Green);

    menu::TextInput IPTextInput = menu::TextInput("IP", 15, Medium, "127.0.0.1",
        [](std::string t) { return std::regex_match(t, std::regex("(\\d\\d?\\d?\\.){3}\\d\\d?\\d?")); } );
    menu::Button quitButton = menu::Button("Quit", Medium);

    sf::IpAddress ip_player2 = "127.0.0.1";
    unsigned short tcp_port = 53012;

    Context* join_game();
    Context* p_return_context;

public:

    JoinDialogContext(Context*);

    void update(const float dt, const sf::Vector2f& mousepos) override { m_dt = dt; m_mousepos = mousepos; };
    Context* processEvent(const sf::Event&) override;
    Context* processBackgroundTask(void) override;
    sf::Texture render() override;
};


#endif // JOIN_DIALOG_CONTEXT_LESSCPP
