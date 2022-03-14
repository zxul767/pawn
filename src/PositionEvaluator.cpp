#include "PositionEvaluator.hpp"
#include "BoardTraits.hpp"
#include "GameTraits.hpp"
#include "IBoard.hpp"
#include "King.hpp"
#include "Util.hpp"
#include <iostream>

namespace game_engine
{
using game_rules::BoardSquare;
using game_rules::CastleSide;
using game_rules::IBoard;
using game_rules::Piece;

using util::bitboard;

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
    int sign = (board->get_player_in_turn() == Piece::WHITE ? 1 : -1);

    material = evaluate_material(board);
    mobility = evaluate_mobility(board);
    center_control = evaluate_center_control(board);
    king_safety = evaluate_king_safety(board);

    return sign * (factor_weight[MATERIAL] * material + factor_weight[MOBILITY] * mobility +
                   factor_weight[CENTER_CONTROL] * center_control + factor_weight[KING_SAFETY] * king_safety);
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

        material += (material_value(player_piece, piece) - material_value(opponent_piece, piece));
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

        mobility += (mobility_value(board, player_piece, piece) - mobility_value(board, opponent_piece, piece));
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
            (center_control_value(board, player_piece, piece) - center_control_value(board, opponent_piece, piece));
    }
    return center_control;
}

int PositionEvaluator::evaluate_king_safety(const IBoard *board) const
{
    return (king_safety_value(board, Piece::WHITE) - king_safety_value(board, Piece::BLACK));
}

int PositionEvaluator::material_value(bitboard piece, Piece::Type piece_type) const
{
    return util::Util::count_set_bits(piece) * piece_value[piece_type];
}

int PositionEvaluator::mobility_value(const IBoard *board, bitboard piece, Piece::Type piece_type) const
{
    bitboard moves = 0;
    uint n_moves = 0;

    int position = util::Util::MSB_position(piece);
    while (piece && position != -1)
    {
        moves = board->get_moves(piece_type, BoardSquare(position));
        n_moves += util::Util::count_set_bits(moves);
        piece ^= util::constants::ONE << position;
        position = util::Util::MSB_position(piece);
    }

    if (board->get_move_number() <= 10 && piece_type == Piece::QUEEN)
        return 0;

    return n_moves;
}

int PositionEvaluator::center_control_value(const IBoard *board, bitboard piece, Piece::Type piece_type) const
{
    bitboard center = (util::constants::ONE << 27) | (util::constants::ONE << 28) | (util::constants::ONE << 35) |
                      (util::constants::ONE << 36);

    bitboard attacks;
    int squares_controled = 0;

    squares_controled += util::Util::count_set_bits(piece & center);

    int position = util::Util::MSB_position(piece);
    while (piece && position != -1)
    {
        attacks = board->get_moves(piece_type, BoardSquare(position));
        squares_controled += util::Util::count_set_bits(attacks & center);
        piece ^= util::constants::ONE << position;
        position = util::Util::MSB_position(piece);
    }

    return squares_controled;
}

int PositionEvaluator::king_safety_value(const IBoard *board, Piece::Player player) const
{
    static bitboard pawns[game_rules::PLAYERS_COUNT][game_rules::PLAYERS_COUNT] = {
        {util::Util::to_bitboard[BoardSquare::f2] | util::Util::to_bitboard[BoardSquare::g2] |
             util::Util::to_bitboard[BoardSquare::h2],
         util::Util::to_bitboard[BoardSquare::a2] | util::Util::to_bitboard[BoardSquare::b2] |
             util::Util::to_bitboard[BoardSquare::c2]},

        {util::Util::to_bitboard[BoardSquare::f7] | util::Util::to_bitboard[BoardSquare::g7] |
             util::Util::to_bitboard[BoardSquare::h7],
         util::Util::to_bitboard[BoardSquare::a7] | util::Util::to_bitboard[BoardSquare::b7] |
             util::Util::to_bitboard[BoardSquare::c7]}};

    int safety_value = 0;
    bool has_moved; // king has moved from its original square

    has_moved = false;
    if (!((util::constants::ONE << board->get_initial_king_square(player)) & board->get_pieces(player, Piece::KING)))
    {
        has_moved = true;
    }

    if (board->is_castled(player, CastleSide::KING_SIDE))
    {
        if (pawns[player][CastleSide::KING_SIDE] ==
            (pawns[player][CastleSide::KING_SIDE] & board->get_pieces(player, Piece::PAWN)))
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
            (pawns[player][CastleSide::QUEEN_SIDE] & board->get_pieces(player, Piece::PAWN)))
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

} // namespace game_engine
