/**
* Copyright (c) 2020 Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

#include "constants.hpp"
#include "nnproperties.hpp"

#include "lstmnet.h"

#define TRAINING_ITERATIONS 2000
#define NUM_SAMPLES_AVG_ERROR 20

static void generateXorTrainingValue(ValueType& x, ValueType& y, ValueType& z)
{
    const int randX = rand() & 0x1;
    const int randY = rand() & 0x1;
    const int result = (randX ^ randY);

    x = ValueType(randX, 0);
    y = ValueType(randY, 0);
    z = ValueType(result, 0);
}

extern NeuralNetworkType testNeuralNet;

int main(const int argc, char *argv[])
{
    using namespace std;

    srand(static_cast<unsigned int>(time(0))); // seed random number generator

    char const* const path = "nn_fixed_lstm_xor.txt";
    ofstream results(path);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType error;

    // tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(testNeuralNet, results);

    for (auto i = 0; i < TRAINING_ITERATIONS; ++i)
    {
        generateXorTrainingValue(values[0], values[1], output[0]);

        testNeuralNet.feedForward(&values[0]);
        error = testNeuralNet.calculateError(&output[0]);
        if (!NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(error))
        {
            testNeuralNet.trainNetwork(&output[0]);
        }
        testNeuralNet.getLearnedValues(&learnedValues[0]);

        // tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(testNeuralNet, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    return 0;
}
