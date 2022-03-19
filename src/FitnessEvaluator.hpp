#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

/*==============================================================================
  Compares chess evaluation functions (encoded as chromosomes) in order to find
  the best ones when running a genetic algorithm
  ==============================================================================*/

#include "Util.hpp"
#include <string>

namespace engine
{
class IEngine;
class PositionEvaluator;
} // namespace engine

namespace rules
{
class IBoard;
}

namespace learning
{
class Chromosome;

class FitnessEvaluator
{
  public:
    FitnessEvaluator(engine::IEngine *);
    ~FitnessEvaluator();

    double evaluate(Chromosome &, Chromosome &);
    static const uint MAX_ALLOWED_MOVEMENTS = 70;

  private:
    rules::IBoard *board;
    engine::IEngine *chess_engine;
    engine::PositionEvaluator *evaluator;
};

} // namespace learning

#endif // FITNESS_FUNCTION_H
