#include "contexts/server_room_context.h"
#include "contexts/game_context.h"
#include "contexts/dialog_context.h"
#include <fstream>
#include <array>
#include <iostream>

#include "helpers.h"

using namespace std::string_literals;

std::string request_room_names(sf::TcpSocket* tcp_socket) {
    sf::Packet packet;
    packet << "lobby"s << "list_rooms"s;
    tcp_socket->send(packet);
    packet.clear();

    tcp_socket->receive(packet);
    tcp_socket->disconnect();
    std::string room_names;
    packet >> room_names;
    return room_names;
}

ServerRoomContext::ServerRoomContext(Context* previous)
    : Context(previous)
{
    title.setString("Server Room Browser");

    RoomNameTextInput.setPosition(50, window_height / 3 - 10);
    IPTextInput.setPosition(450, window_height / 3 - 10);

    ConnectButton.setPosition(450, window_height / 3 + 50);

    joinRoomButton.setPosition(50, window_height / 3 + 50);
    listRoomsButton.setPosition(50, window_height / 3 + 100);
    postGameButton.setPosition(50, window_height / 3 + 150);
    returnButton.setPosition(50, window_height - 100);
}

void ServerRoomContext::processBackgroundTask()
{
    if (!m_connected) {
        ConnectButton.setTextFillColor(sf::Color::Green);
    } else {
        ConnectButton.setTextFillColor(sf::Color::Red);
    }
}

Context* ServerRoomContext::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (returnButton.contains(m_mousepos)) setReturnContext(m_previous);
		else if (postGameButton.contains(m_mousepos)) {
            if (m_tcp_socket) return new GameContext(this, wall::generateNwallconfigs<9>(), std::move(m_tcp_socket), RoomNameTextInput.getText());
		}
		else if (listRoomsButton.contains(m_mousepos)) {
//			std::cout << request_room_names(m_tcp_socket) << "\n";
		}
        else if (joinRoomButton.contains(m_mousepos)) {
            if (m_tcp_socket) return join_game_server(std::move(m_tcp_socket), RoomNameTextInput.getText());
		}
        else if (ConnectButton.contains(m_mousepos)) {
            focusedTextInput = nullptr;
            if (!m_connected) {
                std::cout << "Attempting connection" << std::endl;
                m_tcp_socket = std::make_unique<sf::TcpSocket>();
                auto status = m_tcp_socket->connect(IPTextInput.getText(), 53012);
                if ( status == sf::Socket::Done ) {
                    m_tcp_socket->setBlocking(false);
                    m_connected = true;
                } else {
                    std::cout << "Error during socket connection: " << status << std::endl;
                }
            } else {
                std::cout << "Attempting disconnection" << std::endl;
                m_tcp_socket.reset();
                m_connected = false;
            }
        }
        else if (RoomNameTextInput.contains(m_mousepos)) {
            focusedTextInput = &RoomNameTextInput;
        }
        else if (IPTextInput.contains(m_mousepos) and !m_connected) {
            focusedTextInput = &IPTextInput;
        }
        else {
            focusedTextInput = nullptr;
        }
	}

    if (focusedTextInput != nullptr) focusedTextInput->processInputEvent(event);
	return nullptr;
}

sf::Texture ServerRoomContext::render() {
	rentex.clear();

    rentex.draw(title);
    rentex.draw(RoomNameTextInput(focusedTextInput));
    rentex.draw(IPTextInput(focusedTextInput));
    rentex.draw(ConnectButton);
	rentex.draw(postGameButton);
	rentex.draw(listRoomsButton);
	rentex.draw(joinRoomButton);
    rentex.draw(returnButton);
	rentex.display();

	return rentex.getTexture();
}


Context* ServerRoomContext::join_game_server(std::unique_ptr<sf::TcpSocket> tcp_socket, std::string room_name) {
	sf::Packet packet;
    std::string recipient, msg;
    std::string json_string;

    packet << "lobby"s << "join_game"s << room_name;
    tcp_socket->send(packet);
    
    do {
        packet.clear();
        tcp_socket->setBlocking(true); /*implement timeout*/
        tcp_socket->receive(packet);
        tcp_socket->setBlocking(false);

        packet >> recipient >> msg >> json_string;
    } while (recipient != "peer");

    return new GameContext(this, nlohmann::json::parse(json_string), std::move(tcp_socket));
}

