all: processor

CXX = g++

EXTRA_FLAGS = -std=c++11 -o processor -g

SRC = main.cpp processor.cpp

processor: $(SRC)
	$(CXX) $(EXTRA_FLAGS) $(SRC)

clean:
	rm processor
