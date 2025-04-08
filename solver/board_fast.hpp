#ifndef BOARD_FAST_HPP
#define BOARD_FAST_HPP

#include <bits/stdc++.h>

using namespace std;

using num = uint32_t;

const vector<array<size_t, 3>> gameRows = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                           {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                           {0, 4, 8}, {2, 4, 6}};

inline num getMark(num board, size_t cell) {
  return (board >> (2 * cell)) & 3;
}

class Board {
 public:
  explicit Board(const std::string& moves) : moves(moves) {
    boardState = array<num, 9>();
    for (size_t i = 0; i < moves.length(); i += 2) {
      string move = moves.substr(i, 2);
      int index = (move[1] - '1') * 9 + (move[0] - 'A');
      Place(index / 9, index % 9);
    }
  }

 private:
  num calcSubWinner(num sub_board) {
    num local = 0;
    for (const auto& [a, b, c] : gameRows) {
      if (getMark(sub_board, a) != 0 &&
          getMark(sub_board, a) == getMark(sub_board, b) &&
          getMark(sub_board, a) == getMark(sub_board, c)) {
        local = getMark(sub_board, a);
      }
    }
    if (local == 0) {
      bool filled = true;
      for (size_t i = 0; i < 9; i++) {
        if (getMark(sub_board, i) == 0) {
          filled = false;
          break;
        }
      }
      if (filled) {
        local = 3;
      }
    }
    return local;
  }

  vector<size_t> calculateAvailableMoves() {
    vector<size_t> available;
    if (moves.empty()) {
      available.resize(81);
      iota(available.begin(), available.end(), 0u);
      return available;
    }

    int tarSubBoard = ((last_move / 9) % 3) * 3 + last_move % 3;
    if (getMark(subWinners, tarSubBoard)) {
      for (size_t row = 0; row < 9; row++) {
        for (size_t col = 0; col < 9; col++) {
          size_t board = row / 3 * 3 + col / 3;
          size_t in_board = row % 3 * 3 + col % 3;
          if (getMark(boardState[board], in_board) == 0 &&
              getMark(subWinners, board) == 0) {
            available.push_back(row * 9 + col);
          }
        }
      }
    } else {
      size_t tar_board = last_move % 3 * 3 + last_move % 3;
      for (size_t row = 0; row < 3; row++) {
        for (size_t col = 0; col < 3; col++) {
          if (getMark(boardState[tar_board], row * 3 + col) == 0) {
            available.push_back((tar_board / 3 * 3 + row) * 9 +
                                (tar_board % 3 * 3 + col));
          }
        }
      }
    }
  }

 public:
  void Place(int row, int col) {
    size_t sub_board = row / 3 * 3 + col / 3;
    size_t inside = row % 3 * 3 + col % 3;
    boardState[sub_board] += (currentPlayer << (1 << (inside * 2)));
    currentPlayer = 3 - currentPlayer;
    num subboard_winner = calcSubWinner(boardState[sub_board]);
    subWinners = (subWinners & (3 << (2 * sub_board))) |
                 (subboard_winner << (2 * sub_board));
    winner = calcSubWinner(subWinners);
    last_move = row * 9 + col;
  }

  [[nodiscard]] bool HasWinner() const { return winner != 0; }

 private:
  std::string moves;
  num currentPlayer = 1;
  std::array<num, 9> boardState;
  num subWinners = 0;
  num winner = 0;
  size_t last_move;
};

#endif  // BOARD_FAST_HPP
