# Simple Makefile for the q-learn unit tests
# Enable C++ iostreams
# Tell the code to compile the Q16.16 tanh activation function LUT
qlearn_unit_test: qlearn_unit_test.cpp ../../cpp/lookupTables.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/qlearn
#	Build the unit tests
	g++ -o ~/qlearn/qlearn_unit_test qlearn_unit_test.cpp ../../cpp/lookupTables.cpp -DENABLE_OSTREAMS -I../../cpp -I../../apps/include -DTINYMIND_USE_TANH_16_16 -I${BOOST_HOME}

# Remove all object files
clean:
	rm *.o