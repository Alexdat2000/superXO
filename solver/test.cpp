#include <gtest/gtest.h>
#include "board_fast.hpp"

TEST(BoardTest, TestEmptyBoard) {
    auto board = BoardFast("");
    EXPECT_EQ(board.calculateAvailableMoves().size(), 81);
    EXPECT_EQ(board.GetSubWinner(4), 0);
    EXPECT_EQ(board.Winner(), 0);
    EXPECT_EQ(board.CurrentPlayer(), 1);
}

TEST(BoardTest, TestStartedGame) {
    auto board = BoardFast("E5F4H3E8F6");
    EXPECT_EQ(board.calculateAvailableMoves().size(), 9);
    EXPECT_EQ(board.GetSubWinner(4), 0);
    EXPECT_EQ(board.Winner(), 0);
    EXPECT_EQ(board.CurrentPlayer(), 2);
}

TEST(BoardTest, TestEndedGame) {
    auto board = BoardFast("E5F4I3I9H8E4D2C6G9A7A1A3B9F8H6D8A6C9I7G1B1F1I2G5B4F2H4D3A8C4I1D4A2A4B3D7C1D9A9B7E2F3");
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
