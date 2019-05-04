#include "block_shape.hh"
#include <SFML/Graphics.hpp>
#include <random>

int main() {

	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	sf::RenderWindow window(sf::VideoMode(600, 600), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));

	std::vector<std::vector<Block>> field;
	for (int y = 0; y < 3; ++y) {
		std::vector<Block> tmp;
		for (int x = 0; x < 3; ++x) {
			Block tmp_block{200.f, 2.f, rand0to6(eng)};
			tmp_block.setPosition(x * 200 + 100, y * 200 + 100);
			tmp_block.setRotation(rand0to3(eng) * 90);
			tmp.emplace_back(tmp_block);
		}
		field.emplace_back(tmp);
	}

	std::vector<sf::CircleShape> whitePlayers(4, sf::CircleShape{30});
	std::vector<sf::CircleShape> blackPlayers(4, sf::CircleShape{30});

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			whitePlayers[j + 2 * i].setPosition(sf::Vector2f(100 * j + 20, 100 * i + 20));
			whitePlayers[j + 2 * i].setFillColor(sf::Color::Yellow);
			blackPlayers[j + 2 * i].setPosition(sf::Vector2f(100 * j + 420, 100 * i + 420));
			blackPlayers[j + 2 * i].setFillColor(sf::Color::Black);
		}
	}

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
		for (auto &row: field) {
			for (auto &block: row) {
				window.draw(block);
			}
		}
		for (auto &player: whitePlayers) {
			window.draw(player);
		}
		for (auto &player: blackPlayers) {
			window.draw(player);
		}
		window.display();
	}

	return 0;
}
