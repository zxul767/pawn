#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

class FitnessEvaluator;

#include "Util.h"
#include "Chromosome.h"

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

#endif // GENETIC_ALGORITHM
