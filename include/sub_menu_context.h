#ifndef SUB_MENU_CONTEXT_LESSCPP
#define SUB_MENU_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"

#include "menu.h"
#include "game.h"

class SubMenuContext : public Context {
private:
	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(font, 60, window_width/3, window_height/3, sf::Color::Green);

	menu::Button returnButton = menu::Button("Return to Menu", window_width/2 - 200, window_height - 50, 30);
	menu::Button continueButton = menu::Button("Continue", window_width/2 + 60, window_height - 50, 30);
	menu::Button saveGameButton = menu::Button("Save Game", window_width/3 + 50, window_height/3 + 100, 30);

	sf::Texture m_pretext;
	const Game & m_game;

public:

	SubMenuContext(int, sf::Texture, const Game &);

	void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };
	Context* processEvent(const sf::Event &) override;
	sf::Texture render() override;


};


#endif // SUB_MENU_CONTEXT_LESSCPP

