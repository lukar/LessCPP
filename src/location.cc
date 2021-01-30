#include <optional>
#include "location.h"
#include <ciso646>


bool Location::operator==(const Location &rhs) const {
    return x == rhs.x and y == rhs.y;
}

bool Location::operator!=(const Location &rhs) const 
{
    return x != rhs.x or y != rhs.y;
}

bool Location::operator<(const Location &rhs) const 
{
    return x*10+y < rhs.x*10 +rhs.y;
}

std::optional<Location> Location::operator+(const Direction direction) const 
{
    Location tmp = *this;
    if(direction == Direction::UP) {
            if (tmp.y == 0) return {};
            tmp.y--;
    }
    else if(direction == Direction::DOWN){
        if (tmp.y == 5) return {};
        tmp.y++;
    }
    else if(direction == Direction::LEFT){
        if (tmp.x == 0) return {};
        tmp.x--;
    }
    else {  // (direction == Direction::RIGHT)
        if (tmp.x == 5) return {};
        tmp.x++;
    }
    return tmp;
}

Location Location::operator+(const Location& location) const 
{
    return {this->x + location.x, this->y + location.y};
}

Location Location::operator-(const Location& location) const 
{
    return {this->x - location.x, this->y - location.y};
}

void to_json(nlohmann::json & j, const Location & location)
{
    j = nlohmann::json::array({location.x, location.y});
}

void from_json(const nlohmann::json & j, Location & location)
{
    location.x = j.at(0);
    location.y = j.at(1);
}
