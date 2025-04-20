#include <string>

#include "board_fast.hpp"
#include "coord.hpp"

std::mt19937 gen1(static_cast<unsigned int>(std::time(0)));

std::string Bot1Move(BoardFast board) {
    auto moves = board.calculateAvailableMoves();
    return Coord(moves[gen1() % moves.size()]).str;
}
