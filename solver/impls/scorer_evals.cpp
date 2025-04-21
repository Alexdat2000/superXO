#include "../board/board_fast.hpp"
#include "minimax_alpha_beta.cpp"
#include "minimax_better_formula.cpp"
#include "../coord.hpp"

int main() {
    string ans = "E5F4I3I9H8E4D2C6G9A7A1A3B9F8H6D8A6C9I7G1B1F1I2G5B4F2H4D3A8C4I1D4A2A4B3D7C1D9A9B7E2F3";
    for (int i = 0; i < ans.size(); i += 2) {
        BoardFast board(ans.substr(0, i));
        if (calculate_score(board) != calculate_score_better(board)) {
            cout << ans.substr(0, i) << " " << calculate_score(board) << " " << calculate_score_better(board) << "\n";
            return 0;
        }
    }
}
