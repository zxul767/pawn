#ifndef IPOSITION_EVALUATOR_H
#define IPOSITION_EVALUATOR_H

#include "Piece.hpp"
#include <vector>

namespace rules
{
class IBoard;
}

namespace engine
{
class IPositionEvaluator
{
  public:
    virtual ~IPositionEvaluator()
    {
    }
    virtual int static_evaluation(const rules::IBoard *board) const = 0;

    virtual int evaluate_material(const rules::IBoard *board) const = 0;
    virtual int evaluate_mobility(const rules::IBoard *board) const = 0;
    virtual int evaluate_center_control(const rules::IBoard *board) const = 0;
    virtual int evaluate_king_safety(const rules::IBoard *board) const = 0;

    virtual void load_factor_weights(std::vector<int> &weights) = 0;
    virtual int get_piece_value(rules::Piece::Type piece_type) const = 0;
};

} // namespace engine

#endif // IPOSITION_EVALUATOR
