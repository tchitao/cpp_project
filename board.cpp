#include "board.h"
#include "piece.h"
#include "concretepieces.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstring>
#include <utility>
#include <string>

int pawn_strength(Piece *p) {
  """
  returns the weight of the pawn depending on its position
  """
  Color color = p->getColor();
  int step[2] = {1, -1};
  Position pos = p->getPosition();
  unsigned int color_idx[2] = {0, 7};
  if (pos.first == (color_idx[color] + step[color])) {
    return 4;
  } else if (pos.first == (color_idx[color] + 2*step[color])) {
    if (pos.second > 5 || pos.second < 2) {
      return 2;
    }
    return 3;
  } else if (pos.first == (color_idx[color] + 3*step[color])) {
    if (pos.second > 4 || pos.second < 3) {
      return 1;
    }
    return 2;
  }
  return 0;
}

int piece_strength(Piece *p) {
    """
    returns the weight of the piece depending on its position
    """
    Position pos = p->getPosition();
    if ((pos.first == 3 || pos.first == 4) && (pos.second == 3 || pos.second == 4)) {
      return 2;
    } else if ((pos.first > 1 || pos.first < 6) && (pos.second > 1 || pos.second < 6)) {
      return 1;
    }
    return 0;
}

int Board::heuristic() {
    """
    calculates the heuristic value at this point of the game
    as explained in the report
    """
    int sign[2] = {1, -1};
    int res = 0;
    std::vector<Move *> moves = getAllLegalMoves();
    if (moves.size() == 0) {
      if (current_player_ == BLACK) {
        return INF;
      }
      return MINF;
    }
    for (size_t i = 0; i < 2; i++) {
      for (auto p : pieces_[i]) {
        if (p->isCaptured()) {
          switch (p->notation()) {
            case ' ':
              res += sign[i]*3;
              break;
            case 'N':
            case 'B':
              res += sign[i]*3*3;
              break;
            case 'R':
              res += sign[i]*5*3;
              break;
            case 'Q':
              res += sign[i]*10*3;
              break;
            default:
              break;
          }
        } else {
          switch (p->notation()) {
            case ' ':
              res += sign[1-i]*pawn_strength(p);
              break;
            case 'N':
            case 'B':
              res += sign[1-i]*3*piece_strength(p);
              break;
            case 'R':
              res += sign[1-i]*5*piece_strength(p);
              break;
            case 'Q':
              res += sign[1-i]*10*piece_strength(p);
              break;
            default:
              break;
          }
        }
      }
    }
    return res;
}

bool Board::isInside(int i, int j) const {
    return i >= 0 && i < 8 && j >= 0 && j < 8;
}

void Board::reachablePositionsAlongStraightLine(Position start, int di, int dj,
    int max, Color pl, bool canCapture, std::vector<Position> &res) const {
    int step = 0;
    int si = start.first;
    int sj = start.second;
    while (true) {
        step++;
        si += di;
        sj += dj;
        if (!isInside(si, sj) ||
            step > max) {
            break;
        }
        if (board_[si][sj] != NULL &&
            (board_[si][sj]->getColor() == pl || !canCapture)) {
            break;
        }
        res.push_back({si, sj});
        if (board_[si][sj] != NULL) {
            break;
        }
    }
}

void Board::filter(Position start, const std::vector<Position> &rel, Color pl,
    bool onlyIfCapture, std::vector<Position> &res) const {
    int si = start.first;
    int sj = start.second;
    for (auto d : rel) {
        int nsi = si + d.first;
        int nsj = sj + d.second;
        if (!isInside(nsi, nsj)) {
            continue;
        }
        Piece *p = board_[nsi][nsj];
        if ((p != NULL) && p->getColor() == pl) {
            continue;
        }
        if (onlyIfCapture && p == NULL) {
            continue;
        }
        res.push_back({nsi,nsj});
    }
}

Board::Board() {
    memset(board_, (int) NULL, 64 * sizeof(Piece *));
    for (int i = 0; i < 8; i++) {
        board_[6][i] = addPiece(new Pawn({6,i}, BLACK));
        board_[1][i] = addPiece(new Pawn({1,i}, WHITE));
    }

    board_[7][0] = addPiece(new Rook({7,0}, BLACK));
    board_[7][1] = addPiece(new Knight({7,1}, BLACK));
    board_[7][2] = addPiece(new Bishop({7,2}, BLACK));
    board_[7][3] = addPiece(new Queen({7,3}, BLACK));
    board_[7][4] = king_[BLACK] = addPiece(new King({7,4}, BLACK));
    board_[7][5] = addPiece(new Bishop({7,5}, BLACK));
    board_[7][6] = addPiece(new Knight({7,6}, BLACK));
    board_[7][7] = addPiece(new Rook({7,7}, BLACK));

    board_[0][0] = addPiece(new Rook({0,0}, WHITE));
    board_[0][1] = addPiece(new Knight({0,1}, WHITE));
    board_[0][2] = addPiece(new Bishop({0,2}, WHITE));
    board_[0][3] = addPiece(new Queen({0,3}, WHITE));
    board_[0][4] = king_[WHITE] = addPiece(new King({0,4}, WHITE));
    board_[0][5] = addPiece(new Bishop({0,5}, WHITE));
    board_[0][6] = addPiece(new Knight({0,6}, WHITE));
    board_[0][7] = addPiece(new Rook({0,7}, WHITE));

}

Piece * Board::addPiece(Piece *p) {
  pieces_[p->getColor()].push_back(p);
  return p;
}

Color Board::getPlayer() const {
    return current_player_;
}

void Board::switch_player() {
    current_player_ = current_player_?BLACK:WHITE;
}

void Board::display() {
    std::string player = current_player_?"White":"Black";
    std::cout << " Current player " << player << std::endl;
    std::cout << "  ABCDEFGH " << std::endl;
    for (int i = 7; i >= 0; i--) {
        std::cout << i+1 << " ";
        for (int j = 0; j < 8; j++) {
            if (board_[i][j]) {
                std::cout << board_[i][j]->toChar();
            } else {
                std::cout << ' ';
            }
        }
        std::cout << " " << i+1 << std::endl;
    }
    std::cout << "  ABCDEFGH" << std::endl;
    if (isInCheck(WHITE)) {
        std::cout << "White is checked" << std::endl;
    }
    if (isInCheck(BLACK)) {
        std::cout << "Black is checked" << std::endl;
    }
    if (getAllLegalMoves().size() == 0) {
        std::cout << "Game over" << std::endl;
    }

}

int idx_inside_cap(std::vector< std::pair<int, char> > cap, char p) {
    """
    returns the index of the pair that contains the char p in the vector of pair
    if not found returns -1
    """
    if (cap.size() == 0) {
      return -1;
    }
    for (size_t i = 0; i < cap.size(); i++) {
      if (cap[i].second == p) {
        return i;
      }
    }
    return -1;
}

std::string found_pieces(std::pair<int, char> p) {
    """
    return a string with the number of pieces and its type

    Example:
    found_pieces({2, 'k'}) gives us '2 Rooks'
    """
    std::string res = std::to_string(p.first);
    switch (p.second) {
      case 'p' :
      case 'P':
        res += " Pawn";
        break;
      case 'B':
      case 'b':
        res += " Bishop";
        break;
      case 'K':
      case 'k':
        res += "King";
        break;
      case 'R':
      case 'r':
        res += "Rook";
        break;
      case 'Q':
      case 'q':
        res += "Queen";
        break;
      case 'N':
      case 'n':
        res += "Knight";
        break;
      default:
        break;
    }
    if (p.first != 1) {
      res += "s";
    }
    return res;
}

void Board::displayCaptured(){
    """
    displays the captured pieces in this form for example:

    The captured white pieces are:
    6 Pawns
    1 Rook
    2 Knights
    The captured black pieces are:
    4 Pawns
    2 Bishops
    1 Knight
    """
    std::vector< std::string> col(2);
    col[0] = "black";
    col[1] = "white";
    std::vector< std::pair<int, char> > cap;
    for (size_t k = 0; k < 2; k++) {
      for (auto p : pieces_[k]) {
        if (p->isCaptured()) {
          if (idx_inside_cap(cap, p->toChar()) == -1) {
            std::pair<int, char> temp;
            temp.first = 1;
            temp.second = p->toChar();
            cap.push_back(temp);
          } else {
            cap[idx_inside_cap(cap, p->toChar())].first += 1;
          }
        }
      }
      if (cap.size() != 0) {
        std::cout << "The captured " << col[k] <<" pieces are: " << '\n';
        for (size_t i = 0; i < cap.size(); i++) {
          std::cout << found_pieces(cap[i]) << '\n';
        }
      }
    cap.clear();
  }
  std::cout << std::endl;
}

std::vector<Move *> Board::getAllMoves(Color player) const {
        std::vector<Move *> moves;
        for (auto p : pieces_[player]) {
            if (p->isCaptured()) {
                continue;
            }
            p->getMoves(*this, moves);
        }
        return moves;
}

std::vector<Move *> Board::getAllMoves() const {
        return getAllMoves(current_player_);
}

std::vector<Move *> Board::getAllLegalMoves() {
        int line[2] = {7, 0};
        int color = (int) current_player_;
        std::vector<Move *> moves = getAllMoves();
        std::vector<Move *> res;
        for (auto x : moves) {
            if (isLegal(x)) {
                res.push_back(x);
           }
        }
        if ((*this).castling_permitted(board_[line[color]][4], board_[line[color]][7], 2, line[color])) {
            Castling *move = new Castling(board_[line[color]][4], board_[line[color]][7]);
            res.push_back(move);
        } else if ((*this).castling_permitted(board_[line[color]][4], board_[line[color]][0], 3, line[color])) {
            Castling *move = new Castling(board_[line[color]][4], board_[line[color]][0]);
            res.push_back(move);
        }
        return res;
}

bool Board::isLegal(Move *m) {
    bool res = false;
    m->perform(this);
    res = !isInCheck(current_player_);
    m->unPerform(this);
    return res;
}

bool Board::getPiece(Position pos, Piece **p) const {
    *p = board_[pos.first][pos.second];
    return *p != NULL;
}

void Board::setPiece(Position pos, Piece *p) {
    assert(p);
    board_[pos.first][pos.second] = p;
}

void Board::removePiece(Position pos) {
    board_[pos.first][pos.second] = NULL;
}

bool Board::isInCheck(Color p) const {
    Color other = p?BLACK:WHITE;
    std::vector<Move *> moves = getAllMoves(other);
    Piece *k = king_[p];
    for (auto m : moves) {
        if (m->doesCapture(k)) {
            return true;
        }
    }
    return false;
}

void Board::add_to_achieved_moves(Move *move) {
    achieved_moves_.push_back(move);
}

Move *Board::get_last_move() {
  Move *move = NULL;
  if (achieved_moves_.size() != 0) {
    move = achieved_moves_.back();
    achieved_moves_.pop_back();
  }
  return move;
}

void Board::promote_pawn_b(Move *m, std::string last_member) {
    """
    Removes the pawn to be promoted and creating the wanted piece
    """
    Position pos = m->getPosition_promotion();
    int line[2] = {0, 7};
    this->removePiece(pos);
    this->switch_player();
    if (pos.first != line[current_player_]) {
      this->switch_player();
      return;
    }

    if (last_member == "B") {
        board_[pos.first][pos.second] = addPiece(new Bishop({pos.first, pos.second}, current_player_));
    } else if (last_member == "R") {
      board_[pos.first][pos.second] = addPiece(new Rook({pos.first, pos.second}, current_player_));
    } else if (last_member == "Q") {
      board_[pos.first][pos.second] = addPiece(new Queen({pos.first, pos.second}, current_player_));
    } else {
      board_[pos.first][pos.second] = addPiece(new Knight({pos.first, pos.second}, current_player_));
    }
    this->switch_player();
}

bool Board::castling_permitted(Piece *moved_k_, Piece *moved_r_, int moves_todo, int line) {
    """
    Checks if the castling move is permitted
    """

    int dir = (moves_todo == 2)? 1 : -1;

    if ((moved_r_ == NULL) || (moved_k_ == NULL)) {
      return false;
    }
    if ((moved_r_->notation() != 'R') || (moved_k_->notation() != 'K')) {
      return false;
    }
    //Is the king in check
    if ((*this).isInCheck()) {
      return false;
    }
    //Are the pieces between the king and rook empty ?
    for (int i = 1; i < moves_todo+1; i++) {
      if (board_[line][4+i] != NULL) {
        return false;
      }
    }
    //Check if the king doesn't pass through or end up in a square that is
    // under attack by an enemy piece
    BasicMove move_1 = BasicMove({line, 4}, {line, 4 + dir}, moved_k_);
    if (!((*this).isLegal(&move_1))) {
      return false;
    } else {
      move_1.perform(this);
      BasicMove move_2 = BasicMove({line, 4 + dir}, {line, 4 + 2*dir}, moved_k_);
      if (!((*this).isLegal(&move_2))) {
        move_1.unPerform(this);
        return false;
      } else if (moves_todo == 3) {
        move_2.perform(this);
        BasicMove move_3 = BasicMove({line, 4 + 2*dir}, {line, 4 + 3*dir}, moved_k_);
        if (!((*this).isLegal(&move_3))) {
          move_2.unPerform(this);
          move_1.unPerform(this);
          return false;
        }
        move_2.unPerform(this);
      }
      move_1.unPerform(this);
    }

    return true;
}
