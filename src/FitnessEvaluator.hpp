#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

/*==============================================================================
  Compares chess evaluation functions (encoded as chromosomes) in order to find
  the best ones when running a genetic algorithm
  ==============================================================================*/

#include "Util.hpp"
#include <string>

namespace game_engine
{
class IEngine;
class PositionEvaluator;
} // namespace game_engine
namespace game_rules
{
class IBoard;
}

namespace learning
{
class Chromosome;

class FitnessEvaluator
{
  public:
    FitnessEvaluator(game_engine::IEngine *);
    ~FitnessEvaluator();

    double evaluate(Chromosome &, Chromosome &);
    static const uint MAX_ALLOWED_MOVEMENTS = 70;

  private:
    game_rules::IBoard *board;
    game_engine::IEngine *chess_engine;
    game_engine::PositionEvaluator *evaluator;
};

} // namespace learning

#endif // FITNESS_FUNCTION_H
