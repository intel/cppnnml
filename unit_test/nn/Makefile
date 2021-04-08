# Simple Makefile for the neural network unit tests
# Enable C++ iostreams
# Tell the code to compile the activation function LUTs used by the unit tests
nn_unit_test: nn_unit_test.cpp ../../cpp/lookupTables.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/nn
#	Build the unit tests
	g++ -o ~/nn/nn_unit_test nn_unit_test.cpp ../../cpp/lookupTables.cpp -DENABLE_OSTREAMS -DTINYMIND_USE_SIGMOID_8_8 -DTINYMIND_USE_SIGMOID_16_16 -DTINYMIND_USE_LOG_16_16 -DTINYMIND_USE_TANH_8_8 -DTINYMIND_USE_TANH_8_24 -DTINYMIND_USE_TANH_16_16 -DTINYMIND_USE_EXP_16_16 -I../../cpp -I../../apps/include -I${BOOST_HOME}

# Remove all object files
clean:
	rm *.o