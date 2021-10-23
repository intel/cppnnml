# Simple Makefile for the q-format unit tests
# Enable C++ iostreams
qformat_unit_test: qformat_unit_test.cpp
#	Make an output dir to hold the executable
	mkdir -p ~/qformat
#	Build the unit tests
	g++ -g -Wall -o ~/qformat/qformat_unit_test qformat_unit_test.cpp -DENABLE_OSTREAMS -I../../cpp -I${BOOST_HOME}

# Remove all object files
clean:
	rm *.o