// Implementation of simple Monte-Carlo Tree Search

#include <string>

#include "board/board_fast.hpp"
#include "impls/monte_carlo_tree_simple.cpp"

std::mt19937 gen3(static_cast<unsigned int>(std::time(0)));

std::string Bot3Move(BoardFast board) {
  return GetBestMove(board, 10'000);
}
