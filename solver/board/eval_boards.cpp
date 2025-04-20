#include <iostream>

#include "board.hpp"
#include "board_fast.hpp"

void testSlow() {
  for (int _ = 0; _ < 100'000; _++) {
    Board b("");

    while (true) {
      int cnt = b.availableMoves.count();
      if (cnt == 0) {
        break;
      }
      int take = rand() % cnt;
      int idx = -1;
      while (take >= 0) {
        idx++;
        if (b.availableMoves[idx]) {
          take -= 1;
        }
      }
      b.Place(Coord(idx / 9, idx % 9));
    }
  }
}

void testFast() {
  for (int _ = 0; _ < 100'000; _++) {
    BoardFast b("");

    while (true) {
      auto av2 = b.calculateAvailableMoves();
      if (av2.size() == 0) {
        break;
      }
      int move = av2[rand() % av2.size()];
      b.Place(move / 9, move % 9);
    }
  }
}
