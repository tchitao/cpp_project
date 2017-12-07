// This module defines the virtual class Move and its subclasses BasicMove
// BasicMoveWithCapture. New subclasses can be added here, such as KingCastling, QueenCastling,
// or PawnPromotion

#ifndef MOVE_H_
#define MOVE_H_

#include <vector>
#include "global.h"
#include "board.h"

class Piece;
class Board;

// There are different types of moves in the game of Chess. They can be
// quite different. Basic moves, with or without capture.
// But also castling, "en passant", and pawn promotion.
// see https://en.wikipedia.org/wiki/Rules_of_chess#Movement
// In this file, only BasicMove and BasicMoveWithCapture are defined.
//
// The virtual class Move defines what all moves have in common. Essentially,
// they pertain to a player and can be performed on a board. We also need the
// ability to unperform (or undo a move).
//
// See in board.h an example that shows how Board, Piece and Move work together
class Move {
public:

    // Modify b by performing the move. The move object must have been computed
    // on this instance of b.
    //
    // The attributes (position, capture state) of the pieces involved are updated
    virtual void perform(Board *b) const = 0;

    // Modify b by unperforming the move. This move must have been performed just
    // before.
    //
    // The attributes (position, capture state) of the pieces involved are updated
    virtual void unPerform(Board *b) const = 0;

    // return algebraic notation for this move
    // from more precise to less precise
    // Example for basicmove : King move from A1 to B2, without and with capture
    // i = 0    Ka1b2,  Ka1xb2
    // i = 1    K1b2    K1xb2
    // i = 2    Kab2    Kaxb2
    // i = 3    Kb2     Kxb2
    virtual std::string toAlgebraicNotation(int i) const = 0;

    virtual std::string toBasicNotation() const = 0;

    // if p == NULL, returns true iff any piece is captured
    // if p != NULL, returns true iff  p is captured
    virtual bool doesCapture(Piece *p) const = 0;

protected:
    Color player_;
};


class BasicMove : public Move {

public:
  BasicMove(Position from, Position to, Piece *moved);

  std::string toAlgebraicNotation(int i) const;

  std::string toBasicNotation() const;

  virtual void unPerform(Board *b) const;

  virtual void perform(Board *b) const;

  virtual bool doesCapture(Piece*) const;


private:
  Position from_;
  Position to_;
  Piece *moved_;
};

class BasicMoveWithCapture : public BasicMove {

public:
    BasicMoveWithCapture(Position from, Position to, Piece *moved, Piece *captured);

    void unPerform(Board *b) const;

    void perform(Board *b) const;

    bool doesCapture(Piece *p) const;

private:
    Piece *captured_;
    //Piece *moved_;
};
/*
class Castling : public Move {

public:
    Castling(Position pos_k, Position pos_r, Piece *moved_k, Piece *moved_r);

    void unPerform(Board *b) const;

    void perform(Board *b) const;

    int castling_permitted(Board *b, Color col);

private:
    Position pos_k_;
    Position pos_r_;
    Piece *moved_k_;
    Piece *moved_r_;

};
*/
#endif // MOVE_H_
