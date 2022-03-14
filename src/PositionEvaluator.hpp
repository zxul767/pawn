#ifndef POSITION_EVALUATOR_H
#define POSITION_EVALUATOR_H

#include "IPositionEvaluator.hpp"
#include "Piece.hpp"

#include <vector>

namespace game_engine
{
class PositionEvaluator : public IPositionEvaluator
{
  public:
    PositionEvaluator();
    int static_evaluation(const game_rules::IBoard *) const;
    int evaluate_material(const game_rules::IBoard *) const;
    int evaluate_mobility(const game_rules::IBoard *) const;
    int evaluate_center_control(const game_rules::IBoard *) const;
    int evaluate_king_safety(const game_rules::IBoard *) const;

    int get_piece_value(game_rules::Piece::Type) const;
    void load_factor_weights(std::vector<int> &weights);

  private:
    int material_value(util::bitboard piece, game_rules::Piece::Type) const;
    int mobility_value(
        const game_rules::IBoard *, util::bitboard piece, game_rules::Piece::Type) const;

    int center_control_value(
        const game_rules::IBoard *, util::bitboard piece, game_rules::Piece::Type) const;

    int king_safety_value(const game_rules::IBoard *, game_rules::Piece::Player) const;
    int development_value(const game_rules::IBoard *, game_rules::Piece::Player) const;

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

} // namespace game_engine

#endif // POSITION_EVALUATOR_H
