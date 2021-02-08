#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include <server/game_room.h>


int main() {
	unsigned short tcp_port = 53012;
	sf::TcpListener listener;
    listener.setBlocking(false);

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
