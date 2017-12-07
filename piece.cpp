#include "piece.h"
#include "global.h"
#include "assert.h"

Piece::Piece(Position pos, Color color) : color_(color), position_(pos) {}

void Piece::setCaptured(bool b) {
    is_captured_ = b;
}

bool Piece::isCaptured() const  {
  return is_captured_;
}

Color Piece::getColor() const {
  return color_;
}

void Piece::setPosition(Position pos) {
    position_ = pos;
}

Position Piece::getPosition() const { return position_; }

void Piece::positionsToMoves(const Board &g, Position from, 
                  const std::vector<Position> &tos, std::vector<Move *> &res) {
    Piece *src;
    assert(g.getPiece(from, &src));
    for (Position to : tos) {
        Piece *captured;
        if (!g.getPiece(to, &captured)) {
            Move *m = new BasicMove(from, to, src);
            res.push_back(m);
        } else {
            assert(captured);
            Move *m = new BasicMoveWithCapture(from, to, src, captured);
            res.push_back(m);
        } 
    }
}