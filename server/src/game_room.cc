#include <server/game_room.h>

#include <iso646.h>
#include <string>

void GameRoom::handle_packets() { // TODO: need to improve the logic when one is missing
    if (!m_p2_connected) return; /* no second player */

    sf::Packet packet;
    std::string recipient;
    std::string msg;
    if (m_tcp_socket_p1->receive(packet) == sf::Socket::Status::Done) {
        packet >> recipient >> msg;
        if (recipient == "peer") m_tcp_socket_p2->send(packet);
        else if (recipient == "server") {
            if (msg == "leave_room") m_tcp_socket_p1.reset();
        }
    }
    packet.clear();
    if (m_tcp_socket_p2->receive(packet) == sf::Socket::Status::Done) {
        packet >> recipient >> msg;
        if (recipient == "peer") m_tcp_socket_p1->send(packet);
        else if (recipient == "server") {
            if (msg == "leave_room") m_tcp_socket_p2.reset();
        } 
    }
    packet.clear();
}


void GameRoom::connect_p2(std::unique_ptr<sf::TcpSocket> tcp_socket_p2) {
    sf::Packet packet;
    m_tcp_socket_p2 = std::move(tcp_socket_p2);
    m_p2_connected = true;
    packet << "peer" << "game_json" << m_game_json;
    m_tcp_socket_p2->send(packet);
}
