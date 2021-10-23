# Simple Makefile for the neural network unit tests
# Enable C++ iostreams
# Tell the code to compile the activation function LUTs used by the unit tests
nn_unit_test: nn_unit_test.cpp ../../cpp/lookupTables.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/nn
#	Build the unit tests
	g++ -g -Wall -o ~/nn/nn_unit_test nn_unit_test.cpp ../../cpp/lookupTables.cpp -DENABLE_OSTREAMS -DTINYMIND_USE_SIGMOID_8_8 -DTINYMIND_USE_SIGMOID_16_16 -DTINYMIND_USE_LOG_16_16 -DTINYMIND_USE_TANH_8_8 -DTINYMIND_USE_TANH_8_24 -DTINYMIND_USE_TANH_16_16 -DTINYMIND_USE_EXP_16_16 -I../../cpp -I../../apps/include -I${BOOST_HOME}

nn_unit_test_cov: nn_unit_test.cpp ../../cpp/lookupTables.cpp
#	Build the unit tests with coverage info
	g++ --coverage -o nn_unit_test nn_unit_test.cpp ../../cpp/lookupTables.cpp -DENABLE_OSTREAMS -DTINYMIND_USE_SIGMOID_8_8 -DTINYMIND_USE_SIGMOID_16_16 -DTINYMIND_USE_LOG_16_16 -DTINYMIND_USE_TANH_8_8 -DTINYMIND_USE_TANH_8_24 -DTINYMIND_USE_TANH_16_16 -DTINYMIND_USE_EXP_16_16 -I../../cpp -I../../apps/include -I${BOOST_HOME}
#	Run the executable program
	./nn_unit_test
#	Run lcov
	lcov -t "nn_unit_test" -c -d . -o nn_unit_test_cov.info
#   Make the output dir for HTML reports
	mkdir -p nn_unit_test_cov_html
#   Generate the HTML report
	genhtml -o ./nn_unit_test_cov_html nn_unit_test_cov.info

clean:
	rm -f nn_unit_test
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.txt
	rm -f *.bin
	rm -drf nn_unit_test_cov_html
	rm -f nn_unit_test_cov.info
