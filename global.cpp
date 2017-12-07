#include <utility>
#include <cassert>
#include "global.h"

using namespace std;

char getFileLetter(Position p) {
    return p.second + 'a';
}

char getRank(Position p) {
    return p.first + '1';
}

std::string getFileRank(Position p) {
  std::string s = {(char) (p.second + 'a'), (char) (p.first + '1')};
  return s;
}