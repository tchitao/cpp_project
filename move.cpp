#include "global.h"
#include "move.h"
#include <iostream>
#include <cassert>

BasicMove::BasicMove(Position from, Position to, Piece *moved) : from_(from),
                     to_(to), moved_(moved) {
   assert(moved);
}

void BasicMove::perform(Board *b) const {
    b->setPiece(to_, moved_);
    moved_->setPosition(to_);
    b->removePiece(from_);
}

void BasicMove::unPerform(Board *b) const {
    b->setPiece(from_, moved_);
    moved_->setPosition(from_);
    b->removePiece(to_);
}

bool BasicMove::doesCapture(Piece*) const {
    return false;
}

std::string BasicMove::toAlgebraicNotation(int i) const {
    std::string notation = "";
    if (moved_->notation() != ' ') {
      notation += moved_->notation();
    }
    switch (i) {
      case 0:
        notation += getFileRank(from_);
        break;
      case 1:
        notation += getRank(from_);
        break;
      case 2:
        notation += getFileLetter(from_);
        break;
      default:
        break;
    }
    notation += getFileRank(to_);
    return notation;
}

std::string BasicMove::toBasicNotation() const {
     return getFileRank(from_) + getFileRank(to_);
}

Position BasicMove::getPosition_promotion() const {
    return to_;
}

bool BasicMoveWithCapture::doesCapture(Piece *p) const {
    return (p == NULL) || (p == captured_);
}

BasicMoveWithCapture::BasicMoveWithCapture(Position from, Position to,
                                           Piece *moved, Piece *captured) :
   BasicMove(from, to, moved), captured_(captured) {
   assert(captured);
}

std::string BasicMoveWithCapture::toAlgebraicNotation(int i) const {
    std::string notation = BasicMove::toAlgebraicNotation(i);
    std::string target_string = notation.substr(notation.size()-2, 2);
    notation[notation.size() - 2] = 'x';
    notation[notation.size() - 1] = target_string[0];
    notation += target_string[1];
    return notation;
}

void BasicMoveWithCapture::perform(Board *b) const {
    BasicMove::perform(b);
    captured_->setCaptured(true);
}

void BasicMoveWithCapture::unPerform(Board *b) const {
    BasicMove::unPerform(b);
    b->setPiece(captured_->getPosition(), captured_);
    captured_->setCaptured(false);
}

Position BasicMoveWithCapture::getPosition_promotion() const {
    return BasicMove::getPosition_promotion();
}

Castling::Castling(Piece *moved_k, Piece *moved_r) : moved_k_(moved_k), moved_r_(moved_r) {

      assert(moved_k);
      assert(moved_r);
    }

void Castling::perform(Board *b) const {
    Position pos_k_ = moved_k_->getPosition();
    Position pos_r_ = moved_r_->getPosition();
    int dir = ((pos_r_.second - pos_k_.second) < 0)? -1 : 1;
    b->setPiece({pos_k_.first, pos_k_.second + 2*dir}, moved_k_);
    moved_k_->setPosition({pos_k_.first, pos_k_.second + 2*dir});
    b->removePiece(pos_k_);
    Position pos_k_new = moved_k_->getPosition();
    b->setPiece({pos_r_.first, pos_k_new.second - dir}, moved_r_);
    moved_r_->setPosition({pos_k_.first, pos_k_new.second - dir});
    b->removePiece(pos_r_);
}

void Castling::unPerform(Board *b) const {
    Position pos_k_ = moved_k_->getPosition();
    Position pos_r_ = moved_r_->getPosition();
    int dir = ((pos_r_.second - pos_k_.second) < 0)? -1 : 1;
    int pos_r_new = (pos_r_.second == 3)? 0 : 7;
    b->setPiece({pos_k_.first, 4}, moved_k_);
    moved_k_->setPosition({pos_k_.first, pos_k_.second + 2*dir});
    b->removePiece(pos_k_);
    b->setPiece({pos_k_.first, pos_r_new}, moved_r_);
    moved_r_->setPosition({pos_k_.first, pos_r_new});
    b->removePiece(pos_r_);
}

std::string Castling::toAlgebraicNotation(int i) const {
    return (*this).toBasicNotation();
}

std::string Castling::toBasicNotation() const {
    std::string res;
    Position pos_r_ = moved_r_->getPosition();
    if (pos_r_.second == 0) {
      res = "O-O-O";
    } else {
      res = "O-O";
    }
    return res;
}

bool Castling::doesCapture(Piece *p) const {
    return false;
}

Position Castling::getPosition_promotion() const {
    Position pos;
    return pos;
}
