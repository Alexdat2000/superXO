#include "../board/board_fast.hpp"
#include "../board/board_fastest.hpp"
#include "../coord.hpp"
#include "mcts.cpp"
#include "mcts_improved_exploration.cpp"

int main() {
  std::array<int, 4> winners{};
  for (int i = 0; i < 100; i++) {
    BoardFast board("");
    BoardFastest board2("");
    while (board.Winner() == 0) {
      if ((board.CurrentPlayer() == 1) ^ (i >= 50)) {
        Coord move = better_run_mcts(board2);
        board.Place(move.row, move.col);
        board2.Place(move.row, move.col);
        std::cout << move.str;
        cout.flush();
      } else {
        Coord move = run_mcts(board);
        board.Place(move.row, move.col);
        board2.Place(move.row, move.col);
        std::cout << move.str;
        cout.flush();
      }
    }
    int win =
        i < 50 || board.Winner() == 3 ? board.Winner() : 3 - board.Winner();
    winners[win]++;
    std::cout << std::endl << win << std::endl;
  }
  std::cout << winners[1] << " " << winners[2] << " " << winners[3]
            << std::endl;
}
