// Formula from https://raw.githubusercontent.com/jatin7gupta/Ultimate-tic-tac-toe/refs/heads/master/tictactoe.py

#include "../board/board_fast.hpp"

namespace heur {

int three_in_a_row(const BoardFast board, int player) {
  int ans = 0;
  for (size_t i = 0; i < 9; i++) {
    if (board.GetSubWinner(i) == player) {
      ans++;
    }
  }
  return ans;
}

int two_p1_one_p2(const BoardFast board, int player) {
  int ans = 0;
  for (size_t i = 0; i < 9; i++) {
    if (board.GetSubWinner(i) != 0) {
      continue;
    }
    for (const auto [a, b, c] : gameRows2) {
      std::array<size_t, 3> cnt{};
      cnt[board.GetMarkInSubboard(i, a)]++;
      cnt[board.GetMarkInSubboard(i, b)]++;
      cnt[board.GetMarkInSubboard(i, c)]++;
      if (cnt[player] == 2 && cnt[3 - player] == 1) {
        ans++;
      }
    }
  }
  return ans;
}

int fork(const BoardFast board, int player) {
  int ans = 0;
  for (int i = 0; i < 9; i++) {
    for (auto line1 : gameRows2) {
      for (auto line2 : gameRows2) {
        int same = -1;
        for (auto x : line1) {
          for (auto y : line2) {
            if (x == y) {
              same = x;
              break;
            }
          }
        }
        if (same == -1) {
          continue;
        }

        std::array<size_t, 3> cnt1{};
        cnt1[board.GetMarkInSubboard(i, line1[0])]++;
        cnt1[board.GetMarkInSubboard(i, line1[1])]++;
        cnt1[board.GetMarkInSubboard(i, line1[2])]++;
        std::array<size_t, 3> cnt2{};
        cnt2[board.GetMarkInSubboard(i, line2[0])]++;
        cnt2[board.GetMarkInSubboard(i, line2[1])]++;
        cnt2[board.GetMarkInSubboard(i, line2[2])]++;
        if (cnt1[player] == 2 && cnt1[0] == 1 && cnt2[player] == 2 &&
            cnt2[0] == 1 && board.GetMarkInSubboard(i, same) == player) {
          ans++;
        }
      }
    }
  }
  return ans;
}

int play_centre(const BoardFast board, int player) {
  int ans = 0;
  for (size_t i = 0; i < 9; i++) {
    if (board.GetMarkInSubboard(i, 4) == 0) {
      ans++;
    }
  }
  return ans;
}

int block_opposite_corner(const BoardFast board, int player) {
  int ans = 0;
  for (size_t i = 0; i < 9; i++) {
    for (size_t a = 0; a < 3; a += 2) {
      for (size_t b = 0; b < 3; b += 2) {
        if (board.GetMarkInSubboard(i, a * 3 + b) == player &&
            board.GetMarkInSubboard(i, (2 - a) * 3 + (2 - b)) == 0) {
          ans++;
        }
      }
    }
  }
  return ans;
}

int play_empty_corner(const BoardFast board, int player) {
  int ans = 0;
  for (int i = 0; i < 9; i++) {
    for (auto j : {0, 2, 6, 8}) {
      if (board.GetMarkInSubboard(i, j) == 0) {
        ans++;
      }
    }
  }
  return ans;
}

int two_p1_next_empty(const BoardFast board, int player) {
  int ans = 0;
  for (size_t i = 0; i < 9; i++) {
    if (board.GetSubWinner(i) != 0) {
      continue;
    }
    for (const auto [a, b, c] : gameRows2) {
      std::array<size_t, 3> cnt{};
      cnt[board.GetMarkInSubboard(i, a)]++;
      cnt[board.GetMarkInSubboard(i, b)]++;
      cnt[board.GetMarkInSubboard(i, c)]++;
      if (cnt[player] == 2 && cnt[0] == 1) {
        ans++;
      }
    }
  }
  return ans;
}

int create_fork(const BoardFast board, int player) {
  int ans = 0;
  for (int i = 0; i < 9; i++) {
    for (auto line1 : gameRows2) {
      for (auto line2 : gameRows2) {
        int same = -1;
        for (auto x : line1) {
          for (auto y : line2) {
            if (x == y) {
              same = x;
              break;
            }
          }
        }
        if (same == -1) {
          continue;
        }

        std::array<size_t, 3> cnt1{};
        cnt1[board.GetMarkInSubboard(i, line1[0])]++;
        cnt1[board.GetMarkInSubboard(i, line1[1])]++;
        cnt1[board.GetMarkInSubboard(i, line1[2])]++;
        std::array<size_t, 3> cnt2{};
        cnt2[board.GetMarkInSubboard(i, line2[0])]++;
        cnt2[board.GetMarkInSubboard(i, line2[1])]++;
        cnt2[board.GetMarkInSubboard(i, line2[2])]++;
        if (cnt1[player] == 1 && cnt1[0] == 2 && cnt2[player] == 1 &&
            cnt2[0] == 2 && board.GetMarkInSubboard(i, same) == 0) {
          ans++;
        }
      }
    }
  }
  return ans;
}

int block_opp_fork(const BoardFast board, int player) {
  int ans = 0;
  for (int i = 0; i < 9; i++) {
    for (auto line1 : gameRows2) {
      for (auto line2 : gameRows2) {
        int same = -1;
        for (auto x : line1) {
          for (auto y : line2) {
            if (x == y) {
              same = x;
              break;
            }
          }
        }
        if (same == -1) {
          continue;
        }

        std::array<size_t, 3> cnt1{};
        cnt1[board.GetMarkInSubboard(i, line1[0])]++;
        cnt1[board.GetMarkInSubboard(i, line1[1])]++;
        cnt1[board.GetMarkInSubboard(i, line1[2])]++;
        std::array<size_t, 3> cnt2{};
        cnt2[board.GetMarkInSubboard(i, line2[0])]++;
        cnt2[board.GetMarkInSubboard(i, line2[1])]++;
        cnt2[board.GetMarkInSubboard(i, line2[2])]++;
        if (cnt1[player] == 1 && cnt1[3 - player] == 1 && cnt2[player] == 1 &&
            cnt2[3 - player] == 1 &&
            board.GetMarkInSubboard(i, same) == player) {
          ans++;
        }
      }
    }
  }
  return ans;
}

}  // namespace heur

vector<int> coefs = {0, 20, -1, 0, 0, 1, -4, 3, 0, 0};

int calculate_score_better(BoardFast board) {
  double x1 = heur::three_in_a_row(board, 1) - heur::three_in_a_row(board, 2);
  double x2 = heur::two_p1_one_p2(board, 1) - heur::two_p1_one_p2(board, 2);
  // double x3 = heur::fork(board, 1) - heur::fork(board, 2);
  // double x4 = heur::play_centre(board, 1) - heur::play_centre(board, 2);
  double x5 = heur::block_opposite_corner(board, 1) -
              heur::block_opposite_corner(board, 2);
  double x6 =
      heur::play_empty_corner(board, 1) - heur::play_empty_corner(board, 2);
  double x7 =
      heur::two_p1_next_empty(board, 1) - heur::two_p1_next_empty(board, 2);
  // double x8 = heur::create_fork(board, 1) - heur::create_fork(board, 2);
  // double x9 = heur::block_opp_fork(board, 1) - heur::block_opp_fork(board, 2);
  return x1 * coefs[1] + x2 * coefs[2] + x5 * coefs[5] + x6 * coefs[6] +
         x7 * coefs[7];
}

std::mt19937 gen_minimax2(static_cast<unsigned int>(std::time(0)));

std::pair<int, size_t> minimax2(BoardFast board, bool maximizing_player,
                                int depth, int alpha, int beta,
                                clock_t time_start) {
  if (board.Winner() == 1) {
    return {100000, 0};
  } else if (board.Winner() == 2) {
    return {-100000, 0};
  } else if (board.Winner() == 3) {
    return {0, 0};
  } else if (depth == 0) {
    return {calculate_score_better(board), 0};
  }
  if ((clock() - time_start) / (double)CLOCKS_PER_SEC > 1.0) {
    return {calculate_score_better(board), 0};
  }

  if (maximizing_player) {
    int best_score = INT_MIN;
    size_t move = 0;
    auto opts = board.calculateAvailableMoves();
    std::shuffle(opts.begin(), opts.end(), gen_minimax2);
    for (int i : opts) {
      auto cur = board;
      cur.Place(i / 9, i % 9);
      auto [score, _] =
          minimax2(cur, false, depth - 1, alpha, beta, time_start);
      if (score > best_score) {
        best_score = score;
        move = i;
      }
      if (best_score >= beta) {
        break;
      }
      alpha = max(alpha, best_score);
    }
    return {best_score, move};
  } else {
    int best_score = INT_MAX;
    size_t move = 0;
    for (int i : board.calculateAvailableMoves()) {
      auto cur = board;
      cur.Place(i / 9, i % 9);
      auto [score, _] = minimax2(cur, true, depth - 1, alpha, beta, time_start);
      if (score < best_score) {
        best_score = score;
        move = i;
      }
      if (best_score <= alpha) {
        break;
      }
      beta = min(beta, best_score);
    }
    return {best_score, move};
  }
}
