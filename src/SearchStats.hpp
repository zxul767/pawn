#ifndef SEARCH_STATS_H_
#define SEARCH_STATS_H_

#include "Util.hpp"
#include <iostream>
#include <locale>
#include <cmath>

namespace game_engine
{
  using std::cerr;
  using std::endl;

  struct separate_thousands : std::numpunct<char> {
    char_type do_thousands_sep() const override { return ','; }  // separate with commas
    string_type do_grouping() const override { return "\3"; } // groups of 3 digit
  };

  class SearchStats {
  public:
    SearchStats() {
      // add thousands separators to numbers to make them easier to read
      auto thousands = std::make_unique<separate_thousands>();
      std::cerr.imbue(std::locale(std::cerr.getloc(), thousands.release()));
    }

    void print () const {
      cerr << "-------------------------------------------------------" << endl;
      cerr << "Nodes evaluated: " << this->nodes_evaluated << endl;
      cerr << "\tLeaf nodes: " << this->leaf_nodes << endl;
      cerr << "\tInternal nodes: " << this->internal_nodes << endl;
      cerr << "Average branching factor: " << int(round(this->average_branching_factor)) << endl;
      cerr << "Transposition table hits: " << this->cache_hits << endl;
      cerr << "AlphaBeta cutoffs: " << this->alpha_beta_cutoffs << endl;
      cerr << "-------------------------------------------------------" << endl;
    }

    void add_branching_factor(uint factor) {
      // https://math.stackexchange.com/questions/106700/incremental-averaging
      double mean = this->average_branching_factor;
      mean += (factor - mean) / this->internal_nodes;
      this->average_branching_factor = mean;
    }

    void reset ()
    {
      cerr << "Resetting search statistics..." << endl;
      this->cache_hits = 0;
      this->leaf_nodes = 0;
      this->internal_nodes = 0;
      this->nodes_evaluated = 0;
      this->average_branching_factor = 0.0;
      this->alpha_beta_cutoffs = 0;
    }

    uint cache_hits = 0;
    uint leaf_nodes = 0;
    uint internal_nodes = 0;
    uint nodes_evaluated = 0;
    double average_branching_factor = 0.0;
    uint alpha_beta_cutoffs = 0;
  };
}

#endif // SEARCH_STATS_H_
