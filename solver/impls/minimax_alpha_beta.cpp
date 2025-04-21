#include "../board/board_fast.hpp"

const int SCORE_TILE_CLAIMED = 20;
const int SCORE_TWO_IN_A_ROW = 3;
const int SCORE_SINGLE_POINT = 1;
const int SCORE_WIN = (SCORE_TILE_CLAIMED * 9 + SCORE_TWO_IN_A_ROW * 9 * 12 +
                       SCORE_SINGLE_POINT * 81) *
                      10;

int calculate_score(BoardFast board) {
  int score = 0;

  for (size_t i = 0; i < 9; i++) {
    int inner_board_score = 0;
    if (board.GetSubWinner(i) == 1) {
      inner_board_score += SCORE_TILE_CLAIMED;
      score += inner_board_score;
      continue;
    } else if (board.GetSubWinner(i) == 2) {
      inner_board_score -= SCORE_TILE_CLAIMED;
      score += inner_board_score;
      continue;
    }

    for (const auto [a, b, c] : gameRows2) {
      std::array<size_t, 3> cnt{};
      cnt[board.GetMarkInSubboard(i, a)]++;
      cnt[board.GetMarkInSubboard(i, b)]++;
      cnt[board.GetMarkInSubboard(i, c)]++;
      if (cnt[1] == 2 && cnt[0] == 1) {
        inner_board_score += SCORE_TWO_IN_A_ROW;
      } else if (cnt[2] == 2 && cnt[0] == 1) {
        inner_board_score -= SCORE_TWO_IN_A_ROW;
      }
    }

    score += inner_board_score;
  }

  return score;
}


std::mt19937 gen_minimax(static_cast<unsigned int>(std::time(0)));

std::pair<int, size_t> minimax(BoardFast board, bool maximizing_player,
                               int depth, int alpha, int beta, clock_t time_start) {
  if (board.Winner() == 1) {
    return {SCORE_WIN, 0};
  } else if (board.Winner() == 2) {
    return {-SCORE_WIN, 0};
  } else if (board.Winner() == 3) {
    return {0, 0};
  } else if (depth == 0) {
    return {calculate_score(board), 0};
  }
  if ((clock() - time_start) / (double) CLOCKS_PER_SEC > 1.0) {
    return {calculate_score(board), 0};
  }

  if (maximizing_player) {
    int best_score = INT_MIN;
    size_t move = 0;
    auto opts = board.calculateAvailableMoves();
    std::shuffle(opts.begin(), opts.end(), gen_minimax);
    for (int i : opts) {
      auto cur = board;
      cur.Place(i / 9, i % 9);
      auto [score, _] = minimax(cur, false, depth - 1, alpha, beta, time_start);
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
      auto [score, _] = minimax(cur, true, depth - 1, alpha, beta, time_start);
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
