#include "state.h"
#include <algorithm>
#include <iostream>

bool life::Grid::operator==(const life::Grid& other) const {
    if (rows() != other.rows() || columns() != other.columns()) {
        return false;
    }

    return std::equal(begin(), end(), other.begin());
}

bool allZero(const life::Nibbles& nibbles) {
    return std::all_of(
            nibbles.begin(),
            nibbles.end(),
            [](auto word){ return word == 0; });
}

std::ostream& life::operator<<(std::ostream& os, const life::Grid& grid) {

    os << grid.rows() << " x " << grid.columns() << " grid:\n";

    auto start = grid.begin();
    // Only print up to last non-zero row
    auto end = std::find_if_not(
            grid.rbegin(),
            grid.rend(),
            allZero).base();

    std::for_each(start, end, [&](auto& row) {
        os << row << '\n';
    });
    return os;
}

std::istream& life::operator>>(std::istream& is, life::Grid& grid) {

    grid.Reset();

    std::string line;
    for (int i = 0; i < grid.rows() && std::getline(is, line); ++i) {
        if (line == "...") {
            break;
        }
        for (int j = 0; j < std::min(grid.columns(), line.size()); ++j) {
            char c = line[j];
            if (c == '#') {
                grid[i][j] = 1;
            }
        }
    }

    return is;
}
