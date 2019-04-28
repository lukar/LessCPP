#include "block_shape.hh"
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(400, 400), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

	Block block1{200.f, 2.f};

	Block block2{200.f, 2.f};
	block2.setPosition(200, 0);

	Block block3{200.f, 2.f};
	block3.setPosition(0, 200);

	Block block4{200.f, 2.f};
	block4.setPosition(200, 200);

	while (window.isOpen()) {
		sf::Event event{};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q) {
					window.close();
				}
			}
		}

		window.clear();
		window.draw(block1);
		window.draw(block2);
		window.draw(block3);
		window.draw(block4);
		window.display();
	}

	return 0;
}
