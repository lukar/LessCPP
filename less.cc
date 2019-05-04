#include "block_shape.hh"
#include <SFML/Graphics.hpp>
#include <random>

int main() {

	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> idis(0, 6);

	sf::RenderWindow window(sf::VideoMode(600, 600), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

//	std::vector<std::vector<Block>> field;
//
//	for (int y = 0; y < 3; ++y) {
//		std::vector<Block> tmp;
//		for (int x = 0; x < 3; ++x) {
//			tmp.emplace_back(Block{200.f, 2.f, idis(eng)}.setPosition(x * 200 + 100, y * 200 + 100));
//		}
//	}

	Block block1{200.f, 2.f, idis(eng)};
	block1.setPosition(100, 100);
	block1.setRotation(0);


//	Block block2{200.f, 2.f, 0};
//	block2.setPosition(300, 100);
//
//	Block block3{200.f, 2.f, 0};
//	block3.setPosition(100, 300);
//
//	Block block4{200.f, 2.f, 0};
//	block4.setPosition(300, 300);


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
//		window.draw(block2);
//		window.draw(block3);
//		window.draw(block4);
		window.display();
	}

	return 0;
}
