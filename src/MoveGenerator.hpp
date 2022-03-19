#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "IMoveGenerator.hpp"

namespace engine
{
using std::vector;

class MoveGenerator : public IMoveGenerator
{
  public:
    /*======================================================================
      Return a vector of legal moves from the current BOARD configuration
      =====================================================================*/
    bool generate_moves(
        rules::IBoard *, vector<rules::Move> &moves, ushort kind_of_moves);
    bool generate_moves(rules::IBoard *, vector<rules::Move> &moves);
    bool generate_en_prise_evations(rules::IBoard *, vector<rules::Move> &moves);

    ~MoveGenerator()
    {
    }
};

} // namespace engine

#endif // MOVE_GENERATOR_H
