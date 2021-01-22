//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include "const_globals.h"
#include <iostream>
#include "osdialog.h"

sf::Vector2f getMousePosition(sf::RenderWindow  const& window) {
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}

std::optional<Location> getMouseLocation(sf::RenderWindow  const& window) {
	auto mouse = sf::Mouse::getPosition(window);
	if (mouse.x < 0 || mouse.x >= block_size * 3 ||
			mouse.y < 0 || mouse.y >= block_size * 3)
		return {};
	return Location{static_cast<int>(mouse.x / (block_size / 2)),
					static_cast<int>(mouse.y / (block_size / 2))};
}

sf::Vector2f positionFromLocation(const Location& location) {
    return {
        location.x * (block_size / 2) + block_size / 4,
        location.y * (block_size / 2) + block_size / 4
    };
}

Location locationFromPosition(const sf::Vector2f& position) {
    return {
		static_cast<int>(position.x / (block_size / 2)),
		static_cast<int>(position.y / (block_size / 2))
    };
}

bool piecesInLocations(const Locations<4>& pieces, const Locations<4>& locations) {
//    return std::all_of(pieces.begin(), pieces.end(), [=](const Location & piece) {
//        return std::find(locations.begin(), locations.end(), piece) != locations.end();
//    });
//    return std::set<Location>(pieces.begin(), pieces.end()) == std::set<Location>(locations.begin(), locations.end());
    uint count = 0;
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (pieces[i] == locations[j]) ++count;
        }
    }
    return count == 4;
}

std::optional<Direction> getDirection(Location oldL, Location newL) {
	int dx = newL.x - oldL.x;
	int dy = newL.y - oldL.y;

	if (dx != 0 && dy != 0) return {};

	if (dx > 0) return Direction::RIGHT;
	else if (dx < 0) return Direction::LEFT;
	else if (dy > 0) return Direction::DOWN;
	else if (dy < 0) return Direction::UP;
	else return {};
}

#ifdef _WIN32
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h  - from WinSock2.h*/
#include <windows.h>

void sleep(unsigned milliseconds)
{
	Sleep(milliseconds);
}
#else
#include <unistd.h>

void sleep(unsigned milliseconds)
{
	usleep(milliseconds * 1000); // takes microseconds
}
#endif

sf::IpAddress host_game_tcp_packets(std::string data_string,
	sf::TcpListener & listener) {
	sf::TcpSocket tcp_socket;

	if (listener.accept(tcp_socket) != sf::Socket::Done) {
		std::cout << "Error accepr socket\n"; return 0;
	}
	std::cout << "Tcp socket connected\n";

	sf::Packet packet;
	tcp_socket.receive(packet);
	std::string s;
	std::string substring="init";
	packet >> s;
	packet.clear();
	if (s.find(substring) != std::string::npos) {
		std::cout << substring << " found!" << '\n';
		packet << data_string;
		tcp_socket.send(packet);
		std::cout << "Sent!\n";
	}
	return tcp_socket.getRemoteAddress();
}

std::string get_game_tcp_packets(sf::IpAddress& ip_player2,	unsigned short& tcp_port) {
	sf::TcpSocket tcp_socket;
	//if (ip_player2 == 0) ip_player2 = "127.0.0.1";
	if (true) {
		std::string port_str;
		std::cout << "Please, enter port of player2: (default " << tcp_port << ")\n";
		std::getline(std::cin, port_str);
		if(port_str.length()>0)
			tcp_port = std::stoi(port_str);
	}
	sf::Socket::Status status = tcp_socket.connect(ip_player2, tcp_port);

	if (status != sf::Socket::Done) {
		std::cout << "Error connecting tcp - get_game_tcp_packets " << status << "\n"; assert(0);
	}
	std::cout << "Tcp socket connected - get_game_tcp_packets " << status << "\n";

	sf::Packet packet;
	std::string message = "init";
	packet << message;
	tcp_socket.send(packet); packet.clear();

	tcp_socket.receive(packet);
	std::string json_string;
	packet >> json_string;
	return json_string;
}