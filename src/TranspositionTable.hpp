#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

/*==============================================================================
  Implements a transposition table, used to improve performance of search
  algorithms such as iterative deepening search
  ==============================================================================*/

#include <map>
#include <unordered_map>

#include "BoardKey.hpp"
#include "MaeBoard.hpp"
#include "Move.hpp"
#include "Util.hpp"

namespace engine
{
using rules::BoardKey;

enum Accuracy
{
    EXACT,
    UPPER_BOUND,
    LOWER_BOUND,
    UNKNOWN
};

struct BoardEntry
{
    int score;
    int depth;
    Accuracy accuracy;
    rules::Move best_move;
};

// This maps boards to a bucket in the hash table
class BoardHasher
{
  public:
    BoardHasher(size_t size_in_bits) : size_in_bits{size_in_bits}
    {
    }

    size_t operator()(const BoardKey &key) const
    {
        return (size_t)(key.hash_key % size_in_bits);
    }

  private:
    size_t size_in_bits;
};

// This helps differentiate between Boards that mapped to the same bucket
class BoardHashComparer
{
  public:
    bool operator()(const BoardKey &key, const BoardKey &other_key) const
    {
        return key.hash_lock == other_key.hash_lock;
    }
};

using BoardMap = std::unordered_map<BoardKey, BoardEntry, BoardHasher, BoardHashComparer>;

class TranspositionTable
{
  public:
    TranspositionTable(uint size_in_bits);

    bool add(const BoardKey &, BoardEntry);
    void clear();

    bool get(const BoardKey &, BoardEntry &) const;
    uint size() const;
    uint capacity() const;

    static const uint DEFAULT_SIZE_IN_BITS = 64;
    static std::map<uint, size_t> bits_to_table_size;

    static size_t compute_table_size(uint size_in_bits);
    static BoardMap create_entries_table(uint size_in_bits);

  private:
    BoardMap entries;
};

} // namespace engine

#endif // HASH_TRANSPOSITION_TABLE_H
