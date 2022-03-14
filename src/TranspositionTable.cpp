#include "TranspositionTable.hpp"
#include "MaeBoard.hpp"

#include <iostream>

namespace game_engine
{
using game_rules::Move;
using std::unordered_map;

using TT = TranspositionTable;

std::map<uint, size_t> TranspositionTable::bits_to_table_size = {
    {16, 1'085'137}, {32, 2'159'363}, {64, 4'142'027}, {128, 8'888'437}, {256, 1'7311'163}};

size_t TranspositionTable::compute_table_size(uint size_in_bits)
{
    if (TT::bits_to_table_size.find(size_in_bits) == TT::bits_to_table_size.end())
        size_in_bits = TT::DEFAULT_SIZE_IN_BITS;
    return TT::bits_to_table_size[size_in_bits];
}

BoardMap TranspositionTable::create_entries_table(uint size_in_bits)
{
    size_t table_size = TranspositionTable::compute_table_size(size_in_bits);
    return BoardMap(table_size, BoardHasher(table_size));
}

TranspositionTable::TranspositionTable(uint size_in_bits) : entries{TT::create_entries_table(size_in_bits)}
{
    std::cerr << "transposition table size: " << this->entries.bucket_count() << std::endl;
}

bool TranspositionTable::add(const BoardKey &key, BoardEntry entry)
{
    auto iter = this->entries.find(key);
    if (iter != this->entries.end())
    {
        // Presumably, the deeper we searched, the more reliable is the information.
        if (entry.depth >= (*iter).second.depth)
        {
            (*iter).second = entry;
            return true;
        }
        return false;
    }
    this->entries.insert(std::pair<BoardKey, BoardEntry>(key, entry));
    return true;
}

bool TranspositionTable::get(const BoardKey &key, BoardEntry &entry) const
{
    auto iter = this->entries.find(key);
    if (iter != this->entries.end())
    {
        entry = (*iter).second;
        return true;
    }
    return false;
}

uint TranspositionTable::size() const
{
    return this->entries.size();
}

uint TranspositionTable::capacity() const
{
    return this->entries.bucket_count();
}

void TranspositionTable::clear()
{
    this->entries.clear();
}

} // namespace game_engine
