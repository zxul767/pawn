#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

/*==============================================================================
  Represents a basic genetic algorithm to find good chess evaluation functions
  automatically.

  The idea is to think of evaluation functions as members of a population whose
  fitness is measured by having matches between pairs of them (keeping all other
  parts of the chess engine constant, e.g. the search algorithm)
  ==============================================================================*/

#include "Util.hpp"
#include "Chromosome.hpp"

namespace learning
{
class FitnessEvaluator;

class GeneticAlgorithm
{
  public:
   GeneticAlgorithm (
       uint population_size, uint n_iterations, double mutation_probability,
       FitnessEvaluator* fitness);

   static constexpr double SELECTION_PERCENTAGE = 0.65;

   void run ();
   void set_seed (Chromosome& seed);
   Chromosome get_fittest_member ();

  private:
   void initialize_population ();
   void evaluate_population ();
   void select_breeding_individuals (
       std::vector<Chromosome>& population, uint breeding_population_size,
       std::vector<Chromosome>& selection);

   void reduce_population (uint size);
   void add_on_population (const std::vector<Chromosome>& elements);
   bool converge ();

   void set_actual_fitness ();
   void set_material_fitness (std::vector<Chromosome>& sub_population);
   void set_duration_fitness (
       std::vector<Chromosome>& sub_population, Chromosome::Outcome population_type);

   uint population_size;
   uint n_iterations;
   double mutation_probability;
   FitnessEvaluator* fitness_evaluator;

   std::vector<Chromosome> population;
   Chromosome fittest_member;
};

} // namespace learning

#endif // GENETIC_ALGORITHM
