#include "Chromosome.hpp"
#include "Board.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

using std::vector;

Chromosome::Chromosome ()
{
   initialize_data ();
}

// Create a random chromosome
Chromosome::Chromosome (uint n_features)
{
   initialize_data ();

   for (uint i = 0; i < n_features * BITS_PER_FEATURE; ++i)
   {
      this->gene_string.push_back ((rand () & 1) ? true : false);
   }
}

Chromosome::Chromosome (const Chromosome& copy)
{
   this->selection_probability = copy.selection_probability;
   this->cumulative_probability = copy.cumulative_probability;
   this->fitness = copy.fitness;

   this->game_duration = copy.game_duration;
   this->material_balance = copy.material_balance;
   this->result = copy.result;

   for (uint i = 0; i < copy.gene_string.size (); ++i)
      this->gene_string.push_back (copy.get_gene (i));
}

Chromosome::Chromosome (const vector<int>& features)
{
   initialize_data ();
   encode (features);
}

Chromosome::Chromosome (const vector<bool>& genes)
{
   initialize_data ();
   gene_string = genes;
   decode (features);
}

void
Chromosome::initialize_data ()
{
   this->fitness = 0.0;
   this->selection_probability = 0.0;
   this->cumulative_probability = 0.0;

   this->result = ERROR;
   this->game_duration = 0;
   this->material_balance = 0;
}

void
Chromosome::decode (vector<int>& features)
{
   int feature_value = 0;

   features.clear ();
   for (uint i = 0, j = 0; i < this->gene_string.size (); ++i, ++j)
   {
      if (this->gene_string[i])
      {
         feature_value |= (1 << j);
      }

	   if (i % BITS_PER_FEATURE == (BITS_PER_FEATURE-1))
      {
         features.push_back (feature_value);
         feature_value = 0;
         j = 0;
      }
   }
}

// Mutate random genes in the chromosome with the given PROBABILITY
void
Chromosome::mutate (double probability)
{
   for (uint i = 1; i <= this->features.size(); ++i)
   {
      if ((RAND_MAX * probability) <= rand ())
      {
         uint position = rand() % this->gene_string.size();
         flip (position);
      }
   }
}

// Combine two chromosomes to produce two offspring
void
Chromosome::reproduce (
    Chromosome& other, std::pair<Chromosome, Chromosome>& children)
{
   unsigned short half = this->gene_string.size() / 2;

   vector<bool> first_born;
   vector<bool> second_born;
   uint i = 0;


   for (uint k = 0; k < this->gene_string.size(); ++k)
   {
      first_born.push_back(false);
      second_born.push_back(false);
   }

   for (; i < half; ++i)
   {
      first_born[i] = this->gene_string[i];
      second_born[i] = other.gene_string[i];

      first_born[half+i] = other.gene_string[half+i];
      second_born[half+i]= this->gene_string[half+i];
   }

   if (this->gene_string.size() & 1)
   {
      first_born[i] = other.gene_string[i];
      second_born[i] = this->gene_string[i];
   }

   children.first = Chromosome (first_born);
   children.second= Chromosome (second_born);
}

void
Chromosome::flip (uint position)
{
   if (position < this->gene_string.size ())
      this->gene_string[position] = !this->gene_string[position];
}

void
Chromosome::set (uint position)
{
   if (position < this->gene_string.size ())
      this->gene_string[position] = true;
}

void
Chromosome::encode (const vector<int>& features)
{
   uint locus = 0;

   for (uint i = 0; i < features.size (); ++i)
   {
      int feature_value = features[i];
      for (uint j = 0; j < BITS_PER_FEATURE; ++j, ++locus)
      {
         if (feature_value & (1 << j))
            this->gene_string.push_back (true);
         else
            this->gene_string.push_back (false);
      }
   }
}

uint
Chromosome::how_many_features () const
{
   return this->gene_string.size () / BITS_PER_FEATURE;
}

double
Chromosome::get_fitness () const
{
   return this->fitness;
}

bool
Chromosome::get_gene (uint position) const
{
   if (position < this->gene_string.size ())
      return this->gene_string[position];
   else
   {
      return false;
   }
}

std::string
Chromosome::get_genes () const
{
   std::string genes = "";
   for (uint i = 0; i < this->gene_string.size (); ++i)
         genes += (this->gene_string[i] ? "1" : "0");
   return genes;
}

uint
Chromosome::get_game_duration () const
{
   return this->game_duration;
}

double
Chromosome::get_cumulative_probability () const
{
   return this->cumulative_probability;
}

double
Chromosome::get_selection_probability () const
{
   return this->selection_probability;
}

int
Chromosome::get_material_balance() const
{
   return this->material_balance;
}

Chromosome::Outcome
Chromosome::get_result () const
{
   return this->result;
}

void
Chromosome::set_fitness (double fitness)
{
   this->fitness = fitness;
}

void
Chromosome::set_selection_probability (double probability)
{
   this->selection_probability = probability;
}

void
Chromosome::set_cumulative_probability (double probability)
{
   this->cumulative_probability = probability;
}

void
Chromosome::set_game_duration (uint game_duration)
{
   this->game_duration = game_duration;
}

void
Chromosome::set_material_balance (int material)
{
   this->material_balance = material;
}

void
Chromosome::set_result (Chromosome::Outcome result)
{
   this->result = result;
}

bool
Chromosome::operator == (const Chromosome& other) const
{
   if (this->gene_string.size () != other.gene_string.size ())
      return false;

   for (uint i = 0, n = other.gene_string.size (); i < n; ++i)
      if (this->gene_string[i] != other.gene_string[i])
         return false;

   return true;
}

bool
Chromosome::operator < (const Chromosome& other) const
{
   if (fabs (this->fitness - other.fitness) < Util::EPSILON)
      return false;

   return this->fitness < other.fitness;
}

Chromosome&
Chromosome::operator = (const Chromosome& other)
{
   this->fitness = other.fitness;
   this->selection_probability = other.selection_probability;
   this->cumulative_probability = other.cumulative_probability;
   this->result = other.result;
   this->game_duration = other.game_duration;
   this->material_balance = other.material_balance;

   this->gene_string.clear ();
   for (uint i = 0; i < other.gene_string.size (); ++i)
      this->gene_string.push_back (other.get_gene (i));

   return *this;
}

std::ostream&
operator << (std::ostream& out, const Chromosome& other)
{
   out << "Fitness:\t" << other.fitness << '\n';
   out << "Game result:\t" << other.result << '\n';
   out << "Game duration:\t" << other.game_duration << '\n';
   for (uint i = 0; i < other.gene_string.size (); ++i)
      out << (other.gene_string[i] ? '1' : '0');
   out << "\n\n";

   return out;
}
