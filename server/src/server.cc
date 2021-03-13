#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include <server/game_room.h>
#include <iso646.h>


#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#endif // __linux__

bool room_name_exists(const std::vector<GameRoom>& all_rooms, const std::string& room_name) {
	for (auto& room : all_rooms) {
		if (room_name == room.get_room_name()) return true;
	}
	return false;
}

static bool RUN = true;
#ifdef __linux__
void ctrlCHandler(int)
{
    RUN = false;
}
#endif // __linux__

int main() {
#ifdef __linux__
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = ctrlCHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);
#endif // __linux__


	unsigned short tcp_port = 53012;
	sf::TcpListener listener;
	listener.listen(tcp_port);

    std::vector<GameRoom> all_rooms;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;

	sf::SocketSelector selector;
	selector.add(listener);

    while (RUN and selector.wait()) { // wait inifinitely long for _anything_ to change
		if (selector.isReady(listener)) { // new pending connection
			std::unique_ptr<sf::TcpSocket> new_socket = std::make_unique<sf::TcpSocket>();
			new_socket->setBlocking(false);
			if (listener.accept(*new_socket) == sf::Socket::Done) {
				printf("Client connected\n");
				// Add the new client to the selector so that we will
				// be notified when he sends something
				selector.add(*new_socket);

				// Add the new client to the clients list
				clients.push_back(std::move(new_socket));
			}
		} else { // check the unassigned clients for a request
			for (auto& client : clients) {
				if (selector.isReady(*client)) { // TODO: can it be more than one?
					// The client has sent some data, we can receive it
					sf::Packet packet;
					auto status = client->receive(packet);
					if (status == sf::Socket::Done) {
						std::string recipient, msg;
						packet >> recipient >> msg;
						if (recipient != "lobby") continue;
						if 		  (msg == "post_game") {
							std::string p_name;
							packet >> p_name;
							if (room_name_exists(all_rooms, p_name)) {
								std::cout << "Room: " << p_name << " already exists." << '\n';
								client->send(peer_msg("error"));
							}
							else {
								std::cout << "Room: " << p_name << " created." << '\n';
								client->send(peer_msg("sucess"));
								all_rooms.push_back(GameRoom(std::move(client), p_name));
								clients.erase(std::remove(std::begin(clients), std::end(clients), client), std::end(clients));

							}
						} else if (msg == "join_game") {
							std::string p_name;
							packet >> p_name;
							auto room_it = std::find_if(std::begin(all_rooms), std::end(all_rooms), [p_name](const GameRoom& room) {
								return room.get_room_name() == p_name;
							});
							if (room_it != all_rooms.end()) {
								room_it->connect_p2(std::move(client));
								std::cout << "A player joined room: " << p_name << '\n';
								clients.erase(std::remove(std::begin(clients), std::end(clients), client), std::end(clients));
							}
							else {
								std::cout << "Room: " << p_name << " does not exists." << '\n';
								client->send(peer_msg("error"));
							}
						} else if (msg == "list_rooms") {}
					} else if (status == sf::Socket::Disconnected) {
						selector.remove(*client);
						client->disconnect();
						clients.erase(std::remove(std::begin(clients), std::end(clients), client), std::end(clients));
						printf("Client disconnected\n");
					}
				}
			}
			for (auto& room : all_rooms) {
				if (room.hasReady(selector)) {
					room.handle_packets(); // TODO: should remove a client if it detects a disconnection
				}
			}
		}
	}
}
