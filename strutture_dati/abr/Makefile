CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17

SRC = main.cpp

EXEC = main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)

rebuild: clean all