#include "../board/board_fast.hpp"
#include "../coord.hpp"
#include "minimax_alpha_beta.cpp"
#include "minimax_better_formula.cpp"

int main() {
  vector<int> best = {0, 0, 0, 0, 0, 0, 0};
  int eval = 0;

  while (true) {
    vector<int> cur = {0, 0, 0, 0, 0, 0, 0};
    for (auto& i : cur) {
      i = rand() % 3;
    }
    cur[0] *= -1;
    vector<int> insert = {2, 3, 4, 5, 6, 8, 9};
    for (int i = 0; i < insert.size(); i++) {
        coefs[insert[i]] = cur[i];
    }
    std::array<int, 4> winners{};
    for (int i = 0; i < 200; i++) {
      BoardFast board("");
      while (board.Winner() == 0) {
        if ((board.CurrentPlayer() == 1) ^ (i >= 100)) {
          Coord move = Coord(minimax(board, board.CurrentPlayer() == 1, 8,
                                     INT_MIN, INT_MAX, clock())
                                 .second);
          board.Place(move.row, move.col);
        } else {
          Coord move = Coord(minimax2(board, board.CurrentPlayer() == 1, 8,
                                      INT_MIN, INT_MAX, clock())
                                 .second);
          board.Place(move.row, move.col);
        }
      }
      int win =
          i < 100 || board.Winner() == 3 ? board.Winner() : 3 - board.Winner();
      winners[win]++;
    }
    if (winners[2] - winners[1] > eval) {
      eval = winners[2] - winners[1];
      best = cur;
    }
    cout << "New conf: ";
    for (int i : cur) {
      cout << i << " ";
    }
    cout << ": " << winners[2] - winners[1] << endl;
    cout << "Best conf: ";
    for (int i : best) {
      cout << i << " ";
    }
    cout << ": " << eval << endl;
  }
}
