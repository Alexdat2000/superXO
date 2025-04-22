// Based on https://github.com/The-Ofek-Foundation/UltimateTicTacToe/blob/gh-pages/script.js

#include "algo_ofek.h"

#include <utility>
#include "../board/board_fast.hpp"
#include "../coord.hpp"

static std::mt19937 gen_ofek(static_cast<unsigned int>(std::time(nullptr)));

MCTSNode::MCTSNode(MCTSNode* parent, bool turn,
                   std::optional<Coord> last_move)
    : parent(parent), turn(turn), last_move(std::move(last_move)) {}

MCTSNode::~MCTSNode() {
  for (auto x : children) {
    delete x;
  }
}

void MCTSNode::choose_child(Board board, Subs empty_spots, size_t total_empty) {
  if (!has_children) {
    has_children = true;
    children = mcts_get_children(this, board);
    count_unexplored = children.size();
  }
  if (result != 10) {
    back_propogate(result);
  } else {
    size_t i, empty_left, play_move_result;
    size_t unexplored = count_unexplored;
    if (unexplored > 0) {
      count_unexplored--;
      size_t ran = gen_ofek() % unexplored;
      for (i = 0; i < children.size(); i++) {
        if (children[i]->total_tries == 0) {
          unexplored--;
          if (unexplored == 0) {
            Coord last_move = *children[i]->last_move;
            empty_left = empty_spots[last_move.inSubBoardRow()]
                                    [last_move.inSubBoardCol()];
            play_move_result = play_move_empty_left(
                board, last_move, !children[i]->turn, empty_left);

            if (play_move_result == 0) {
              total_empty--;
              empty_spots[last_move.inSubBoardRow()]
                         [last_move.inSubBoardCol()]--;
            } else {
              total_empty -= empty_left;
              empty_spots[last_move.inSubBoardRow()]
                         [last_move.inSubBoardCol()] = 0;
            }

            children[i]->back_propogate(mcts_simulate(children[i], board,
                                                      empty_spots, total_empty,
                                                      play_move_result));
            return;
          }
        }
      }
    } else {
      MCTSNode* best_child = children[0];
      double best_potential = mcts_child_potential(best_child, total_tries);
      double potential;
      for (int i = 1; i < children.size(); i++) {
        potential = mcts_child_potential(children[i], total_tries);
        if (potential > best_potential) {
          best_child = children[i];
          best_potential = potential;
        }
      }
      Coord last_move = *best_child->last_move;
      empty_left =
          empty_spots[last_move.inSubBoardRow()][last_move.inSubBoardCol()];
      play_move_result =
          play_move_empty_left(board, last_move, !best_child->turn, empty_left);

      if (play_move_result == 0) {
        total_empty--;
        empty_spots[last_move.inSubBoardRow()][last_move.inSubBoardCol()]--;
      } else {
        total_empty -= empty_left;
        empty_spots[last_move.inSubBoardRow()][last_move.inSubBoardCol()] = 0;
      }

      best_child->choose_child(board, empty_spots, total_empty);
    }
  }
}

void MCTSNode::back_propogate(int simulation) {
  if (simulation == 1) {
    hits++;
  } else if (simulation == -1) {
    misses++;
  }
  total_tries++;
  if (parent != nullptr) {
    parent->back_propogate(-simulation);
  }
}

double mcts_child_potential(MCTSNode* child, size_t total_tries) {
  double w = child->misses - child->hits;
  double n = child->total_tries;
  double c = EXPANSION_CONSTANT;
  return w / n + c * sqrt(log(total_tries) / n);
}

int play_move_empty_left(Board& board, Coord last_move, bool xturn,
                         size_t empty_left) {
  int color = xturn ? 1 : 2;
  Coord start(last_move.row - last_move.row % 3,
              last_move.col - last_move.col % 3);
  Coord center(start.row + 1, start.col + 1);
  board[last_move.row][last_move.col] = color;
  if (empty_left < 8 && local_win(board, color, last_move, start)) {
    board[center.row][center.col] = color + 4;
    return 1;
  } else if (empty_left == 1) {
    board[center.row][center.col] += 2;
    return 2;
  }
  return 0;
}

bool local_win(Board board, int color, Coord last_move, Coord start) {
  for (auto [a, b, c] : gameRows2) {
    if (board[start.row + a / 3][start.col + a % 3] == color &&
        board[start.row + b / 3][start.col + b % 3] == color &&
        board[start.row + c / 3][start.col + c % 3] == color) {
      return true;
    }
  }
  return false;
}

bool game_over(Board board, int color, Coord start) {
  for (auto [a, b, c] : gameRows2) {
    if (board[start.row - start.row % 3 + a / 3]
             [start.col - start.col % 3 + a % 3] == color &&
        board[start.row - start.row % 3 + b / 3]
             [start.col - start.col % 3 + b % 3] == color &&
        board[start.row - start.row % 3 + c / 3]
             [start.col - start.col % 3 + c % 3] == color) {
      return true;
    }
  }
  return false;
}

int mcts_simulate(MCTSNode* parent, Board board, Subs empty_spots,
                  int total_empty, int play_move_result) {
  if (parent->result != 10) {
    return parent->result;
  }

  if (play_move_result == 1 && total_empty <= 54 &&
      game_over(board, parent->turn ? 6 : 5, *parent->last_move)) {
    return 0;
  }

  if (total_empty == 0) {
    return 0;
  }

  auto lm = *parent->last_move;
  auto turn = parent->turn;
  auto done = false;
  Coord next_center(0);
  Coord goal(0);

  while (!done) {
    next_center = Coord(lm.row % 3 * 3 + 1, lm.col % 3 * 3 + 1);
    int current_empty = empty_spots[next_center.row / 3][next_center.col / 3];
    if (current_empty != 0) {
      int count = gen_ofek() % current_empty;

      for (int i = 0; i < 9; i++) {
        if (board[next_center.row - 1 + i / 3][next_center.col - 1 + i % 3] ==
            0) {
          if (count == 0) {
            goal =
                Coord(next_center.row - 1 + i / 3, next_center.col - 1 + i % 3);
            break;
          } else {
            count--;
          }
        }
      }
    } else {
      int count = gen_ofek() % total_empty;
      for (int i = 0; i < 81; i++) {
        Coord c(i);
        if (board[c.row - c.row % 3 + 1][c.col - c.col % 3 + 1] < 3) {
          if (board[c.row][c.col] == 0) {
            if (count == 0) {
              goal = c;
            } else {
              count--;
            }
          }
        }
      }
    }

    int empty_left = empty_spots[goal.row / 3][goal.col / 3];
    int play_move_result = play_move_empty_left(board, goal, turn, empty_left);
    if (play_move_result == 0) {
      total_empty--;
      empty_spots[goal.row / 3][goal.col / 3]--;
    } else {
      total_empty -= empty_left;
      empty_spots[goal.row / 3][goal.col / 3] = 0;
      if (play_move_result == 1 && total_empty <= 54) {
        done = game_over(board, turn ? 5 : 6, goal);
      }
      if (total_empty == 0) {
        return 0;
      }
    }
    lm = goal;
    turn = !turn;
  }
  if (turn == parent->turn) {
    return -1;
  } else {
    return 1;
  }
}

vector<MCTSNode*> mcts_get_children(MCTSNode* parent,
                                               Board board) {
  if (parent->result != 10) {
    return {};
  }

  bool turn = parent->turn;
  vector<MCTSNode*> children;

  if (parent->last_move) {
    Coord next_center = Coord(parent->last_move->toSubBoardRow() * 3 + 1,
                              parent->last_move->toSubBoardCol() * 3 + 1);
    if (board[next_center.row][next_center.col] < 3) {
      for (int i = 0; i < 9; i++) {
        if (board[next_center.row - 1 + i / 3][next_center.col - 1 + i % 3] ==
            0) {
          Coord goal =
              Coord(next_center.row - 1 + i / 3, next_center.col - 1 + i % 3);
          children.push_back(new MCTSNode(parent, !turn, goal));
        }
      }
      return children;
    } else {
      for (int i = 0; i < 81; i++) {
        Coord c(i);
        if (board[c.row - c.row % 3 + 1][c.col - c.col % 3 + 1] < 3) {
          if (board[c.row][c.col] == 0) {
            children.push_back(new MCTSNode(parent, !turn, c));
          }
        }
      }
      return children;
    }
  } else {
    for (int i = 0; i < 81; i++) {
      Coord c(i);
      children.push_back(new MCTSNode(parent, !turn, c));
    }
    return children;  // if ransom is paid
  }
}

Coord run_mcts(BoardFast board, size_t iters) {
  Board mcts_board{};
  for (size_t i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      mcts_board[i][j] = board.GetMark(i, j);
    }
  }
  Subs empty_spots{};
  size_t total_empty = 0;
  for (size_t i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (board.GetMarkInSubboard(i, j) == 0) {
        empty_spots[i / 3][i % 3]++;
        total_empty++;
      }
    }
  }
  auto* root = new MCTSNode(
      nullptr, board.CurrentPlayer() == 1,
      board.LastMove() == -1 ? std::nullopt
                             : make_optional(Coord(board.LastMove())));

  time_t start = clock();
  while ((double)(clock() - start) / CLOCKS_PER_SEC < 1) {
    root->choose_child(mcts_board, empty_spots, total_empty);
  }

  if (root->children.empty()) {
    return Coord(-1, -1);
  }
  size_t most_trials = root->children[0]->total_tries;
  Coord ans = *root->children[0]->last_move;
  for (size_t i = 1; i < root->children.size(); i++) {
    if (root->children[i]->total_tries > most_trials) {
      most_trials = root->children[i]->total_tries;
      ans = *root->children[i]->last_move;
    }
  }
  return ans;
}
