#ifndef POSITION_EVALUATOR_H
#define POSITION_EVALUATOR_H

#include <vector>
#include "Piece.h"

class Board;

class PositionEvaluator
{
 public:
   virtual ~PositionEvaluator () {}
   virtual int  static_evaluation (const Board* board) const = 0;

   virtual int evaluate_material (const Board* board) const = 0;
   virtual int evaluate_mobility (const Board* board) const = 0;
   virtual int evaluate_center_control (const Board* board) const = 0;
   virtual int evaluate_king_safety (const Board* board) const = 0;

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual int  get_piece_value (Piece::Type piece_type) const = 0;
};

#endif // POSITION_EVALUATOR
