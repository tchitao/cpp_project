#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <utility>
#include <string>
#include <limits>

// We rely on the property WHITE == true
enum Color { WHITE = 1, BLACK = 0 };

// A position is of the form {i,j} with i,j ∈ 0..7  
// In chess notation {0,0} = A1, {7,7} = G8 
typedef std::pair<unsigned int, unsigned int> Position;

// returns the file letter of p
// e.g. getFileLetter({2,3}) == 'd'
char getFileLetter(Position p);

// returns the rank of p
// e.g. getRank({2,3}) == '3'
char getRank(Position p);

// returns the string made of file letter and rank, 
// e.g. getFileRank({2,3}) == "d3" 
std::string getFileRank(Position p);

const int MINF = std::numeric_limits<int>::min();
const int INF = std::numeric_limits<int>::max();

#endif // GLOBAL_H_
