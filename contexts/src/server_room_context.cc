#include "contexts/server_room_context.h"
#include "contexts/game_context.h"
#include "contexts/dialog_context.h"
#include <fstream>
#include <array>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "helpers.h"

ServerRoomContext::ServerRoomContext(Context* pretext, sf::TcpSocket* tcp_socket)
	: Context(pretext), m_tcp_socket(tcp_socket)
{
	text.setString("Server Room Browser");
	joinRoomButton.setPosition(100, window_height / 3 + 50);
	listRoomsButton.setPosition(100, window_height / 3 + 100);
	postGameButton.setPosition(100, window_height / 3 + 150);
	returnButton.setPosition(100, window_height - 100);
}

Context* ServerRoomContext::processBackgroundTask() { return nullptr; }
Context* ServerRoomContext::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (returnButton.contains(m_mousepos)) setReturnContext(nullptr);
		else if (postGameButton.contains(m_mousepos)) {
			return new DialogContext(this, [this](std::string str) {return new GameContext(this, wall::generateNwallconfigs<9>(), m_tcp_socket, str); }, "game1");
		}
		else if (listRoomsButton.contains(m_mousepos)) {
			std::cout << request_room_names(m_tcp_socket) << "\n";
		}
		else if (joinRoomButton.contains(m_mousepos)) {
			return new DialogContext(this, [this](std::string str) {return join_game_server(m_tcp_socket, str); }, "game1");
		}
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture ServerRoomContext::render() {
	rentex.clear();

	rentex.draw(text);
	rentex.draw(returnButton);
	rentex.draw(postGameButton);
	rentex.draw(listRoomsButton);
	rentex.draw(joinRoomButton);
	rentex.display();

	return rentex.getTexture();
}


Context* ServerRoomContext::join_game_server(sf::TcpSocket* tcp_socket, std::string room_name) {
	sf::Packet packet;
	packet << std::string("join_game") << room_name;
	tcp_socket->send(packet);
	packet.clear();
	tcp_socket->setBlocking(true); /*implement timeout*/
	tcp_socket->receive(packet);
	tcp_socket->setBlocking(false);
	std::string json_string;
	packet >> json_string;
	return new GameContext(this, nlohmann::json::parse(json_string), tcp_socket);
}

