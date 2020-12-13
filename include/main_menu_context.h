#ifndef MAIN_MENU_CONTEXT_LESSCPP
#define MAIN_MENU_CONTEXT_LESSCPP

#include "context.h"

#include "const_globals.h"
#include "less.h"

#include "menu.h"

class MainMenuContext : public Context {
private:
	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(font, 60, window_width/3, window_height/3, sf::Color::Green);

	menu::Button quitButton = menu::Button("Quit", window_width/2 - 15, window_height - 50, 30);
	menu::Button startGameButton = menu::Button("Start Game", window_width/3 + 50, window_height/3 + 100, 30);
	menu::Button loadGameButton = menu::Button("Load Game", window_width/3 + 50, window_height/3 + 150, 30);

public:

	MainMenuContext();

	Context* update(const sf::Event &,  const sf::Vector2f & /* Mouse position */) override;
	sf::Texture render(const sf::Vector2f &) override;

};


#endif // MAIN_MENU_CONTEXT_LESSCPP

