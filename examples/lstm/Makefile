# Simple Makefile for the LSTM example program
default :
#	Make an output dir to hold the executable
	mkdir -p ~/lstm
#	Build the example with default build flags
	g++ -O3 -Wall -I../../apps/include -DENABLE_OSTREAMS -DTINYMIND_USE_TANH_8_8 -DTINYMIND_USE_SIGMOID_8_8 -o ~/lstm/lstm ../../cpp/lookupTables.cpp lstm.cpp lstmnet.cpp -I../../cpp

debug:
#	Make an output dir to hold the executable
	mkdir -p ~/lstm
#	Build the example with default build flags
	g++ -g -Wall -I../../apps/include -DENABLE_OSTREAMS -DTINYMIND_USE_TANH_8_8 -DTINYMIND_USE_SIGMOID_8_8 -o ~/lstm/lstm ../../cpp/lookupTables.cpp lstm.cpp lstmnet.cpp -I../../cpp

# Remove all object files
clean:
	rm *.o
