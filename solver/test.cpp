// Run: g++ test.cpp -o test -lgtest -pthread && ./test

#include <gtest/gtest.h>
#include "board/board_fastest.hpp"
#include "coord.hpp"

TEST(BoardTest, TestEmptyBoard) {
  auto board = BoardFastest("");
  EXPECT_EQ(board.calculateAvailableMoves().size(), 81);
  EXPECT_EQ(board.GetSubWinner(4), 0);
  EXPECT_EQ(board.Winner(), 0);
  EXPECT_EQ(board.CurrentPlayer(), 1);
}

TEST(BoardTest, TestStartedGame) {
  auto board = BoardFastest("E5F4H3E8F6");
  EXPECT_EQ(board.calculateAvailableMoves().size(), 9);
  EXPECT_EQ(board.GetSubWinner(4), 0);
  EXPECT_EQ(board.Winner(), 0);
  EXPECT_EQ(board.CurrentPlayer(), 2);
}

TEST(BoardTest, TestEndedGame) {
  auto board = BoardFastest(
      "E5F4I3I9H8E4D2C6G9A7A1A3B9F8H6D8A6C9I7G1B1F1I2G5B4F2H4D3A8C4I1D4A2A4B3D7"
      "C1D9A9B7E2F3");
  EXPECT_EQ(board.calculateAvailableMoves().size(), 0);

  EXPECT_EQ(board.GetSubWinner(0), 1);
  EXPECT_EQ(board.GetSubWinner(1), 2);
  EXPECT_EQ(board.GetSubWinner(2), 1);
  EXPECT_EQ(board.GetSubWinner(3), 0);
  EXPECT_EQ(board.GetSubWinner(4), 2);
  EXPECT_EQ(board.GetSubWinner(5), 0);
  EXPECT_EQ(board.GetSubWinner(6), 0);
  EXPECT_EQ(board.GetSubWinner(7), 2);
  EXPECT_EQ(board.GetSubWinner(8), 1);

  EXPECT_EQ(board.Winner(), 2);
  EXPECT_EQ(board.CurrentPlayer(), 1);

  EXPECT_EQ(board.GetMarkInSubboard(0, 0), 1);
  EXPECT_EQ(board.GetMarkInSubboard(0, 5), 0);
  EXPECT_EQ(board.GetMarkInSubboard(0, 6), 2);
  EXPECT_EQ(board.GetMarkInSubboard(1, 5), 2);
  EXPECT_EQ(board.GetMarkInSubboard(8, 8), 2);
}

TEST(CoordTest, TestCoord) {
  auto C4 = Coord("C4");
  EXPECT_EQ(C4.row, 3);
  EXPECT_EQ(C4.col, 2);
  EXPECT_EQ(C4.index, 29);
  EXPECT_EQ(C4.str, "C4");
  EXPECT_EQ(C4.inSubBoard(), 3);
  EXPECT_EQ(C4.toSubBoard(), 2);
  EXPECT_EQ(C4.toSubBoardRow(), 0);
  EXPECT_EQ(C4.toSubBoardCol(), 2);

  auto H5 = Coord(4, 7);
  EXPECT_EQ(H5.row, 4);
  EXPECT_EQ(H5.col, 7);
  EXPECT_EQ(H5.index, 43);
  EXPECT_EQ(H5.str, "H5");

  auto I9 = Coord(80);
  EXPECT_EQ(I9.row, 8);
  EXPECT_EQ(I9.col, 8);
  EXPECT_EQ(I9.index, 80);
  EXPECT_EQ(I9.str, "I9");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
