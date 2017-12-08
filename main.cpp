// This module defines the entry point for the program. It
// instanciate a Game object and then runs a simple REPL
// (read-eval-print-loop). Most commands are evaluted by calling a
// corresponding method on the Game object.

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <map>
#include "game.h"
#include "move.h"
#include "piece.h"
#include "tree.h"

bool isFinished(Game &g) {
    return g.getAllLegalMoves().size() == 0;
}

std::vector<int> is_repetetive(std::vector<std::string> moves_str) {
    std::vector<int> rep_idx;
    for (int i = 0; i < (int) moves_str.size(); i++) {
      for (int j = i+1; j < (int) moves_str.size(); j++) {
        if (moves_str[i] == moves_str[j]) {
          rep_idx.push_back(i);
          rep_idx.push_back(j);
        }
      }
    }
    return rep_idx;
}

std::vector<std::string> AlgebraicNotation(std::vector<Move *> moves) {
    std::vector<std::string> moves_str;
    for (auto m : moves) {
      moves_str.push_back(m->toAlgebraicNotation(3));
    }
    std::vector<int> rep_idx = is_repetetive(moves_str);
    int i_AlgebraicNotation = 2;
    while (rep_idx.size() != 0) {
      for (auto idx : rep_idx) {
        moves_str[idx] = moves[idx]->toAlgebraicNotation(i_AlgebraicNotation);
      }
      rep_idx.clear();
      rep_idx = is_repetetive(moves_str);
      i_AlgebraicNotation--;
    }
    return moves_str;
}

// We need to parse a line, construct a Move, and make sure
// the move is valid in the current Game.
// What we do instead is to get all the valid moves, and see
// if line is equal to the string representation of one of these moves.
Move *parseAndValidate(Game &g, const std::string &line) {
    std::vector<Move *> moves = g.getAllLegalMoves();
    std::vector<std::string> moves_str = AlgebraicNotation(moves);
    for (int i = 0; i < (int) moves_str.size(); i++) {
      if (line == moves_str[i]) {
        return moves[i];
      }
    }
    return NULL;
}

// Transforms a string s into a vector of words (substrings not containing
// spaces)
void tokenize(const std::string &s, std::vector<std::string> &tokens) {
    std::istringstream f(s);
    std::string word;
    while (f >> word) {
        tokens.push_back(word);
    }
}

//Process the openings given in the file
void process_openings(Game &g, std::string filename) {
    std::ifstream file(filename);
    if (file) {
      std::string line;
      std::vector<std::string> moves_str;
      getline(file, line);
      int Nb_openings = std::stoi(line);
      std::vector<Move *> moves;
      Tree *t = new Tree();
      for (int i = 0; i < Nb_openings; i++) {
          getline(file, line);
          tokenize(line, moves_str);
          int Nb_moves = std::stoi(moves_str[0]);
          for (int j = 1; j <= Nb_moves; j++) {
              Move *move = parseAndValidate(g, moves_str[j]);
              moves.push_back(move);
              if (move == NULL) {
                  std::cout << "The sequence of moves in the file is unvalid, try '?' for list of moves or 'help'" << std::endl;
                  return;
              }
              g.perform_temp();
          }
          t->addOpening(moves, 0);
          if (Nb_moves % 2 == 1) {
            g.unPerform_temp();
          }
          moves_str.clear();
          moves.clear();
      }
      g.add_openings(t);
      file.close();
    } else {
      std::cout << "Impossible to read the file" << std::endl;
    }
    return;
}

void print_vector(std::vector<Move *> v) {
    std::cout << "vector: ";
    for (auto s : v) {
        std::cout << s->toBasicNotation() << " ";
    }
    std::cout << std::endl;
}

void find_wanted_tree(std::vector<Move *> legalmoves, Tree *t, Move **move_to_do, Tree **t_wanted) {
    std::vector<Move *> allmoves = t->allMoves();
    if (allmoves.size() == 0 || *move_to_do != NULL) {
      return;
    }
    for (auto m : allmoves) {
      for (auto m1 : legalmoves) {
        if (m->toBasicNotation() == m1->toBasicNotation()) {
          *t_wanted = t->playMove(m);
          *move_to_do = m;
          return;
        }
      }
      Tree *t_child = t->playMove(m);
      find_wanted_tree(legalmoves, t_child, move_to_do, t_wanted);
    }
}
/* expression */
// Asks the computer what next move to play.
void computerPlay(Game &g, int strength) {
    if (isFinished(g)) {
       std::cout << "Nothing to play !" << std::endl;
       return;
    }
    Tree *t = g.getOpenings();
    if (t != NULL) {
      std::vector<Move *> legalmoves = g.getAllLegalMoves();
      Move *move_to_do = NULL;
      Tree *t_wanted;
      find_wanted_tree(legalmoves, t, &move_to_do, &t_wanted);
      if (move_to_do != NULL) {
        g.setOpenings(t_wanted);
        g.play(move_to_do);
        std::cout << "Computer played " << move_to_do->toBasicNotation() << std::endl;
        g.display();
        return;
      }
    }
    Move *m = g.computerSuggestion(strength);
    // should not be null as there is always something to play if the game is not
    // finished
    assert(m != NULL);
    g.play(m);
    std::cout << "Computer played " << m->toBasicNotation() << std::endl;
    g.display();
}

void evaluateCommand(Game &g, const std::string &line) {
        std::vector<std::string> commands;
        tokenize(line, commands);
        if (commands.size() == 0) {
            return;
        }
        std::string &command = commands[0];
        if (command == "d") {
            g.display();
        } else if (command == "?") {
            std::vector<Move *> moves = g.getAllLegalMoves();
            std::vector<std::string> moves_str = AlgebraicNotation(moves);
            for (auto x : moves_str) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        } else if (command == "help" || command == "h") {
            std::cout << "*move*: play *move* (type '?' for list of possible moves)" << std::endl;
            std::cout << "play s, p s, p: computer plays next move, s = strength" << std::endl;
            std::cout << "display, d: display current state of the game" << std::endl;
            std::cout << "O-O, O-O-O, O-O: kingside castling, O-O-O queenside castling" << std::endl;
            std::cout << "captured, c: display all the captured pieces during the current game" << std::endl;
            std::cout << "undo, u: cancel last move" << std::endl;
            std::cout << "score, s: display the score of the game" << std::endl;
            std::cout << "openings file.txt, o file.txt: process the openings set in file.txt" << std::endl;
            std::cout << "?: print all possible moves" << std::endl;
            std::cout << "quit, q: quit game" << std::endl;
            std::cout << "help, h: this message" << std::endl;
        } else if (command == "quit" || command == "q") {
            std::cout << "bye bye" << std::endl;
            exit(0);
        } else if (command == "undo" || command == "u") {
            g.undo();
            g.display();
        } else if (command == "openings" || command == "o") {
            std::string filename = commands[1];
            process_openings(g, filename);
        } else if (command == "play" || command == "p") {
            int strength = std::stoi(commands[1]);
            if (strength < 0 || strength > 5) {
              std::cout << "The strength should be between 0 and 5" << '\n';
              exit(0);
            }
            computerPlay(g, strength);
            return;
        } else if (command == "captured" || command == "c") {
          g.displayCaptured();
        } else if (command == "score" || command == "s") {
          g.display_heuristic();
        } else {
            Move *m = parseAndValidate(g, line);
            if (m == NULL) {
               std::string last_member = line.substr(line.size()-1, 1);
               if (last_member == "+") {
                    std::cout << "Check boi" << '\n';
                    m = parseAndValidate(g, line.substr(0, line.size()-1));
                    if (m != NULL) {
                      g.play(m);
                      std::cout << m->toAlgebraicNotation(3) << std::endl;
                      g.display();
                      return;
                    }
               } else if (last_member == "B" || last_member == "R" || last_member == "Q" || last_member == "N") {
                    m = parseAndValidate(g, line.substr(0, line.size()-1));
                    if (m != NULL) {
                      g.play(m);
                      std::cout << line << std::endl;
                      g.promote_pawn(m, last_member);
                      g.display();
                      return;
                    }
               }
               std::cout << "I didn't understand your move, try '?' for list of moves or 'help'" << std::endl;
            } else {
               g.play(m);
               std::cout << m->toAlgebraicNotation(3) << std::endl;
               g.display();
            }
        }
    }

std::vector<std::string> parsing_file() {
  std::ifstream file("Akobian.pgn");
  std::string line;
  std::vector<std::string > game;
  if (file) {
    getline(file, line);
    while (line.length() > 1) {
      std::cout << line.substr(1, line.size()-3) << '\n';
      getline(file, line);
    }
    getline(file, line);
    while (line.substr(line.size()-4, 3) != "0-1" && line.substr(line.size()-4, 3) != "1-0" && line.substr(line.size()-8, 7) != "1/2-1/2") {
      tokenize(line, game);
      getline(file, line);
    }
    tokenize(line, game);
    for (size_t i = 0; i < 17; i+=2) {
      game[i] = game[i].substr(2, game.size()-2);
    }
    for (size_t i = 18; i < game.size(); i+=2) {
      game[i] = game[i].substr(3, game.size()-3);
    }
    game.pop_back();
  } else {
    std::cout << "Make sure that the selected file is a .pgn one" << std::endl;
  }
  return game;
}

int main() {
    Game g;
    std::string line;
    std::vector<std::string> gameMoves = parsing_file();
    for (auto m : gameMoves) {
      std::cout << m << '\n';
      evaluateCommand(g, m);
    }
    while(true) {
        std::cout << "> ";
        getline(std::cin, line);
        evaluateCommand(g, line);
    }
    return 0;
}
