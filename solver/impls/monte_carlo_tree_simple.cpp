// Implementation of simple Monte-Carlo Tree Search

#include <string>

#include "../board/board_fast.hpp"

std::mt19937 gen(static_cast<unsigned int>(std::time(0)));

std::string GetBestMove(BoardFast board, size_t tries) {
  assert(tries > 0);
  auto options = board.calculateAvailableMoves();
  if (options.empty()) {
    return "";
  }
  vector<array<int, 3>> stats(options.size());
  int move = 0;
  for (size_t _ = 0; _ < tries; _++) {
    auto new_board = board;
    move = gen() % options.size();
    new_board.Place(options[move] / 9, options[move] % 9);
    while (new_board.Winner() == 0) {
      auto cur_opts = new_board.calculateAvailableMoves();
      assert(!cur_opts.empty());
      int cur_move = gen() % cur_opts.size();
      new_board.Place(cur_opts[cur_move] / 9, cur_opts[cur_move] % 9);
    }
    stats[move][new_board.Winner() - 1]++;
  }
  size_t best = move;
  double best_ratio =
      stats[move][board.CurrentPlayer() - 1] / (stats[move][0] + stats[move][1] + stats[move][2]);
  cout << "Stats:" << endl;
  for (size_t i = 0; i < options.size(); i++) {
    if (stats[i][0] + stats[i][1] + stats[i][2] == 0) {
      continue;
    }
    double ratio = stats[i][board.CurrentPlayer() - 1] / (stats[i][0] + stats[i][1] + stats[i][2]);
    cout << options[i] << " " << stats[i][0] << " " << stats[i][1] << " " << stats[i][2] << endl;
    if (ratio > best_ratio) {
      best = i;
      best_ratio = ratio;
    }
  }
  cout << endl;
  return string(1, options[best] % 9 + 'A') + string(1, options[best] / 9 + '1');
}
