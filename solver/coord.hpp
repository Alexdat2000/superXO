#pragma once

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
  