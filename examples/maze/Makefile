# Simple Makefile for the table-based q-learner example
default :
#	Make an output dir to hold the executable
	mkdir -p ~/maze
#	Build the example with default build flags
	g++ -O3 -Wall -o ~/maze/maze maze.cpp mazelearner.cpp -I../../cpp

debug :
#	Make an output dir to hold the executable
	mkdir -p ~/maze
#	Build the example with default build flags
	g++ -g -Wall -o ~/maze/maze maze.cpp mazelearner.cpp -I../../cpp

# Remove all object files
clean:
	rm *.o
