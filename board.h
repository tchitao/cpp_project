#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include "piece.h"
#include "global.h"

class Piece;
class Move;

// Defines the complete state of the game at a given time.
// This includes:
//  . the current player
//  . the set of pieces and their position on the board
// 16 Pieces are created at the beginning of the game, and placed on the Board
// according to the rules of the game.
//
// This class is tightly coupled with classed Piece and Move.
// The follow example shows how these classes work together
//
// Board b;
// Piece *p;
// b.getPiece({1,1}, &p);      // p contains now the pawn at position {1,1} (i.e. B2)
// std::vector<Move *> moves;
// p->getMoves(b, moves);     // fill moves with all possible moves for this piece
// Move *m = moves[0];
// m->perform(&b);            // move m is performed on board b
// b.display();
// m->unPerform(&b);          // move m is "unperformed" on board b
// b.display();
//
// The moves computed by getMoves only work on board used to compute them
//

class Board {
public:
    // 16 Pieces are created at the beginning of the game, and placed on the Board
    // according to the rules of the game.
    Board();

    // returns all the moves that can be performed in the current state
    // of the game, this include some 'illegal' moves that would put the player
    // in check
    std::vector<Move *> getAllMoves() const;

    // returns all the legal moves that can be performed in the current
    // state of the game.
    std::vector<Move *> getAllLegalMoves();

    // A move is legal if after performing it, the current player is not in
    // check. Note: This method leaves the board as it found it, but can't
    // be labeled const because it has to temporarly modify it.
    bool isLegal(Move *);

    Color getPlayer() const;

    bool getPiece(Position, Piece **) const;

    void setPiece(Position, Piece *);

    void removePiece(Position);

    // returns true if Player p is in check
    bool isInCheck(Color p) const;

    void display();

    //displays the captured pieces in the current game
    void displayCaptured();

    // returns a "score" that estimates how favorable the game is for a given
    // player. A stricly positive score means that White is winning.
    // heuristic() == INF <=> Black is checkmate
    // heuristic() == MINF <=> White is checkmate
    // A simple way to compute this heuristic is to count the pieces captured
    // by each player. Other ingredients can be added, such as the control of
    // the central positions
    int heuristic();

    void switch_player();

    // this is a utility function that returns the positions that can be reached
    // by a piece traveling in a straight line.
    // start is the starting position of the piece
    // di, dj is the direction of the line to consider (e.g for a rook, the
    // directions are di, dj = 1, 0; 0, 1, -1, 0  0, -1)
    // max is the maximal number of steps to consider (e.g. 8 for a rook, 1 for
    // the king)
    // pl is the player
    // canCapture = true if the piece can capture that the pieces (of opposite color)
    // it encounters. It is true for most of pieces but not all.
    // res is the vector that will store the result. All positions will be
    // push_back'ed on res.
    // This function don't return positions that would fall outside of the board
    void reachablePositionsAlongStraightLine(Position start, int di, int dj,
       int max_steps, Color pl, bool canCapture, std::vector<Position> &res) const;

    // this is a utility function that returns the positions that can be reached
    // by a piece, among the positions given by a vector of relative displacements
    // rel. For instance, for a knight rel = {{2,1}, {-2,1}, {-1,-2} ....}.
    // start is the starting position of the piece
    // pl is the player
    // onlyIfCapture = true if the piece has to capture a piece (of opposite color)
    // in order to move to the position (normally false except for pawns moving in diagonal).
    // res is the vector that will store the result. All positions will be
    // push_back'ed on res.
    // This function don't return positions that would fall outside of the board
    void filter(Position start, const std::vector<Position> &rel, Color pl,
                bool onlyIfCapture, std::vector<Position> &res) const;

    bool castling_permitted(Piece *, Piece *, int, int);

    void promote_pawn_b(Move *, std::string);

    void add_to_achieved_moves(Move *);

    Move *get_last_move();


private:
   Piece *addPiece(Piece *);
   std::vector<Move *> getAllMoves(Color player) const;
   bool isInside(int i, int j) const;

   Piece* board_[8][8];
   Piece *king_[2];
   std::vector<Piece *> pieces_[2];
   Color current_player_ = WHITE;
   std::vector<Move *> achieved_moves_;
};

#endif // BOARD_H_
