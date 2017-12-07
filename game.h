#ifndef GAME_H_
#define GAME_H_

#include <vector>
#include <stack>
#include <string>
#include "piece.h"
#include "move.h"
#include "board.h"
#include "tree.h"

// This class defines a game as seen by the 'main' module. It has the following
// roles:
// 1. It is a wrapper to a Board object
// 2. It maintains the history of the game
// 3. It implements a computer opponent

class Tree;

class Game {
public:
    Game();

    void display();

    void displayCaptured();

    void display_heuristic();

    bool undo();

    std::vector<Move *> getAllLegalMoves();

    Move *computerSuggestion(int strength);

    void play(Move *);

    void castling(std::string);

    void setMove(Move *move);

    void perform_temp();

    void unPerform_temp();

    void add_openings(Tree *);

    Tree *getOpenings();

    void setOpenings(Tree *);

    void setNbmoves_small(int, int);

    void setNbmoves_big(int , int);

private:

    Board board_;
    Tree *openings_ = NULL;
    std::vector<Move *> achieved_moves_;
    int Nbmoves_small_[2] = {0, 0};
    int Nbmoves_big_[2] = {0, 0};
};

#endif // GAME_H_
