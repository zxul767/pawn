#include "GeneticAlgorithm.h"
#include "FitnessEvaluator.h"
#include "Chromosome.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm (u_int             population_size, 
                                    u_int             n_iterations,
                                    double            mutation_probability,
                                    FitnessEvaluator* fitness_evaluator)
{
   std::cerr << "Initializing parameters for GA" << std::endl;
   this->population_size = population_size;
   this->n_iterations = n_iterations;
   this->mutation_probability = mutation_probability;
   this->fitness_evaluator = fitness_evaluator;
}

void
GeneticAlgorithm::run ()
{
   std::cerr << "Main loop of the GA is running now" << std::endl;
   
   initialize_population ();
   u_int k = (u_int)(population_size * SELECTION_PERCENTAGE);
   if (k & 1) ++k;

   /*vector<Chromosome::Outcome> out;
   out.push_back(Chromosome::WIN);
   out.push_back(Chromosome::LOSS);
   out.push_back(Chromosome::DRAW);
   */

   for (u_int i = 0; i < n_iterations; ++i)
   {
      std::cerr << "iteration # " << i << std::endl;

      evaluate_population ();
      
      // Testing with random values
      /*for (u_int i=0; i<population_size; ++i)
      {
         population[i].set_fitness (Util::random(3.0/9.0, 6.0/9.0));
         population[i].set_result (out[rand()%3]);
         population[i].set_material_balance(rand()%100);
         population[i].set_game_duration(rand()%50);
      }

      sort (population.begin(), population.end());
      for (u_int i=0; i<population_size;++i)
         std::cerr << population[i].get_fitness() << ',';
      std::cerr << '\n';

      set_actual_fitness ();

      sort (population.begin(), population.end());
      std::cerr << "\n new fitness\n";
      for (u_int i=0; i<population_size;++i)
         std::cerr << population[i].get_fitness() << ',';
      std::cerr << '\n';
      */

      set_actual_fitness ();

      vector<Chromosome> parents;
      parents.clear ();
      select_breeding_individuals (this->population,
                                   k, parents);

      std::cerr << "\nStarting reproduction ... ";

      // reproduce individuals to create offspring
      vector<Chromosome> offspring;
      offspring.clear ();
      u_int size = parents.size ();
      while (size >= 2)
      {
         pair<Chromosome, Chromosome> children;

         u_int selected = rand() % size;
         Chromosome individualA = parents[selected];
         parents[selected] = parents[size-1];
         parents.pop_back ();
         
         selected = rand() % (--size);
         Chromosome individualB = parents[selected];
         parents[selected] = parents[size-1];
         parents.pop_back ();

         individualA.reproduce (individualB, children);
         offspring.push_back (children.first);
         offspring.push_back (children.second);

         size --;
      }

      // eliminate the K worst individuals of the current population
      reduce_population (k);

      // add offspring to the current population in order to make up
      // a new one
      add_on_population (offspring);

      // randomly mutate some individuals with low probability
      u_int individuals = rand() % population_size;
      while (individuals)
      {
         population[rand() % population_size].mutate(mutation_probability);
         --individuals;
      }

      // If convergence criteria is reached, halt the algorithm
      if (converge ())
         break;

      //std::cin.get();
   }

   vector<int> ft;
   std::cerr << "\nThe fittest member in evolution is: \n";
   std::cerr << fittest_member << std::endl;
   fittest_member.decode(ft);
   for (u_int i=0; i<ft.size(); ++i)
      std::cerr << ft[i] << ',';
   std::cerr << std::endl;
}

bool
GeneticAlgorithm::converge ()
{
   return false;
}

void
GeneticAlgorithm::reduce_population (u_int size)
{
   std::cerr << "Reducing this population . . ." << '\n';

   sort (population.begin (), population.end ());

   for (u_int i=0; i<size; ++i)
      population.erase (population.begin());

   population_size = population.size ();
}

void
GeneticAlgorithm::add_on_population (const vector<Chromosome>& elements)
{
   u_int size = elements.size();

   for (u_int i=0; i<size; ++i)
      population.push_back(elements[i]);

   population_size = population.size ();
}

void
GeneticAlgorithm::set_seed (Chromosome& seed)
{
   std::cerr << "Setting seed for the GA" << std::endl;
   fittest_member = seed;
}

Chromosome
GeneticAlgorithm::get_fittest_member ()
{
   return Chromosome (fittest_member);
}

// Initialize the population with random values in their gene strings
// so that we can get a uniform sample of the solution space
void
GeneticAlgorithm::initialize_population ()
{
   std::cerr << "Setting initial population" << std::endl;

   u_int        n_features = fittest_member.how_many_features ();
   vector<int>  features;

   for (u_int i = 0; i < population_size; ++i)
   {
      // Random chromosome consisting of four features
      Chromosome individual (n_features);
      population.push_back (individual);

      individual.decode (features);
      for (u_int j = 0; j < features.size (); ++j)
         std::cerr << features[j] << '\t';
      std::cerr << std::endl;
   }
}

// Evaluate each individual chromosome against the best member so far
void
GeneticAlgorithm::evaluate_population ()
{
   std::cerr << "Evaluating fitness of each candidate solution" << std::endl;

   u_int best_index = 0;
   u_int average_game_duration = 0;

   for (u_int i = 0; i < population.size (); ++i)
   {
      std::cerr << "Evaluating individual #" << i << std::endl;
      double score = 
         fitness_evaluator->evaluate (fittest_member, population[i]);
      population[i].set_fitness (score);

      average_game_duration += population[i].get_game_duration ();

      // Update the fittest member so far
      if (fittest_member.get_fitness () < population[i].get_fitness ())
         best_index = i;
   }
   average_game_duration /= population.size ();

   std::cerr << "Average game duration: " << average_game_duration << std::endl;

   fittest_member = population[best_index];
}

// Use fitness proportionate selection here (aka roulette-wheel selection) to
// a second chance to not-so-good candidate solutions.
void
GeneticAlgorithm::select_breeding_individuals (vector<Chromosome>& population,
                                               u_int breeding_population_size,
                                               vector<Chromosome>& selection)
{
   if (breeding_population_size == 0)
      return;

   double population_fitness = 0.0;

   sort (population.begin (), population.end ());

   for (u_int i = 0; i < population.size (); ++i)
      population_fitness += population[i].get_fitness();

   for (u_int i = 0; i < population.size (); ++i)
   {
      if (population[i].get_fitness () == 0)
      {
         std::cerr << "this individual's got 0 as a fitness";
         abort();
      }
      population[i].set_selection_probability (population[i].get_fitness() / population_fitness);
   }

   double cumulative_probability = 0.0;
   for (u_int i = 0; i < population.size (); ++i)
   {
      cumulative_probability += population[i].get_selection_probability();
      population[i].set_cumulative_probability (cumulative_probability);
   }

   vector<Chromosome> aux = population;
   double r = Util::random (Util::EPSILON, cumulative_probability);
   
   u_int i = 0;
   for (; i < aux.size (); ++i)
   {
      if (r > aux[i].get_cumulative_probability() && i+1 < aux.size())
         continue;
      else  break;
   }

   selection.push_back (population[i]);
   aux.erase(aux.begin() + i);
   select_breeding_individuals (aux, breeding_population_size-1, 
                                selection);
}

void
GeneticAlgorithm::set_actual_fitness ()
{
   vector<Chromosome> winners, losers, mediocres;
   population_size = population.size();

   for (u_int i=0; i<population_size; ++i)
      switch(population[i].get_result())
      {
         case Chromosome::WIN:
            winners.push_back(population[i]); break;
         case Chromosome::LOSS:
            losers.push_back(population[i]); break;
         case Chromosome::DRAW:
            mediocres.push_back(population[i]); break;
         default:
            std::cerr<< "an unclasified individual";
            abort();
      }

   set_material_fitness(winners);
   set_material_fitness(losers);
   set_material_fitness(mediocres);
   set_duration_fitness(winners, Chromosome::WIN);
   set_duration_fitness(losers, Chromosome::LOSS);

   population.clear();
   u_int size = winners.size();
   for (u_int i=0; i<size; ++i)
      population.push_back(winners[i]);
   size = losers.size();
   for (u_int i=0; i<size; ++i)
      population.push_back(losers[i]);
   size = mediocres.size();
   for (u_int i=0; i<size; ++i)
      population.push_back(mediocres[i]);

   if (population_size != population.size())
   {
      std::cerr << "error in population size";
      abort();
   }
}

void
GeneticAlgorithm::set_material_fitness(vector<Chromosome>& sub_population)
{
   u_int size = sub_population.size();

   if (size == 0)
      return;

   vector<int> total;
   for (u_int i=0; i<size; ++i)
      total.push_back( sub_population[i].get_material_balance() );

   sort (total.begin(), total.end());
   int best_result = total.back();

   for (u_int i=0; i<size; ++i)
   {
      double bonus;

      if (sub_population[i].get_material_balance() != 0)
      {
         double portion = (double)best_result / 
            (double)sub_population[i].get_material_balance();

         if (portion > 1.0)
            portion = 1.0 / portion;

         bonus = portion * (2.0 / 9.0);

      } else bonus = 0.0;

      if (bonus > (2.0 / 9.0))
      {
         std::cerr << "\nerror computing bonus [" << bonus << "] on material balance\n";
         std::cerr << "best :" << best_result << ", material : " 
                   << sub_population[i].get_material_balance() << '\n';
         abort ();
      }

      sub_population[i].set_fitness (sub_population[i].get_fitness() + bonus);
   }
}

void
GeneticAlgorithm::set_duration_fitness(vector<Chromosome>& sub_population,
                                       Chromosome::Outcome type)
{
   u_int size = sub_population.size();

   if (size == 0)
      return;

   vector<u_int> total;
   for (u_int i=0; i<size; ++i)
      total.push_back( sub_population[i].get_game_duration() );

   sort (total.begin(), total.end());
   u_int best_result;

   if (type == Chromosome::WIN)
      best_result = total.front();
   else if (type == Chromosome::LOSS)
      best_result = total.back ();
   else 
   {
      std::cerr << "this is an invalid kind of sub-population";
      abort ();
   }

   for (u_int i=0; i<size; ++i)
   {
      if (sub_population[i].get_game_duration() == 0)
      {
         std::cerr <<"\nDivision by zero";
         abort();
      }

      double portion = (double)best_result / 
         (double)sub_population[i].get_game_duration();

      if (type == Chromosome::LOSS)
         portion = 1 / portion;

      double bonus = portion * (1.0 / 9.0);

      if (bonus > (1.0 / 9.0))
      {
         std::cerr << "\nerror computing bonus on duration\n";
         abort ();
      }

      sub_population[i].set_fitness (sub_population[i].get_fitness() + bonus);
   }
}
