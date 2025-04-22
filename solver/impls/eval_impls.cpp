#include "../board/board_fast.hpp"
#include "../coord.hpp"
#include "algo_ofek.h"
#include "minimax_better_formula.cpp"

int main() {
  // std::array<int, 4> winners{};
  // for (int i = 0; i < 100; i++) {
  //   BoardFast board("");
  //   while (board.Winner() == 0) {
  //     if ((board.CurrentPlayer() == 1) ^ (i >= 50)) {
  //       Coord move = Coord(minimax2(board, board.CurrentPlayer() == 1, 8,
  //                                   INT_MIN, INT_MAX, clock())
  //                              .second);
  //       board.Place(move.row, move.col);
  //     } else {
  //       Coord move = run_mcts(board, 100);
  //       board.Place(move.row, move.col);
  //     }
  //   }
  //   int win =
  //       i < 50 || board.Winner() == 3 ? board.Winner() : 3 - board.Winner();
  //   winners[win]++;
  //   std::cout << win << std::endl;
  // }
  // std::cout << winners[1] << " " << winners[2] << " " << winners[3]
  //           << std::endl;
  BoardFast board("E5F6H8E6E8");
  cout << run_mcts(board, 100).str << endl;
}
