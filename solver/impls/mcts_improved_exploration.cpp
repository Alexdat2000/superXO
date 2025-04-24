// Idea from https://github.com/The-Ofek-Foundation/UltimateTicTacToe/blob/gh-pages/script.js

#include <optional>
#include <vector>

#include "../board/board_fast.hpp"
#include "../coord.hpp"
#include "minimax_alpha_beta.cpp"

namespace better_mcts {

constexpr double ERROR_THRESHOLD = 0.05;
constexpr double CERTAINTY_THRESHOLD = 0.9;
constexpr double EXPANSION_CONSTANT = 1.03125;

static std::mt19937 gen_mcts(static_cast<unsigned int>(std::time(nullptr)));

std::map<uint32_t, size_t> move1, move2;

void init() {
  for (size_t i = 0; i < 19683; i++) {
    vector<int> conf(9);
    int n = i;
    for (size_t j = 0; j < 9; j++) {
      conf[j] = n % 3;
      n /= 3;
    }

    int win = 0;
    for (auto [a, b, c] : gameRows2) {
      if (conf[a] == conf[b] && conf[a] == conf[c] && conf[a] != 0) {
        win = conf[a];
        break;
      }
    }
    if (win) {
      continue;
    }

    for (int j = 0; j < 9; j++) {
      if (conf[j] != 0) {
        continue;
      }
      auto conf2 = conf;
      conf2[j] = 1;
      int win1 = 0;
      for (auto [a, b, c] : gameRows2) {
        if (conf2[a] == conf2[b] && conf2[a] == conf2[c] && conf2[a] != 0) {
          win1 = conf2[a];
          break;
        }
      }
      if (win1 == 1) {
        move1[i] = j;
      }

      conf2 = conf;
      conf2[j] = 1;
      int win2 = 0;
      for (auto [a, b, c] : gameRows2) {
        if (conf2[a] == conf2[b] && conf2[a] == conf2[c] && conf2[a] != 0) {
          win2 = conf2[a];
          break;
        }
      }
      if (win2 == 2) {
        move2[i] = j;
      }
    }
  }
}

struct MCTSNode {
 public:
  MCTSNode* parent;
  BoardFast board;

  int hits = 0, misses = 0, total_tries = 0;
  bool has_children = false;
  std::vector<MCTSNode*> children;
  size_t count_unexplored = 0, result = 0;

  MCTSNode(MCTSNode* parent, BoardFast board) : parent(parent), board(board) {};

  ~MCTSNode() {
    for (auto x : children) {
      delete x;
    }
  }

  // Algorithm
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

          child->back_propagate(child->mcts_simulate());
          count_unexplored--;
          break;
        }
      }
    } else {
      MCTSNode* best_child = best_potential_child();
      best_child->choose_child();
    }
  }

  int mcts_simulate() {
    if (board.Winner() != 0) {
      return board.Winner();
    }

    auto new_board = board;
    while (new_board.Winner() == 0) {
      auto moves = new_board.calculateAvailableMoves();
      if (moves.size() > 9) {
        auto move = moves[gen_mcts() % moves.size()];
        new_board.Place(move / 9, move % 9);
        continue;
      }

      int board = Coord(moves[0]).inSubBoard();
      int board_content = new_board.GetSubboard(board);
      if (new_board.CurrentPlayer() == 1) {
        if (move1.count(board_content)) {
          Coord move = Coord(board / 3 * 3 + move1[board_content] / 3,
                             board % 3 * 3 + move1[board_content] % 3);
          if (find(moves.begin(), moves.end(), move.index) != moves.end()) {
            new_board.Place(move.row, move.col);
            continue;
          }
        }
      } else {
        if (move2.count(board_content)) {
          Coord move = Coord(board / 3 * 3 + move2[board_content] / 3,
                             board % 3 * 3 + move2[board_content] % 3);
          if (find(moves.begin(), moves.end(), move.index) != moves.end()) {
            new_board.Place(move.row, move.col);
            continue;
          }
        }
      }
      auto move = moves[gen_mcts() % moves.size()];
      new_board.Place(move / 9, move % 9);
    }
    return new_board.Winner();
  }

  void back_propagate(int result) {
    total_tries++;
    if (result == board.CurrentPlayer()) {
      hits++;
    } else if (result == 3 - board.CurrentPlayer()) {
      misses++;
    }
    if (parent != nullptr) {
      parent->back_propagate(result);
    }
  }

  // Misc stats
  double get_certainty() {
    MCTSNode* best_child = most_tried_child();
    MCTSNode* second_best_child = second_most_tried_child(best_child);
    if (second_best_child->total_tries == 0) {
      return 0;
    }
    double ratio = static_cast<double>(second_best_child->total_tries) /
                   best_child->total_tries;
    if (best_child->hits == 0 && best_child->misses == 0) {
      return ratio;
    }
    double ratio_wins =
        best_child->hits < best_child->misses
            ? (static_cast<double>(best_child->hits) / best_child->misses) * 2
            : (static_cast<double>(best_child->misses) / best_child->hits) * 3;
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

    if (n == 0)
      return std::numeric_limits<double>::infinity();
    return w / n + c * sqrt(log(t) / n);
  }

  // Child selectors
  vector<MCTSNode*> mcts_get_children() {
    vector<MCTSNode*> children;
    for (auto x : board.calculateAvailableMoves()) {
      children.push_back(new MCTSNode(this, board));
      children.back()->board.Place(Coord(x).row, Coord(x).col);
    }
    return children;
  }

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
    return best;
  }
};

}  // namespace better_mcts

// Entry point
Coord better_run_mcts(BoardFast board) {
  auto moves = board.calculateAvailableMoves();
  if (moves.empty()) {
    abort();
  }
  if (moves.size() == 1) {
    return Coord(moves[0]);
  }

  auto* root = new better_mcts::MCTSNode(nullptr, board);
  clock_t start = clock();
  while ((double)(clock() - start) / CLOCKS_PER_SEC < 1) {
    for (int _ = 0; _ < 2000; _++) {
      root->choose_child();
    }
    double error = root->get_certainty();
    double certainty = root->result_certainty();
    if (error < better_mcts::ERROR_THRESHOLD ||
        certainty > better_mcts::CERTAINTY_THRESHOLD) {
      break;
    }
  }
  auto ans = Coord(root->most_tried_child()->board.LastMove());
  delete root;
  return ans;
}
