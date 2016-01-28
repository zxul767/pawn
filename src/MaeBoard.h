#ifndef MAE_BOARD_H
#define MAE_BOARD_H

#include "Board.h"
#include "Square.h"
#include "BoardStatus.h"
#include "Record.h"
#include <stack>

using std::stack;

class MaeBoard : public Board
{
 public:

   MaeBoard ();
   MaeBoard (const string& file);
   ~MaeBoard ();

   void clear ();
   void reset ();

   bool load_game (const string& file);
   bool save_game (const string& file);

   bool add_piece (const string& location, Piece::Type type, Piece::Player player);
   bool add_piece (Squares square, Piece::Type type, Piece::Player player);

   bool remove_piece (const string& location);
   bool remove_piece (Squares square);

   Error make_move (Move& move, bool is_computer_move);
   bool undo_move ();

   void label_move (Move& move) const;

   bool is_king_in_check () const;
   bitboard attacks_to (Squares location, bool include_king) const;
   bitboard threats_to (Squares location, Piece::Type type) const;

   // ACCESSORS
   bitboard get_moves (Piece::Type piece, Squares square) const;
   bitboard get_all_pieces () const;
   bitboard get_pieces (Piece::Player player) const;
   bitboard get_pieces (Piece::Player player, Piece::Type piece) const;

   bool is_en_passant_on () const;
   bool can_castle (Piece::Player player, CastleSide side) const;
   bool is_castled (Piece::Player player, CastleSide side) const;

   bitboard get_en_passant_square () const;
   Squares get_initial_king_square (Piece::Player player) const;

   Piece::Player get_piece_color (Squares square) const;
   Piece::Player get_turn () const;
   Piece::Type get_piece (Squares square) const;
   ullong get_hash_key () const;
   ullong get_hash_lock () const;
   uint get_move_number () const;
   ushort get_repetition_count () const;

   // MUTATORS
   void set_game_status (GameStatus status);
   void set_en_passant (Squares en_passant_capture_square);
   void set_turn (Piece::Player player);
   void set_castling_privilege (Piece::Player player, CastleSide side, bool value);

   void turn_debugging (bool is_on) { debugging = is_on; }

 private:
   // Do not allow users of this class to make copies
   MaeBoard (const MaeBoard& other);

   /*==========================================================================
     PRIVATE CONSTANTS
     ========================================================================*/
   static const uint PIECE_TYPES  =  6;
   static const uint CASTLE_SIDES =  2;
   static const uint RANDOM_SEED  =  8;
   static const uint N_HASH_KEYS  =  2;
   static const Square EMPTY_SQUARE;

   /*==========================================================================
     PRIVATE DATA TO REPRESENT THE BOARD
     ========================================================================*/
   // Basic board representation
   bitboard piece[PLAYERS][PIECE_TYPES];
   bitboard pieces[PLAYERS];
   bitboard all_pieces;
   Square board[SQUARES];

   // Hash key information
   ullong zobrist[PIECE_TYPES][PLAYERS][SQUARES][N_HASH_KEYS];
   ullong hash_key;
   ullong hash_lock;
   ullong turn_key;
   ullong castle_key[PLAYERS][CASTLE_SIDES];
   ullong en_passant_key[SQUARES];

   // Special moves information
   bool can_do_castle[PLAYERS][CASTLE_SIDES];
   bool is_castled_[PLAYERS][CASTLE_SIDES];
   bitboard en_passant_capture_square;

   // Turn information
   bool is_whites_turn;
   Piece::Player player, opponent;

   // This information is intended to resume interrupted games
   GameStatus game_status;

   // Dictionary to detect threefold repetition conditions
   Record position_counter;

   // Counter used to detect draws by the 50-move rule
   uint fifty_move_counter;

   // Game history needed to undo moves
   stack<BoardStatus> game_history;

   // Objects implementing the Piece interface to aid in move generation
   Piece* chessmen[PIECE_TYPES];

   // Support Data that makes the code more readable
   bitboard eighth_rank[PLAYERS];
   Squares corner[PLAYERS][CASTLE_SIDES];
   Squares original_king_position[PLAYERS];

   // show debugging information if the flag is on
   bool debugging;

   /*==========================================================================
     PRIVATE METHODS
     ========================================================================*/
   void handle_en_passant_move (const Move& move);
   void handle_castling_privileges (const Move& move);
   void handle_promotion_move (const Move& move);

   Error can_move (const Move& move) const;

   void load_chessmen ();
   void load_support_data ();
   void load_zobrist ();

   void save_restore_information (const Move& move);
   void change_turn ();
};

#endif
