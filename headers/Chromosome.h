#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::pair;
using std::string;
using std::ostream;

class Board;

// Encodes a candidate solution as a fixed-length binary string
class Chromosome
{
 public:
   Chromosome ();

   // Create an individual with random features
   Chromosome (u_int n_features);

   Chromosome (const Chromosome& copy);
   Chromosome (const vector<int>& features);
   Chromosome (const vector<bool>& genes);

   enum Outcome {WIN, LOSS, DRAW, ERROR};

   void decode (vector<int>& features);
   void mutate (double probability);
   void reproduce (Chromosome& other, 
                   pair<Chromosome, Chromosome>& children);

   // ACCESSORS
   double  get_fitness () const;
   bool    get_gene (u_int position) const;
   string  get_genes () const;
   u_int   get_game_duration () const;
   Outcome get_result () const;
   u_int   how_many_features () const;
   double  get_cumulative_probability () const;
   double  get_selection_probability () const;
   int     get_material_balance () const;

   void flip   (u_int position);
   void set    (u_int position);
   void unset  (u_int position);

   // MUTATORS
   void  set_fitness (double fitness);
   void  set_selection_probability (double probability);
   void  set_cumulative_probability (double probability);
   void  set_game_duration (u_int n_moves);
   void  set_material_balance (int material);
   void  set_result (Outcome result);

   // OVERLOADED OPERATORS
   bool operator == (const Chromosome& other) const;
   bool operator < (const Chromosome& other) const;
   Chromosome& operator = (const Chromosome& other);

   friend ostream& operator << (ostream& out, const Chromosome& other);

 private:
   // CONSTANTS
   static const u_int BITS_PER_FEATURE = 9;

   // ATTRIBUTES
   vector<int>  features;
   vector<bool> gene_string;

   double  selection_probability;
   double  cumulative_probability;
   double  fitness;

   Outcome result;
   u_int   game_duration; // in number of moves
   int     material_balance;

   // METHODS
   void encode (const vector<int>& features);
   void initialize_data ();
};

#endif // CHROMOSOME
