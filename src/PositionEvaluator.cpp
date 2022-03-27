#include "PositionEvaluator.hpp"
#include "BoardTraits.hpp"
#include "GameTraits.hpp"
#include "IBoard.hpp"
#include "King.hpp"
#include "bitboard.hpp"
#include "util.hpp"
#include <iostream>

namespace engine
{
using SQ = rules::BoardSquare;
using bits::bitboard;
using bits::to_bitboard;
using rules::BoardSquare;
using rules::CastleSide;
using rules::IBoard;
using rules::Piece;

PositionEvaluator::PositionEvaluator()
{
    int KING_VALUE = util::constants::INFINITUM;

    this->piece_value.push_back(100);        // PAWN
    this->piece_value.push_back(300);        // KNIGHT
    this->piece_value.push_back(325);        // BISHOP
    this->piece_value.push_back(500);        // ROOK
    this->piece_value.push_back(900);        // QUEEN
    this->piece_value.push_back(KING_VALUE); // KING

    this->factor_weight.push_back(502); // MATERIAL
    this->factor_weight.push_back(780); // MOBILITY
    this->factor_weight.push_back(916); // CENTER_CONTROL
    this->factor_weight.push_back(22);  // KING_SAFETY
}

int PositionEvaluator::static_evaluation(const IBoard *board) const
{
    int material;
    int mobility;
    int king_safety = 0;
    int center_control;
    int sign = (board->current_player() == Piece::WHITE ? 1 : -1);

    material = evaluate_material(board);
    mobility = evaluate_mobility(board);
    center_control = evaluate_center_control(board);
    king_safety = evaluate_king_safety(board);

    return sign *
           (factor_weight[MATERIAL] * material + factor_weight[MOBILITY] * mobility +
            factor_weight[CENTER_CONTROL] * center_control +
            factor_weight[KING_SAFETY] * king_safety);
}

int PositionEvaluator::evaluate_material(const IBoard *board) const
{
    bitboard player_piece;
    bitboard opponent_piece;
    int material = 0;

    for (Piece::Type piece = Piece::PAWN; piece <= Piece::QUEEN; ++piece)
    {
        player_piece = board->get_pieces(Piece::WHITE, piece);
        opponent_piece = board->get_pieces(Piece::BLACK, piece);

        material +=
            (material_value(player_piece, piece) - material_value(opponent_piece, piece));
    }
    return material;
}

int PositionEvaluator::evaluate_mobility(const IBoard *board) const
{
    bitboard player_piece;
    bitboard opponent_piece;
    int mobility = 0;

    for (Piece::Type piece = Piece::PAWN; piece <= Piece::QUEEN; ++piece)
    {
        player_piece = board->get_pieces(Piece::WHITE, piece);
        opponent_piece = board->get_pieces(Piece::BLACK, piece);

        mobility +=
            (mobility_value(board, player_piece, piece) -
             mobility_value(board, opponent_piece, piece));
    }
    return mobility;
}

int PositionEvaluator::evaluate_center_control(const IBoard *board) const
{
    bitboard player_piece;
    bitboard opponent_piece;
    int center_control = 0;

    for (Piece::Type piece = Piece::PAWN; piece <= Piece::QUEEN; ++piece)
    {
        player_piece = board->get_pieces(Piece::WHITE, piece);
        opponent_piece = board->get_pieces(Piece::BLACK, piece);

        center_control +=
            (center_control_value(board, player_piece, piece) -
             center_control_value(board, opponent_piece, piece));
    }
    return center_control;
}

int PositionEvaluator::evaluate_king_safety(const IBoard *board) const
{
    return (
        king_safety_value(board, Piece::WHITE) - king_safety_value(board, Piece::BLACK));
}

int PositionEvaluator::material_value(bitboard piece, Piece::Type piece_type) const
{
    return bits::count_ones(piece) * piece_value[piece_type];
}

int PositionEvaluator::mobility_value(
    const IBoard *board, bitboard piece, Piece::Type piece_type) const
{
    bitboard moves = 0;
    uint n_moves = 0;

    int position = bits::msb_position(piece);
    while (piece && position != -1)
    {
        moves = board->get_moves(piece_type, BoardSquare(position));
        n_moves += bits::count_ones(moves);
        piece ^= to_bitboard[position];
        position = bits::msb_position(piece);
    }

    if (board->get_move_number() <= 10 && piece_type == Piece::QUEEN)
        return 0;

    return n_moves;
}

int PositionEvaluator::center_control_value(
    const IBoard *board, bitboard piece, Piece::Type piece_type) const
{
    bitboard center = to_bitboard[SQ::d4] | to_bitboard[SQ::e4] | to_bitboard[SQ::e5] |
                      to_bitboard[SQ::d5];
    bitboard attacks;
    int squares_controled = 0;

    squares_controled += bits::count_ones(piece & center);

    int position = bits::msb_position(piece);
    while (piece && position != -1)
    {
        attacks = board->get_moves(piece_type, BoardSquare(position));
        squares_controled += bits::count_ones(attacks & center);
        piece ^= to_bitboard[position];
        position = bits::msb_position(piece);
    }

    return squares_controled;
}

int PositionEvaluator::king_safety_value(const IBoard *board, Piece::Player player) const
{
    static bitboard pawns[rules::PLAYERS_COUNT][rules::PLAYERS_COUNT] = {
        {to_bitboard[SQ::f2] | to_bitboard[SQ::g2] | to_bitboard[SQ::h2],
         to_bitboard[SQ::a2] | to_bitboard[SQ::b2] | to_bitboard[SQ::c2]},
        {to_bitboard[SQ::f7] | to_bitboard[SQ::g7] | to_bitboard[SQ::h7],
         to_bitboard[SQ::a7] | to_bitboard[SQ::b7] | to_bitboard[SQ::c7]}};

    int safety_value = 0;
    bool has_moved; // king has moved from its original square

    has_moved = false;
    if (!(to_bitboard[board->get_initial_king_square(player)] &
          board->get_pieces(player, Piece::KING)))
    {
        has_moved = true;
    }

    if (board->is_castled(player, CastleSide::KING_SIDE))
    {
        if (pawns[player][CastleSide::KING_SIDE] ==
            (pawns[player][CastleSide::KING_SIDE] &
             board->get_pieces(player, Piece::PAWN)))
        {
            safety_value = 20;
        }
        else
        {
            safety_value = -40;
        }
    }
    else if (board->is_castled(player, CastleSide::QUEEN_SIDE))
    {
        if (pawns[player][CastleSide::QUEEN_SIDE] ==
            (pawns[player][CastleSide::QUEEN_SIDE] &
             board->get_pieces(player, Piece::PAWN)))
        {
            safety_value = 20;
        }
        else
        {
            safety_value = -40;
        }
    }
    else if (has_moved)
    {
        safety_value = -50;
    }
    else
    {
        safety_value = -20;
    }

    return safety_value;
}

int PositionEvaluator::get_piece_value(Piece::Type piece_type) const
{
    return this->piece_value[piece_type];
}

void PositionEvaluator::load_factor_weights(std::vector<int> &weights)
{
    for (uint i = 0; i < weights.size(); ++i)
    {
        if (i >= this->factor_weight.size())
            break;

        this->factor_weight[i] = weights[i];
    }
}

} // namespace engine
