#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#include <server/game_room.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


static bool RUN = true;
void ctrlCHandler(int)
{
    RUN = false;
}


int main() {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = ctrlCHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);


	unsigned short tcp_port = 53012;
	sf::TcpListener listener;

    std::vector<GameRoom> all_rooms;
    std::vector<std::unique_ptr<sf::TcpSocket>> pending_sockets;
	std::string p_request, p_name, p_game;
	sf::Packet packet;

	sf::SocketSelector selector;
	selector.add(listener);


	if (listener.listen(tcp_port) != sf::Socket::Done) { /* fail bind port*/ }
    while (RUN) {
		// bind the listener to a port

		if (selector.wait()) { // wait inifinitely long for _anything_ to change
			if (selector.isReady(listener)) {
    			std::unique_ptr<sf::TcpSocket> new_socket = std::make_unique<sf::TcpSocket>();
				if (listener.accept(*new_socket) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					pending_sockets.push_back(new_socket);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*new_socket);
				}
			}
		}

		// if (listener.accept(*new_socket) != sf::Socket::Done)
		// {/*Fail connect new socket*/
		// }
		// else {
        //     std::cout << "STATUS: " << new_socket->receive(packet) << std::endl;
        //     pending_sockets.emplace_back(std::move(new_socket));
        //     new_socket = std::make_unique<sf::TcpSocket>();

		// 	std::cout << "Socket added\n";
		// }

		// // Handle new connections
        // for (unsigned int i = 0; i < pending_sockets.size(); ) {
		// 	packet.clear();
        //     if (pending_sockets[i]->receive(packet)) { i++; }
		// 	else {
		// 		packet >> p_request;
		// 		std::cout << p_request << '\n';
		// 		if (p_request == "post_game") {
		// 			packet >> p_name >> p_game;
		// 			std::cout << p_name << p_game << '\n';
        //             all_rooms.emplace_back(GameRoom(std::move(pending_sockets[i]), p_name, p_game));
        //             pending_sockets.erase(pending_sockets.begin() + i);
		// 		}
		// 		else if (p_request == "join_game") {
		// 			packet >> p_name;
		// 			std::cout << p_name << '\n';
        //             for (auto& room : all_rooms) {
        //                 if (room.get_room_name() == p_name) {
        //                     room.connect_p2(std::move(pending_sockets[i]));
        //                     pending_sockets.erase(pending_sockets.begin() + i);
		// 				}
		// 			}
		// 		}
		// 		else if (p_request == "list_rooms") {
		// 			std::string all_room_names;
        //             for (auto& room : all_rooms) {
        //                 all_room_names += (room.get_room_name() + "\n");
		// 			}
					
		// 			sf::Packet tmp_packet;
		// 			tmp_packet << all_room_names;
        //             pending_sockets[i]->send(tmp_packet);
		// 		}
		// 	}
		// }
		// // exchange data between socket pairs
        // for (auto& room : all_rooms) {
        //     room.exchange_packets();
		// }
    }

    listener.close();

}
