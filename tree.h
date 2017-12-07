#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <map>

#include "move.h"
// This class defines a tree as seen by the 'main' module.


class Tree {
public:
    Tree();

    void addOpening(const std::vector<Move *> &opening, int i);

    std::vector<Move *> allMoves();

    Tree *playMove(Move *move);

    void print_vector();

private:
    // a tree is a map from string (ie. move) to trees
    std::map<Move *, Tree *> children_;

};

#endif // TREE_H_
