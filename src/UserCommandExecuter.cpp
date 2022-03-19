#include "UserCommandExecuter.hpp"
#include "FitnessEvaluator.hpp"
#include "GeneticAlgorithm.hpp"
#include "IBoard.hpp"
#include "IEngine.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"
#include "Timer.hpp"
#include "UserCommand.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace game_ui
{
using std::cerr;
using std::cout;
using std::string;
using std::vector;

using rules::IBoard;
using rules::Move;

using engine::IEngine;

using learning::Chromosome;
using learning::FitnessEvaluator;
using learning::GeneticAlgorithm;

UserCommandExecuter::UserCommandExecuter(
    IBoard *board, IEngine *engine, diagnostics::Timer *timer)
{
    this->timer = timer;
    this->board = board;
    this->engine = engine;
    this->move_generator = new engine::MoveGenerator();
}

bool UserCommandExecuter::execute(const UserCommand &command)
{
    switch (command.get_key())
    {
    case UserCommand::NEW_GAME:
        this->board->reset();
        break;

    case UserCommand::UNDO_MOVE:
        this->board->undo_move();
        break;

    case UserCommand::REMOVE:
        this->board->undo_move();
        this->board->undo_move();
        break;

    case UserCommand::SEE_MOVES:
        show_possible_moves();
        break;

    case UserCommand::USER_MOVE:
        make_user_move(command.get_notation());
        break;

    case UserCommand::FEATURES:
        cout << "feature setboard=1 usermove=1 time=0 draw=0 sigint=0 "
             << "sigterm=0 variants=\"normal\" analyze=0 colors=0 "
             << "myname=\"Pawn\" done=1" << std::endl;
        break;

    case UserCommand::XBOARD_MODE:
        cout << std::endl;
        break;

    case UserCommand::THINK:
        think();
        break;

    case UserCommand::TRAIN:
        train_by_genetic_algorithm(
            /* population_size: */ 6,
            /* generations_count: */ 15,
            /* mutation_probability: */ 0.004);
        break;

    default:
        break;
    }
    return true;
}

/*==============================================================================
    Show the set of all valid moves starting from the current configuration of
    THIS->BOARD.
  ==============================================================================*/
void UserCommandExecuter::show_possible_moves()
{
    vector<Move> possible_moves;

    this->move_generator->generate_moves(board, possible_moves);
    cerr << possible_moves.size() << " moves" << std::endl;

    for (uint i = 0; i < possible_moves.size(); ++i)
        if (this->board->make_move(possible_moves[i], true) == IBoard::NO_ERROR)
        {
            cerr << possible_moves[i] << " -> " << possible_moves[i].get_score()
                 << std::endl;

            cerr << (*board) << std::endl;

            this->timer->set_timer(2.0);
            this->timer->sleep();

            this->board->undo_move();
        }
}

/*==============================================================================
    Communicate to the engine a move made by the user in the GUI
  ==============================================================================*/
void UserCommandExecuter::make_user_move(const string &command)
{
    string::size_type i = command.find(" ");

    if (i != string::npos)
    {
        // Strip off the string 'usermove ' sent by Xboard before the actual move
        string notation = command.substr(i + 1);
        Move move(notation);
        IBoard::Error error = this->board->make_move(move, false);

        switch (error)
        {
        case IBoard::NO_ERROR:
            think();
            break;

        case IBoard::DRAW_BY_REPETITION:
            cout << "1/2-1/2 {Draw by repetition}" << std::endl;
            break;

        case IBoard::WRONG_MOVEMENT:
            cout << "Illegal move: " << notation;
            break;

        case IBoard::OPPONENTS_TURN:
            cout << "Illegal move: " << notation;
            break;

        default:
            break;
        }
    }
}

/*==============================================================================
    Query the engine for the most promising move and communicate the response
    to the GUI. It also communicates check mates and draws.
  ==============================================================================*/
void UserCommandExecuter::think()
{
    Move best_move;
    uint depth = 5;

    IEngine::GameResult result = this->engine->get_best_move(depth, board, best_move);

    if (result == IEngine::NORMAL_EVALUATION || result == IEngine::BLACK_MATES ||
        result == IEngine::WHITE_MATES)
    {
        IBoard::Error error = this->board->make_move(best_move, true);
        if (error == IBoard::NO_ERROR)
        {
            string first, second;
            Move::translate_to_notation(best_move.from(), first);
            Move::translate_to_notation(best_move.to(), second);

            // Communicate the move to Xboard
            cout << "move " << first + second << std::endl;
        }
        else
        {
            // These errors are not supposed to happen
            switch (error)
            {
            case IBoard::KING_LEFT_IN_CHECK:
                break;

            case IBoard::OPPONENTS_TURN:
                break;

            case IBoard::WRONG_MOVEMENT:
                break;

            case IBoard::GAME_FINISHED:
                break;

            case IBoard::NO_PIECE_IN_SQUARE:
                break;

            case IBoard::DRAW_BY_REPETITION:
                cout << "1/2-1/2 {Draw by repetition}" << std::endl;
                break;

            default:
                abort();
                break;
            }
        }

        if (result == IEngine::WHITE_MATES)
        {
            cout << "1-0 {White mates}" << std::endl;
        }
        else if (result == IEngine::BLACK_MATES)
        {
            cout << "0-1 {Black mates}" << std::endl;
        }
    }
    else if (result == IEngine::STALEMATE)
    {
        cout << "1/2-1/2 {Stalemate}" << std::endl;
    }
    else if (result == IEngine::DRAW_BY_REPETITION)
    {
        cout << "1/2-1/2 {Draw by repetition}" << std::endl;
    }
}

void UserCommandExecuter::train_by_genetic_algorithm(
    uint population_size, uint n_generations, double mutation_probability)
{
    std::unique_ptr<FitnessEvaluator> fitness_evaluator(
        new FitnessEvaluator(this->engine));

    std::unique_ptr<GeneticAlgorithm> algorithm(new GeneticAlgorithm(
        population_size, n_generations, mutation_probability, fitness_evaluator.get()));

    vector<int> features_a;
    features_a.push_back(80);
    features_a.push_back(10);
    features_a.push_back(20);
    features_a.push_back(10);
    Chromosome seed(features_a);

    algorithm->set_seed(seed);
    algorithm->run();

    vector<int> best_features;
    algorithm->get_fittest_member().decode(best_features);

    this->engine->load_factor_weights(best_features);
}

} // namespace game_ui
