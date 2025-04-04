#ifndef BOARD_H_
#define BOARD_H_

#include "bitboard.hpp"
#include "piece2.hpp"
#include <array>
#include <exception>
#include <fmt/core.h>
#include <string>

namespace rules
{
using bits::bitboard;
using std::string;
using std::vector;

class Board
{
  public:
    static const uint SIZE = 8;
    static const uint SQUARES_COUNT = 64;

    static const uint HASHES_COUNT = 2;
    static const uint ZOBRIST_RANDOM_SEED = 42;
    enum HashKind
    {
        HASH = 0,
        HASH_LOCK = 1
    };
    struct HashKey
    {
        ullong key;
        ullong lock;
    };

    enum File
    {
        FILE_A = 0,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H,
        NULL_FILE
    };
    enum Rank
    {
        RANK_1 = 0,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8,
        NULL_RANK
    };

    enum Square
    {
        // clang-format off
        a1=0, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8,
        NULL_SQUARE
        // clang-format on
    };

    static const bitboard CENTER_BB;

    static const bitboard FILE_A_BB;
    static const bitboard FILE_B_BB;
    static const bitboard FILE_C_BB;
    static const bitboard FILE_D_BB;
    static const bitboard FILE_E_BB;
    static const bitboard FILE_F_BB;
    static const bitboard FILE_G_BB;
    static const bitboard FILE_H_BB;
    static const bitboard FILES_BB[SIZE];

    static const bitboard RANK_1_BB;
    static const bitboard RANK_2_BB;
    static const bitboard RANK_3_BB;
    static const bitboard RANK_4_BB;
    static const bitboard RANK_5_BB;
    static const bitboard RANK_6_BB;
    static const bitboard RANK_7_BB;
    static const bitboard RANK_8_BB;
    static const bitboard RANKS_BB[SIZE];

    static const std::array<bitboard, SQUARES_COUNT> SQUARES_BB;

    inline static Square square(File file, Rank rank)
    {
        return Square(rank * Board::SIZE + file);
    }

    Board();

    //--------------------------------------------------------------------
    // Mutators
    //--------------------------------------------------------------------
    void set(Square, Piece);
    void set(Square, Piece::Color, Piece::Kind);

    void remove_piece(Square);
    void move(Square from, Square to);

    //--------------------------------------------------------------------
    // Queries
    //--------------------------------------------------------------------
    bool is_empty() const;
    bool is_empty(Square) const;

    const Piece &operator[](Square) const;
    bitboard pieces_bitboard() const;
    bitboard pieces_bitboard(Piece::Color, Piece::Kind) const;
    bitboard pieces_bitboard(Piece::Color) const;
    bitboard pieces_bitboard(Piece::Kind) const;
    vector<std::pair<Square, Piece>> pieces() const;

    uint count() const;
    uint count(Piece::Color, Piece::Kind) const;
    uint count(Piece::Color) const;
    uint count(Piece::Kind) const;

    /*
     * Hashing (for transposition tables and position repetition counting)
     */
    HashKey hash() const;

  private:
    // A bitboard representation is very useful for efficient move generation
    // and other kinds of queries (e.g., is the king's vicinity under attack?)
    bitboard _colors[Piece::COLORS_COUNT];
    bitboard _pieces[Piece::PIECES_COUNT];
    // This is not strictly necessary as the bitboards above are enough to
    // calculate the piece and color in any square, but it's significantly
    // slower to do it that way.
    Piece _board[SQUARES_COUNT];

    /*
     * Hashing (for transposition tables and position repetition counting)
     */
    HashKey _hash;
    // TODO: should this be static data?
    ullong _zobrist[Piece::COLORS_COUNT][Piece::PIECES_COUNT][SQUARES_COUNT]
                   [HASHES_COUNT];
    void update_hashes(Square square, Piece::Color color, Piece::Kind kind);
};

class empty_square_error : public std::exception
{
  public:
    empty_square_error(Board::Square square)
    {
        this->message = fmt::format("Cannot take piece from an empty square: {}", square);
    }

    virtual const char *what() const throw()
    {
        return this->message.c_str();
    }

  private:
    string message;
};

class occupied_square_error : public std::exception
{
  public:
    occupied_square_error(Board::Square square)
    {
        this->message = fmt::format("Cannot set piece on an occupied square: {}", square);
    }

    virtual const char *what() const throw()
    {
        return this->message.c_str();
    }

  private:
    string message;
};

//--------------------------------------------------------------------
// Operator overloading
//--------------------------------------------------------------------
Board::Square operator++(Board::Square &square);
Board::Rank operator++(Board::Rank &rank);
Board::File operator++(Board::File &file);

//--------------------------------------------------------------------
// Static constants generation helpers
//--------------------------------------------------------------------

// Even though we have bits::as_bitboard (which works on `uint`), we
// want to have a version that takes `Board::Square` so we can enforce
// the usage of symbolic constants (i.e., Board::a1)
inline bitboard as_bitboard(Board::Square square)
{
    return bits::to_bitboard[square];
}

bitboard as_bitboard(const vector<Board::Square> &squares);
vector<bitboard> as_bitboards(const vector<Board::Square> &squares);

vector<Board::Square> square_range(Board::Square start, Board::Square end, int offset);

} // namespace rules

#endif // BOARD_H_
