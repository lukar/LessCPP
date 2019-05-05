#include "block_shape.hh"
#include <SFML/Graphics.hpp>
#include <random>

float block_size = 200;
float block_border = 2;
float player_size = 30;

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
			Block tmp_block{block_size, block_border, rand0to6(eng)};
			tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
			tmp_block.setRotation(rand0to3(eng) * 90);
			tmp.emplace_back(tmp_block);
		}
		field.emplace_back(tmp);
	}

	std::vector<sf::CircleShape> whitePlayers(4, sf::CircleShape{player_size});
	std::vector<sf::CircleShape> blackPlayers(4, sf::CircleShape{player_size});

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			whitePlayers[j + 2 * i].setPosition(
					sf::Vector2f((block_size / 2) * j + block_size / 4,
								 (block_size / 2) * i + block_size / 4));
			whitePlayers[j + 2 * i].setFillColor(sf::Color::Yellow);
			whitePlayers[j + 2 * i].setOrigin(sf::Vector2f(player_size, player_size));
			blackPlayers[j + 2 * i].setPosition(
					sf::Vector2f((block_size / 2) * (j + 4) + block_size / 4,
								 (block_size / 2) * (i + 4) + block_size / 4));
			blackPlayers[j + 2 * i].setFillColor(sf::Color::Black);
			blackPlayers[j + 2 * i].setOrigin(sf::Vector2f(player_size, player_size));
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
