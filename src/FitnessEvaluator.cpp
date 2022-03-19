#include "FitnessEvaluator.hpp"
#include "Chromosome.hpp"
#include "IEngine.hpp"
#include "MaeBoard.hpp"
#include "Move.hpp"
#include "PositionEvaluator.hpp"
#include "Timer.hpp"

#include <cstdlib>

namespace learning
{
using rules::IBoard;
using rules::Piece;

using engine::IEngine;

FitnessEvaluator::FitnessEvaluator(IEngine *chess_engine)
{
    this->chess_engine = chess_engine;
    this->board = new rules::MaeBoard();
    this->evaluator = new engine::PositionEvaluator();
}

FitnessEvaluator::~FitnessEvaluator()
{
    delete this->board;
    delete this->evaluator;
}

// Chromosome A is assumed to be stronger than B, though this may not be true
double FitnessEvaluator::evaluate(Chromosome &a, Chromosome &b)
{
    double evaluation = 0.0;
    std::vector<int> features[2];

    a.decode(features[0]);
    b.decode(features[1]);

    /*--------------------------------------------------------------------------
      Set up a game between the two and see who wins and by how much...
      Use a single engine, just load and unload the factor weights after
      every move...
      -------------------------------------------------------------------------*/
    auto timer = new diagnostics::Timer();
    timer->start();

    this->board->reset();

    // Chromosome A plays white
    IEngine::GameResult result;
    uint turn = 0;
    do
    {
        this->chess_engine->load_factor_weights(features[turn]);

        rules::Move move;
        result = this->chess_engine->get_best_move(3, this->board, move);
        if (result == IEngine::WHITE_MATES || result == IEngine::BLACK_MATES ||
            result == IEngine::STALEMATE)
            break;

        IBoard::Error error = this->board->make_move(move, true);
        if (error != IBoard::NO_ERROR && error != IBoard::DRAW_BY_REPETITION)
        {
            abort();
        }
        else if (error == IBoard::DRAW_BY_REPETITION)
        {
            this->board->undo_move();
            result = IEngine::DRAW_BY_REPETITION;
            break;
        }
        turn = (turn + 1) % 2;

        if (this->board->get_move_number() >= MAX_ALLOWED_MOVEMENTS)
        {
            int material = this->evaluator->evaluate_material(board);

            if (material == 0)
            {
                result = IEngine::STALEMATE;
            }
            else if (board->current_player() == Piece::WHITE)
            {
                if (material > 0)
                    result = IEngine::WHITE_MATES;
                else
                    result = IEngine::BLACK_MATES;
            }
            else
            {
                if (material > 0)
                    result = IEngine::BLACK_MATES;
                else
                    result = IEngine::WHITE_MATES;
            }
            break;
        }
    } while (true);

    timer->stop();

    if (result == IEngine::STALEMATE || result == IEngine::DRAW_BY_REPETITION)
    {
        evaluation = 1.0 / 2.0;
        b.set_result(Chromosome::DRAW);
    }
    else
    {
        evaluation = 6.0 / 9.0;

        if (result == IEngine::WHITE_MATES)
        {
            evaluation = 1 - evaluation;
            b.set_result(Chromosome::LOSS);
        }
        else
        {
            b.set_result(Chromosome::WIN);
        }

        b.set_game_duration(board->get_move_number());
    }

    b.set_material_balance(this->evaluator->evaluate_material(board));
    b.set_fitness(evaluation);

    delete timer;

    return evaluation;
}

} // namespace learning
