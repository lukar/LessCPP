//
// Created by aviany on 5/9/19.
//

#include "helpers.h"
#include "const_globals.h"
#include <iostream>
#include "osdialog.h"

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

sf::TcpSocket* connect_to_server(sf::IpAddress url, unsigned short port) {
    sf::TcpSocket* tcp_socket = new sf::TcpSocket;
    sf::Socket::Status status = tcp_socket->connect(url, port);
    if (status != sf::Socket::Done) {
        std::cout << "Error connecting tcp - get_game_tcp_packets " << status << "\n";
        assert(0);
    }
    std::cout << "Tcp socket connected - get_game_tcp_packets " << status << "\n";
    return tcp_socket;
}

std::string request_room_names(sf::TcpSocket* tcp_socket) {
    sf::Packet packet;
    packet << std::string("list_rooms");
    tcp_socket->send(packet);
    packet.clear();

    tcp_socket->receive(packet);
    tcp_socket->disconnect();
    std::string room_names;
    packet >> room_names;
    return room_names;
}

void send_move(sf::TcpSocket& tcp_socket,
    Location location_old,
    Location location_new) {
    sf::Packet packet;
    packet << location_old.x << location_old.y << location_new.x << location_new.y;
    tcp_socket.send(packet);
}

std::optional<Link> wait_move(sf::TcpSocket& tcp_socket) {

    Location location_old;
    Location location_new;

    sf::Packet packet;
    if (tcp_socket.receive(packet)) {
        return {};
    }
    packet >> location_old.x >> location_old.y >> location_new.x >> location_new.y;
    return { {location_old, location_new} };
}

void board_json2png(const nlohmann::json& game_json)
{
    sf::RenderStates states;
    sf::RenderTexture rtexture;
    rtexture.create(window_width, window_height);
    rtexture.clear();
    // generate field blocks
    for (size_t y = 0; y < 3; ++y) {
        for (size_t x = 0; x < 3; ++x) {
            Block a_block(game_json["wall_matrix"][y][x]);
            a_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
            rtexture.draw(a_block, states);
        }
    }
    rtexture.display(); // flips the image
    rtexture.getTexture().copyToImage().saveToFile("board.png");
}
