#ifndef SERVER_GAME_ROOM_H
#define SERVER_GAME_ROOM_H

#include <iso646.h>
#include <SFML/Network.hpp>

sf::Packet peer_msg(const std::string& message);

class GameRoom {
    std::string m_room_name;
    std::string m_game_json;
    bool m_p2_connected = false;
    std::unique_ptr<sf::TcpSocket> m_tcp_socket_p1; // White by default
    std::unique_ptr<sf::TcpSocket> m_tcp_socket_p2; // Black by default

public:
    std::string get_room_name() const { return m_room_name; };

    void handle_packets();

    GameRoom(std::unique_ptr<sf::TcpSocket> tcp_socket_p1, std::string game_name)
        : m_room_name(game_name), m_tcp_socket_p1(std::move(tcp_socket_p1)) {}

    void connect_p2(std::unique_ptr<sf::TcpSocket> tcp_socket_p2);

    bool empty() const {
        return !m_tcp_socket_p1 and !m_tcp_socket_p2;
    }

    bool hasReady(const sf::SocketSelector& selector) const {
        return  (m_tcp_socket_p1 ? selector.isReady(*m_tcp_socket_p1) : false) or
                (m_tcp_socket_p2 ? selector.isReady(*m_tcp_socket_p2) : false);
    }

};


#endif // SERVER_GAME_ROOM_H
