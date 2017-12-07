#ifndef PIECE_H_
#define PIECE_H_

#include <vector>
#include "global.h"
#include "move.h"
#include "board.h"

class Board;
class Move;

// Abstract class for the pieces of the game. A piece has a color, a position on
// the board, and a captured attribute. The only non-trivial method is getMoves()
// It is reponsible to compute all possible Moves for this piece on a board.
//
// Typically, the 16 pieces need for the game are created at the beginning and
// deleted at the end.
class Piece {
public:
    Piece(Position, Color);

    // returns the char used in the standard algebric notation of the piece
    // exception returns ' ' for a Pawn (see Pawn)
    virtual char notation() const = 0;


    // returns the char used for display of the board
    virtual char toChar() const = 0;

    // push_back in res all possible moves for this piece on board b
    virtual void getMoves(const Board &b, std::vector<Move *> &res) const = 0;

    bool isCaptured() const;

    void setCaptured(bool);

    Color getColor() const;

    void setPosition(Position);

    Position getPosition() const;

protected:
    // Utility function used by the various "getMoves()" methods to transform
    // a starting position 'from' and a vector of position 'tos' to a vector
    // of moves. Each move is a basic move, with or without capture, from
    // position 'from' to a position in vector 'tos'.
    //
    // More specifically, the resulting moves are 'pushed back' on the vector res
    // given a parameter.
    static void positionsToMoves(const Board &g, Position from,
                      const std::vector<Position> &tos,
                      std::vector<Move *> &res);

private:
    Color color_;
    Position position_;
    bool is_captured_ = false;
};

#endif // PIECE_H_
