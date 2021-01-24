#ifndef JOIN_DIALOG_CONTEXT_LESSCPP
#define JOIN_DIALOG_CONTEXT_LESSCPP

#include "context.h"

#include "const_globals.h"
#include "less.h"

#include "menu.h"
#include "helpers.h"

class JoinDialogContext : public Context {
private:
	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(font, 60, window_width / 3, window_height / 3, sf::Color::Green);
	menu::TextInput IPTextInput = menu::TextInput("127.0.0.1      ", window_width / 3 - 100, window_height / 3 + 100, 30);
	menu::Button quitButton = menu::Button("Quit", window_width / 2 - 15, window_height - 50, 30);

	sf::IpAddress ip_player2 = "127.0.0.1";
	unsigned short tcp_port = 53012;

	Context* join_game();

public:

	JoinDialogContext() = default;

	void update(const float dt, const sf::Vector2f& mousepos) override { m_dt = dt; m_mousepos = mousepos; };
	Context* processEvent(const sf::Event&) override;
	Context* processBackgroundTask(void) override;
	sf::Texture render() override;
};


#endif // JOIN_DIALOG_CONTEXT_LESSCPP