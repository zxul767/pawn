#ifndef BOARD_H
#define BOARD_H

/*==============================================================================
  Represents a non-static chess board configuration (i.e. where the pieces are,
  whether there is an en-passant capture possibility, what castling privileges
  remain, etc.) that also allows making and taking back moves, according to the
  usual rules of chess.
  ==============================================================================*/

#include <iostream>
#include <string>

#include "BoardTraits.hpp"
#include "Piece.hpp"
#include "Util.hpp"

namespace game_rules
{
class Move;

class IBoard
{
  public:
    IBoard();
    virtual ~IBoard();

    enum GameStatus
    {
        GAME_OVER,
        PENDING_GAME
    };

    enum Error
    {
        KING_LEFT_IN_CHECK,
        OPPONENTS_TURN,
        WRONG_MOVEMENT,
        GAME_FINISHED,
        NO_PIECE_IN_SQUARE,
        DRAW_BY_REPETITION,
        NO_ERROR
    };

    virtual void clear() = 0;
    virtual void reset() = 0;

    virtual bool load_game(const std::string &file) = 0;
    virtual bool save_game(const std::string &file) = 0;

    virtual bool add_piece(
        const std::string &location, Piece::Type piece, Piece::Player player) = 0;

    virtual bool add_piece(
        BoardSquare square, Piece::Type piece, Piece::Player player) = 0;

    virtual bool remove_piece(const std::string &location) = 0;
    virtual bool remove_piece(BoardSquare square) = 0;

    virtual Error make_move(Move &move, bool is_computer_move) = 0;
    virtual bool undo_move() = 0;

    virtual void label_move(Move &move) const = 0;

    virtual bool is_king_in_check() const = 0;
    virtual bitboard attacks_to(BoardSquare location, bool include_king) const = 0;
    virtual bitboard threats_to(BoardSquare location, Piece::Type type) const = 0;

    virtual bitboard get_moves(Piece::Type piece, BoardSquare square) const = 0;
    virtual bitboard get_all_pieces() const = 0;
    virtual bitboard get_pieces(Piece::Player player) const = 0;
    virtual bitboard get_pieces(Piece::Player player, Piece::Type piece) const = 0;

    virtual bool is_en_passant_on() const = 0;
    virtual bool can_castle(Piece::Player player, CastleSide side) const = 0;
    virtual bool is_castled(Piece::Player player, CastleSide side) const = 0;

    virtual bitboard get_en_passant_square() const = 0;
    virtual BoardSquare get_initial_king_square(Piece::Player player) const = 0;

    virtual Piece::Player get_piece_color(BoardSquare square) const = 0;
    virtual Piece::Player get_player_in_turn() const = 0;
    virtual Piece::Type get_piece(BoardSquare square) const = 0;
    virtual ullong get_hash_key() const = 0;
    virtual ullong get_hash_lock() const = 0;
    virtual uint get_move_number() const = 0;
    virtual ushort get_repetition_count() const = 0;

    virtual void set_game_status(GameStatus status) = 0;
    virtual void set_en_passant_capture_square(BoardSquare en_passant_capture_square) = 0;
    virtual void set_player_in_turn(Piece::Player player) = 0;
    virtual void set_castling_privilege(
        Piece::Player player, CastleSide side, bool value) = 0;

    static bool is_inside_board(int row, int col);
    static bool is_inside_board(uint row, uint col);
    static bool is_inside_board(uint square);

    friend std::ostream &operator<<(std::ostream &out, const IBoard &board);
};

inline BoardSquare &operator++(BoardSquare &square)
{
    return (square = BoardSquare(square + 1));
}

} // namespace game_rules

#endif // BOARD_H
