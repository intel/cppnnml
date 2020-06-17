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

#include "activation.hpp"
#include "activationFunctions.hpp"
#include "sigmoid.hpp"
#include "tanh.hpp"
#include "interpolate.hpp"
#include "error.hpp"
#include "nninit.hpp"
#include "zeroTolerance.hpp"
#include "constants.hpp"

namespace tinymind {
    template<
            typename ValueType,
            class TransferFunctionRandomNumberGeneratorPolicy,
            class TransferFunctionHiddenNeuronActivationPolicy,
            class TransferFunctionOutputNeuronActivationPolicy,
            unsigned NumberOfOutputNeurons = 1,
            class TransferFunctionNetworkInitializationPolicy = tinymind::DefaultNetworkInitializer<ValueType>,
            class TransferFunctionErrorCalculatorPolicy = tinymind::MeanSquaredErrorCalculator<ValueType, NumberOfOutputNeurons>,
            class TransferFunctionZeroTolerancePolicy = tinymind::ZeroToleranceCalculator<ValueType> >
    struct FixedPointTransferFunctions
    {
        typedef ValueType TransferFunctionsValueType;
        typedef TransferFunctionRandomNumberGeneratorPolicy  RandomNumberGeneratorPolicy;
        typedef TransferFunctionHiddenNeuronActivationPolicy HiddenNeuronActivationPolicy;
        typedef TransferFunctionOutputNeuronActivationPolicy OutputNeuronActivationPolicy;
        typedef TransferFunctionNetworkInitializationPolicy  NetworkIntializationPolicy;
        typedef TransferFunctionErrorCalculatorPolicy        ErrorCalulatorPolicy;
        typedef TransferFunctionZeroTolerancePolicy          ZeroToleranceCalculatorPolicy;
        typedef typename ValueType::FullWidthFieldType       FullWidthFieldType;

        static const unsigned NumberOfTransferFunctionsOutputNeurons = NumberOfOutputNeurons;

        static ValueType calculateError(ValueType const* const targetValues, ValueType const* const outputValues)
        {
            return ErrorCalulatorPolicy::calculateError(targetValues, outputValues);
        }

        static ValueType calculateOutputGradient(const ValueType& targetValue, const ValueType& outputValue)
        {
            const ValueType delta = targetValue - outputValue;

            return (delta * OutputNeuronActivationPolicy::activationFunctionDerivative(outputValue));
        }

        static ValueType generateRandomWeight()
        {
            return RandomNumberGeneratorPolicy::generateRandomWeight();
        }

        static ValueType hiddenNeuronActivationFunction(const ValueType& value)
        {
            return HiddenNeuronActivationPolicy::activationFunction(value);
        }
        
        static ValueType hiddenNeuronActivationFunctionDerivative(const ValueType& value)
        {
            return HiddenNeuronActivationPolicy::activationFunctionDerivative(value);
        }

        static ValueType initialAccelerationRate()
        {
            return NetworkIntializationPolicy::initialAccelerationRate();
        }

        static ValueType initialBiasOutputValue()
        {
            return NetworkIntializationPolicy::initialBiasOutputValue();
        }

        static ValueType initialDeltaWeight()
        {
            return NetworkIntializationPolicy::initialDeltaWeight();
        }

        static ValueType initialGradientValue()
        {
            return NetworkIntializationPolicy::initialGradientValue();
        }

        static ValueType initialLearningRate()
        {
            return NetworkIntializationPolicy::initialLearningRate();
        }

        static ValueType initialMomentumRate()
        {
            return NetworkIntializationPolicy::initialMomentumRate();
        }

        static ValueType initialOutputValue()
        {
            return NetworkIntializationPolicy::initialOutputValue();
        }

        static bool isWithinZeroTolerance(const ValueType& value)
        {
            return ZeroToleranceCalculatorPolicy::isWithinZeroTolerance(value);
        }

        static ValueType negate(const ValueType& value)
        {
            const ValueType result = value * Constants<ValueType>::negativeOne();

            return result;
        }

        static ValueType noOpDeltaWeight()
        {
            return NetworkIntializationPolicy::noOpDeltaWeight();
        }

        static ValueType noOpWeight()
        {
            return NetworkIntializationPolicy::noOpWeight();
        }

        static ValueType outputNeuronActivationFunction(const ValueType& value)
        {
            return OutputNeuronActivationPolicy::activationFunction(value);
        }
        
        static void outputNeuronActivationFunction(ValueType const* const values, ValueType* results, const size_t numberOfNerons)
        {
            OutputNeuronActivationPolicy::activationFunction(values, results, numberOfNerons);
        }

        static ValueType outputNeuronActivationFunctionDerivative(const ValueType& value)
        {
            return OutputNeuronActivationPolicy::activationFunctionDerivative(value);
        }

        static void outputNeuronActivationFunctionDerivative(ValueType const* const values, ValueType const* const targetValues, ValueType* results, const size_t numberOfNerons)
        {
            OutputNeuronActivationPolicy::activationFunctionDerivative(values, targetValues, results, numberOfNerons);
        }
    };
}
