#ifndef BOARD_FAST_HPP
#define BOARD_FAST_HPP

#include <bits/stdc++.h>

using namespace std;

const vector<array<size_t, 3>> gameRows2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                            {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                            {0, 4, 8}, {2, 4, 6}};

inline uint32_t getMark(uint32_t board, size_t cell) {
  return (board >> (2 * cell)) & 3;
}

class BoardFast {
 public:
  explicit BoardFast(const std::string& moves) {
    boardState = array<uint32_t, 9>();
    for (size_t i = 0; i < moves.length(); i += 2) {
      string move = moves.substr(i, 2);
      int index = (move[1] - '1') * 9 + (move[0] - 'A');
      Place(index / 9, index % 9);
    }
  }

 private:
  uint32_t calcSubWinner(uint32_t sub_board) {
    uint32_t local = 0;
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
    if (winner != 0) {
      return available;
    }
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

  void Place(size_t row, size_t col) {
    size_t sub_board = row / 3 * 3 + col / 3;
    size_t inside = row % 3 * 3 + col % 3;
    boardState[sub_board] += (currentPlayer << (inside * 2));
    currentPlayer = 3 - currentPlayer;
    uint32_t subboard_winner = calcSubWinner(boardState[sub_board]);
    subWinners = (subWinners & (~(3 << (2 * sub_board)))) |
                 (subboard_winner << (2 * sub_board));
    winner = calcSubWinner(subWinners);
    last_move = row * 9 + col;
  }

  [[nodiscard]] uint32_t GetSubWinner(size_t field) const {
    return (subWinners >> (2 * field)) & 3;
  }

  [[nodiscard]] uint32_t GetMarkInSubboard(size_t field, size_t cell) const {
    return (boardState[field] >> (2 * cell)) & 3;
  }

  [[nodiscard]] uint32_t Winner() const { return winner; }

  [[nodiscard]] uint32_t CurrentPlayer() const { return currentPlayer; }

 private:
  uint32_t currentPlayer = 1;
  std::array<uint32_t, 9> boardState;
  uint32_t subWinners = 0;
  uint32_t winner = 0;
  size_t last_move = -1;
};

#endif  // BOARD_FAST_HPP
