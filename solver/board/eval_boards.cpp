#include <iostream>

#include "board.hpp"
#include "board_fastest.hpp"

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
    BoardFastest b("");

    while (b.Winner() == 0) {
    int move = b.GetRandomAvailableMove();
    b.Place(move / 9, move % 9);
    }
}
}

int main() {
  testFast();
  std::cout << (double)clock() / CLOCKS_PER_SEC << std::endl;
}
