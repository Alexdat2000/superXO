#include <string>

#include "board/board_fast.hpp"
#include "coord.hpp"
#include "impls/minimax_alpha_beta.cpp"

std::string Bot2Move(std::string moves) {
  BoardFast board(moves);
  auto ans =
      minimax(board, board.CurrentPlayer() == 1, 8, INT_MIN, INT_MAX, clock())
          .second;
  return Coord(ans).str;
}
