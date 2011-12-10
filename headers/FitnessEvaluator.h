#ifndef FITNESS_EVALUATOR_H
#define FITNESS_EVALUATOR_H

#include <string>
#include "common.h"

class Search;
class Board;
class Chromosome;
class PositionEvaluator;

// Evaluate the fitness of an individual
class FitnessEvaluator
{
 public:
   FitnessEvaluator (Search* engine);
   ~FitnessEvaluator ();

   double evaluate  (Chromosome& a, Chromosome& b);
   static const uint MAX_ALLOWED_MOVEMENTS = 70;

 private:
   Search*             engine;
   Board*              board;
   PositionEvaluator*  evaluator;
};

#endif // FITNESS_FUNCTION_H
