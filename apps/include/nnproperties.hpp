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

#include <cstdlib>
#include <fstream>
#include <vector>
#include <cstdio>

namespace tinymind {
    template<typename SourceType>
    struct ValueParser
    {
        typedef typename SourceType::FullWidthValueType FullWidthValueType;
        typedef int ParsedValueType;

        static int parseValue(char const* const buffer)
        {
            return atoi(buffer);
        }
    };

    template<>
    struct ValueParser<double>
    {
        typedef double FullWidthValueType;
        typedef double ParsedValueType;

        static double parseValue(char const* const buffer)
        {
            return atof(buffer);
        }
    };

    template<typename SourceType, typename DestinationType>
    struct ValueConverter
    {
        static DestinationType convertToDestinationType(const SourceType& value)
        {
            return value;
        }
    };

    template<typename DestinationType>
    struct ValueConverter<double, DestinationType>
    {
        typedef typename DestinationType::FullWidthValueType FullWidthValueType;

        static DestinationType convertToDestinationType(const double& value)
        {
            const FullWidthValueType fullWidthValue = static_cast<FullWidthValueType>(value * static_cast<double>(1 << DestinationType::NumberOfFractionalBits));
            const DestinationType weight(fullWidthValue);

            return weight;
        }
    };

    template<typename SourceType>
    struct ValueConverter<SourceType, double>
    {
        static double convertToDestinationType(const SourceType& value)
        {
            static const double factor = pow(2, -1.0 * SourceType::NumberOfFractionalBits);
            const double result = (static_cast<double>(value.getValue()) * factor);

            return result;
        }
    };

    template<>
    struct ValueConverter<double, double>
    {
        static double convertToDestinationType(const double& value)
        {
            return value;
        }
    };

    template<typename NeuralNetworkType>
    struct NetworkPropertiesFileManager
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        static const size_t NumberOfInputLayerNeurons = NeuralNetworkType::NumberOfInputLayerNeurons;
        static const size_t NumberOfHiddenLayers = NeuralNetworkType::NeuralNetworkNumberOfHiddenLayers;
        static const size_t NumberOfHiddenLayerNeurons = NeuralNetworkType::NumberOfHiddenLayerNeurons;
        static const size_t NumberOfOutputLayerNeurons = NeuralNetworkType::NumberOfOutputLayerNeurons;
        static_assert(NumberOfHiddenLayers <= 2, "> 2 hidden layers not supported yet");

        template<typename SourceType, typename DestinationType>
        static void loadNetworkWeights(NeuralNetworkType& neuralNetwork, std::ifstream& inFile)
        {
            typedef ValueParser<SourceType> ValueParserType;
            typedef ValueConverter<SourceType, DestinationType> ValueConverterType;
            typedef typename ValueParserType::ParsedValueType ParsedValueType;
            char buffer[256];
            ParsedValueType weight;
            ValueType weightValue;
            int hiddenLayer = 0;

            for (uint32_t i = 0; i < NumberOfInputLayerNeurons; ++i)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    inFile.getline(buffer, 255);
                    weight = ValueParserType::parseValue(buffer);
                    weightValue = ValueConverterType::convertToDestinationType(weight);
                    neuralNetwork.setInputLayerWeightForNeuronAndConnection(i, h, weightValue);
                }
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                inFile.getline(buffer, 255);
                weight = ValueParserType::parseValue(buffer);
                weightValue = ValueConverterType::convertToDestinationType(weight);
                neuralNetwork.setInputLayerBiasWeightForConnection(h, weightValue);
            }

            if (2 == NumberOfHiddenLayers)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                    {
                        inFile.getline(buffer, 255);
                        weight = ValueParserType::parseValue(buffer);
                        weightValue = ValueConverterType::convertToDestinationType(weight);
                        neuralNetwork.setHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, h1, weightValue);
                    }
                }

                for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                {
                    inFile.getline(buffer, 255);
                    weight = ValueParserType::parseValue(buffer);
                    weightValue = ValueConverterType::convertToDestinationType(weight);
                    neuralNetwork.setHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, h1, weightValue);
                }

                ++hiddenLayer;
            }

            for (uint32_t hiddenNeuron = 0; hiddenNeuron < NumberOfHiddenLayerNeurons; ++hiddenNeuron)
            {
                for (uint32_t outputNeuron = 0; outputNeuron < NumberOfOutputLayerNeurons; ++outputNeuron)
                {
                    inFile.getline(buffer, 255);
                    weight = ValueParserType::parseValue(buffer);
                    weightValue = ValueConverterType::convertToDestinationType(weight);
                    neuralNetwork.setHiddenLayerWeightForNeuronAndConnection(hiddenLayer, hiddenNeuron, outputNeuron, weightValue);
                }
            }

            for (uint32_t outputNeuron = 0; outputNeuron < NumberOfOutputLayerNeurons; ++outputNeuron)
            {
                inFile.getline(buffer, 255);
                weight = ValueParserType::parseValue(buffer);
                weightValue = ValueConverterType::convertToDestinationType(weight);
                neuralNetwork.setHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, outputNeuron, weightValue);
            }
        }

        template<typename SourceType, typename DestinationType>
        static void loadStates(
                               NeuralNetworkType& neuralNetwork,
                               std::ifstream& inFile,
                               std::vector<SourceType>& sourceTypeStates,
                               std::vector<DestinationType>& destinationTypeStates)
        {
            typedef ValueParser<SourceType> ValueParserType;
            typedef ValueConverter<SourceType, DestinationType> ValueConverterType;
            typedef typename ValueParserType::ParsedValueType ParsedValueType;
            char buffer[256];
            ParsedValueType parsedValue;
            ValueType destinationValue;

            while(!inFile.eof())
            {
                inFile.getline(buffer, 255);
                parsedValue = ValueParserType::parseValue(buffer);
                destinationValue = ValueConverterType::convertToDestinationType(parsedValue);

                sourceTypeStates.push_back(parsedValue);
                destinationTypeStates.push_back(destinationValue);
            }
        }

        static void storeNetworkProperties(NeuralNetworkType& neuralNetwork, std::ofstream& outFile, ValueType const* const output, ValueType const* const learnedValues)
        {
            storeNetworkWeights(neuralNetwork, outFile, ",");

            outFile << output[0] << ",";
            outFile << learnedValues[0] << ",";
        }

        static void storeNetworkWeights(NeuralNetworkType& neuralNetwork, std::ofstream& outFile, char const* const delimiter = "\n")
        {
            int hiddenLayer = 0;

            for (uint32_t i = 0; i < NumberOfInputLayerNeurons; ++i)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    outFile << neuralNetwork.getInputLayerWeightForNeuronAndConnection(i, h) << delimiter;
                }
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                outFile << neuralNetwork.getInputLayerBiasNeuronWeightForConnection(h) << delimiter;
            }

            if (2 == NumberOfHiddenLayers)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                    {
                        outFile << neuralNetwork.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, h1) << delimiter;
                    }
                }

                for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                {
                    outFile << neuralNetwork.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, h1) << delimiter;
                }

                ++hiddenLayer;
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
                {
                    outFile << neuralNetwork.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, o) << delimiter;
                }
            }

            for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
            {
                outFile << neuralNetwork.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, o) << delimiter;
            }
        }

        static void storeNetworkWeights(NeuralNetworkType& neuralNetwork, char const* const binaryFilePath)
        {
            typedef typename NeuralNetworkType::NeuralNetworkValueType::FullWidthValueType FullWidthValueType;
            std::ofstream outFile(binaryFilePath, std::ios::binary);
            FullWidthValueType value;
            int hiddenLayer = 0;

            for (uint32_t i = 0; i < NumberOfInputLayerNeurons; ++i)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    value = neuralNetwork.getInputLayerWeightForNeuronAndConnection(i, h).getValue();
                    outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
                }
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                value = neuralNetwork.getInputLayerBiasNeuronWeightForConnection(h).getValue();
                outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
            }

            if (2 == NumberOfHiddenLayers)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                    {
                        value = neuralNetwork.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, h1).getValue();
                        outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
                    }
                }

                for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                {
                    value = neuralNetwork.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, h1).getValue();
                    outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
                }

                ++hiddenLayer;
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
                {
                    value = neuralNetwork.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, o).getValue();
                    outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
                }
            }

            for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
            {
                value = neuralNetwork.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, o).getValue();
                outFile.write(reinterpret_cast<char*>(&value), sizeof(value));
            }
        }

        static void writeHeader(NeuralNetworkType& neuralNetwork, std::ofstream& outFile)
        {
            int hiddenLayer = 0;

            for (uint32_t i = 0; i < NumberOfInputLayerNeurons; ++i)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    outFile << "Input" << i << h << "Weight,";
                }
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                outFile << "InputBias" << hiddenLayer << h << "Weight,";
            }

            if (2 == NumberOfHiddenLayers)
            {
                for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                    {
                        outFile << "Hidden" << hiddenLayer << h << h1 << "Weight,";
                    }
                }

                for (uint32_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                {
                    outFile << "Hidden0ToHidden1Bias" << h1 << "Weight,";
                }

                ++hiddenLayer;
            }

            for (uint32_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
                {
                    outFile << "Hidden" << hiddenLayer << h << o << "Weight,";
                }
            }

            for (uint32_t o = 0; o < NumberOfOutputLayerNeurons; ++o)
            {
                outFile << "Hidden" << hiddenLayer << "Bias" << o << "Weight,";
            }

            outFile << "Expected,";
            outFile << "Learned,";
            outFile << "Error" << std::endl;
            outFile << std::dec;
        }

    };
}
