// Subclasses of Pieces: Pawn, Bishop, Knight, King, Queen and Rook.

#ifndef CONCRETEPIECES_H_
#define CONCRETEPIECES_H_

#include <vector>
#include "piece.h"
#include "board.h"
#include "move.h"

class Pawn : public Piece {

public:
    Pawn(Position, Color);


    void getMoves(const Board &b, std::vector<Move *> &res) const;

    char notation() const;

    char toChar() const;
};

class Bishop : public Piece {

public:
    Bishop(Position, Color);


    void getMoves(const Board &b, std::vector<Move *> &res) const;

    char notation() const;

    char toChar() const;
};


class King : public Piece {

public:
    King(Position, Color);


    void getMoves(const Board &, std::vector<Move *> &) const;

    char notation() const;

    char toChar() const;
};

class Rook : public Piece {

public:
    Rook(Position, Color);


    void getMoves(const Board &, std::vector<Move *> &) const;

    char notation() const;

    char toChar() const;
};

class Queen : public Piece {

public:
    Queen(Position, Color);


    void getMoves(const Board &, std::vector<Move *> &) const;

    char notation() const;

    char toChar() const;
};

class Knight : public Piece {

public:
    Knight(Position, Color);


    void getMoves(const Board &, std::vector<Move *> &) const;

    char notation() const;

    char toChar() const;
};

#endif // CONCRETEPIECES_H_
