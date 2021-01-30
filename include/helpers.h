//
// Created by aviany on 5/5/19.
//

#ifndef LESSCPP_HELPERS_H
#define LESSCPP_HELPERS_H
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include "location.h"

template<typename T, typename R>
constexpr float euclideanDistance(const sf::Vector2<T> &pointA, const sf::Vector2<R> &pointB) {
    return sqrt(
                    pow(pointA.x - pointB.x, 2) +
                    pow(pointA.y - pointB.y, 2)
    );
}

sf::Vector2f getMousePosition(sf::RenderWindow const &);

std::optional<Location> getMouseLocation(sf::RenderWindow const&);

sf::Vector2f positionFromLocation(const Location&);

Location locationFromPosition(const sf::Vector2f&);

bool piecesInLocations(const Locations<4>& pieces, const Locations<4>& locations);

std::optional<Direction> getDirection(Location oldL, Location newL);

std::string get_game_tcp_packets(sf::IpAddress& ip_player2,
    unsigned short& tcp_port);
sf::IpAddress host_game_tcp_packets(std::string data_string,
    sf::TcpListener& listener);

void send_move(sf::TcpSocket& tcp_socket,
    Location location_old,
    Location location_new);
std::optional<Link> wait_move(sf::TcpSocket& tcp_socket);

#endif //LESSCPP_HELPERS_H
