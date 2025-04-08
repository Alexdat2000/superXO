#ifndef BOARD_HPP
#define BOARD_HPP

#include <bits/stdc++.h>

using namespace std;

enum Mark {
  EMPTY,
  X,
  O,
};

namespace {

const std::vector<std::vector<int>> gameRows = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                                {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                                {0, 4, 8}, {2, 4, 6}};
                                                
}

struct Coord {
  int row;
  int col;
  int index;
  std::string str;

  explicit Coord(const int index) : index(index) {
    row = index / 9;
    col = index % 9;
    str = std::string(1, 'A' + col) + std::string(1, '1' + row);
  }

  explicit Coord(const int row, const int col) : row(row), col(col) {
    index = row * 9 + col;
    str = std::string(1, 'A' + col) + std::string(1, '1' + row);
  }

  explicit Coord(std::string str) : str(str) {
    index = str[0] - 'A' + (str[1] - '1') * 9;
    row = index / 9;
    col = index % 9;
  }

  [[nodiscard]] int inSubBoard() const { return (row / 3) * 3 + (col / 3); }

  [[nodiscard]] int toSubBoard() const { return row % 3 * 3 + col % 3; }

  [[nodiscard]] int toSubBoardRow() const { return row % 3; }

  [[nodiscard]] int toSubBoardCol() const { return col % 3; }
};

class Board {
 public:
  explicit Board(const std::string& moves) : moves(moves) {
    currentPlayer = (moves.length() % 4 == 0) ? "X" : "O";
    calculateBoard();
    calculateSubBoards();
    calculateWinner();
    calculateAvailableMoves();
  }

 private:
  void calculateBoard() {
    board = std::string(81, ' ');
    std::string mark = "X";
    for (size_t i = 0; i < moves.length(); i += 2) {
      Coord coord = Coord(moves.substr(i, 2));
      board[coord.index] = mark[0];
      mark = (mark == "X") ? "O" : "X";
    }
  }

  void calculateSubBoards() {
    subBoards = std::string(9, ' ');
    for (int rowStart = 0; rowStart <= 6; rowStart += 3) {
      for (int colStart = 0; colStart <= 6; colStart += 3) {
        std::vector<char> square;
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            int index = (rowStart + i) * 9 + (colStart + j);
            square.push_back(board[index]);
          }
        }

        char winner = ' ';
        for (const auto& row : gameRows) {
          if (row[0] < 9 && row[1] < 9 && row[2] < 9) {
            if (square[row[0]] != ' ' && square[row[0]] == square[row[1]] &&
                square[row[0]] == square[row[2]]) {
              winner = square[row[0]];
            }
          }
        }
        if (winner == ' ' &&
            std::all_of(square.begin(), square.end(),
                        [](char mark) { return mark != ' '; })) {
          winner = 'D';
        }
        subBoards[(rowStart / 3) * 3 + (colStart / 3)] = winner;
      }
    }
  }

  void calculateWinner() {
    winner = ' ';
    for (const auto& row : gameRows) {
      if (subBoards[row[0]] != ' ' && subBoards[row[1]] != 'D' &&
          subBoards[row[0]] == subBoards[row[1]] &&
          subBoards[row[0]] == subBoards[row[2]]) {
        winner = subBoards[row[0]];
      }
    }
    if (winner == ' ' && std::all_of(subBoards.begin(), subBoards.end(),
                                     [](char mark) { return mark != ' '; })) {
      winner = 'D';
    }
  }

  void calculateAvailableMoves() {
    availableMoves.reset();
    if (moves.empty()) {
      availableMoves.set();
      return;
    }

    const Coord lastMove = *GetLastMove();
    int tarSubBoard = lastMove.toSubBoard();
    if (subBoards[tarSubBoard] != ' ') {
      for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
          Coord coord = Coord(row, col);
          if (board[coord.index] == ' ' &&
              subBoards[coord.inSubBoard()] == ' ') {
            availableMoves[coord.index] = true;
          }
        }
      }
    } else {
      const auto edge = Coord((lastMove.row % 3) * 3, (lastMove.col % 3) * 3);
      for (int row = edge.row; row < edge.row + 3; row++) {
        for (int col = edge.col; col < edge.col + 3; col++) {
          const auto coord = Coord(row, col);
          if (board[coord.index] == ' ') {
            availableMoves[coord.index] = true;
          }
        }
      }
    }
  }

 public:
  void Place(const Coord& coord) {
    moves += coord.str;
    board[coord.index] = currentPlayer[0];
    currentPlayer = (currentPlayer == "X") ? "O" : "X";

    calculateSubBoards();
    calculateWinner();
    calculateAvailableMoves();
  }

  [[nodiscard]] std::optional<Coord> GetLastMove() const {
    if (moves.empty()) {
      return std::nullopt;
    } else {
      return Coord(moves.substr(moves.length() - 2));
    }
  }

  [[nodiscard]] bool HasWinner() const { return winner != ' '; }

 public:
  std::string moves;
  std::string currentPlayer;
  std::string board;
  std::string subBoards;
  char winner = ' ';

 public:
  std::bitset<81> availableMoves;
};

#endif  //BOARD_HPP
