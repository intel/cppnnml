# cppnnml
The C++ Neural Network and Machine Learning project is intended to provide a C++ template library for neural nets and machine learning algorithms as well as a Q format library.
The neural networks and machine learning algorithms combined with Q format can be run on embedded systems with very low compute resources. It does not rely upon FPU, GPU, or vectorized
instructions.

The project wiki is at https://github.com/intel/cppnnml/wiki. I would recommend reading thru some of the examples which will guide you in compiling the code. In order
to produce code with the smallest possible footprint, the code relies upon preprocessor symbols defined either on the command line or within the build system
to ensure that only needed code is compiled (e.g. LUTs for activation functions).
