#include <string>

#include "board/board_fast.hpp"
#include "impls/minimax_alpha_beta.cpp"
#include "coord.hpp"

std::mt19937 gen2(static_cast<unsigned int>(std::time(0)));

std::string Bot2Move(BoardFast board) {
    auto ans = minimax(board, board.CurrentPlayer() == 1, 8, INT_MIN, INT_MAX, clock()).second;
    return Coord(ans).str;
}
