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
    Position pos = p->getPosition();
    if ((pos.first == 3 || pos.first == 4) && (pos.second == 3 || pos.second == 4)) {
      return 2;
    } else if ((pos.first > 1 || pos.first < 6) && (pos.second > 1 || pos.second < 6)) {
      return 1;
    }
    return 0;
}

int Board::heuristic() {
    int sign[2] = {1, -1};
    int res;
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
        std::vector<Move *> moves = getAllMoves();
        std::vector<Move *> res;
        for (auto x : moves) {
            if (isLegal(x)) {
                res.push_back(x);
           }
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
