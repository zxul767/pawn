#ifndef BOARD_STATUS_H
#define BOARD_STATUS_H

#include "Move.h"

class BoardStatus
{
public:
  BoardStatus () { }

  BoardStatus (const Move& move,
               bitboard en_passant,
               bool can_castle_king,
               bool can_castle_queen,
               ullong hash_key,
               ullong hash_lock)
  {
    this->move = move;
    this->en_passant_capture_square = en_passant;
    this->can_castle_king = can_castle_king;
    this->can_castle_queen = can_castle_queen;
    this->hash_key = hash_key;
    this->hash_lock = hash_lock;
  }

  Move get_move () const
  {
    return move;
  }

  bitboard get_en_passant () const
  {
    return en_passant_capture_square;
  }

  bool get_king_castle () const
  {
    return can_castle_king;
  }

  bool get_queen_castle () const
  {
    return can_castle_queen;
  }

  ullong get_hash_key () const
  {
    return hash_key;
  }

  ullong get_hash_lock () const
  {
    return hash_lock;
  }

private:
  Move move;
  bitboard en_passant_capture_square;
  bool can_castle_king;
  bool can_castle_queen;
  ullong hash_key;
  ullong hash_lock;
};

#endif // BOARD_STATUS_H
