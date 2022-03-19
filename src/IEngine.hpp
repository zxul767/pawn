#ifndef ICHESS_ENGINE_H
#define ICHESS_ENGINE_H

#include "SearchStats.hpp"
#include "Util.hpp"

namespace rules
{
class IBoard;
class Move;
} // namespace rules

namespace engine
{
using std::vector;

class IEngine
{
  public:
    enum GameResult
    {
        BLACK_MATES = -util::constants::INFINITUM,
        WHITE_MATES = +util::constants::INFINITUM,
        STALEMATE,
        DRAW_BY_REPETITION,
        NORMAL_EVALUATION,
        ERROR
    };

    static const int DRAW_VALUE = 0;
    static const int MATE_VALUE = -util::constants::INFINITUM;
    static const uint MAX_QUIESCENCE_DEPTH = 10;

    IEngine()
    {
    }
    virtual ~IEngine()
    {
    }

    virtual void load_factor_weights(vector<int> &weights) = 0;
    virtual GameResult get_best_move(
        int max_depth, rules::IBoard *, rules::Move &best_move) = 0;

    SearchStats statistics;
};

} // namespace engine

#endif // ICHESS_ENGINE_H
