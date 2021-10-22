# Simple Makefile for the DQN maze example
# Tell the code to compile the Q16.16 tanh activation function LUT
xor: dqn_maze.cpp dqn_mazelearner.cpp ../../cpp/lookupTables.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/dqn_maze
#	Build the example with default build flags
	g++ -O3 -Wall -o ~/dqn_maze/dqn_maze dqn_maze.cpp dqn_mazelearner.cpp ../../cpp/lookupTables.cpp -I../../cpp -I../../apps/include/ -DTINYMIND_USE_TANH_16_16

# Remove all object files
clean:
	rm *.o
