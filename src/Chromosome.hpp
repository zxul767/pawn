#ifndef CHROMOSOME_H
#define CHROMOSOME_H

/*==============================================================================
  Encodes an evaluation function of the chess engine (its weights actually, since
  the values of the features depend on the board) for use in a genetic algorithm.

  The weights in the evaluation function characterize the playing nature of the
  engine, including strength but also style (e.g. aggresive vs cautious)
  ==============================================================================*/

#include <iostream>
#include <vector>
#include <string>

#include "Util.hpp"

namespace learning
{
class Chromosome
{
  public:
   Chromosome ();

   // Create an individual with random features
   Chromosome (uint n_features);
   Chromosome (const Chromosome& copy);
   Chromosome (const std::vector<int>& features);
   Chromosome (const std::vector<bool>& genes);

   enum Outcome {WIN, LOSS, DRAW, ERROR};

   void decode (std::vector<int>& features);
   void mutate (double probability);
   void reproduce (
       Chromosome& other, std::pair<Chromosome, Chromosome>& children);

   double get_fitness () const;
   bool get_gene (uint position) const;
   std::string get_genes () const;
   uint get_game_duration () const;
   Outcome get_result () const;
   uint how_many_features () const;
   double get_cumulative_probability () const;
   double get_selection_probability () const;
   int get_material_balance () const;
   void flip (uint position);
   void set (uint position);
   void unset (uint position);

   void set_fitness (double fitness);
   void set_selection_probability (double probability);
   void set_cumulative_probability (double probability);
   void set_game_duration (uint n_moves);
   void set_material_balance (int material);
   void set_result (Outcome result);

   bool operator == (const Chromosome& other) const;
   bool operator < (const Chromosome& other) const;
   Chromosome& operator = (const Chromosome& other);

   friend std::ostream& operator << (std::ostream& out, const Chromosome& other);

  private:
   static const uint BITS_PER_FEATURE = 9;

   std::vector<int> features;
   std::vector<bool> gene_string;

   double selection_probability;
   double cumulative_probability;
   double fitness;

   Outcome result;
   uint game_duration; // in number of moves
   int material_balance;

   void encode (const std::vector<int>& features);
   void initialize_data ();
};

} // namespace learning

#endif // CHROMOSOME
