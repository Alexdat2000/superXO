#ifndef BOARD_FAST_HPP
#define BOARD_FAST_HPP

#include <bits/stdc++.h>

using namespace std;

using num = uint32_t;

const vector<array<size_t, 3>> gameRows2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                           {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                           {0, 4, 8}, {2, 4, 6}};

inline num getMark(num board, size_t cell) {
  return (board >> (2 * cell)) & 3;
}

class BoardFast {
 public:
  explicit BoardFast(const std::string& moves) {
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
    for (const auto& [a, b, c] : gameRows2) {
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

 public:
  vector<size_t> calculateAvailableMoves() {
    vector<size_t> available;
    if (last_move == -1) {
      available.resize(81);
      iota(available.begin(), available.end(), 0u);
      return available;
    }

    size_t tarSubBoard = last_move / 9 % 3 * 3 + last_move % 3;
    if (getMark(subWinners, tarSubBoard)) {
      for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
          size_t board = row / 3 * 3 + col / 3;
          size_t in_board = row % 3 * 3 + col % 3;
          if (getMark(boardState[board], in_board) == 0 &&
              getMark(subWinners, board) == 0) {
            available.push_back(row * 9 + col);
          }
        }
      }
    } else {
      for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
          if (getMark(boardState[tarSubBoard], row * 3 + col) == 0) {
            available.push_back((tarSubBoard / 3 * 3 + row) * 9 +
                                (tarSubBoard % 3 * 3 + col));
          }
        }
      }
    }
    return available;
  }

  void Place(int row, int col) {
    size_t sub_board = row / 3 * 3 + col / 3;
    size_t inside = row % 3 * 3 + col % 3;
    boardState[sub_board] += (currentPlayer << (inside * 2));
    currentPlayer = 3 - currentPlayer;
    num subboard_winner = calcSubWinner(boardState[sub_board]);
    subWinners = (subWinners & (~(3 << (2 * sub_board)))) |
                 (subboard_winner << (2 * sub_board));
    winner = calcSubWinner(subWinners);
    last_move = row * 9 + col;
  }

  [[nodiscard]] bool HasWinner() const { return winner != 0; }

 private:
  num currentPlayer = 1;
  std::array<num, 9> boardState;
  num subWinners = 0;
  num winner = 0;
  size_t last_move = -1;
};

#endif  // BOARD_FAST_HPP
