#include <string>

#include "board/board_fast.hpp"
#include "coord.hpp"
#include "impls/random.cpp"

std::string Bot1Move(BoardFast board) {
    return Coord(get_random_move(board)).str;
}
