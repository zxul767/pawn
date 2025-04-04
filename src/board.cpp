#include "board.hpp"
#include "util.hpp"
#include <algorithm>

namespace rules
{
using bits::count_ones;
using bits::lsb_position;
using bits::merge;
using bits::merge_into;
using bits::turn_off_bit;
using bits::xor_into;
using rules::as_bitboard;
using Color = Piece::Color;
using PieceKind = Piece::Kind;

//--------------------------------------------------------------------
// Bitboard Constants
//--------------------------------------------------------------------
const bitboard Board::CENTER_BB =
    as_bitboard({Board::e4, Board::d4, Board::e5, Board::d5});

const bitboard Board::FILE_A_BB =
    as_bitboard(square_range(Board::a1, Board::a8, Board::SIZE));
const bitboard Board::FILE_B_BB =
    as_bitboard(square_range(Board::b1, Board::b8, Board::SIZE));
const bitboard Board::FILE_C_BB =
    as_bitboard(square_range(Board::c1, Board::c8, Board::SIZE));
const bitboard Board::FILE_D_BB =
    as_bitboard(square_range(Board::d1, Board::d8, Board::SIZE));
const bitboard Board::FILE_E_BB =
    as_bitboard(square_range(Board::e1, Board::e8, Board::SIZE));
const bitboard Board::FILE_F_BB =
    as_bitboard(square_range(Board::f1, Board::f8, Board::SIZE));
const bitboard Board::FILE_G_BB =
    as_bitboard(square_range(Board::g1, Board::g8, Board::SIZE));
const bitboard Board::FILE_H_BB =
    as_bitboard(square_range(Board::h1, Board::h8, Board::SIZE));
const bitboard Board::FILES_BB[Board::SIZE] = {
    Board::FILE_A_BB, Board::FILE_B_BB, Board::FILE_C_BB, Board::FILE_D_BB,
    Board::FILE_E_BB, Board::FILE_F_BB, Board::FILE_G_BB, Board::FILE_H_BB,
};

const bitboard Board::RANK_1_BB = as_bitboard(square_range(Board::a1, Board::h1, 1));
const bitboard Board::RANK_2_BB = as_bitboard(square_range(Board::a2, Board::h2, 1));
const bitboard Board::RANK_3_BB = as_bitboard(square_range(Board::a3, Board::h3, 1));
const bitboard Board::RANK_4_BB = as_bitboard(square_range(Board::a4, Board::h4, 1));
const bitboard Board::RANK_5_BB = as_bitboard(square_range(Board::a5, Board::h5, 1));
const bitboard Board::RANK_6_BB = as_bitboard(square_range(Board::a6, Board::h6, 1));
const bitboard Board::RANK_7_BB = as_bitboard(square_range(Board::a7, Board::h7, 1));
const bitboard Board::RANK_8_BB = as_bitboard(square_range(Board::a8, Board::h8, 1));

const bitboard Board::RANKS_BB[Board::SIZE] = {
    Board::RANK_1_BB, Board::RANK_2_BB, Board::RANK_3_BB, Board::RANK_4_BB,
    Board::RANK_5_BB, Board::RANK_6_BB, Board::RANK_7_BB, Board::RANK_8_BB,
};

std::array<bitboard, Board::SQUARES_COUNT> generate_squares_bitboards()
{
    std::array<bitboard, Board::SQUARES_COUNT> squares_bb;
    vector<bitboard> squares = as_bitboards(square_range(Board::a1, Board::h8, 1));
    std::copy(squares.begin(), squares.end(), squares_bb.begin());

    return squares_bb;
}

const std::array<bitboard, Board::SQUARES_COUNT> Board::SQUARES_BB =
    generate_squares_bitboards();

Board::Board()
{
    for (uint color = 0; color < Piece::COLORS_COUNT; ++color)
        this->_colors[color] = 0;

    for (uint piece = 0; piece < Piece::PIECES_COUNT; ++piece)
        this->_pieces[piece] = 0;

    for (uint square = 0; square < SQUARES_COUNT; ++square)
        this->_board[square] = Piece::NULL_PIECE;

    // TODO: add link to documentation on how zobrist hashing works.
    // TODO: research how to use modern C++ random number generators
    //       as this is likely to be an okay but not great generator.
    srand(ZOBRIST_RANDOM_SEED);
    this->_hash = HashKey{.key = 0, .lock = 0};
    for (uint color = 0; color < Piece::COLORS_COUNT; ++color)
        for (uint piece = 0; piece < Piece::PIECES_COUNT; ++piece)
            for (uint square = 0; square < SQUARES_COUNT; ++square)
                for (uint hash = 0; hash < HASHES_COUNT; hash++)
                    this->_zobrist[color][piece][square][hash] = util::random_ullong();
}

//--------------------------------------------------------------------
// Mutators
//--------------------------------------------------------------------

void Board::set(Square square, Piece piece)
{
    set(square, piece.color, piece.kind);
}

void Board::set(Square square, Color color, PieceKind kind)
{
    if (!is_empty(square))
    {
        throw occupied_square_error(square);
    }
    merge_into(this->_pieces[kind], Board::SQUARES_BB[square]);
    merge_into(this->_colors[color], Board::SQUARES_BB[square]);
    this->_board[square] = Piece{.kind = kind, .color = color};

    update_hashes(square, color, kind);
}

// TODO: should we return the piece that was removed?
void Board::remove_piece(Square square)
{
    Piece piece = this->_board[square];
    if (piece.is_null())
    {
        throw empty_square_error(square);
    }
    turn_off_bit(this->_colors[piece.color], square);
    turn_off_bit(this->_pieces[piece.kind], square);
    this->_board[square] = Piece::NULL_PIECE;

    update_hashes(square, piece.color, piece.kind);
}

// TODO: should we return the piece that was captured?
void Board::move(Square from, Square to)
{
    Piece piece = this->_board[from];
    if (piece.is_null())
    {
        throw empty_square_error(from);
    }
    Piece capture = this->_board[to];
    if (capture.color == piece.color)
    {
        throw occupied_square_error(to);
    }
    if (!capture.is_null())
    {
        remove_piece(to);
    }
    set(to, piece.color, piece.kind);
    remove_piece(from);
}

//--------------------------------------------------------------------
// Queries
//--------------------------------------------------------------------

bool Board::is_empty() const
{
    return count() == 0;
}

bool Board::is_empty(Square square) const
{
    return (pieces_bitboard() & Board::SQUARES_BB[square]) == 0;
}

const Piece &Board::operator[](Square square) const
{
    return this->_board[square];
}

bitboard Board::pieces_bitboard() const
{
    return merge(this->_colors[Color::WHITE], this->_colors[Color::BLACK]);
}

bitboard Board::pieces_bitboard(Color color) const
{
    return this->_colors[color];
}

bitboard Board::pieces_bitboard(PieceKind piece) const
{
    return this->_pieces[piece];
}

bitboard Board::pieces_bitboard(Color color, PieceKind piece) const
{
    return pieces_bitboard(color) & pieces_bitboard(piece);
}

vector<std::pair<Board::Square, Piece>> Board::pieces() const
{
    vector<std::pair<Square, Piece>> result;

    bitboard occupied = pieces_bitboard();
    while (occupied)
    {
        auto square = Square(bits::lsb_position(occupied));
        result.push_back(std::make_pair(square, this->_board[square]));
        turn_off_bit(occupied, square);
    }
    return result;
}

uint Board::count() const
{
    return count_ones(pieces_bitboard());
}

uint Board::count(PieceKind piece) const
{
    return count_ones(pieces_bitboard(piece));
}

uint Board::count(Color color) const
{
    return count_ones(pieces_bitboard(color));
}

uint Board::count(Color color, PieceKind piece) const
{
    return count_ones(pieces_bitboard(color) & pieces_bitboard(piece));
}

//--------------------------------------------------------------------
// Hashes (for transposition tables and position repetition counting)
//--------------------------------------------------------------------
Board::HashKey Board::hash() const
{
    return this->_hash;
}

void Board::update_hashes(Square square, Color color, PieceKind kind)
{
    xor_into(this->_hash.key, this->_zobrist[color][kind][square][HASH]);
    xor_into(this->_hash.lock, this->_zobrist[color][kind][square][HASH_LOCK]);
}

//--------------------------------------------------------------------
// Operator overloading
//--------------------------------------------------------------------

Board::Rank operator++(Board::Rank &rank)
{
    if (rank == Board::Rank::NULL_RANK)
        return rank;
    return (rank = static_cast<Board::Rank>(static_cast<int>(rank) + 1));
}

Board::File operator++(Board::File &file)
{
    if (file == Board::File::NULL_FILE)
        return file;
    return (file = static_cast<Board::File>(static_cast<int>(file) + 1));
}

Board::Square operator++(Board::Square &square)
{
    if (square == Board::Square::NULL_SQUARE)
        return square;
    return (square = static_cast<Board::Square>(static_cast<int>(square) + 1));
}

//--------------------------------------------------------------------
// Static constants generation helpers
//--------------------------------------------------------------------

bitboard as_bitboard(const vector<Board::Square> &squares)
{
    bitboard result = 0;
    for (auto square : squares)
        merge_into(result, bits::as_bitboard(square));
    return result;
}

vector<bitboard> as_bitboards(const vector<Board::Square> &squares)
{
    vector<bitboard> result;
    for (auto square : squares)
        result.push_back(bits::as_bitboard(square));
    return result;
}

vector<Board::Square> square_range(Board::Square start, Board::Square end, int by)
{
    vector<Board::Square> squares;
    Board::Square current = start;
    while (current < end)
    {
        squares.push_back(current);
        current =
            static_cast<Board::Square>(std::min(current + by, static_cast<int>(end)));
    }
    if (current == end)
        squares.push_back(end);
    return squares;
}

} // namespace rules
