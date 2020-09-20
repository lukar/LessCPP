#ifndef CONTEXT_LESSCPP
#define CONTEXT_LESSCPP

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const_globals.h"

// Need to predeclare it due to circular dependency
class Context;

typedef std::variant<Context*, sf::Texture, std::nullopt_t> Result;

class Context {
private:
	Context* subcontext = nullptr;
public:
	virtual Result process(sf::Event, sf::Vector2f /* Mouse position */) = 0;
};




#endif // CONTEXT_LESSCPP