#ifndef ALPHA_BETA_SEARCH_H
#define ALPHA_BETA_SEARCH_H

/*==============================================================================
  Implements the AI engine of the game (whose interface can be found in IEngine.h)
  using the alpha-beta search algorithm.
  ==============================================================================*/

#include "IEngine.hpp"
#include "Move.hpp"

#include <fstream>
#include <stack>
#include <vector>

namespace engine
{
using std::vector;

class IMoveGenerator;
class IPositionEvaluator;
class TranspositionTable;

class AlphaBetaSearch : public IEngine
{
  private:
    int search(int depth, int alpha, int beta);
    int quiescence_search(int depth, int alpha, int beta);
    int iterative_deepening_search(int depth, vector<rules::Move> &principal_variation);
    int evaluate_position(const rules::IBoard *board);

    bool build_principal_variation(
        rules::IBoard *, vector<rules::Move> &principal_variation);
    void load_factor_weights(vector<int> &weights);

    IPositionEvaluator *position_evaluator;
    IMoveGenerator *move_generator;
    TranspositionTable *transposition_table;
    rules::IBoard *board;

    GameResult result;
    rules::Move best_move;

  public:
    AlphaBetaSearch(IPositionEvaluator *, IMoveGenerator *);
    ~AlphaBetaSearch();

    GameResult get_best_move(int depth, rules::IBoard *, rules::Move &best_move);
};

} // namespace engine

#endif // ALPHA_BETA_SEARCH_H
