#ifndef MENU_CONTEXT_LESSCPP
#define MENU_CONTEXT_LESSCPP

#include "const_globals.h"
#include "helpers.h"
#include "less.h"

#include "context.h"
#include "game_context.h"

#include <array>

class MenuContext : public Context {
private:
	// side text
	sf::Font font;
	sf::Text text;

public:

	MenuContext();

	Context* update(const sf::Event &,  const sf::Vector2f & /* Mouse position */) override;
	sf::Texture render(const sf::Vector2f &) override;

};


#endif // GAME_CONTEXT_LESSCPP
