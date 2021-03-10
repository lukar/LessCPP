//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include "const_globals.h"
#include <iostream>
#include "osdialog.h"

#include <string>
using namespace std::string_literals;

sf::Vector2f getMousePosition(sf::RenderWindow  const& window) {
    return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}

std::optional<Location> getMouseLocation(sf::RenderWindow  const& window) {
    auto mouse = sf::Mouse::getPosition(window);
    if (mouse.x < 0 || mouse.x >= block_size * 3 ||
            mouse.y < 0 || mouse.y >= block_size * 3)
        return {};
    return Location{static_cast<int>(mouse.x / (block_size / 2)),
                    static_cast<int>(mouse.y / (block_size / 2))};
}

sf::Vector2f positionFromLocation(const Location& location) {
    return {
        location.x * (block_size / 2) + block_size / 4,
        location.y * (block_size / 2) + block_size / 4
    };
}

Location locationFromPosition(const sf::Vector2f& position) {
    return {
        static_cast<int>(position.x / (block_size / 2)),
        static_cast<int>(position.y / (block_size / 2))
    };
}

bool piecesInLocations(const Locations<4>& pieces, const Locations<4>& locations) {
//    return std::all_of(pieces.begin(), pieces.end(), [=](const Location & piece) {
//        return std::find(locations.begin(), locations.end(), piece) != locations.end();
//    });
//    return std::set<Location>(pieces.begin(), pieces.end()) == std::set<Location>(locations.begin(), locations.end());
    uint count = 0;
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            if (pieces[i] == locations[j]) ++count;
        }
    }
    return count == 4;
}

std::optional<Direction> getDirection(Location oldL, Location newL) {
    int dx = newL.x - oldL.x;
    int dy = newL.y - oldL.y;

    if (dx != 0 && dy != 0) return {};

    if (dx > 0) return Direction::RIGHT;
    else if (dx < 0) return Direction::LEFT;
    else if (dy > 0) return Direction::DOWN;
    else if (dy < 0) return Direction::UP;
    else return {};
}

std::unique_ptr<sf::TcpSocket> connect_to_server(sf::IpAddress url, unsigned short port) {
    std::unique_ptr<sf::TcpSocket> tcp_socket = std::make_unique<sf::TcpSocket>();
    sf::Socket::Status status = tcp_socket->connect(url, port);
    if (status != sf::Socket::Done) {
        std::cout << "Error connecting tcp - get_game_tcp_packets "s << status << "\n";
        assert(0);
    }
    std::cout << "Tcp socket connected - get_game_tcp_packets "s << status << "\n";

    return std::move(tcp_socket);
}

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

Link parse_move(sf::Packet packet) {

    Location location_old;
    Location location_new;

    packet >> location_old.x >> location_old.y >> location_new.x >> location_new.y;
    return {location_old, location_new};
}
