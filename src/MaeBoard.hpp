#ifndef MAE_BOARD_H
#define MAE_BOARD_H

#include "IBoard.hpp"
#include "Square.hpp"
#include "BoardConfiguration.hpp"
#include "BoardConfigurationTracker.hpp"
#include <stack>

namespace game_rules
{
class MaeBoard : public IBoard
{
  public:
   MaeBoard ();
   MaeBoard (const std::string& file);
   ~MaeBoard ();

   void clear ();
   void reset ();

   bool load_game (const std::string& file);
   bool save_game (const std::string& file);

   bool add_piece (const std::string& location, Piece::Type type, Piece::Player);
   bool add_piece (Squares square, Piece::Type, Piece::Player);

   bool remove_piece (const std::string& location);
   bool remove_piece (Squares square);

   Error make_move (Move& move, bool is_computer_move);
   bool undo_move ();

   void label_move (Move& move) const;

   bool is_king_in_check () const;
   bitboard attacks_to (Squares location, bool include_king) const;
   bitboard threats_to (Squares location, Piece::Type type) const;

   bitboard get_moves (Piece::Type piece, Squares square) const;
   bitboard get_all_pieces () const;
   bitboard get_pieces (Piece::Player) const;
   bitboard get_pieces (Piece::Player, Piece::Type) const;

   bool is_en_passant_on () const;
   bool can_castle (Piece::Player, CastleSide) const;
   bool is_castled (Piece::Player, CastleSide) const;

   bitboard get_en_passant_square () const;
   Squares get_initial_king_square (Piece::Player) const;

   Piece::Player get_piece_color (Squares square) const;
   Piece::Player get_player_in_turn () const;
   Piece::Type get_piece (Squares square) const;
   ullong get_hash_key () const;
   ullong get_hash_lock () const;
   uint get_move_number () const;
   ushort get_repetition_count () const;

   void set_game_status (GameStatus status);
   void set_en_passant_capture_square (Squares en_passant_capture_square);
   void set_player_in_turn (Piece::Player);
   void set_castling_privilege (Piece::Player, CastleSide, bool value);

  private:
   // Do not allow users of this class to make copies
   MaeBoard (const MaeBoard&);

   static const uint CASTLE_SIDES_COUNT =  2;
   static const uint RANDOM_SEED  =  8;
   static const uint HASH_KEYS_COUNT  =  2;
   static const Square EMPTY_SQUARE;

   // Basic board representation
   bitboard piece[PLAYERS_COUNT][Piece::PIECES_COUNT];
   bitboard pieces[PLAYERS_COUNT];
   bitboard all_pieces;
   Square board[SQUARES_COUNT];

   // Hash key information
   ullong zobrist[Piece::PIECES_COUNT][PLAYERS_COUNT][SQUARES_COUNT][HASH_KEYS_COUNT];
   ullong hash_key;
   ullong hash_lock;
   ullong turn_key;
   ullong castle_key[PLAYERS_COUNT][CASTLE_SIDES_COUNT];
   ullong en_passant_key[SQUARES_COUNT];

   // Special moves information
   bool can_do_castle[PLAYERS_COUNT][CASTLE_SIDES_COUNT];
   bool is_castled_[PLAYERS_COUNT][CASTLE_SIDES_COUNT];
   bitboard en_passant_capture_square;

   // Turn information
   bool is_whites_turn;
   Piece::Player player, opponent;

   // This information is intended to resume interrupted games
   GameStatus game_status;

   // Useful to detect threefold repetition conditions
   BoardConfigurationTracker position_counter;

   // Counter used to detect draws by the 50-move rule
   uint fifty_move_counter;

   std::stack<BoardConfiguration> game_history;
   Piece* chessmen[Piece::PIECES_COUNT];

   bitboard eighth_rank[PLAYERS_COUNT];
   Squares corner[PLAYERS_COUNT][CASTLE_SIDES_COUNT];
   Squares original_king_position[PLAYERS_COUNT];

   void handle_en_passant_move (const Move&);
   void handle_castling_privileges (const Move&);
   void handle_promotion_move (const Move&);

   Error can_move (const Move&) const;

   void load_chessmen ();
   void load_support_data ();
   void load_zobrist ();

   void save_restore_information (const Move&);
   void change_turn ();
};

} // namespace game_rules

#endif // MAE_BOARD_H
