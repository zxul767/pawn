#ifndef IPOSITION_EVALUATOR_H
#define IPOSITION_EVALUATOR_H

#include <vector>
#include "Piece.hpp"

namespace game_rules { class IBoard; }

namespace game_engine
{
class IPositionEvaluator
{
  public:
   virtual ~IPositionEvaluator () {}
   virtual int static_evaluation (const game_rules::IBoard* board) const = 0;

   virtual int evaluate_material (const game_rules::IBoard* board) const = 0;
   virtual int evaluate_mobility (const game_rules::IBoard* board) const = 0;
   virtual int evaluate_center_control (const game_rules::IBoard* board) const = 0;
   virtual int evaluate_king_safety (const game_rules::IBoard* board) const = 0;

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual int get_piece_value (game_rules::Piece::Type piece_type) const = 0;
};

} // namespace game_engine

#endif // IPOSITION_EVALUATOR
