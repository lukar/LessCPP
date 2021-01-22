//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include "const_globals.h"
#include <iostream>
#include "osdialog.h"

sf::IpAddress ip_player2 = "127.0.0.1";
unsigned short port_player2 = 0;
unsigned short port_recieve = 5554;

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

void udpSendStr(	std::string data_string,
					sf::IpAddress recipient,
					unsigned short port) {
	sf::UdpSocket socket;
	if (socket.send(data_string.c_str(), data_string.length(), recipient, port) != sf::Socket::Done)
	{
		// error...
	}
	socket.unbind();
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

sf::IpAddress host_game(std::string data_string,
	sf::UdpSocket &socket_recieve,
	unsigned short &port_recieve,
	sf::IpAddress &ip_player2,
	unsigned short &port_player2) {
	sf::IpAddress sender = "0.0.0.0";
	static unsigned short port_sent_from = 0;
	// NETWORKING
	// temporary; to be moved to main menu - Join game
	// bind the socket to a port
	if (socket_recieve.bind(port_recieve) != sf::Socket::Done)
	{
		// error...
		// if port not ok, find new port
		if (port_recieve == socket_recieve.getLocalPort())
			std::cout << "Port " << port_recieve << " already bound\n";
		std::cout << "Unable to bind port" << port_recieve << "\n";
	}
	port_recieve = socket_recieve.getLocalPort();
	std::cout << "Socket recieve - game context context bound to :" << port_recieve << "\n";
	char data[100];
	socket_recieve.setBlocking(true);

	std::size_t received = 0;
	if (socket_recieve.receive(data, 100, received, sender, port_sent_from) != sf::Socket::Done) {
		// error...
		std::cout << "Error recieve\n";
	}
	ip_player2 = sender;
	if (received > 0) {
		std::cout << "Received " << received << " bytes from " << sender << " on port " << port_sent_from << std::endl;
		if (strncmp("init", data, 4) == 0) {
			std::cout << "Init recieved\n";
			std::cout << "message: " << data << "recieved\n";
			int a = 0;
			std::sscanf(data,"init %d %*s", &a);
			port_player2 = a;			
			std::cout << "sending to:" << ip_player2 << " " << port_player2 << "\n";
			sleep(2000); // miliseconds
			udpSendStr(data_string, ip_player2, port_player2);
			std::cout << "Sent!\n";
		}
	}
	return ip_player2;
}

void get_game(char* data, int length,
	sf::UdpSocket &socket_recieve,
	unsigned short &port_recieve,
	sf::IpAddress &ip_player2,
	unsigned short &port_player2) {
	sf::IpAddress sender = "0.0.0.0";

	static unsigned short port_sent_from = 0;

	if (socket_recieve.bind(port_recieve) != sf::Socket::Done)
	{
		// error...
		// if port not ok, find new port
		if (port_recieve == socket_recieve.getLocalPort())
			std::cout << "Port " << port_recieve << " already bound\n";
		else
			std::cout << "Unable to bind port" << port_recieve << "\n";
	}
	if (socket_recieve.bind(0) != sf::Socket::Done)
	{
		// error...
		// if port not ok, find new port
		if (port_recieve == socket_recieve.getLocalPort())
			std::cout << "Port " << port_recieve << " already bound\n";
		else
			std::cout << "Unable to bind port" << port_recieve << "\n";
	}
	port_recieve = socket_recieve.getLocalPort();
	std::cout << "Socket recieve - game context context bound to :" << port_recieve << "\n";

	if (port_player2 == 0) {
		std::string port_str;
		std::cout << "Please, enter port of player2:\n" ;
		std::getline(std::cin, port_str);
		port_player2 = std::stoi(port_str);
	}
	//if (ip_player2 == 0) ip_player2 = "127.0.0.1";

	std::string message = "init " + std::to_string(port_recieve) + " ";
	udpSendStr(message, ip_player2,port_player2);
	// temporary; to be moved to main menu - Join game
	// bind the socket to a port

	std::cout << "Socket recieve - main menu context bound to :" << socket_recieve.getLocalPort() << "\n";

	socket_recieve.setBlocking(true);

	std::size_t received = 0;
	if (socket_recieve.receive(data, length, received, sender, port_sent_from) != sf::Socket::Done) {
		// error...
		std::cout << "Error recieve\n";
	}
	if (received > 0) {
		std::cout << "Received " << received << " bytes from " << sender << " on port " << port_sent_from << std::endl;
		if (4 >= strlen(data) && (strncmp("init", data, 4) == 0)) {
			std::cout << "Game recieved\n";
		}
	}
}

void udpSendStr_player2(std::string data_string) {
	udpSendStr(data_string, ip_player2, port_player2);
}
void get_game(char* data, int length) {
	sf::UdpSocket socket;
	get_game(data, length, socket, port_recieve, ip_player2, port_player2);
	socket.unbind();
}

sf::IpAddress host_game(std::string data_string) {
	sf::UdpSocket socket;
	auto retv = host_game(data_string,		socket,		port_recieve,		ip_player2,		port_player2);
	socket.unbind();
	return retv;
}

sf::IpAddress host_game_tcp_packets(std::string data_string,
	unsigned short& port_tcp_listener) {
	sf::TcpSocket tcp_socket;
	//tcp_socket.setBlocking(true);
	sf::TcpListener listener;
	sf::IpAddress sender = "0.0.0.0";
	// bind the listener to a port
	if (listener.listen(port_tcp_listener) != sf::Socket::Done) {
		std::cout << "Error accepr socket\n"; return 0;
	}
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
	if (tcp_port == 0) {
		std::string port_str;
		std::cout << "Please, enter port of player2:\n53012";
		std::getline(std::cin, port_str);
		//if(port_str.length()>0) 	
		tcp_port = std::stoi(port_str);
		//else tcp_port= 53012
	}
	sf::Socket::Status status = tcp_socket.connect(ip_player2, tcp_port);

	if (status != sf::Socket::Done) {
		std::cout << "Error connecting tcp - constructor " << status << "\n";
	}
	else {
		std::cout << "Tcp socket connected " << status << "\n";
	}

	sf::Packet packet;
	std::string message = "init";
	packet << message;
	tcp_socket.send(packet); packet.clear();

	tcp_socket.receive(packet);
	std::string json_string;
	packet >> json_string;
	return json_string;
}