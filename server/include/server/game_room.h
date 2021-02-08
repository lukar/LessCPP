#ifndef SERVER_GAME_ROOM_H
#define SERVER_GAME_ROOM_H

#include <SFML/Network.hpp>

class GameRoom {
    std::string m_room_name;
    std::string m_game_json;
    bool m_p2_connected = false;
    sf::TcpSocket* m_tcp_socket_p1; // White by default
    sf::TcpSocket* m_tcp_socket_p2; // Black by default

public:
    std::string get_room_name() { return m_room_name; };

    void exchange_packets();

    GameRoom(sf::TcpSocket* tcp_socket_p1, std::string game_name, std::string game_json)
        : m_tcp_socket_p1(tcp_socket_p1), m_room_name(game_name), m_game_json(game_json) {}

    void connect_p2(sf::TcpSocket* tcp_socket_p2);

};


#endif // SERVER_GAME_ROOM_H
