#ifndef BOARD_CONFIGURATION_TRACKER_H
#define BOARD_CONFIGURATION_TRACKER_H

/*==============================================================================
  Holds a chess board configuration key to be used transposition table lookups
  ==============================================================================*/

#include "BoardKey.hpp"
#include <unordered_map>

namespace rules
{
class BoardConfigurationTracker
{
  public:
    BoardConfigurationTracker()
    {
    }

    bool add_record(const BoardKey &key, ushort &);
    bool decrease_record(const BoardKey &key);
    ushort get_repetitions(const BoardKey &key) const;
    void reset();

    static const std::size_t size = 4142027;

  private:
    class board_hasher
    {
      public:
        std::size_t operator()(const BoardKey &board) const
        {
            return (std::size_t)(board.hash_key % size);
        }
    };
    class board_comparer
    {
      public:
        bool operator()(const BoardKey &board, const BoardKey &other) const
        {
            return board.hash_lock == other.hash_lock;
        }
    };
    std::unordered_map<BoardKey, ushort, board_hasher, board_comparer> tracker;
};

} // namespace rules

#endif // BOARD_CONFIGURATION_TRACKER_H
