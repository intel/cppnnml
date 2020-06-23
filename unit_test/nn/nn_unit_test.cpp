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

// nn_unit_test.cpp : Defines the entry point for the neural network template unit tests.
#define BOOST_TEST_MODULE nn_unit_test
#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <string.h>

#include "qformat.hpp"
#include "neuralnet.hpp"
#include "activationFunctions.hpp"
#include "fixedPointTransferFunctions.hpp"
#include "random.hpp"
#include "nnproperties.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>
#include <vector>
#include <numeric>
#include <deque>

namespace tinymind {
    template<>
    struct TanhActivationPolicy<double>
    {
        static double activationFunction(const double& value)
        {
            return tanh(value);
        }

        static double activationFunctionDerivative(const double& value)
        {
            //Approximation for 1st derivative of tanh
            return (static_cast<double>(1.0) - (value * value));
        }
    };
}

namespace tinymind {
    template<>
    struct ZeroToleranceCalculator<double>
    {
        static bool isWithinZeroTolerance(const double& value)
        {
            static const double zeroTolerance(0.004);
            static const double negativeTolerance = (static_cast<double>(-1.0) * zeroTolerance);

            return ((0 == value) || ((value < zeroTolerance) && (value > negativeTolerance)));
        }
    };
}

using namespace std;

#define TRAINING_ITERATIONS 2000
#define NUM_SAMPLES_AVG_ERROR 20
#define STOP_ON_AVG_ERROR 0

template<typename ValueType>
struct ValueHelper
{
    typedef typename ValueType::FullWidthValueType FullWidthValueType;

    static FullWidthValueType getErrorLimit()
    {
        static const FullWidthValueType ERROR_LIMIT = (1 << (ValueType::NumberOfFractionalBits - 6));

        return ERROR_LIMIT;
    }
};

template<>
struct ValueHelper<double>
{
    static double getErrorLimit()
    {
        return 0.1;
    }
};

template<typename ValueType>
struct UniformRealRandomNumberGenerator
{
    typedef tinymind::ValueConverter<double, ValueType> WeightConverterPolicy;

    static ValueType generateRandomWeight()
    {
        const double temp = distribution(generator);
        const ValueType weight = WeightConverterPolicy::convertToDestinationType(temp);

        return weight;
    }

private:
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution;
};

template<typename ValueType>
std::default_random_engine UniformRealRandomNumberGenerator<ValueType>::generator;

template<typename ValueType>
std::uniform_real_distribution<double> UniformRealRandomNumberGenerator<ValueType>::distribution(-1.0, 1.0);

template<
        typename ValueType,
        template<typename> class TransferFunctionRandomNumberGeneratorPolicy,
        template<typename> class TransferFunctionHiddenNeuronActivationPolicy,
        template<typename> class TransferFunctionOutputNeuronActivationPolicy,
        template<typename> class TransferFunctionGatedNeuronActivationPolicy = tinymind::NullActivationPolicy,
        template<typename> class TransferFunctionZeroTolerancePolicy = tinymind::ZeroToleranceCalculator,
        unsigned NumberOfOutputNeurons = 1>
struct FloatingPointTransferFunctions
{
    typedef ValueType TransferFunctionsValueType;
    typedef TransferFunctionRandomNumberGeneratorPolicy<ValueType> RandomNumberGeneratorPolicy;
    typedef TransferFunctionHiddenNeuronActivationPolicy<ValueType> HiddenNeuronActivationPolicy;
    typedef TransferFunctionOutputNeuronActivationPolicy<ValueType> OutputNeuronActivationPolicy;
    typedef TransferFunctionGatedNeuronActivationPolicy<ValueType> GatedNeuronActivationPolicy;
    typedef TransferFunctionZeroTolerancePolicy<ValueType> ZeroToleranceCalculatorPolicy;

    static const unsigned NumberOfTransferFunctionsOutputNeurons = NumberOfOutputNeurons;

    static ValueType calculateError(ValueType const* const targetValues, ValueType const* const outputValues)
    {
        ValueType error(0);

        //calculate overall error RMS
        for (uint32_t neuron = 0; neuron < NumberOfOutputNeurons; neuron++)
        {
            const ValueType delta = (targetValues[neuron] - outputValues[neuron]);
            error += (delta * delta);
        }

        if (NumberOfOutputNeurons > 1)
        {
            error /= NumberOfOutputNeurons;
        }

        return error;
    }

    static ValueType calculateOutputGradient(const ValueType& targetValue, const ValueType& outputValue)
    {
        const ValueType delta = targetValue - outputValue;

        return (delta * OutputNeuronActivationPolicy::activationFunctionDerivative(outputValue));
    }

    static ValueType gateActivationFunction(const ValueType& value)
    {
        return GatedNeuronActivationPolicy::activationFunction(value);
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

    static ValueType outputNeuronActivationFunction(const ValueType& value)
    {
        return OutputNeuronActivationPolicy::activationFunction(value);
    }
    
    static ValueType outputNeuronActivationFunctionDerivative(const ValueType& value)
    {
        return OutputNeuronActivationPolicy::activationFunctionDerivative(value);
    }

    static ValueType initialAccelerationRate()
    {
        ValueType rate(0.1);

        return rate;
    }

    static ValueType initialBiasOutputValue()
    {
        const ValueType value(1.0);

        return value;
    }

    static ValueType initialDeltaWeight()
    {
        const ValueType delta(0);

        return delta;
    }

    static ValueType initialGradientValue()
    {
        const ValueType gradient(0);

        return gradient;
    }

    static ValueType initialLearningRate()
    {
        ValueType rate(0.15);

        return rate;
    }

    static ValueType initialMomentumRate()
    {
        ValueType rate(0.5);

        return rate;
    }

    static ValueType initialOutputValue()
    {
        const ValueType value(0);

        return value;
    }

    static bool isWithinZeroTolerance(const ValueType& value)
    {
        return ZeroToleranceCalculatorPolicy::isWithinZeroTolerance(value);
    }

    static ValueType negate(const ValueType& value)
    {
        return (-1.0 * value);
    }

    static ValueType neuronActivationFunction(const ValueType& value)
    {
        return HiddenNeuronActivationPolicy::activationFunction(value);
    }

    static ValueType neuronActivationFunctionDerivative(const ValueType& value)
    {
        return HiddenNeuronActivationPolicy::activationFunctionDerivative(value);
    }

    static ValueType noOpDeltaWeight()
    {
        const ValueType value(1.0);

        return value;
    }

    static ValueType noOpWeight()
    {
        const ValueType value(1.0);

        return value;
    }
};

template<typename T>
static void generateXorValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x ^ y;

    *values = static_cast<T>(x);
    ++values;
    *values = static_cast<T>(y);
    *output = z;
}

template<typename T>
static void generateFixedPointXorValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x ^ y;

    *values = static_cast<T>(x << T::NumberOfFractionalBits);
    ++values;
    *values = static_cast<T>(y << T::NumberOfFractionalBits);
    *output = (z << T::NumberOfFractionalBits);
}

template<typename T>
static void generateAndValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x & y;

    *values = static_cast<T>(x);
    ++values;
    *values = static_cast<T>(y);
    *output = z;
}

template<typename T>
static void generateFixedPointAndValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x & y;

    *values = static_cast<T>(x << T::NumberOfFractionalBits);
    ++values;
    *values = static_cast<T>(y << T::NumberOfFractionalBits);
    *output = (z << T::NumberOfFractionalBits);
}

template<typename T>
static void generateOrValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x | y;

    *values = static_cast<T>(x);
    ++values;
    *values = static_cast<T>(y);
    *output = z;
}

template<typename T>
static void generateFixedPointOrValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = x | y;

    *values = static_cast<T>(x << T::NumberOfFractionalBits);
    ++values;
    *values = static_cast<T>(y << T::NumberOfFractionalBits);
    *output = (z << T::NumberOfFractionalBits);
}

template<typename T>
static void generateFixedPointNorValues(T* values, T* output)
{
    const uint32_t x = (rand() & 1);
    const uint32_t y = (rand() & 1);
    const uint32_t z = !(x | y);

    *values = static_cast<T>(x << T::NumberOfFractionalBits);
    ++values;
    *values = static_cast<T>(y << T::NumberOfFractionalBits);
    *output = (z << T::NumberOfFractionalBits);
}

template<typename T>
static void generateRecurrentValues(T* values, T* output)
{
    static int32_t x = -1;
    static int32_t y = 0;
    const int32_t z = x + y;

    *values = static_cast<T>(x);
    ++values;
    *values = static_cast<T>(y);
    *output = static_cast<T>(z);

    if (++x > 1)
        x = -1;

    if (++y > 1)
        y = -1;
}

template<typename T>
static void generateFixedPointRecurrentValues(T* values, T* output)
{
    static int32_t x = -1;
    static int32_t y = 0;
    const int32_t z = x + y;

    *values = static_cast<T>(x << T::NumberOfFractionalBits);
    ++values;
    *values = static_cast<T>(y << T::NumberOfFractionalBits);
    *output = (z << T::NumberOfFractionalBits);

    if (++x > 1)
        x = -1;

    if (++y > 1)
        y = -1;
}

template<typename NeuralNetworkType>
static void testFloatingPointNN(    NeuralNetworkType& neuralNetwork,
                                    void(*pValuesFn)(typename NeuralNetworkType::NeuralNetworkValueType*, typename NeuralNetworkType::NeuralNetworkValueType*),
                                    char const* const path,
                                    const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    static const ValueType ERROR_LIMIT = 0.1;
    ofstream results(path);
    ofstream weightsOutputFile;
    std::string weightsOutputPath(path);
    std::deque<double> errors;
    ValueType error;

    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    double learnedValues[NeuralNetworkType::NumberOfOutputLayerNeurons];

    weightsOutputPath.replace(weightsOutputPath.find("."), std::string::npos, "_weights.txt");

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < numberOfTrainingIterations; ++i)
    {
        pValuesFn(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        if (!NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(error))
        {
            neuralNetwork.trainNetwork(&output[0]);
        }
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error);
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const double totalError = std::accumulate(errors.begin(), errors.end(), static_cast<double>(0));
            const double averageError = (totalError / static_cast<double>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }

        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    weightsOutputFile.open(weightsOutputPath.c_str());

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkWeights(neuralNetwork, weightsOutputFile);

    weightsOutputFile.close();

    const double totalError = std::accumulate(errors.begin(), errors.end(), static_cast<double>(0));
    const double averageError = (totalError / static_cast<double>(NUM_SAMPLES_AVG_ERROR));
    BOOST_TEST(averageError <= ERROR_LIMIT);
}

template<typename NeuralNetworkType>
static void testFloatingPointNN_Xor(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFloatingPointNN(neuralNetwork, generateXorValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFloatingPointNN_And(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFloatingPointNN(neuralNetwork, generateAndValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFloatingPointNN_Or(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFloatingPointNN(neuralNetwork, generateOrValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork(  NeuralNetworkType& neuralNetwork,
                                void(*pValuesFn)(typename NeuralNetworkType::NeuralNetworkValueType*, typename NeuralNetworkType::NeuralNetworkValueType*),
                                char const* const path,
                                const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    typedef typename ValueType::FullWidthValueType FullWidthValueType;
    typedef ValueHelper<ValueType> ValueHelperType;
    static const FullWidthValueType ERROR_LIMIT = ValueHelperType::getErrorLimit();
    ofstream results(path);
    ofstream weightsOutputFile;
    std::string weightsOutputPath(path);
    std::string binaryWeightsOutputPath(path);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    std::deque<FullWidthValueType> errors;
    ValueType error;

    weightsOutputPath.replace(weightsOutputPath.find("."), std::string::npos, "_weights.txt");
    binaryWeightsOutputPath.replace(binaryWeightsOutputPath.find(".txt"), std::string::npos, ".bin");

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < numberOfTrainingIterations; ++i)
    {
        pValuesFn(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        if (!NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(error))
        {
            neuralNetwork.trainNetwork(&output[0]);
        }
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error.getValue());
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
            const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }
        
        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
    const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
    
    weightsOutputFile.open(weightsOutputPath.c_str());

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkWeights(neuralNetwork, weightsOutputFile);
    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkWeights(neuralNetwork, binaryWeightsOutputPath.c_str());

    weightsOutputFile.close();

    BOOST_TEST(averageError <= ERROR_LIMIT);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Xor(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFixedPointNeuralNetwork(neuralNetwork, generateFixedPointXorValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_And(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFixedPointNeuralNetwork(neuralNetwork, generateFixedPointAndValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Or(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFixedPointNeuralNetwork(neuralNetwork, generateFixedPointOrValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Nor(NeuralNetworkType& neuralNetwork, char const* const path, const int numberOfTrainingIterations = TRAINING_ITERATIONS)
{
    testFixedPointNeuralNetwork(neuralNetwork, generateFixedPointNorValues, path, numberOfTrainingIterations);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_No_Train( NeuralNetworkType& neuralNetwork,
                                        void(*pValuesFn)(typename NeuralNetworkType::NeuralNetworkValueType*, typename NeuralNetworkType::NeuralNetworkValueType*),
                                        char const* const path,
                                        char const* const weightsInputPath)
{
    typedef tinymind::NetworkPropertiesFileManager<NeuralNetworkType> NetworkPropertiesFileManagerType;
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    typedef typename ValueType::FullWidthValueType FullWidthValueType;
    typedef ValueHelper<ValueType> ValueHelperType;
    static const FullWidthValueType ERROR_LIMIT = ValueHelperType::getErrorLimit();
    ofstream results(path);
    ifstream weightsInputFile(weightsInputPath);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    std::deque<FullWidthValueType> errors;
    ValueType error;

    NetworkPropertiesFileManagerType::template loadNetworkWeights<ValueType, ValueType>(neuralNetwork, weightsInputFile);
    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < TRAINING_ITERATIONS; ++i)
    {
        pValuesFn(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error.getValue());
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
            const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }

        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
    const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
    
    BOOST_TEST(averageError <= ERROR_LIMIT);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Xor_No_Train(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train(neuralNetwork, generateFixedPointXorValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_And_No_Train(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train(neuralNetwork, generateFixedPointAndValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Or_No_Train(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train(neuralNetwork, generateFixedPointOrValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Nor_No_Train(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train(neuralNetwork, generateFixedPointNorValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_No_Train_Float_Weights( NeuralNetworkType& neuralNetwork,
                                                                void(*pValuesFn)(typename NeuralNetworkType::NeuralNetworkValueType*, typename NeuralNetworkType::NeuralNetworkValueType*),
                                                                char const* const path,
                                                                char const* const weightsInputPath)
{
    typedef tinymind::NetworkPropertiesFileManager<NeuralNetworkType> NetworkPropertiesFileManagerType;
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    typedef typename ValueType::FullWidthValueType FullWidthValueType;
    typedef ValueHelper<ValueType> ValueHelperType;
    static const FullWidthValueType ERROR_LIMIT = ValueHelperType::getErrorLimit();
    ofstream results(path);
    ifstream weightsInputFile(weightsInputPath);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    std::deque<FullWidthValueType> errors;
    ValueType error;

    NetworkPropertiesFileManagerType::template loadNetworkWeights<double, ValueType>(neuralNetwork, weightsInputFile);
    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < TRAINING_ITERATIONS; ++i)
    {
        pValuesFn(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error.getValue());
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
            const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }

        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
    const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
    
    BOOST_TEST(averageError <= ERROR_LIMIT);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train_Float_Weights(neuralNetwork, generateFixedPointXorValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_And_No_Train_Float_Weights(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train_Float_Weights(neuralNetwork, generateFixedPointAndValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testFixedPointNeuralNetwork_Or_No_Train_Float_Weights(NeuralNetworkType& neuralNetwork, char const* const path, char const* const weightsInputPath)
{
    testFixedPointNeuralNetwork_No_Train_Float_Weights(neuralNetwork, generateFixedPointOrValues, path, weightsInputPath);
}

template<typename NeuralNetworkType>
static void testNeuralNetwork_Recurrent(NeuralNetworkType& neuralNetwork, char const* const path)
{
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    typedef typename ValueType::FullWidthValueType FullWidthValueType;
    typedef ValueHelper<ValueType> ValueHelperType;
    static const FullWidthValueType ERROR_LIMIT = ValueHelperType::getErrorLimit();
    ofstream results(path);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    std::deque<FullWidthValueType> errors;
    ValueType error;

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < TRAINING_ITERATIONS; ++i)
    {
        generateFixedPointRecurrentValues(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        if (!NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(error))
        {
            neuralNetwork.trainNetwork(&output[0]);
        }
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error.getValue());
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
            const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }

        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
    const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
    
    BOOST_TEST(averageError <= ERROR_LIMIT);
}

template<typename NeuralNetworkType>
static void testFloatingPointNeuralNetwork_Recurrent(NeuralNetworkType& neuralNetwork, char const* const path)
{
    typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
    typedef double FullWidthValueType;
    typedef ValueHelper<ValueType> ValueHelperType;
    static const FullWidthValueType ERROR_LIMIT = ValueHelperType::getErrorLimit();
    ofstream results(path);
    ValueType values[NeuralNetworkType::NumberOfInputLayerNeurons];
    ValueType output[NeuralNetworkType::NumberOfOutputLayerNeurons];
    ValueType learnedValues[NeuralNetworkType::NumberOfInputLayerNeurons];
    std::deque<FullWidthValueType> errors;
    ValueType error;

    tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::writeHeader(neuralNetwork, results);

    for (int i = 0; i < TRAINING_ITERATIONS; ++i)
    {
        generateRecurrentValues(values, output);

        neuralNetwork.feedForward(&values[0]);
        error = neuralNetwork.calculateError(&output[0]);
        if (!NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(error))
        {
            neuralNetwork.trainNetwork(&output[0]);
        }
        neuralNetwork.getLearnedValues(&learnedValues[0]);

        errors.push_front(error);
        if (errors.size() > NUM_SAMPLES_AVG_ERROR)
        {
            errors.pop_back();

#if STOP_ON_AVG_ERROR
            const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
            const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
            if (averageError <= ERROR_LIMIT)
            {
                break;
            }
#endif // STOP_ON_AVG_ERROR
        }

        tinymind::NetworkPropertiesFileManager<NeuralNetworkType>::storeNetworkProperties(neuralNetwork, results, &output[0], &learnedValues[0]);
        results << error << std::endl;
    }

    const FullWidthValueType totalError = std::accumulate(errors.begin(), errors.end(), static_cast<FullWidthValueType>(0));
    const FullWidthValueType averageError = (totalError / static_cast<FullWidthValueType>(NUM_SAMPLES_AVG_ERROR));
    
    BOOST_TEST(averageError <= ERROR_LIMIT);
}

BOOST_AUTO_TEST_SUITE(test_suite_nn)

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Or(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_nor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_nor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Nor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    static constexpr bool TRAINABLE = false;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train(nn, path, "nn_fixed_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    static constexpr bool TRAINABLE = false;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And_No_Train(nn, path, "nn_fixed_and_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    static constexpr bool TRAINABLE = false;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Or_No_Train(nn, path, "nn_fixed_or_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_nor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    static constexpr bool TRAINABLE = false;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_nor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Nor_No_Train(nn, path, "nn_fixed_nor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_5_hidden_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_5_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_5_hidden_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_5_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_5_hidden_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_5_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Or(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_16_16_nn_5_hidden_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 16;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 16;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_16_16_5_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_16_16_nn_5_hidden_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 16;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 16;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_16_16_5_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_16_16_nn_5_hidden_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 16;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 16;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_16_16_5_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Or(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_8_24_nn_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_8_24_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_8_24_nn_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_8_24_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_8_24_nn_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_8_24_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Or(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_2_8_24_nn_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 2;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_2_8_24_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_Xor(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_2_8_24_nn_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 2;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_2_8_24_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_And(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_2_8_24_nn_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 2;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_2_8_24_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_Or(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_4_8_24_nn_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 4;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_4_8_24_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_Xor(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_4_8_24_nn_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 4;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_4_8_24_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_And(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_batch_4_8_24_nn_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = true;
    static constexpr size_t BATCH_SIZE = 4;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE,
                                            BATCH_SIZE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_batch_4_8_24_or.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    const ValueType learningRate(nn.getLearningRate() / 4);
    const ValueType accelerationRate(nn.getAccelerationRate() / 4);
    const ValueType momentumRate(nn.getMomentumRate() / 4);

    nn.setLearningRate(learningRate);
    nn.setAccelerationRate(accelerationRate);
    nn.setMomentumRate(momentumRate);

    testFixedPointNeuralNetwork_Or(nn, path, 10000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_elman_nn)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::ElmanNetwork< ValueType,
                                    NUMBER_OF_INPUTS,
                                    NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                    NUMBER_OF_OUTPUTS,
                                    TransferFunctionsType> FixedPointElmanNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_elman.txt";
    FixedPointElmanNetworkType nn;

    testNeuralNetwork_Recurrent(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_elman_nn)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::TanhActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::ElmanNetwork< ValueType,
                                    NUMBER_OF_INPUTS,
                                    NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                    NUMBER_OF_OUTPUTS,
                                    TransferFunctionsType> FloatingPointElmanNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_elman.txt";
    FloatingPointElmanNetworkType nn;

    testFloatingPointNeuralNetwork_Recurrent(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_nn_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::TanhActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FloatingPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_xor.txt";
    FloatingPointMultiLayerPerceptronNetworkType nn;

    testFloatingPointNN_Xor(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_nn_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::TanhActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FloatingPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_and.txt";
    FloatingPointMultiLayerPerceptronNetworkType nn;

    testFloatingPointNN_And(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_nn_or)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::TanhActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FloatingPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_or.txt";
    FloatingPointMultiLayerPerceptronNetworkType nn;

    testFloatingPointNN_Or(nn, path);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_float_weights_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = false;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_float_weights_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(nn, path, "nn_float_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_no_train_float_weights_and)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = false;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_no_train_float_weights_and.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_And_No_Train_Float_Weights(nn, path, "nn_float_and_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_nn_relu_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::ReluActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FloatingPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_relu_xor.txt";
    FloatingPointMultiLayerPerceptronNetworkType nn;

    nn.setLearningRate(0.005);
    nn.setAccelerationRate(0.03);
    nn.setMomentumRate(0.16);

    testFloatingPointNN_Xor(nn, path, 200000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_relu_xor_no_train)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = false;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 16;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 16;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    tinymind::NullRandomNumberPolicy<ValueType>,
                                                    tinymind::ReluActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_relu_xor_no_train.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(nn, path, "nn_float_relu_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_8_24_nn_relu_xor_no_train)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = false;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 24;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    tinymind::NullRandomNumberPolicy<ValueType>,
                                                    tinymind::ReluActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_8_24_relu_xor_no_train.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(nn, path, "nn_float_relu_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_8_8_nn_relu_xor_no_train)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr bool TRAINABLE = false;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    tinymind::NullRandomNumberPolicy<ValueType>,
                                                    tinymind::ReluActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType,
                                            TRAINABLE> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_8_8_relu_xor_no_train.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(nn, path, "nn_float_relu_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_floatingpoint_2_hidden_nn_relu_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 2;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    typedef double ValueType;
    typedef FloatingPointTransferFunctions<
                                            ValueType,
                                            UniformRealRandomNumberGenerator,
                                            tinymind::ReluActivationPolicy,
                                            tinymind::TanhActivationPolicy> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FloatingPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_float_2_hidden_relu_xor.txt";
    FloatingPointMultiLayerPerceptronNetworkType nn;

    nn.setLearningRate(0.005);
    nn.setAccelerationRate(0.03);
    nn.setMomentumRate(0.16);

    testFloatingPointNN_Xor(nn, path, 100000);
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_2_hidden_nn_relu_xor_no_train)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 2;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 5;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 16;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 16;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    tinymind::NullRandomNumberPolicy<ValueType>,
                                                    tinymind::ReluActivationPolicy<ValueType>,
                                                    tinymind::TanhActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron<ValueType, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_NEURONS_PER_HIDDEN_LAYER, NUMBER_OF_OUTPUTS, TransferFunctionsType, false> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_2_hidden_relu_xor_no_train.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor_No_Train_Float_Weights(nn, path, "nn_float_2_hidden_relu_xor_weights.txt");
}

BOOST_AUTO_TEST_CASE(test_case_fixedpoint_nn_sigmoid_xor)
{
    static constexpr size_t NUMBER_OF_INPUTS = 2;
    static constexpr size_t NUMBER_OF_HIDDEN_LAYERS = 1;
    static constexpr size_t NUMBER_OF_NEURONS_PER_HIDDEN_LAYER = 3;
    static constexpr size_t NUMBER_OF_OUTPUTS = 1;
    static constexpr size_t NUMBER_OF_FIXED_BITS = 8;
    static constexpr size_t NUMBER_OF_FRACTIONAL_BITS = 8;
    typedef tinymind::QValue<NUMBER_OF_FIXED_BITS, NUMBER_OF_FRACTIONAL_BITS, true, tinymind::RoundUpPolicy> ValueType;
    typedef tinymind::FixedPointTransferFunctions<
                                                    ValueType,
                                                    UniformRealRandomNumberGenerator<ValueType>,
                                                    tinymind::SigmoidActivationPolicy<ValueType>,
                                                    tinymind::SigmoidActivationPolicy<ValueType>> TransferFunctionsType;
    typedef tinymind::MultilayerPerceptron< ValueType,
                                            NUMBER_OF_INPUTS,
                                            NUMBER_OF_HIDDEN_LAYERS,
                                            NUMBER_OF_NEURONS_PER_HIDDEN_LAYER,
                                            NUMBER_OF_OUTPUTS,
                                            TransferFunctionsType> FixedPointMultiLayerPerceptronNetworkType;
    srand(static_cast<unsigned int>(time(NULL)));
    char const* const path = "nn_fixed_sigmoid_xor.txt";
    FixedPointMultiLayerPerceptronNetworkType nn;

    testFixedPointNeuralNetwork_Xor(nn, path, 75000);
}

BOOST_AUTO_TEST_SUITE_END()
