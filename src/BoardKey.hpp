#ifndef BOARD_KEY_H
#define BOARD_KEY_H

#include "type_aliases.hpp"

namespace rules
{
struct BoardKey
{
    ullong hash_key;
    ullong hash_lock;
};
} // namespace rules

#endif // BOARD_KEY_H
