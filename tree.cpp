// This is a simple implementation of a "prefix tree"
// https://en.wikipedia.org/wiki/Trie
// used to represent and play openings in chess.
//
// An *opening* is a sequence of move. For instance, the following openings
// 1. a, f
// 2. a, b
// 3. a, b, h
// 4. a, c
// 5. e, f, g
// correspond the tree
//
//   --a--*-f--*
//   |    |--b-*-h-*
// *-|    |--c-----*
//   |
//    -e--*-f-*-g--*

#include <iostream>
#include <cassert>
#include <map>
#include <vector>

#include "tree.h"
#include "move.h"

Tree::Tree() { }


// add the opening given in `opening` vector starting from index i
// ...exercise to the reader: use an iterator instead
void Tree::addOpening(const std::vector<Move *> &opening, int i) {
    if ((size_t)i == opening.size()) {
        return;
    }
    Move *move = opening[i];
    Tree *t = NULL;
    if (children_.find(move) == children_.end()) {
        t = new Tree();
        children_[move] = t;
    } else {
        t = children_[move];
    }
    t->addOpening(opening, i+1);
}

// print all possible moves from current node/tree
std::vector<Move *> Tree::allMoves() {
    std::vector<Move *> res;
    // kv goes over all pairs in map children_
    // kv = (kv.first, kv.second)
    for (const auto &kv : children_) {
        res.push_back(kv.first);
    }
    return res;
}

// return node/tree after playing move s
// from current node/tree
Tree *Tree::playMove(Move *move) {
    assert(children_.find(move) != children_.end());
    return children_[move];
}


// print a vector of string on one line
void Tree::print_vector() {
    std::cout << "vector: ";
    for (auto s : this->allMoves()) {
        std::cout << s->toBasicNotation() << " ";
    }
    std::cout << std::endl;
}
/*
int main() {
    Tree *t = new Tree();

    // add all the openings to the initially empty tree
    t->addOpening({"a", "f"}, 0);
    t->addOpening({"a", "b"}, 0);
    t->addOpening({"a", "b", "h"}, 0);
    t->addOpening({"a", "c"}, 0);
    t->addOpening({"e", "f", "g"}, 0);

    // initially, moves a and e are possible
    print_vector(t->allMoves());
    // if we play a
    t = t->playMove("a");
    // moves b, c and f become possible
    print_vector(t->allMoves());
    t = t->playMove("b");
    print_vector(t->allMoves());
}
*/
