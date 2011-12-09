#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

class FitnessEvaluator;

#include "common.h"
#include "Chromosome.h"

class GeneticAlgorithm
{
 public:
   GeneticAlgorithm (u_int             population_size, 
                     u_int             n_iterations,
                     double            mutation_probability,
                     FitnessEvaluator* fitness);

   static const double SELECTION_PERCENTAGE = 0.65;

   void       run ();
   void       set_seed (Chromosome& seed);
   Chromosome get_fittest_member ();

 private:
   void initialize_population ();
   void evaluate_population ();
   void select_breeding_individuals (vector<Chromosome>& population,
                                     u_int breeding_population_size, 
                                     vector<Chromosome>& selection);
   void reduce_population (u_int size);
   void add_on_population (const vector<Chromosome>& elements);
   bool converge ();

   void set_actual_fitness ();
   void set_material_fitness  (vector<Chromosome>& sub_population);
   void set_duration_fitness  (vector<Chromosome>& sub_population,
                               Chromosome::Outcome population_type);

   u_int              population_size;
   u_int              n_iterations;
   double             mutation_probability;
   FitnessEvaluator*  fitness_evaluator;

   vector<Chromosome> population;
   Chromosome         fittest_member;
};

#endif // GENETIC_ALGORITHM
