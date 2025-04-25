#ifndef BOARD_FASTEST_HPP
#define BOARD_FASTEST_HPP

#include <bits/stdc++.h>

using namespace std;

mt19937 gen_board(static_cast<unsigned int>(time(nullptr)));

const vector<vector<size_t>> game_rows_custom = {
    {21, 4161, 65793},          {21, 16644},
    {21, 66576, 4368},          {1344, 4161},
    {1344, 16644, 65793, 4368}, {1344, 66576},
    {86016, 4161, 4368},        {86016, 16644},
    {86016, 66576, 65793}};

class BoardFastest {
 public:
  explicit BoardFastest(const std::string& moves) {
    boardState = array<uint32_t, 9>();
    for (size_t i = 0; i < moves.length(); i += 2) {
      string move = moves.substr(i, 2);
      int index = (move[1] - '1') * 9 + (move[0] - 'A');
      Place(index / 9, index % 9);
    }
  }

 private:
  static inline uint32_t getMark(uint32_t board, size_t cell) {
    return (board >> (2 * cell)) & 3;
  }

  uint32_t calcSubWinner(uint32_t sub_board, int changed_index) {
    uint32_t local = 0;
    for (const auto& mask : game_rows_custom[changed_index]) {
      if ((sub_board & mask) == mask && (sub_board & (mask << 1)) == 0) {
        local = 1;
        break;
      } else if ((sub_board & (mask << 1)) == (mask << 1) &&
                 (sub_board & mask) == 0) {
        local = 2;
        break;
      }
    }
    if (local == 0) {
      bool filled =
          __builtin_popcount((sub_board | (sub_board >> 1)) & 0x15555) == 9;
      if (filled) {
        local = 3;
      }
    }
    return local;
  }

 public:
  vector<size_t> calculateAvailableMoves() const {
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

  int GetRandomAvailableMove() const {
    if (winner != 0) {
      return -1;
    }
    if (last_move == -1) {
      return gen_board() % 81;
    }

    size_t tarSubBoard = last_move / 9 % 3 * 3 + last_move % 3;
    if (getMark(subWinners, tarSubBoard)) {
      int take = gen_board() % free_places + 1;
      for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
          size_t board = row / 3 * 3 + col / 3;
          size_t in_board = row % 3 * 3 + col % 3;
          if (getMark(boardState[board], in_board) == 0 &&
              getMark(subWinners, board) == 0) {
            take--;
            if (take == 0) {
              return row * 9 + col;
            }
          }
        }
      }
      return -1;
    } else {
      int av = 9 - __builtin_popcount((boardState[tarSubBoard] |
                                       (boardState[tarSubBoard] >> 1)) &
                                      0x15555);
      int take = gen_board() % av + 1;
      for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
          if (getMark(boardState[tarSubBoard], row * 3 + col) == 0) {
            take--;
            if (take == 0) {
              return (tarSubBoard / 3 * 3 + row) * 9 +
                     (tarSubBoard % 3 * 3 + col);
            }
          }
        }
      }
      return -1;
    }
    return -1;
  }

  void Place(size_t row, size_t col) {
    free_places--;
    size_t sub_board = row / 3 * 3 + col / 3;
    size_t inside = row % 3 * 3 + col % 3;
    boardState[sub_board] += (currentPlayer << (inside * 2));
    currentPlayer = 3 - currentPlayer;
    uint32_t subboard_winner =
        calcSubWinner(boardState[sub_board], row % 3 * 3 + col % 3);
    subWinners = (subWinners & (~(3 << (2 * sub_board)))) |
                 (subboard_winner << (2 * sub_board));
    if (getMark(subWinners, row / 3 * 3 + col / 3)) {
      free_places -=
          9 - __builtin_popcount((boardState[row / 3 * 3 + col / 3] |
                                  (boardState[row / 3 * 3 + col / 3] >> 1)) &
                                 0x15555);
    }
    winner = calcSubWinner(subWinners, row / 3 * 3 + col / 3);
    last_move = row * 9 + col;
  }

  [[nodiscard]] uint32_t GetSubWinner(size_t field) const {
    return (subWinners >> (2 * field)) & 3;
  }

  [[nodiscard]] uint32_t GetMarkInSubboard(size_t field, size_t cell) const {
    return (boardState[field] >> (2 * cell)) & 3;
  }

  [[nodiscard]] uint32_t GetSubboard(size_t field) const {
    return boardState[field];
  }

  [[nodiscard]] uint32_t GetMark(size_t row, size_t col) const {
    return GetMarkInSubboard(row / 3 * 3 + col / 3, row % 3 * 3 + col % 3);
  }

  [[nodiscard]] uint32_t Winner() const { return winner; }

  [[nodiscard]] uint32_t CurrentPlayer() const { return currentPlayer; }

  [[nodiscard]] size_t LastMove() const { return last_move; }

 private:
  uint32_t currentPlayer = 1;
  std::array<uint32_t, 9> boardState;
  uint32_t subWinners = 0;
  uint32_t winner = 0;
  size_t last_move = -1;
  size_t free_places = 81;
};

#endif  // BOARD_FASTEST_HPP
