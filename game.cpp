#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "game.h"
#include "board.h"
#include "global.h"
#include "piece.h"
#include "concretepieces.h"
#include "move.h"
#include "tree.h"

Game::Game() { }


void Game::play(Move *m) {
    assert(m != NULL);
    m->perform(&board_);
    board_.add_to_achieved_moves(m);
    board_.switch_player();
}

bool Game::undo() {
    Move *move = board_.get_last_move();
    if (move != NULL) {
      move->unPerform(&board_);
      board_.switch_player();
      return true;
    }
    std::cout << "There haven't been any moves done yet" << std::endl;
    return false;
}


void Game::display() {
    board_.display();
}

void Game::displayCaptured() {
    board_.displayCaptured();
}

void Game::promote_pawn(Move *m, std::string last_member) {
    board_.promote_pawn_b(m, last_member);
}

void Game::display_heuristic() {
    std::cout << "The score is " << board_.heuristic() << std::endl;
}

std::vector<Move *> Game::getAllLegalMoves() {
    return board_.getAllLegalMoves();
}

Move *greedy_move(Board b) {
    """
    Returns the moves with the most favorables heuristic value
    """
    std::vector<Move *> moves = b.getAllLegalMoves();
    moves[0]->perform(&b);
    int min_strength = b.heuristic();
    moves[0]->unPerform(&b);
    int max_strength = min_strength;
    int min_idx = 0;
    int max_idx = 0;
    int current;
    for (unsigned int i = 1; i < moves.size(); i++) {
      moves[i]->perform(&b);
      current = b.heuristic();
      if (current < min_strength) {
        min_strength = current;
        min_idx = i;
      } else if (current > max_strength) {
        max_strength = current;
        max_idx = i;
      }
      moves[i]->unPerform(&b);
    }
    if (b.getPlayer()) {
      return moves[max_idx];
    } else {
      return moves[min_idx];
    }
}

int minimax(Board b, int depth, int color) {
    """
    returns the best heuristic value by maximizing the H value in this turn and
    minimizing it in the opponent's turn
    """
    std::vector<Move *> moves = b.getAllLegalMoves();
    if (depth == 0 || moves.size() == 1) {
      return b.heuristic();
    } else if (color) {
      int best_value = MINF;
      for (auto m : moves) {
        m->perform(&b);
        int v = minimax(b, depth-1, 1-color);
        m->unPerform(&b);
        best_value = (v > best_value)? v : best_value;
      }
    } else if (!color) {
      int best_value = INF;
      for (auto m : moves) {
        m->perform(&b);
        int v = minimax(b, depth-1, 1-color);
        m->unPerform(&b);
        best_value = (v < best_value)? v : best_value;
      }
    }
    return 0;
}

Move *minimax_move(Board b, int strength) {
    """
    returns the move with the best minimax() value by performing all of the moves
    """
    std::vector<Move *> moves = b.getAllLegalMoves();
    int color = (int) b.getPlayer();
    moves[0]->perform(&b);
    int min_ = b.heuristic();
    moves[0]->unPerform(&b);
    int max_ = min_;
    int min_idx = 0;
    int max_idx = 0;
    int current;
    for (unsigned int i = 1; i < moves.size(); i++) {
      moves[i]->perform(&b);
      current = minimax(b, strength-1, 1-color);
      if (current < min_) {
        min_ = current;
        min_idx = i;
      } else if (current > max_) {
        max_ = current;
        max_idx = i;
      }
      moves[i]->unPerform(&b);
    }
    if (color) {
      return moves[max_idx];
    } else {
      return moves[min_idx];
    }
}


Move *Game::computerSuggestion(int strength) {
    std::vector<Move *> moves = board_.getAllLegalMoves();
    switch (strength) {
      case 0:
        {
        int rand_idx = rand() % moves.size();
        return moves[rand_idx];
        }
      case 1:
        return greedy_move(board_);
      default:
        return minimax_move(board_, strength);
        break;
    }
    return NULL;
}

void Game::switchColor() {
    board_.switch_player();;
}

Tree *Game::getOpenings() {
    return openings_;
}

void Game::setOpenings(Tree *t) {
    openings_ = t;
}
