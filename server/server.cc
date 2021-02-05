#include <SFML/Network.hpp>
#include <iostream>
#include <string>

int main() {
	unsigned short tcp_port = 53012;
	sf::TcpListener listener;
	listener.setBlocking(false);	

	class GameRoom {
		std::string m_room_name;
		std::string m_game_json;
		bool m_p2_connected = false;
		sf::TcpSocket* m_tcp_socket_p1; // White by default
		sf::TcpSocket* m_tcp_socket_p2; // Black by default
	public:
		std::string get_room_name() { return  m_room_name; }
		void exchange_packets() {
			if (!m_p2_connected) return; /* no second player */

			sf::Packet packet;
			if (m_tcp_socket_p1->receive(packet)) {/* no data */ }
			else { m_tcp_socket_p2->send(packet); }
			packet.clear();
			if (m_tcp_socket_p2->receive(packet)) {/* no data */ }
			else { m_tcp_socket_p1->send(packet); }
			packet.clear();
		}
		GameRoom(sf::TcpSocket* tcp_socket_p1, std::string game_name, std::string game_json)
			: m_tcp_socket_p1(tcp_socket_p1), m_room_name(game_name), m_game_json(game_json) {}

		void connect_p2(sf::TcpSocket* tcp_socket_p2) {
			sf::Packet packet;
			m_tcp_socket_p2 = tcp_socket_p2;
			m_p2_connected = true;
			packet << m_game_json;
			m_tcp_socket_p2->send(packet);
		}
	};


	std::vector<GameRoom*> all_rooms;
	std::vector<sf::TcpSocket*> unclassified_sockets;
	sf::TcpSocket* new_socket = new sf::TcpSocket();;
	std::string p_request, p_name, p_game;
	sf::Packet packet;
	sf::Packet tmp_packet;
	if (listener.listen(tcp_port) != sf::Socket::Done) { /* fail bind port*/ }
	while (true) {
		// bind the listener to a port

		new_socket->setBlocking(false);
		if (listener.accept(*new_socket) != sf::Socket::Done)
		{/*Fail connect new socket*/
		}
		else {
			unclassified_sockets.push_back(new_socket);
			new_socket = new sf::TcpSocket();
			std::cout << "Socket added\n";
		}

		// Handle new connections
		for (unsigned int i = 0; i < unclassified_sockets.size(); ) {
			packet.clear();
			if (unclassified_sockets[i]->receive(packet)) { i++; }
			else {
				packet >> p_request;
				std::cout << p_request << '\n';
				if (p_request == "post_game") {
					packet >> p_name >> p_game;
					std::cout << p_name << p_game << '\n';
					all_rooms.push_back(new GameRoom(unclassified_sockets[i], p_name, p_game));
					unclassified_sockets.erase(unclassified_sockets.begin() + i);
				}
				else if (p_request == "join_game") {
					packet >> p_name;
					std::cout << p_name << '\n';
					for (auto room : all_rooms) {
						if (room->get_room_name() == p_name) {
							room->connect_p2(unclassified_sockets[i]);
							unclassified_sockets.erase(unclassified_sockets.begin() + i);
						}
					}
				}
				else if (p_request == "list_rooms") {
					std::string all_room_names;
					for (auto room : all_rooms) {
						all_room_names += (room->get_room_name() + "\n");
					}
					tmp_packet << all_room_names;
					unclassified_sockets[i]->send(tmp_packet);
				}
			}
		}
		// exchange data between socket pairs
		for (auto room : all_rooms) {
			room->exchange_packets();
		}
	}

}
