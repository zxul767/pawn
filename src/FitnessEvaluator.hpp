#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

/*==============================================================================
  Compares chess evaluation functions (encoded as chromosomes) in order to find
  the best ones when running a genetic algorithm
  ==============================================================================*/

#include <string>
#include "Util.hpp"

namespace game_engine { class IChessEngine; class PositionEvaluator; }
namespace game_rules { class Board; }

namespace learning
{
class Chromosome;

class FitnessEvaluator
{
  public:
   FitnessEvaluator (game_engine::IChessEngine*);
   ~FitnessEvaluator ();

   double evaluate (Chromosome&, Chromosome&);
   static const uint MAX_ALLOWED_MOVEMENTS = 70;

  private:
   game_rules::Board* board;
   game_engine::IChessEngine* chess_engine;
   game_engine::PositionEvaluator* evaluator;
};

} // namespace learning

#endif // FITNESS_FUNCTION_H
