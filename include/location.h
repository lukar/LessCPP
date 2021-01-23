#ifndef LESSCPP_LOCATION_H
#define LESSCPP_LOCATION_H

#include "nlohmann/json.hpp"

#include "definition_globals.h"
#include <ciso646>


struct Location {
	int x, y;
	bool operator==(const Location &rhs) const;
	bool operator!=(const Location &rhs) const;
	bool operator<(const Location &rhs) const;
	std::optional<Location> operator+(const Direction direction) const;

	Location operator+(const Location& location) const;

	Location operator-(const Location& location) const;
};

template <size_t N> using Locations = std::array<Location, N>;

using Link = std::pair<Location, Location>;
using History = std::vector<Link>;

void to_json(nlohmann::json & j, const Location & location);
void from_json(const nlohmann::json & j, Location & location);


#endif // LESSCPP_LOCATION_H