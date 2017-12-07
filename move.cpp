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
     // FILL THIS
     return "";
}

std::string BasicMove::toBasicNotation() const {
     return getFileRank(from_) + getFileRank(to_);
}
/*
char BasicMove::moved_toChar() {
  return moved_->toChar();
}*/

bool BasicMoveWithCapture::doesCapture(Piece *p) const {
    return (p == NULL) || (p == captured_);
}

BasicMoveWithCapture::BasicMoveWithCapture(Position from, Position to,
                                           Piece *moved, Piece *captured) :
   BasicMove(from, to, moved), captured_(captured) {
   assert(captured);
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


/*
Castling::Castling(Position pos_k, Position pos_r, Piece *moved_k, Piece *moved_r) :
    pos_k_(pos_k), pos_r_(pos_r), moved_k_(moved_k), moved_r_(moved_r) {

      assert(moved_k);
      assert(moved_r);
    }

void Castling::perform(Board *b) const {
    Color col = moved_k_->getColor();
    int dir = this->castling_permitted(b, col);
    if (dir != 0) {
      b->setPiece({pos_k_.first, pos_k_.second + 2*dir}, moved_k_);
      moved_k_->setPosition({pos_k_.first, pos_k_.second + 2*dir});
      b->removePiece(pos_k_);
      Position pos_k_new = moved_k_->getPosition();
      b->setPiece({pos_r_.first, pos_k_new.second - dir}, moved_r_);
      moved_r_->setPosition({pos_k_.first, pos_k_new.second - dir});
      b->removePiece(pos_r_);
    } else {
      std::cout << "The castling cannot be performed" << '\n' << std::endl;
    }
}

void Castling::unPerform(Board *b) const {
    Color col = moved_k_->getColor();
    int dir = castling_permitted(b, col);
    b->setPiece(pos_k_, moved_k_);
    moved_k_->setPosition(pos_k_);
    b->removePiece({pos_k_.first, pos_k_.second + 2*dir});
    Position pos_k_new = moved_k_->getPosition();
    b->setPiece(pos_r_, moved_r_);
    moved_r_->setPosition(pos_r_);
    b->removePiece({pos_k_.first, pos_k_new.second - dir});
}

int Castling::castling_permitted(Board *b, Color col) {
    int dir;
    unsigned int min;
    unsigned int max;
    if (pos_r_.second < pos_k_.second) {
      min = pos_r_.second;
      max = pos_k_.second;
      dir = -1;
    } else {
      min = pos_k_.second;
      max = pos_r_.second;
      dir = 1;
    }
    if ((moved_r_->toChar() == 'R' && col == BLACK) || (moved_r_->toChar() == 'r' && col == WHITE)) {
      return 0;
    }
    if (Nbmodif_[col] != 0 || pos_k_.first != pos_r_.first || b->isInCheck(col)) {
      return 0;
    }

    for (unsigned int i = 1; i < abs(max-min); i++) {
      if (b[pos_r_.first][min+i] != NULL) {
        return 0;
      }
    }
    BasicMove *move_1 = BasicMove(pos_k_, {pos_k_.first, pos_k_.second + dir}, moved_k_);
    if (!isLegal(move_1)) {
      move_1->unPerform(b);
      return 0;

    } else {
      move_1->perform(b);
      BasicMove *move_2 = BasicMove({pos_k_.first, pos_k_.second + dir}, {pos_k_.first, pos_k_.second + 2*dir}, moved_k_);
      if (!isLegal(move_2)) {
        move_2->unPerform(b);
        move_1->unPerform(b);
        return 0;
      }
      move_2->unPerform(b);
      move_1->unPerform(b);
    }
    return dir;
}
*/
