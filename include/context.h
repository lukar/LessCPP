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
	int quitLevel = 1;
	float m_dt;
	sf::Vector2f m_mousepos;

public:

	virtual ~Context() = default;
	Context() {
		rentex.create(window_width, window_height);
	}

	virtual void update(const float /* delta time */, const sf::Vector2f & /* Mouse position */) = 0;
	virtual Context* processEvent(const sf::Event &, bool bg) = 0;
	virtual sf::Texture render() = 0;

	bool isQuitting() const { return quit; }
	int getQuitLevel() const { return quitLevel; }
};


#endif // CONTEXT_LESSCPP
