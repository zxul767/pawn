#ifndef POSITION_EVALUATOR_H
#define POSITION_EVALUATOR_H

#include "IPositionEvaluator.hpp"

#include <vector>

namespace rules
{
class Piece;
}

namespace engine
{
class PositionEvaluator : public IPositionEvaluator
{
  public:
    PositionEvaluator();
    int static_evaluation(const rules::IBoard *) const;
    int evaluate_material(const rules::IBoard *) const;
    int evaluate_mobility(const rules::IBoard *) const;
    int evaluate_center_control(const rules::IBoard *) const;
    int evaluate_king_safety(const rules::IBoard *) const;

    int get_piece_value(rules::Piece::Type) const;
    void load_factor_weights(std::vector<int> &weights);

  private:
    int material_value(bits::bitboard piece, rules::Piece::Type) const;
    int mobility_value(
        const rules::IBoard *, bits::bitboard piece, rules::Piece::Type) const;

    int center_control_value(
        const rules::IBoard *, bits::bitboard piece, rules::Piece::Type) const;

    int king_safety_value(const rules::IBoard *, rules::Piece::Player) const;
    int development_value(const rules::IBoard *, rules::Piece::Player) const;

    enum Factors
    {
        MATERIAL,
        MOBILITY,
        CENTER_CONTROL,
        KING_SAFETY
    };

    // Piece values are well-known, so we make them static here.
    std::vector<int> piece_value;
    std::vector<int> factor_weight;
};

} // namespace engine

#endif // POSITION_EVALUATOR_H
