#include "const_globals.h"


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <iostream>
#include <stack>

#include "context.h"
#include "main_menu_context.h"

#include "osdialog.h"


int main()
{
    std::stack<Context*> contexts;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(30);

    contexts.push(new MainMenuContext(nullptr));

    sf::Event event;
    sf::Clock dtClock;
    float dt;

    while (window.isOpen()) {
        dt = dtClock.getElapsedTime().asSeconds();

        if (contexts.top()->isQuitting()) {
            Context* return_context = contexts.top()->getReturnContext();

            if (return_context == nullptr) break;

            while (contexts.top() != return_context) {
                delete contexts.top();
                contexts.pop();
            }
        }

        contexts.top()->update(dt, getMousePosition(window));
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            Context* newcontext = contexts.top()->processEvent(event);
            if (newcontext != nullptr) contexts.push(newcontext);
        }

        contexts.top()->processBackgroundTask(); 

        window.clear();

        window.draw(sf::Sprite(contexts.top()->render()));

        window.display();
    }

    return 0;
}



