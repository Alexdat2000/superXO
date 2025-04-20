#include <random>
#include <ctime>
#include "../board/board_fast.hpp"

std::mt19937 gen_random(static_cast<unsigned int>(std::time(0)));

size_t get_random_move(BoardFast board) {
    auto moves = board.calculateAvailableMoves();
    return moves[gen_random() % moves.size()];
}
