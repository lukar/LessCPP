#include <server/game_room.h>

#include <iso646.h>
#include <string>

void GameRoom::handle_packets() { // TODO: need to improve the logic when one is missing
    sf::Packet packet;
    std::string recipient;
    std::string msg;
    if (m_tcp_socket_p1->receive(packet) == sf::Socket::Status::Done) {
        packet >> recipient >> msg;
        if (recipient == "peer" and m_p2_connected)       m_tcp_socket_p2->send(packet);
        else if (recipient == "peer" and !m_p2_connected) m_tcp_socket_p1->send(peer_msg("error"));
        else if (recipient == "server") {
            if (msg == "leave_room") m_tcp_socket_p1.reset();
            if (msg == "game_json") packet >> m_game_json;
        }
    }
    packet.clear();

    if (!m_p2_connected) return; /* no second player */
    if (m_tcp_socket_p2->receive(packet) == sf::Socket::Status::Done) {
        packet >> recipient >> msg;
        if (recipient == "peer") m_tcp_socket_p1->send(packet);
        else if (recipient == "server") {
            if (msg == "leave_room") m_tcp_socket_p2.reset();
            if (msg == "game_json") packet >> m_game_json;
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

sf::Packet peer_msg(const std::string& message) {
    sf::Packet packet;
    packet << "peer" << message;
    return packet;
}