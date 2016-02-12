#ifndef POSITION_EVALUATOR_H
#define POSITION_EVALUATOR_H

#include <vector>
#include "Piece.hpp"

namespace game_rules { class Board; }

namespace game_engine
{
class PositionEvaluator
{
  public:
   virtual ~PositionEvaluator () {}
   virtual int static_evaluation (const game_rules::Board* board) const = 0;

   virtual int evaluate_material (const game_rules::Board* board) const = 0;
   virtual int evaluate_mobility (const game_rules::Board* board) const = 0;
   virtual int evaluate_center_control (const game_rules::Board* board) const = 0;
   virtual int evaluate_king_safety (const game_rules::Board* board) const = 0;

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual int get_piece_value (game_rules::Piece::Type piece_type) const = 0;
};

} // namespace game_engine

#endif // POSITION_EVALUATOR
