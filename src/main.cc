#include "const_globals.h"


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <iostream>
#include <stack>

#include "context.h"
#include "main_menu_context.h"

#include "osdialog.h"

#include <SFML/Network.hpp>



int main(int argc, char** argv)
{
	std::stack<Context*> contexts;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	contexts.push(new MainMenuContext());

	sf::Event event;
	sf::Clock dtClock;
	float dt;

	char data[100];
	std::size_t received = 0;
	// UDP socket:
	sf::IpAddress sender = "0.0.0.0";
	unsigned short port = 5554;
	sf::UdpSocket socket_recieve;

	// bind the socket to a port
	if (socket_recieve.bind(5554) != sf::Socket::Done)
	{
		// error...
	}
	socket_recieve.setBlocking(false);

	while (window.isOpen()) {
		dt = dtClock.getElapsedTime().asSeconds();

		if (contexts.top()->isQuitting()) {
			for (int i = contexts.top()->getQuitLevel(); i > 0; --i) {
				delete contexts.top();
				contexts.pop();
			}
			if (contexts.size() == 0) break;
		}
		if (socket_recieve.receive(data, 100, received, sender, port) != sf::Socket::Done)	{
			// error...
		}
		if (received > 0) {
			std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
		}
		contexts.top()->update(dt, getMousePosition(window));
		while (window.pollEvent(event)) {
			Context* newcontext = contexts.top()->processEvent(event);
			if (newcontext != nullptr) contexts.push(newcontext);
		}

		window.clear();

		window.draw(sf::Sprite(contexts.top()->render()));

		window.display();
	}

	return 0;
}



