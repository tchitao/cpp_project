CXX=g++
SOURCES=main.cpp concretepieces.cpp piece.cpp global.cpp move.cpp game.cpp board.cpp tree.cpp
INCLUDES=concretepieces.h piece.h global.h move.h game.h board.h tree.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
CFLAGS=-c -Wall -std=c++11

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(INCLUDES)
	$(CXX) -o $@ $(OBJECTS)

%.o: %.cpp $(INCLUDES)
	$(CXX) $(CFLAGS) $< -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)


clean:
	rm -rf *.dSYM $(EXECUTABLE) $(OBJECTS)
