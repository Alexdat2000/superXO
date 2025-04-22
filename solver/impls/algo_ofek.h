#ifndef ALGO_OFEK_H
#define ALGO_OFEK_H

#include <array>
#include <cmath>
#include <ctime>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "../board/board_fast.hpp"
#include "../coord.hpp"

const double EXPANSION_CONSTANT = 1.03125;

using Board = std::array<std::array<int, 9>, 9>;
using Subs = std::array<std::array<int, 3>, 3>;

class MCTSNode {
 public:
  std::shared_ptr<MCTSNode> parent;
  bool turn;
  std::optional<Coord> last_move;

  size_t hits = 0, misses = 0, total_tries = 0;
  bool has_children = false;
  std::vector<std::shared_ptr<MCTSNode>> children;
  int result = 10;
  size_t count_unexplored = 0;

  MCTSNode(std::shared_ptr<MCTSNode> parent, bool turn,
           std::optional<Coord> last_move);

  void choose_child(Board board, Subs empty_spots, size_t total_empty);
  void back_propogate(int simulation);
};

double mcts_child_potential(std::shared_ptr<MCTSNode> child,
                            size_t total_tries);
int play_move_empty_left(Board board, Coord last_move, bool xturn,
                         size_t empty_left);
bool local_win(Board board, int color, Coord last_move, Coord start);
bool game_over(Board board, int color, Coord start);
int mcts_simulate(std::shared_ptr<MCTSNode> parent, Board board,
                  Subs empty_spots, int total_empty, int play_move_result);
std::vector<std::shared_ptr<MCTSNode>> mcts_get_children(
    std::shared_ptr<MCTSNode> parent, Board board);
Coord run_mcts(BoardFast board, size_t iters);

#endif  // ALGO_OFEK_H
