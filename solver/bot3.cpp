// Implementation of simple Monte-Carlo Tree Search

#include <string>

#include "board/board_fastest.hpp"
#include "impls/mcts_improved.cpp"

std::mt19937 gen3(static_cast<unsigned int>(std::time(0)));

std::string Bot3Move(std::string moves) {
  BoardFastest board(moves);
  Coord move = better_run_mcts(board);
  return move.str;
}
