// Idea from https://github.com/The-Ofek-Foundation/UltimateTicTacToe/blob/gh-pages/script.js

class MCTSNode {
public:
  MCTSNode* parent;
  bool turn;
  std::optional<Coord> last_move;

  int hits = 0, misses = 0, total_tries = 0;
  bool has_children = false;
  std::vector<MCTSNode*> children;
  int result = 10;
  int count_unexplored = 0;

  MCTSNode(MCTSNode* parent, bool turn,
           std::optional<Coord> last_move);

  void choose_child(Board& board, Subs& empty_spots, int& total_empty);
  void back_propogate(int simulation);

  ~MCTSNode();
};
