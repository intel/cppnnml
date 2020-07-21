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

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <new>

#include "qformat.hpp"
#include "activationFunctions.hpp"
#include "fixedPointTransferFunctions.hpp"
#include "neuralnet.hpp"

// Q-Format value type
static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
// typedef the underlying full-width representation type
typedef typename ValueType::FullWidthValueType FullWidthValueType;
// Neural network architecture
static constexpr size_t NUMBER_OF_INPUTS = 2;
static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
static constexpr size_t NUMBER_OF_OUTPUTS = 1;

// Random number generator
struct RandomNumberGenerator
{
    static ValueType generateRandomWeight()
    {
        // Generate a random number between -1..1 in the Q Format full width type
        const FullWidthValueType weight = (rand() % 
                                                    (tinymind::Constants<ValueType>::one().getValue() +
                                                     tinymind::Constants<ValueType>::one().getValue() -
                                                     tinymind::Constants<ValueType>::negativeOne().getValue())) +
                                                     tinymind::Constants<ValueType>::negativeOne().getValue();

        return weight;
    }
};

// Typedef of transfer functions for the fixed-point neural network
typedef tinymind::FixedPointTransferFunctions<  ValueType,
                                                RandomNumberGenerator,
                                                tinymind::TanhActivationPolicy<ValueType>,
                                                tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;

// typedef the neural network itself
typedef tinymind::MultilayerPerceptron< ValueType,
                                        NUMBER_OF_INPUTS,
                                        NUMBER_OF_HIDDEN_LAYERS,
                                        NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                        NUMBER_OF_OUTPUTS,
                                        TransferFunctionsType> NeuralNetworkType;
