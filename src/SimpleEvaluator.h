#ifndef SIMPLE_EVALUATOR_H
#define SIMPLE_EVALUATOR_H

#include "PositionEvaluator.h"
#include "Piece.h"

#include <vector>

class SimpleEvaluator : public PositionEvaluator
{
 public:
   SimpleEvaluator ();
   int static_evaluation (const Board* board) const;
   int evaluate_material (const Board* board) const;
   int evaluate_mobility (const Board* board) const;
   int evaluate_center_control (const Board* board) const;
   int evaluate_king_safety (const Board* board) const;

   int get_piece_value (Piece::Type piece_type) const;
   void load_factor_weights (std::vector<int>& weights);

 private:
   int material_value (bitboard piece, Piece::Type piece_type) const;
   int mobility_value (
       const Board* board, bitboard piece, Piece::Type piece_type) const;

   int center_control_value (
       const Board* board, bitboard piece, Piece::Type piece_type) const;

   int king_safety_value (const Board* board, Piece::Player player) const;
   int development_value (const Board* board, Piece::Player turn) const;

   enum Factors {
      MATERIAL,
      MOBILITY,
      CENTER_CONTROL,
      KING_SAFETY
   };

   // Piece values are well-known, so we make them static here.
   std::vector<int> piece_value;
   std::vector<int> factor_weight;
};

#endif // SIMPLE_EVALUATOR_H
