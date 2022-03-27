#ifndef GAME_TRAITS_H
#define GAME_TRAITS_H

/*==============================================================================
  Defines constants that represent the basic traits of the game
  ==============================================================================*/

#include "type_aliases.hpp"

namespace rules
{
constexpr uint BOARD_SIZE = 8;
constexpr uint BOARD_SQUARES_COUNT = BOARD_SIZE * BOARD_SIZE;

constexpr uint PLAYERS_COUNT = 2;

constexpr uint PIECE_KINDS_COUNT = 6;
constexpr uint CASTLE_SIDES_COUNT = 2;
} // namespace rules

#endif // GAME_TRAITS_H
