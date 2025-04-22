// Idea from https://github.com/The-Ofek-Foundation/UltimateTicTacToe/blob/gh-pages/script.js

#include <optional>
#include <vector>

#include "../board/board_fast.hpp"
#include "../coord.hpp"

constexpr double ERROR_THRESHOLD = 0.05;
constexpr double CERTAINTY_THRESHOLD = 0.9;
constexpr double EXPANSION_CONSTANT = 1.03125;

static std::mt19937 gen_mcts(static_cast<unsigned int>(std::time(nullptr)));

struct MCTSNode {
 public:
  MCTSNode* parent;
  BoardFast board;

  size_t hits = 0, misses = 0, total_tries = 0;
  bool has_children = false;
  std::vector<MCTSNode*> children;
  size_t count_unexplored = 0, result = 0;

  MCTSNode(MCTSNode* parent, BoardFast board) : parent(parent), board(board) {};

  ~MCTSNode() {
    for (auto x : children) {
      delete x;
    }
  }

  void choose_child() {
    if (board.Winner() != 0) {
      result = board.Winner();
      back_propagate(result);
      return;
    }
    if (!has_children) {
      has_children = true;
      children = mcts_get_children();
      count_unexplored = children.size();
    }

    if (count_unexplored) {
      int choose_unexplored = gen_mcts() % count_unexplored + 1;
      for (auto& child : children) {
        if (child->total_tries == 0) {
          choose_unexplored--;
          if (choose_unexplored != 0) {
            continue;
          }

          child->back_propagate(mcts_simulate(child));
        }
      }
    } else {
      MCTSNode* best_child = best_potential_child();
      best_child->choose_child();
    }
  }

  // Misc stats
  double get_certainty() {
    MCTSNode* best_child = most_tried_child();
    MCTSNode* second_best_child = second_most_tried_child(best_child);
    double ratio = second_best_child->total_tries / best_child->total_tries;
    double ratio_wins = best_child->hits < best_child->misses
                            ? (best_child->hits / best_child->misses) * 2
                            : (best_child->misses / best_child->hits) * 3;
    return min(ratio, ratio_wins);
  }

  double result_certainty() {
    if (total_tries > (hits + misses) * 3)
      return 1 - (double)(hits + misses) / total_tries;
    else if (hits > misses)
      return (double)(hits - misses) / total_tries;
    else if (hits < misses)
      return (double)(misses - hits) / total_tries;
    else
      return 1 - (double)(hits + misses) / total_tries;
  }

  double potential(double t) {
    double w = misses - hits;
    double n = total_tries;
    double c = EXPANSION_CONSTANT;

    return w / n + c * sqrt(log(t) / n);
  }

  // Child selectors
  MCTSNode* most_tried_child() {
    size_t most = children[0]->total_tries;
    MCTSNode* best = children[0];
    for (size_t i = 1; i < children.size(); i++) {
      if (children[i]->total_tries > most) {
        most = children[i]->total_tries;
        best = children[i];
      }
    }
    return best;
  }

  MCTSNode* second_most_tried_child(MCTSNode* first) {
    size_t most = children[0]->total_tries;
    MCTSNode* best = children[0];
    if (best == first) {
      most = children[1]->total_tries;
      best = children[1];
    }
    for (size_t i = 1; i < children.size(); i++) {
      if (children[i] != first && children[i]->total_tries > most) {
        most = children[i]->total_tries;
        best = children[i];
      }
    }
    return best;
  }

  MCTSNode* best_potential_child() {
    double most = children[0]->potential(total_tries);
    MCTSNode* best = children[0];
    for (size_t i = 1; i < children.size(); i++) {
      double new_potential = children[i]->potential(total_tries);
      if (new_potential > most) {
        most = new_potential;
        best = children[i];
      }
    }
  }
};

Coord run_mcts(BoardFast board) {
  auto moves = board.calculateAvailableMoves();
  if (moves.empty()) {
    abort();
  }
  if (moves.size() == 1) {
    return Coord(moves[0]);
  }

  MCTSNode* root = new MCTSNode(nullptr, board);
  clock_t start = clock();
  while ((double)(clock() - start) / CLOCKS_PER_SEC < 1) {
    for (int _ = 0; _ < 2000; _++) {
      root->choose_child();
    }
    double error = root->get_certainty();
    double certainty = root->result_certainty();
    if (error < ERROR_THRESHOLD || certainty > CERTAINTY_THRESHOLD) {
      break;
    }
  }
  return Coord(root->most_tried_child()->board.LastMove());
}
