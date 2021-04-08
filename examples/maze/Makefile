# Simple Makefile for the table-based q-learner example
maze: maze.cpp mazelearner.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/maze
#	Build the example with default build flags
	g++ -O3 -o ~/maze/maze maze.cpp mazelearner.cpp -I../../cpp

# Remove all object files
clean:
	rm *.o