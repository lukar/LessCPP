#ifndef CONTEXT_LESSCPP
#define CONTEXT_LESSCPP

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const_globals.h"

class Context {
protected:
	// texture that will be passed out to render
	sf::RenderTexture rentex;

	bool quit = false;


public:
	virtual Context* update(const sf::Event &,  const sf::Vector2f & /* Mouse position */) = 0;
	virtual sf::Texture render(const sf::Vector2f &) = 0;

	bool isQuitting() const { return quit; }
};


#endif // CONTEXT_LESSCPP
