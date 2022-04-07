#include "../../catch.hpp"
#include "bitboard.hpp"
#include "board.hpp"
#include "piece2.hpp"
#include <iostream>

namespace
{
using bits::bitboard;
using bits::msb_position;

using rules::as_bitboard;
using rules::Board;
using rules::Piece;
using Color = rules::Piece::Color;
using PieceKind = rules::Piece::Kind;

TEST_CASE("::game::Board")
{
    Board board;

    SECTION("Can get bitboard of specific colored piece", "[board][bits]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);
        REQUIRE(
            board.pieces_bitboard(Color::WHITE, Piece::KNIGHT) == as_bitboard(Board::a1));
    }

    SECTION("Can get bitboard of a kind of piece", "[board][bits]")
    {
        board.set(Board::d1, Piece::WHITE_QUEEN);
        board.set(Board::e1, Piece::WHITE_KING);
        REQUIRE(
            board.pieces_bitboard(Color::WHITE) == as_bitboard({Board::e1, Board::d1}));
    }

    SECTION("Can get bitboard of colored pieces", "[board][bits]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);
        board.set(Board::h1, Piece::WHITE_ROOK);
        board.set(Board::d5, Piece::BLACK_PAWN);

        bitboard white = as_bitboard({Board::a1, Board::h1});
        bitboard black = as_bitboard({Board::d5});
        REQUIRE(white == board.pieces_bitboard(Color::WHITE));
        REQUIRE(black == board.pieces_bitboard(Color::BLACK));
    }

    SECTION("Rank and file bitboards should intersect in one square", "[board][bits]")
    {
        REQUIRE((Board::FILE_A_BB & Board::RANK_1_BB) == as_bitboard(Board::a1));
        REQUIRE((Board::FILE_D_BB & Board::RANK_5_BB) == as_bitboard(Board::d5));
        REQUIRE((Board::FILE_H_BB & Board::RANK_8_BB) == as_bitboard(Board::h8));

        for (auto file = Board::FILE_A; file <= Board::FILE_H; ++file)
            for (auto rank = Board::RANK_1; rank <= Board::RANK_8; ++rank)
            {
                bitboard intersection = Board::FILES_BB[file] & Board::RANKS_BB[rank];
                REQUIRE(intersection == Board::SQUARES_BB[Board::square(file, rank)]);
            }
    }

    SECTION("Center bitboard should intersect D-E files and 4-5 ranks", "[board][bits]")
    {
        REQUIRE((Board::CENTER_BB & Board::RANK_4_BB) != 0);
        REQUIRE((Board::CENTER_BB & Board::RANK_5_BB) != 0);
        REQUIRE((Board::CENTER_BB & Board::FILE_D_BB) != 0);
        REQUIRE((Board::CENTER_BB & Board::FILE_E_BB) != 0);
    }

    SECTION("Can add a piece to the board", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);

        REQUIRE(board.count(Color::WHITE, Piece::KNIGHT) == 1);
        REQUIRE(board[Board::a1] == Piece::WHITE_KNIGHT);
    }

    SECTION("Should get an empty board after removing all pieces", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);
        board.set(Board::h1, Piece::WHITE_ROOK);

        REQUIRE(!board.is_empty());
        board.remove_piece(Board::a1);
        board.remove_piece(Board::h1);
        REQUIRE(board.is_empty());
    }

    SECTION("Can add and remove multiple pieces to the board", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);
        board.set(Board::h1, Piece::WHITE_ROOK);
        board.remove_piece(Board::a1);

        REQUIRE(board.count() == 1);
        REQUIRE(board.count(Color::WHITE) == 1);
        REQUIRE(board.count(Color::BLACK) == 0);
        REQUIRE(board.count(Piece::ROOK) == 1);
        REQUIRE(board.count(Piece::KNIGHT) == 0);
        REQUIRE(board[Board::h1] == Piece::WHITE_ROOK);
    }

    SECTION("Can add multiple pieces to board", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_ROOK);
        board.set(Board::c1, Piece::WHITE_BISHOP);
        board.set(Board::b1, Piece::BLACK_KNIGHT);
        board.set(Board::h8, Piece::BLACK_ROOK);
        board.set(Board::a8, Piece::BLACK_ROOK);

        REQUIRE(board.count() == 5);
        REQUIRE(board.count(Color::WHITE) == 2);
        REQUIRE(board.count(Color::BLACK) == 3);

        REQUIRE(board.count(Piece::KNIGHT) == 1);
        REQUIRE(board.count(Color::WHITE, Piece::KNIGHT) == 0);
        REQUIRE(board.count(Color::BLACK, Piece::KNIGHT) == 1);

        REQUIRE(board.count(Piece::ROOK) == 3);
        REQUIRE(board.count(Color::WHITE, Piece::ROOK) == 1);
        REQUIRE(board.count(Color::BLACK, Piece::ROOK) == 2);

        REQUIRE(board.count(Piece::BISHOP) == 1);
        REQUIRE(board.count(Color::WHITE, Piece::BISHOP) == 1);
        REQUIRE(board.count(Color::BLACK, Piece::BISHOP) == 0);
    }

    SECTION("Can move a knight all around the board", "[board]")
    {
        board.set(Board::g1, Piece::WHITE_KNIGHT);

        board.move(Board::g1, Board::f3);
        board.move(Board::f3, Board::g5);
        board.move(Board::g5, Board::f7);

        REQUIRE(board.count() == 1);
        REQUIRE(board[Board::f7] == Piece::WHITE_KNIGHT);
    }

    SECTION("Can index into board with brackets", "[board]")
    {
        board.set(Board::g1, Piece::WHITE_KNIGHT);

        REQUIRE(board[Board::a1].is_null());
        REQUIRE(board[Board::g1] == Piece::WHITE_KNIGHT);
    }

    SECTION("Can tell if the board is empty", "[board]")
    {
        REQUIRE(board.is_empty());
        board.set(Board::a1, Piece::BLACK_PAWN);

        REQUIRE(board.is_empty() == false);
    }

    SECTION("Can tell if a square is empty", "[board]")
    {
        board.set(Board::a1, Piece::BLACK_PAWN);
        REQUIRE(board.is_empty(Board::a1) == false);
    }

    SECTION("Can add and remove each piece on the board", "[board][exhaustive]")
    {
        for (auto square = Board::a1; square <= Board::h8; ++square)
        {
            for (auto color = Color::WHITE; color <= Color::BLACK; ++color)
            {
                for (auto piece = PieceKind::PAWN; piece <= PieceKind::KING; ++piece)
                {
                    REQUIRE(board.is_empty());

                    board.set(square, color, piece);

                    REQUIRE(board.is_empty() == false);
                    REQUIRE(board[square].kind == piece);
                    REQUIRE(board[square].color == color);

                    board.remove_piece(square);
                }
            }
        }
    }

    SECTION("Can get list of pieces on board", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_ROOK);
        board.set(Board::h8, Piece::BLACK_ROOK);

        auto pieces = board.pieces();

        REQUIRE(pieces.size() == 2);
        REQUIRE(pieces[0].first == Board::a1);
        REQUIRE(pieces[0].second == Piece::WHITE_ROOK);
        REQUIRE(pieces[1].first == Board::h8);
        REQUIRE(pieces[1].second == Piece::BLACK_ROOK);
    }

    SECTION("Can capture piece", "[board]")
    {
        board.set(Board::g1, Piece::WHITE_KNIGHT);
        board.set(Board::f3, Piece::BLACK_QUEEN);

        board.move(Board::g1, Board::f3);
        REQUIRE(board.count() == 1);
        REQUIRE(board[Board::f3] == Piece::WHITE_KNIGHT);
    }

    SECTION("Should throw when removing a non-existing piece", "[board]")
    {
        REQUIRE_THROWS_AS(board.remove_piece(Board::a1), rules::empty_square_error &);
    }

    SECTION("Should throw when setting a piece on an occupied square", "[board]")
    {
        board.set(Board::a1, Piece::BLACK_PAWN);
        REQUIRE_THROWS_AS(
            board.set(Board::a1, Piece::WHITE_PAWN), rules::occupied_square_error &);
    }

    SECTION("Should throw when moving to a same-color occupied square", "[board]")
    {
        board.set(Board::a1, Piece::BLACK_ROOK);
        board.set(Board::d1, Piece::BLACK_ROOK);

        REQUIRE_THROWS_AS(
            board.move(Board::a1, Board::d1), rules::occupied_square_error &);
    }

    SECTION("Should throw when moving from empty square", "[board]")
    {
        REQUIRE_THROWS_AS(board.move(Board::a1, Board::a8), rules::empty_square_error &);
    }

    SECTION("Should get a zero hash on an empty board", "[board]")
    {
        REQUIRE(board.hash().key == 0);
        REQUIRE(board.hash().lock == 0);
    }

    SECTION("Should get a zero hash when emptying a board", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);

        REQUIRE(board.hash().key != 0);
        REQUIRE(board.hash().lock != 0);
        board.remove_piece(Board::a1);

        REQUIRE(board.hash().key == 0);
        REQUIRE(board.hash().lock == 0);
    }

    SECTION("Should get different hashes for different boards", "[board]")
    {
        board.set(Board::a1, Piece::WHITE_KNIGHT);
        auto [key, lock] = std::tuple(board.hash().key, board.hash().lock);
        REQUIRE(key != lock);

        board.set(Board::g1, Piece::BLACK_KNIGHT);
        REQUIRE(board.hash().key != board.hash().lock);
        REQUIRE(key != board.hash().key);
        REQUIRE(lock != board.hash().lock);
    }

    SECTION("Should get different hashes when moving a piece", "[board]")
    {
        board.set(Board::d1, Piece::WHITE_QUEEN);
        auto [key, lock] = std::tuple(board.hash().key, board.hash().lock);

        board.move(Board::d1, Board::d5);
        REQUIRE(key != board.hash().key);
        REQUIRE(lock != board.hash().lock);
    }
}
} // namespace
