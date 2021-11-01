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

#include "activationFunctions.hpp"

namespace tinymind {
    enum class layerType_e
    {
        InputLayerType,
        LstmInputLayerType,
        HiddenLayerType,
        GruHiddenLayerType,
        LstmHiddenLayerType,
        RecurrentLayerType,
        OutputLayerType,
        ClassifierOutputLayerType,
    };

    typedef enum
    {
        FeedForwardOutputLayerConfiguration,
        ClassifierOutputLayerConfiguration,
    } outputLayerConfiguration_e;

    typedef enum
    {
        NonRecurrentHiddenLayerConfiguration,
        RecurrentHiddenLayerConfiguration,
        GRUHiddenLayerConfiguration,
        LSTMHiddenLayerConfiguration,
    } hiddenLayerConfiguration_e;

    template<typename ValueType, size_t NumberOfGradients, size_t BatchSize>
    struct GradientsHolder
    {
        static_assert(BatchSize > 0, "Invalid batch size.");

        GradientsHolder() : mIndex(0), mPass(0)
        {
            size_t bufferIndex;
            for(size_t gradient = 0;gradient < NumberOfGradients;++gradient)
            {
                bufferIndex = gradient * sizeof(ValueType);
                new (&this->mGradientsBuffer[bufferIndex]) ValueType();
            }

            this->resetGradients();
        }

        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
            ValueType averageGradient;
            ValueType* pValue;
            size_t bufferIndex;

            if(BatchSize == this->mPass)
            {
                bufferIndex = this->mIndex * sizeof(ValueType);
                pValue = reinterpret_cast<ValueType*>(&this->mGradientsBuffer[bufferIndex]);
                averageGradient = (*pValue / BatchSize);

                layer.setBiasNeuronGradientForConnection(nextNeuron, gradient);

                *pValue = 0;
            }

            this->updateGradients(gradient);
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
            ValueType averageGradient;
            ValueType* pValue;
            size_t bufferIndex;
            
            if(BatchSize == this->mPass)
            {
                bufferIndex = this->mIndex * sizeof(ValueType);
                pValue = reinterpret_cast<ValueType*>(&this->mGradientsBuffer[bufferIndex]);
                averageGradient = (*pValue / BatchSize);
                
                layer.setGradientForNeuronAndConnection(neuron, nextNeuron, averageGradient);

                *pValue = 0;
            }

            this->updateGradients(gradient);
        }
    private:
        void resetGradients()
        {
            ValueType* pValue;
            size_t bufferIndex;
            for(size_t gradient = 0;gradient < NumberOfGradients;++gradient)
            {
                bufferIndex = gradient * sizeof(ValueType);
                pValue = reinterpret_cast<ValueType*>(&this->mGradientsBuffer[bufferIndex]);
                *pValue = 0;
            }
        }

        void updateGradients(const ValueType& gradient)
        {
            ValueType oldGradient;
            ValueType* pValue;
            size_t bufferIndex;

            bufferIndex = this->mIndex * sizeof(ValueType);
            pValue = reinterpret_cast<ValueType*>(&this->mGradientsBuffer[bufferIndex]);
            oldGradient = *pValue;
            *pValue = (oldGradient + gradient);
            ++this->mIndex;
            if(NumberOfGradients == this->mIndex)
            {
                this->mIndex = 0;
                ++this->mPass;
                if(this->mPass > BatchSize)
                {
                    this->mPass = 1;
                }
            }
        }

        size_t mIndex;
        size_t mPass;
        unsigned char mGradientsBuffer[NumberOfGradients * sizeof(ValueType)];
    };

    template<typename ValueType, size_t NumberOfGradients>
    struct GradientsHolder<ValueType, NumberOfGradients, 1>
    {
        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
            layer.setBiasNeuronGradientForConnection(nextNeuron, gradient);
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
            layer.setGradientForNeuronAndConnection(neuron, nextNeuron, gradient);
        }
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct NullGradientsManager
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;

        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
        }
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct GradientsManager
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        static constexpr size_t NumberOfInputLayerNeurons = InputLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfInnerHiddenLayerNeurons = InnerHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfLastHiddenLayerNeurons = LastHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfOutputLayerNeurons = OutputLayerType::NumberOfNeuronsInLayer;

        static constexpr size_t LastHiddenToOutputNumGradients = (NumberOfLastHiddenLayerNeurons * NumberOfOutputLayerNeurons) + NumberOfOutputLayerNeurons;
        static constexpr size_t InnerHiddenToLastHiddenNumGradients = (NumberOfInnerHiddenLayerNeurons * NumberOfLastHiddenLayerNeurons) + NumberOfLastHiddenLayerNeurons;
        static constexpr size_t InnerToInnerNumGradients = ((NumberOfInnerHiddenLayers - 1) * NumberOfInnerHiddenLayerNeurons * NumberOfInnerHiddenLayerNeurons) + NumberOfInnerHiddenLayerNeurons;
        static constexpr size_t InputToHiddenNumGradients = (NumberOfInputLayerNeurons * NumberOfInnerHiddenLayerNeurons) + NumberOfInnerHiddenLayerNeurons;
        static constexpr size_t NumberOfGradients = (LastHiddenToOutputNumGradients + InnerHiddenToLastHiddenNumGradients + InnerToInnerNumGradients + InputToHiddenNumGradients);

        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateBiasGradients(layer, nextNeuron, gradient);
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateGradients(layer, neuron, nextNeuron, gradient);
        }
    private:
        GradientsHolder<ValueType, NumberOfGradients, NeuralNetworkType::NeuralNetworkBatchSize> gradientsHolder;
    };

    template<typename NeuralNetworkType>
    struct GradientsManager<NeuralNetworkType, 1>
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        static constexpr size_t NumberOfInputLayerNeurons = InputLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfInnerHiddenLayerNeurons = InnerHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfLastHiddenLayerNeurons = LastHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfOutputLayerNeurons = OutputLayerType::NumberOfNeuronsInLayer;

        static constexpr size_t LastHiddenToOutputNumGradients = (NumberOfLastHiddenLayerNeurons * NumberOfOutputLayerNeurons) + NumberOfOutputLayerNeurons;
        static constexpr size_t InnerHiddenToLastHiddenNumGradients = (NumberOfInnerHiddenLayerNeurons * NumberOfLastHiddenLayerNeurons) + NumberOfLastHiddenLayerNeurons;
        static constexpr size_t InputToHiddenNumGradients = (NumberOfInputLayerNeurons * NumberOfInnerHiddenLayerNeurons) + NumberOfInnerHiddenLayerNeurons;        
        static constexpr size_t NumberOfGradients = (LastHiddenToOutputNumGradients + InnerHiddenToLastHiddenNumGradients + InputToHiddenNumGradients);

        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateBiasGradients(layer, nextNeuron, gradient);
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateGradients(layer, neuron, nextNeuron, gradient);
        }
    private:
        GradientsHolder<ValueType, NumberOfGradients, NeuralNetworkType::NeuralNetworkBatchSize> gradientsHolder;
    };

    template<typename NeuralNetworkType>
    struct GradientsManager<NeuralNetworkType, 0>
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        static constexpr size_t NumberOfInputLayerNeurons = InputLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfHiddenLayerNeurons = LastHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfOutputLayerNeurons = OutputLayerType::NumberOfNeuronsInLayer;
        
        static constexpr size_t HiddenToOutputNumGradients = (NumberOfHiddenLayerNeurons * NumberOfOutputLayerNeurons) + NumberOfOutputLayerNeurons;
        static constexpr size_t InputToHiddenNumGradients = (NumberOfInputLayerNeurons * NumberOfHiddenLayerNeurons) + NumberOfHiddenLayerNeurons;
        static constexpr size_t NumberOfGradients = HiddenToOutputNumGradients + InputToHiddenNumGradients;

        template<typename LayerType>
        void updateBiasGradients(LayerType& layer, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateBiasGradients(layer, nextNeuron, gradient);
        }

        template<typename LayerType>
        void updateGradients(LayerType& layer, const size_t neuron, const size_t nextNeuron, const ValueType& gradient)
        {
            this->gradientsHolder.updateGradients(layer, neuron, nextNeuron, gradient);
        }
    private:
        GradientsHolder<ValueType, NumberOfGradients, NeuralNetworkType::NeuralNetworkBatchSize> gradientsHolder;
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers, bool IsTrainable>
    struct GradientsManagerSelector
    {
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct GradientsManagerSelector<NeuralNetworkType, NumberOfInnerHiddenLayers, true>
    {
        typedef GradientsManager<NeuralNetworkType, NumberOfInnerHiddenLayers> GradientsManagerType;
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct GradientsManagerSelector<NeuralNetworkType, NumberOfInnerHiddenLayers, false>
    {
        typedef NullGradientsManager<NeuralNetworkType, NumberOfInnerHiddenLayers> GradientsManagerType;
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct BackPropConnectionWeightUpdater
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        template<typename TrainingPolicyType>
        static void updateConnectionWeights(TrainingPolicyType& trainingPolicy, NeuralNetworkType& nn)
        {
            OutputLayerType& outputLayer = nn.getOutputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            InputLayerType& inputLayer = nn.getInputLayer();

            trainingPolicy.updateConnectionWeights(outputLayer, lastHiddenLayer);

            trainingPolicy.updateConnectionWeights(lastHiddenLayer, pInnerHiddenLayers[NumberOfInnerHiddenLayers - 1]);

            for (int hiddenLayer = static_cast<int>(NumberOfInnerHiddenLayers) - 1; hiddenLayer > 0; --hiddenLayer)
            {
                trainingPolicy.updateConnectionWeights(pInnerHiddenLayers[hiddenLayer], pInnerHiddenLayers[hiddenLayer - 1]);
            }

            trainingPolicy.updateConnectionWeights(pInnerHiddenLayers[0], inputLayer);
        }

    private:
        static_assert(NeuralNetworkType::RecurrentConnectionDepth == 0, "Invalid use of BackPropConnectionWeightUpdater.");
    };

    template<typename NeuralNetworkType>
    struct BackPropConnectionWeightUpdater<NeuralNetworkType, 1>
    {
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        template<typename TrainingPolicyType>
        static void updateConnectionWeights(TrainingPolicyType& trainingPolicy, NeuralNetworkType& nn)
        {
            OutputLayerType& outputLayer = nn.getOutputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            InputLayerType& inputLayer = nn.getInputLayer();

            trainingPolicy.updateConnectionWeights(outputLayer, lastHiddenLayer);

            trainingPolicy.updateConnectionWeights(lastHiddenLayer, pInnerHiddenLayers[0]);

            trainingPolicy.updateConnectionWeights(pInnerHiddenLayers[0], inputLayer);
        }

    private:
        static_assert(NeuralNetworkType::NeuralNetworkRecurrentConnectionDepth == 0, "Invalid use of BackPropConnectionWeightUpdater.");
    };

    template<typename NeuralNetworkType>
    struct BackPropConnectionWeightUpdater<NeuralNetworkType, 0>
    {
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        template<typename TrainingPolicyType>
        static void updateConnectionWeights(TrainingPolicyType& trainingPolicy, NeuralNetworkType& nn)
        {
            OutputLayerType& outputLayer = nn.getOutputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            InputLayerType& inputLayer = nn.getInputLayer();

            trainingPolicy.updateConnectionWeights(outputLayer, lastHiddenLayer);

            trainingPolicy.updateConnectionWeights(lastHiddenLayer, inputLayer);
        }

    private:
        static_assert(NeuralNetworkType::NeuralNetworkRecurrentConnectionDepth == 0, "Invalid use of BackPropConnectionWeightUpdater.");
    };

    template<typename NeuralNetworkType>
    struct BackPropThruTimeConnectionWeightUpdater
    {
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkRecurrentLayerType RecurrentLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;

        template<typename TrainingPolicyType>
        static void updateConnectionWeights(TrainingPolicyType& trainingPolicy, NeuralNetworkType& nn)
        {
            OutputLayerType& outputLayer = nn.getOutputLayer();
            RecurrentLayerType& recurrentLayer = nn.getRecurrentLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            InputLayerType& inputLayer = nn.getInputLayer();

            trainingPolicy.updateConnectionWeights(outputLayer, lastHiddenLayer);

            trainingPolicy.updateConnectionWeights(lastHiddenLayer, recurrentLayer);

            trainingPolicy.updateConnectionWeights(lastHiddenLayer, inputLayer);
        }

    private:
        static_assert(NeuralNetworkType::NeuralNetworkRecurrentConnectionDepth > 0, "Invalid use of BackPropThruTimeConnectionWeightUpdater.");
    };

    template<typename ValueType, size_t NumberOfBiasNeurons>
    struct BiasNeuronConnectionWeightUpdater
    {
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdater<ValueType, 1>
    {
        template<typename LayerType>
        static void updateBiasConnectionWeights(LayerType& previousLayer, const size_t neuron, const ValueType& learningRate, const ValueType& momentumRate, const ValueType& accelerationRate)
        {
            const ValueType previousDeltaWeight = previousLayer.getBiasNeuronPreviousDeltaWeightForConnection(neuron);
            const ValueType currentDeltaWeight = previousLayer.getBiasNeuronDeltaWeightForConnection(neuron);
            const ValueType newDeltaWeight = (learningRate * previousLayer.getBiasNeuronGradientForConnection(neuron)) + (momentumRate * currentDeltaWeight) + (accelerationRate * previousDeltaWeight);
            const ValueType currentWeight = previousLayer.getBiasNeuronWeightForConnection(neuron);

            previousLayer.setBiasNeuronDeltaWeightForConnection(neuron, newDeltaWeight);
            previousLayer.setBiasNeuronWeightForConnection(neuron, (currentWeight + newDeltaWeight));
        }
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdater<ValueType, 0>
    {
        template<typename LayerType>
        static void updateBiasConnectionWeights(LayerType& layer, const size_t neuron, const ValueType& learningRate, const ValueType& momentumRate, const ValueType& accelerationRate)
        {
        }
    };

    template<typename ValueType, layerType_e LayerType, layerType_e PreviousLayerType, size_t NumberOfBiasNeurons>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector
    {
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::InputLayerType, 0>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 0> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::InputLayerType, 1>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 1> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::HiddenLayerType, 0>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 0> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::HiddenLayerType, 1>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 1> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::RecurrentLayerType, 0>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 0> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::HiddenLayerType, layerType_e::RecurrentLayerType, 1>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 1> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::RecurrentLayerType, layerType_e::HiddenLayerType, 0>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 0> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::RecurrentLayerType, layerType_e::HiddenLayerType, 1>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 1> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::OutputLayerType, layerType_e::HiddenLayerType, 0>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 0> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename ValueType>
    struct BiasNeuronConnectionWeightUpdaterTypeSelector<ValueType, layerType_e::OutputLayerType, layerType_e::HiddenLayerType, 1>
    {
        typedef BiasNeuronConnectionWeightUpdater<ValueType, 1> BiasNeuronConnectionWeightUpdaterType;
    };

    template<typename TransferFunctionsPolicy, hiddenLayerConfiguration_e HiddenLayerConfiguration>
    struct NodeDeltasCalculator
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;
        
        template<typename LayerType, typename NextLayerType>
        static void calculateAndSetNodeDeltas(LayerType& layer, const NextLayerType& nextLayer)
        {
            ValueType sum;
            ValueType nodeDelta;
            ValueType outputValue;

            for(size_t neuron = 0; neuron < LayerType::NumberOfNeuronsInLayer;++neuron)
            {
                sum = 0;
                for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
                {
                    sum += (layer.getWeightForNeuronAndConnection(neuron, nextNeuron) * nextLayer.getNodeDeltaForNeuron(nextNeuron));
                }

                outputValue = layer.getOutputValueForNeuron(neuron);

                nodeDelta = sum * TransferFunctionsPolicy::hiddenNeuronActivationFunctionDerivative(outputValue);

                layer.setNodeDeltaForNeuron(neuron, nodeDelta);
            }
        }
    };

    template<typename TransferFunctionsPolicy>
    struct NodeDeltasCalculator<TransferFunctionsPolicy, LSTMHiddenLayerConfiguration>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename LayerType, typename NextLayerType>
        static void calculateAndSetNodeDeltas(LayerType& layer, const NextLayerType& nextLayer)
        {
            ValueType sum;
            ValueType nodeDelta;
            ValueType outputValue;

            for(size_t neuron = 0; neuron < LayerType::NumberOfNeuronsInLayer;++neuron)
            {
                sum = 0;
                for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
                {
                    sum += (layer.getWeightForNeuronAndConnection(neuron, nextNeuron) * nextLayer.getNodeDeltaForNeuron(nextNeuron));
                }

                outputValue = layer.getOutputValueForNeuron(neuron);

                nodeDelta = sum * TransferFunctionsPolicy::hiddenNeuronActivationFunctionDerivative(outputValue);

                layer.setNodeDeltaForNeuron(neuron, nodeDelta);
            }
        }
    };

    template<typename TransferFunctionsPolicy, typename OutputLayerType>
    struct OutputLayerNodeDeltasCalculator
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static void calculateOutputLayerNodeDeltas(OutputLayerType& outputLayer, ValueType const* const targetValues)
        {
            ValueType error;
            ValueType outputValueForNeuron;
            ValueType activationFunctionDerivative;
            ValueType nodeDelta;

            for(size_t neuron = 0; neuron < OutputLayerType::NumberOfNeuronsInLayer;++neuron)
            {
                outputValueForNeuron = outputLayer.getOutputValueForNeuron(neuron);
                error = (targetValues[neuron] - outputValueForNeuron);
                activationFunctionDerivative = TransferFunctionsPolicy::outputNeuronActivationFunctionDerivative(outputValueForNeuron);
                nodeDelta = (error * activationFunctionDerivative);
                
                outputLayer.setNodeDeltaForNeuron(neuron, nodeDelta);
            }
        }
    };

    template<typename TransferFunctionsPolicy, typename OutputLayerType>
    struct ClassificationOutputLayerNodeDeltasCalculator
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;
        
        static void calculateOutputLayerNodeDeltas(OutputLayerType& outputLayer, ValueType const* const targetValues)
        {
            ValueType outputValues[OutputLayerType::NumberOfNeuronsInLayer];
            ValueType results[OutputLayerType::NumberOfNeuronsInLayer];

            for(size_t neuron = 0; neuron < OutputLayerType::NumberOfNeuronsInLayer;++neuron)
            {
                outputValues[neuron] = outputLayer.getOutputValueForNeuron(neuron);
            }

            TransferFunctionsPolicy::outputNeuronActivationFunctionDerivative(&outputValues[0], targetValues, &results[0], OutputLayerType::NumberOfNeuronsInLayer);

            for(size_t neuron = 0; neuron < OutputLayerType::NumberOfNeuronsInLayer;++neuron)
            {
                outputLayer.setNodeDeltaForNeuron(neuron, results[neuron]);
            }
        }
    };

    template<typename TransferFunctionsPolicy, typename OutputLayerType, outputLayerConfiguration_e OutputLayerConfiguration>
    struct OutputLayerNodeDeltasCalculatorChooser
    {
    };

    template<typename TransferFunctionsPolicy, typename OutputLayerType>
    struct OutputLayerNodeDeltasCalculatorChooser<TransferFunctionsPolicy, OutputLayerType, FeedForwardOutputLayerConfiguration>
    {
        typedef OutputLayerNodeDeltasCalculator<TransferFunctionsPolicy, OutputLayerType> OutputLayerNodeDeltasCalculatorType;
    };

    template<typename TransferFunctionsPolicy, typename OutputLayerType>
    struct OutputLayerNodeDeltasCalculatorChooser<TransferFunctionsPolicy, OutputLayerType, ClassifierOutputLayerConfiguration>
    {
        typedef ClassificationOutputLayerNodeDeltasCalculator<TransferFunctionsPolicy, OutputLayerType> OutputLayerNodeDeltasCalculatorType;
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct NetworkDeltasCalculator
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef NodeDeltasCalculator<TransferFunctionsPolicy, NeuralNetworkType::NeuralNetworkHiddenLayerConfiguration> NodeDeltasCalculatorType;
        typedef typename OutputLayerNodeDeltasCalculatorChooser<
                        TransferFunctionsPolicy,
                        OutputLayerType,
                        NeuralNetworkType::NeuralNetworkOutputLayerConfiguration>::OutputLayerNodeDeltasCalculatorType OutputLayerNodeDeltasCalculatorType;
        
        static void calculateNetworkDeltas(NeuralNetworkType& nn, ValueType const* const targetValues)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();

            OutputLayerNodeDeltasCalculatorType::calculateOutputLayerNodeDeltas(outputLayer, targetValues);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(lastHiddenLayer, outputLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(pInnerHiddenLayers[NumberOfInnerHiddenLayers - 1], lastHiddenLayer);

            for (int hiddenLayer = static_cast<int>(NumberOfInnerHiddenLayers - 2); hiddenLayer >= 0; --hiddenLayer)
            {
                NodeDeltasCalculatorType::calculateAndSetNodeDeltas(pInnerHiddenLayers[hiddenLayer], pInnerHiddenLayers[hiddenLayer + 1]);
            }

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(inputLayer, pInnerHiddenLayers[0]);
        }
    };

    template<typename NeuralNetworkType>
    struct NetworkDeltasCalculator<NeuralNetworkType, 1>
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef NodeDeltasCalculator<TransferFunctionsPolicy, NeuralNetworkType::NeuralNetworkHiddenLayerConfiguration> NodeDeltasCalculatorType;
        typedef typename OutputLayerNodeDeltasCalculatorChooser<
                        TransferFunctionsPolicy,
                        OutputLayerType,
                        NeuralNetworkType::NeuralNetworkOutputLayerConfiguration>::OutputLayerNodeDeltasCalculatorType OutputLayerNodeDeltasCalculatorType;
        
        static void calculateNetworkDeltas(NeuralNetworkType& nn, ValueType const* const targetValues)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();

            OutputLayerNodeDeltasCalculatorType::calculateOutputLayerNodeDeltas(outputLayer, targetValues);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(lastHiddenLayer, outputLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(pInnerHiddenLayers[0], lastHiddenLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(inputLayer, pInnerHiddenLayers[0]);
        }
    };

    template<typename NeuralNetworkType>
    struct NetworkDeltasCalculator<NeuralNetworkType, 0>
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef NodeDeltasCalculator<TransferFunctionsPolicy, NeuralNetworkType::NeuralNetworkHiddenLayerConfiguration> NodeDeltasCalculatorType;
        typedef typename OutputLayerNodeDeltasCalculatorChooser<
                        TransferFunctionsPolicy,
                        OutputLayerType,
                        NeuralNetworkType::NeuralNetworkOutputLayerConfiguration>::OutputLayerNodeDeltasCalculatorType OutputLayerNodeDeltasCalculatorType;
        
        static void calculateNetworkDeltas(NeuralNetworkType& nn, ValueType const* const targetValues)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();

            OutputLayerNodeDeltasCalculatorType::calculateOutputLayerNodeDeltas(outputLayer, targetValues);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(lastHiddenLayer, outputLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(inputLayer, lastHiddenLayer);
        }
    };
    
    template<typename NeuralNetworkType>
    struct RecurrentNetworkDeltasCalculator
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkRecurrentLayerType RecurrentLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef NodeDeltasCalculator<TransferFunctionsPolicy, NeuralNetworkType::NeuralNetworkHiddenLayerConfiguration> NodeDeltasCalculatorType;
        typedef typename OutputLayerNodeDeltasCalculatorChooser<
                        TransferFunctionsPolicy,
                        OutputLayerType,
                        NeuralNetworkType::NeuralNetworkOutputLayerConfiguration>::OutputLayerNodeDeltasCalculatorType OutputLayerNodeDeltasCalculatorType;

        static constexpr size_t RecurrentConnectionDepth = RecurrentLayerType::RecurrentLayerRecurrentConnectionDepth;

        static void calculateNetworkDeltas(NeuralNetworkType& nn, ValueType const* const targetValues)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            RecurrentLayerType& recurrentLayer = nn.getRecurrentLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();

            OutputLayerNodeDeltasCalculatorType::calculateOutputLayerNodeDeltas(outputLayer, targetValues);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(lastHiddenLayer, outputLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(recurrentLayer, lastHiddenLayer);

            NodeDeltasCalculatorType::calculateAndSetNodeDeltas(inputLayer, lastHiddenLayer);
        }
    };    

    template<typename ValueType, typename GradientsManagerType, size_t NumberOfBiasNeurons>
    struct BiasGradientsCalculator
    {
    };

    template<typename ValueType, typename GradientsManagerType>
    struct BiasGradientsCalculator<ValueType, GradientsManagerType, 1>
    {
        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
            const ValueType biasNeuronOutputValue = layer.getBiasNeuronOutputValue();
            ValueType nodeDelta;
            ValueType gradient;

            for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
            {
                nodeDelta = nextLayer.getNodeDeltaForNeuron(nextNeuron);
                gradient = (biasNeuronOutputValue * nodeDelta);
                gradientsManager.updateBiasGradients(layer, nextNeuron, gradient);
            }
        }
    };

    template<typename ValueType, typename GradientsManagerType>
    struct BiasGradientsCalculator<ValueType, GradientsManagerType, 0>
    {
        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
        }
    };

    template<typename ValueType, typename GradientsManagerType, outputLayerConfiguration_e OutputLayerConfiguration, size_t NumberOfBiasNeurons>
    struct OuputLayerBiasGradientsCalculator
    {
    };

    template<typename ValueType, typename GradientsManagerType>
    struct OuputLayerBiasGradientsCalculator<ValueType, GradientsManagerType, FeedForwardOutputLayerConfiguration, 1>
    {
        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
            const ValueType biasNeuronOutputValue = layer.getBiasNeuronOutputValue();
            ValueType nodeDelta;
            ValueType gradient;

            for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
            {
                nodeDelta = nextLayer.getNodeDeltaForNeuron(nextNeuron);
                gradient = (biasNeuronOutputValue * nodeDelta);
                gradientsManager.updateBiasGradients(layer, nextNeuron, gradient);
            }
        }
    };

    template<typename ValueType, typename GradientsManagerType>
    struct OuputLayerBiasGradientsCalculator<ValueType, GradientsManagerType, ClassifierOutputLayerConfiguration, 1>
    {
        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
            const ValueType biasNeuronOutputValue = layer.getBiasNeuronOutputValue();
            ValueType nodeDelta;
            ValueType gradient;

            for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
            {
                nodeDelta = nextLayer.getNodeDeltaForNeuron(nextNeuron);
                gradient = (biasNeuronOutputValue * nodeDelta);
                gradientsManager.updateBiasGradients(layer, nextNeuron, gradient);
            }
        }
    };

    template<typename ValueType, typename GradientsManagerType, outputLayerConfiguration_e OutputLayerConfiguration>
    struct OuputLayerBiasGradientsCalculator<ValueType, GradientsManagerType, OutputLayerConfiguration, 0>
    {
        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
        }
    };

    template<typename TransferFunctionsPolicy, typename GradientsManagerType>
    struct GradientsCalculator
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateGradients(LayerType& layer, const NextLayerType& nextLayer, GradientsManagerType& gradientsManager)
        {
            typedef BiasGradientsCalculator<ValueType, GradientsManagerType, LayerType::NumberOfBiasNeuronsInLayer> BiasGradientsCalculatorType;
            ValueType outputValue;
            ValueType nodeDelta;
            ValueType gradient;

            for (size_t neuron = 0; neuron < LayerType::NumberOfNeuronsInLayer; ++neuron)
            {
                outputValue = layer.getOutputValueForNeuron(neuron);
                for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
                {
                    nodeDelta = nextLayer.getNodeDeltaForNeuron(nextNeuron);
                    gradient = (outputValue * nodeDelta);
                    gradientsManager.updateGradients(layer, neuron, nextNeuron, gradient);
                }
            }

            BiasGradientsCalculatorType::calculateAndUpdateGradients(layer, nextLayer, gradientsManager);
        }

        template<typename LayerType, typename NextLayerType>
        static void calculateAndUpdateOutputLayerGradients(LayerType& hiddenLayer, const NextLayerType& outputLayer, GradientsManagerType& gradientsManager)
        {
            typedef OuputLayerBiasGradientsCalculator<ValueType, GradientsManagerType, NextLayerType::OutputLayerConfiguration, LayerType::NumberOfBiasNeuronsInLayer> BiasGradientsCalculatorType;
            ValueType outputValue;
            ValueType nodeDelta;
            ValueType gradient;

            for (size_t neuron = 0; neuron < LayerType::NumberOfNeuronsInLayer; ++neuron)
            {
                outputValue = hiddenLayer.getOutputValueForNeuron(neuron);
                
                for (size_t nextNeuron = 0; nextNeuron < NextLayerType::NumberOfNeuronsInLayer; ++nextNeuron)
                {
                    nodeDelta = outputLayer.getNodeDeltaForNeuron(nextNeuron);
                    gradient = (outputValue * nodeDelta);
                    gradientsManager.updateGradients(hiddenLayer, neuron, nextNeuron, gradient);
                }
            }

            BiasGradientsCalculatorType::calculateAndUpdateGradients(hiddenLayer, outputLayer, gradientsManager);
        }
    };

    template<typename NeuralNetworkType, size_t NumberOfInnerHiddenLayers>
    struct NetworkGradientsCalculator
    {
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename NeuralNetworkType::GradientsManagerType GradientsManagerType;
        typedef GradientsCalculator<TransferFunctionsPolicy, GradientsManagerType> GradientsCalculatorType;
        
        static void calculateNetworkGradients(NeuralNetworkType& nn)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();
            GradientsManagerType& gradientsManager = nn.getGradientsManager();

            GradientsCalculatorType::calculateAndUpdateOutputLayerGradients(lastHiddenLayer, outputLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(pInnerHiddenLayers[NumberOfInnerHiddenLayers - 1], lastHiddenLayer, gradientsManager);

            for (int hiddenLayer = static_cast<int>(NumberOfInnerHiddenLayers - 2); hiddenLayer >= 0; --hiddenLayer)
            {
                GradientsCalculatorType::calculateAndUpdateGradients(pInnerHiddenLayers[static_cast<size_t>(hiddenLayer)], pInnerHiddenLayers[static_cast<size_t>(hiddenLayer + 1)], gradientsManager);
            }

            GradientsCalculatorType::calculateAndUpdateGradients(inputLayer, pInnerHiddenLayers[0], gradientsManager);
        }
    };

    template<typename NeuralNetworkType>
    struct NetworkGradientsCalculator<NeuralNetworkType, 1>
    {
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename NeuralNetworkType::GradientsManagerType GradientsManagerType;
        typedef GradientsCalculator<TransferFunctionsPolicy, GradientsManagerType> GradientsCalculatorType;
        
        static void calculateNetworkGradients(NeuralNetworkType& nn)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            InnerHiddenLayerType* pInnerHiddenLayers = nn.getPointerToInnerHiddenLayers();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();
            GradientsManagerType& gradientsManager = nn.getGradientsManager();

            GradientsCalculatorType::calculateAndUpdateOutputLayerGradients(lastHiddenLayer, outputLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(pInnerHiddenLayers[0], lastHiddenLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(inputLayer, pInnerHiddenLayers[0], gradientsManager);
        }
    };

    template<typename NeuralNetworkType>
    struct NetworkGradientsCalculator<NeuralNetworkType, 0>
    {
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename NeuralNetworkType::GradientsManagerType GradientsManagerType;
        typedef GradientsCalculator<TransferFunctionsPolicy, GradientsManagerType> GradientsCalculatorType;

        static void calculateNetworkGradients(NeuralNetworkType& nn)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();
            GradientsManagerType& gradientsManager = nn.getGradientsManager();

            GradientsCalculatorType::calculateAndUpdateOutputLayerGradients(lastHiddenLayer, outputLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(inputLayer, lastHiddenLayer, gradientsManager);
        }
    };

    template<typename NeuralNetworkType>
    struct RecurrentNetworkGradientsCalculator
    {
        typedef typename NeuralNetworkType::InputLayerType InputLayerType;
        typedef typename NeuralNetworkType::LastHiddenLayerType LastHiddenLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkOutputLayerType OutputLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkRecurrentLayerType RecurrentLayerType;
        typedef typename NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename NeuralNetworkType::GradientsManagerType GradientsManagerType;
        typedef GradientsCalculator<TransferFunctionsPolicy, GradientsManagerType> GradientsCalculatorType;

        static void calculateNetworkGradients(NeuralNetworkType& nn)
        {
            InputLayerType& inputLayer = nn.getInputLayer();
            LastHiddenLayerType& lastHiddenLayer = nn.getLastHiddenLayer();
            RecurrentLayerType& recurrentLayer = nn.getRecurrentLayer();
            OutputLayerType& outputLayer = nn.getOutputLayer();
            GradientsManagerType& gradientsManager = nn.getGradientsManager();

            GradientsCalculatorType::calculateAndUpdateGradients(lastHiddenLayer, outputLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(recurrentLayer, lastHiddenLayer, gradientsManager);

            GradientsCalculatorType::calculateAndUpdateGradients(inputLayer, lastHiddenLayer, gradientsManager);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropagationParent
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;
        
        ValueType getAccelerationRate() const
        {
            return this->mAccelerationRate;
        }
        
        ValueType getLearningRate() const
        {
            return this->mLearningRate;
        }

        ValueType getMomentumRate() const
        {
            return this->mMomentumRate;
        }

        void initialize()
        {
            this->mLearningRate = TransferFunctionsPolicy::initialLearningRate();
            this->mMomentumRate = TransferFunctionsPolicy::initialMomentumRate();
            this->mAccelerationRate = TransferFunctionsPolicy::initialAccelerationRate();
        }
        
        void setAccelerationRate(const ValueType& value)
        {
            this->mAccelerationRate = value;
        }
        
        void setLearningRate(const ValueType& value)
        {
            this->mLearningRate = value;
        }
        
        void setMomentumRate(const ValueType& value)
        {
            this->mMomentumRate = value;
        }

        template<typename LayerType, typename PreviousLayerType>
        void updateConnectionWeights(LayerType& layer, PreviousLayerType& previousLayer)
        {
            typedef typename BiasNeuronConnectionWeightUpdaterTypeSelector<  ValueType,
                                                                             LayerType::LayerType,
                                                                             PreviousLayerType::LayerType,
                                                                             PreviousLayerType::NumberOfBiasNeuronsInLayer>::BiasNeuronConnectionWeightUpdaterType BiasNeuronConnectionWeightUpdaterType;
            ValueType previousDeltaWeight;
            ValueType currentDeltaWeight;
            ValueType newDeltaWeight;
            ValueType currentWeight;

            for (size_t neuron = 0; neuron < LayerType::NumberOfNeuronsInLayer; ++neuron)
            {
                for (size_t previousNeuron = 0; previousNeuron < PreviousLayerType::NumberOfNeuronsInLayer; ++previousNeuron)
                {
                    previousDeltaWeight = previousLayer.getPreviousDeltaWeightForNeuronAndConnection(previousNeuron, neuron);
                    currentDeltaWeight = previousLayer.getDeltaWeightForNeuronAndConnection(previousNeuron, neuron);
                    newDeltaWeight =    (this->mLearningRate * previousLayer.getGradientForNeuronAndConnection(previousNeuron, neuron)) +
                                        (this->mMomentumRate * currentDeltaWeight) +
                                        (this->mAccelerationRate * previousDeltaWeight);
                    currentWeight = previousLayer.getWeightForNeuronAndConnection(previousNeuron, neuron);

                    previousLayer.setDeltaWeightForNeuronAndConnection(previousNeuron, neuron, newDeltaWeight);
                    previousLayer.setWeightForNeuronAndConnection(previousNeuron, neuron, (currentWeight + newDeltaWeight));
                }

                //Update bias values
                BiasNeuronConnectionWeightUpdaterType::updateBiasConnectionWeights(previousLayer, neuron, this->mLearningRate, this->mMomentumRate, this->mAccelerationRate);
            }
        }   
    protected:
        BackPropagationParent() : mLearningRate(0), mMomentumRate(0), mAccelerationRate(0)
        {
        }
        
        ~BackPropagationParent(){};

        ValueType mLearningRate;
        ValueType mMomentumRate;
        ValueType mAccelerationRate;

    private:
        BackPropagationParent(const BackPropagationParent&) = delete;
        BackPropagationParent(const BackPropagationParent&&) = delete;
        BackPropagationParent& operator=(const BackPropagationParent&) = delete;
        static_assert(BatchSize > 0, "Invalid batch size.");
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropagationPolicy : public BackPropagationParent<TransferFunctionsPolicy, BatchSize>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;
        
        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
            typedef NetworkDeltasCalculator<NNType, NNType::NumberOfInnerHiddenLayers> NetworkDeltasCalculatorType;
            typedef NetworkGradientsCalculator<NNType, NNType::NumberOfInnerHiddenLayers> NetworkGradientsCalculatorType;
            typedef BackPropConnectionWeightUpdater<NNType,NNType::NumberOfInnerHiddenLayers> BackPropConnectionWeightUpdaterType;

            NetworkDeltasCalculatorType::calculateNetworkDeltas(nn, targetValues);

            NetworkGradientsCalculatorType::calculateNetworkGradients(nn);
                
            BackPropConnectionWeightUpdaterType::updateConnectionWeights(*this, nn);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropagationThruTimePolicy : public BackPropagationParent<TransferFunctionsPolicy, BatchSize>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
            typedef RecurrentNetworkDeltasCalculator<NNType> NetworkDeltasCalculatorType;
            typedef RecurrentNetworkGradientsCalculator<NNType> NetworkGradientsCalculatorType;
            typedef BackPropThruTimeConnectionWeightUpdater<NNType> BackPropConnectionWeightUpdaterType;

            NetworkDeltasCalculatorType::calculateNetworkDeltas(nn, targetValues);

            NetworkGradientsCalculatorType::calculateNetworkGradients(nn);

            BackPropConnectionWeightUpdaterType::updateConnectionWeights(*this, nn);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct GatedBackPropagationThruTimePolicy : public BackPropagationParent<TransferFunctionsPolicy, BatchSize>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
            typedef RecurrentNetworkDeltasCalculator<NNType> NetworkDeltasCalculatorType;
            typedef RecurrentNetworkGradientsCalculator<NNType> NetworkGradientsCalculatorType;
            typedef BackPropThruTimeConnectionWeightUpdater<NNType> BackPropConnectionWeightUpdaterType;

            NetworkDeltasCalculatorType::calculateNetworkDeltas(nn, targetValues);

            NetworkGradientsCalculatorType::calculateNetworkGradients(nn);

            BackPropConnectionWeightUpdaterType::updateConnectionWeights(*this, nn);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct ClassifierBackPropagationPolicy : public BackPropagationParent<TransferFunctionsPolicy, BatchSize>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
            typedef NetworkDeltasCalculator<NNType, NNType::NumberOfInnerHiddenLayers> NetworkDeltasCalculatorType;
            typedef NetworkGradientsCalculator<NNType, NNType::NumberOfInnerHiddenLayers> NetworkGradientsCalculatorType;
            typedef BackPropConnectionWeightUpdater<NNType,NNType::NumberOfInnerHiddenLayers> BackPropConnectionWeightUpdaterType;

            NetworkDeltasCalculatorType::calculateNetworkDeltas(nn, targetValues);

            NetworkGradientsCalculatorType::calculateNetworkGradients(nn);
                
            BackPropConnectionWeightUpdaterType::updateConnectionWeights(*this, nn);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct ClassifierBackPropagationThruTimePolicy : public BackPropagationParent<TransferFunctionsPolicy, BatchSize>
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
            typedef RecurrentNetworkDeltasCalculator<NNType> NetworkDeltasCalculatorType;
            typedef RecurrentNetworkGradientsCalculator<NNType> NetworkGradientsCalculatorType;
            typedef BackPropThruTimeConnectionWeightUpdater<NNType> BackPropConnectionWeightUpdaterType;

            NetworkDeltasCalculatorType::calculateNetworkDeltas(nn, targetValues);

            NetworkGradientsCalculatorType::calculateNetworkGradients(nn);

            BackPropConnectionWeightUpdaterType::updateConnectionWeights(*this, nn);
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct NullTrainingPolicy
    {
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        ValueType getAccelerationRate() const
        {
            return 0;
        }
        
        ValueType getLearningRate() const
        {
            return 0;
        }

        ValueType getMomentumRate() const
        {
            return 0;
        }

        void initialize()
        {
        }
        
        void setAccelerationRate(const ValueType& value)
        {
        }
        
        void setLearningRate(const ValueType& value)
        {
        }
        
        void setMomentumRate(const ValueType& value)
        {
        }

        template<typename NNType>
        void trainNetwork(NNType& nn, ValueType const* const targetValues)
        {
        }
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize, bool HasRecurrentLayer, hiddenLayerConfiguration_e hiddenLayerConfiguration, bool IsTrainable, outputLayerConfiguration_e OutputLayerConfiguration>
    struct BackPropTrainingPolicySelector
    {
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, false, NonRecurrentHiddenLayerConfiguration, true, FeedForwardOutputLayerConfiguration>
    {
        typedef BackPropagationPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, false, NonRecurrentHiddenLayerConfiguration, true, ClassifierOutputLayerConfiguration>
    {
        typedef ClassifierBackPropagationPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, RecurrentHiddenLayerConfiguration, true, FeedForwardOutputLayerConfiguration>
    {
        typedef BackPropagationThruTimePolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, GRUHiddenLayerConfiguration, true, FeedForwardOutputLayerConfiguration>
    {
        typedef GatedBackPropagationThruTimePolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, LSTMHiddenLayerConfiguration, true, FeedForwardOutputLayerConfiguration>
    {
        typedef GatedBackPropagationThruTimePolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, NonRecurrentHiddenLayerConfiguration, true, ClassifierOutputLayerConfiguration>
    {
        typedef ClassifierBackPropagationThruTimePolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, false, NonRecurrentHiddenLayerConfiguration, false, FeedForwardOutputLayerConfiguration>
    {
        typedef NullTrainingPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, false, NonRecurrentHiddenLayerConfiguration, false, ClassifierOutputLayerConfiguration>
    {
        typedef NullTrainingPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, NonRecurrentHiddenLayerConfiguration, false, FeedForwardOutputLayerConfiguration>
    {
        typedef NullTrainingPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename TransferFunctionsPolicy, size_t BatchSize>
    struct BackPropTrainingPolicySelector<TransferFunctionsPolicy, BatchSize, true, NonRecurrentHiddenLayerConfiguration, false, ClassifierOutputLayerConfiguration>
    {
        typedef NullTrainingPolicy<TransferFunctionsPolicy, BatchSize> TrainingPolicyType;
    };

    template<typename ValueType>
    struct Connection
    {
        typedef ValueType ConnectionValueType;

        static constexpr bool IsTrainable = false;

        Connection() : mWeight(0)
        {
        }

        ValueType getDeltaWeight() const
        {
            return 0;
        }

        ValueType getGradient() const
        {
            return 0;
        }

        ValueType getPreviousDeltaWeight() const
        {
            return 0;
        }

        ValueType getWeight() const
        {
            return this->mWeight;
        }

        void setDeltaWeight(const ValueType& value)
        {
        }

        void setGradient(const ValueType& value)
        {
        }
        
        void setWeight(const ValueType& value)
        {
            this->mWeight = value;
        }

    protected:
        ValueType mWeight;

    private:
        Connection(const Connection&) = delete;
        Connection(const Connection&&) = delete;
        Connection& operator=(const Connection&) = delete;
    };

    template<typename ValueType>
    struct TrainableConnection : public Connection<ValueType>
    {
        typedef ValueType ConnectionValueType;
        
        static constexpr bool IsTrainable = true;

        TrainableConnection() : mDeltaWeight(0), mPreviousDeltaWeight(0), mGradient(0)
        {
        }

        ValueType getDeltaWeight() const
        {
            return this->mDeltaWeight;
        }

        ValueType getGradient() const
        {
            return this->mGradient;
        }

        ValueType getPreviousDeltaWeight() const
        {
            return this->mPreviousDeltaWeight;
        }

        void setDeltaWeight(const ValueType& value)
        {
            this->mPreviousDeltaWeight = this->mDeltaWeight;
            this->mDeltaWeight = value;
        }

        void setGradient(const ValueType& value)
        {
            this->mGradient = value;
        }
    protected:
        ValueType mDeltaWeight;
        ValueType mPreviousDeltaWeight;
        ValueType mGradient;

    private:
        TrainableConnection(const TrainableConnection&) = delete;
        TrainableConnection(const TrainableConnection&&) = delete;
        TrainableConnection& operator=(const TrainableConnection&) = delete;
    };

    template<typename ValueType>
    struct LstmConnection
    {
        typedef ValueType ConnectionValueType;

        static constexpr bool IsTrainable = false;
        static constexpr size_t NumberOfGates = 4;
        static constexpr size_t ForgetGateIndex = 0;
        static constexpr size_t UpdateGateIndex = 1;
        static constexpr size_t InputGateIndex = 2;
        static constexpr size_t OutputGateIndex = 3;

        LstmConnection()
        {
            for (size_t index = 0;index < NumberOfGates;++index)
            {
                this->mWeights[index] = 0;
            }
        }

        ValueType getForgetGateDeltaWeight() const
        {
            return 0;
        }

        ValueType getInputGateDeltaWeight() const
        {
            return 0;
        }

        ValueType getOutputGateDeltaWeight() const
        {
            return 0;
        }

        ValueType getUpdateGateDeltaWeight() const
        {
            return 0;
        }

        ValueType getForgetGateGradient() const
        {
            return 0;
        }

        ValueType getInputGateGradient() const
        {
            return 0;
        }

        ValueType getOutputGateGradient() const
        {
            return 0;
        }

        ValueType getUpdateGateGradient() const
        {
            return 0;
        }

        ValueType getForgetGatePreviousDeltaWeight() const
        {
            return 0;
        }

        ValueType getInputGatePreviousDeltaWeight() const
        {
            return 0;
        }

        ValueType getOutputGatePreviousDeltaWeight() const
        {
            return 0;
        }

        ValueType getUpdateGatePreviousDeltaWeight() const
        {
            return 0;
        }

        ValueType getForgetGateWeight() const
        {
            return this->mWeight[ForgetGateIndex];
        }

        ValueType getInputGateWeight() const
        {
            return this->mWeight[InputGateIndex];
        }

        ValueType getOuputGateWeight() const
        {
            return this->mWeight[OutputGateIndex];
        }

        ValueType getUpdateGateWeight() const
        {
            return this->mWeight[UpdateGateIndex];
        }

        void setForgetGateDeltaWeight(const ValueType& value)
        {
        }

        void setInputGateDeltaWeight(const ValueType& value)
        {
        }

        void setOutputGateDeltaWeight(const ValueType& value)
        {
        }

        void setUpdateGateDeltaWeight(const ValueType& value)
        {
        }

        void setForgetGateGradient(const ValueType& value)
        {
        }

        void setInputGateGradient(const ValueType& value)
        {
        }

        void setOutputGateGradient(const ValueType& value)
        {
        }

        void setUpdateGateGradient(const ValueType& value)
        {
        }

        void setForgetGateWeight(const ValueType& value)
        {
            this->mWeights[ForgetGateIndex] = value;
        }

        void setInputGateWeight(const ValueType& value)
        {
            this->mWeights[InputGateIndex] = value;
        }

        void setOutputGateWeight(const ValueType& value)
        {
            this->mWeights[OutputGateIndex] = value;
        }

        void setUpdateGateWeight(const ValueType& value)
        {
            this->mWeights[UpdateGateIndex] = value;
        }

    protected:
        ValueType mWeights[NumberOfGates];

    private:
        LstmConnection(const LstmConnection&) = delete;
        LstmConnection(const LstmConnection&&) = delete;
        LstmConnection& operator=(const LstmConnection&) = delete;
    };

    template<typename ValueType>
    struct TrainableLstmConnection : public LstmConnection<ValueType>
    {
        typedef ValueType ConnectionValueType;
        typedef LstmConnection<ValueType> ParentType;
        
        static constexpr bool IsTrainable = true;
        static constexpr size_t NumberOfGates = ParentType::NumberOfGates;
        static constexpr size_t ForgetGateIndex = ParentType::ForgetGateIndex;
        static constexpr size_t UpdateGateIndex = ParentType::UpdateGateIndex;
        static constexpr size_t InputGateIndex = ParentType::InputGateIndex;
        static constexpr size_t OutputGateIndex = ParentType::OutputGateIndex;

        TrainableLstmConnection()
        {
            for (size_t index = 0;index < NumberOfGates;++index)
            {
                this->mDeltaWeight[index] = 0;
                this->mPreviousDeltaWeight[index] = 0;
                this->mGradient[index] = 0;
            }
        }

        ValueType getForgetGateDeltaWeight() const
        {
            return this->mDeltaWeights[ForgetGateIndex];
        }

        ValueType getInputGateDeltaWeight() const
        {
            return this->mDeltaWeights[InputGateIndex];
        }

        ValueType getOutputGateDeltaWeight() const
        {
            return this->mDeltaWeights[OutputGateIndex];
        }

        ValueType getUpdateGateDeltaWeight() const
        {
            return this->mDeltaWeights[UpdateGateIndex];
        }

        ValueType getForgetGateGradient() const
        {
            return this->mGradients[ForgetGateIndex];
        }

        ValueType getInputGateGradient() const
        {
            return this->mGradients[InputGateIndex];
        }

        ValueType getOutputGateGradient() const
        {
            return this->mGradients[OutputGateIndex];
        }

        ValueType getUpdateGateGradient() const
        {
            return this->mGradients[UpdateGateIndex];
        }

        ValueType getForgetGatePreviousDeltaWeight() const
        {
            return this->mPreviousDeltaWeights[ForgetGateIndex];
        }

        ValueType getInputGatePreviousDeltaWeight() const
        {
            return this->mPreviousDeltaWeights[InputGateIndex];
        }

        ValueType getOutputGatePreviousDeltaWeight() const
        {
            return this->mPreviousDeltaWeights[OutputGateIndex];
        }

        ValueType getUpdateGatePreviousDeltaWeight() const
        {
            return this->mPreviousDeltaWeights[UpdateGateIndex];
        }

        void setForgetGateDeltaWeight(const ValueType& value)
        {
            this->mPreviousDeltaWeights[ForgetGateIndex] = this->mDeltaWeights[ForgetGateIndex];
            this->mDeltaWeights[ForgetGateIndex] = value;
        }

        void setInputGateDeltaWeight(const ValueType& value)
        {
            this->mPreviousDeltaWeights[InputGateIndex] = this->mDeltaWeights[InputGateIndex];
            this->mDeltaWeights[InputGateIndex] = value;
        }

        void setOutputGateDeltaWeight(const ValueType& value)
        {
            this->mPreviousDeltaWeights[OutputGateIndex] = this->mDeltaWeights[OutputGateIndex];
            this->mDeltaWeights[OutputGateIndex] = value;
        }

        void setUpdateGateDeltaWeight(const ValueType& value)
        {
            this->mPreviousDeltaWeights[UpdateGateIndex] = this->mDeltaWeights[UpdateGateIndex];
            this->mDeltaWeights[UpdateGateIndex] = value;
        }

        void setForgetGateGradient(const ValueType& value)
        {
            this->mGradients[ForgetGateIndex] = value;
        }

        void setInputGateGradient(const ValueType& value)
        {
            this->mGradients[InputGateIndex] = value;
        }

        void setOutputGateGradient(const ValueType& value)
        {
            this->mGradients[OutputGateIndex] = value;
        }

        void setUpdateGateGradient(const ValueType& value)
        {
            this->mGradients[UpdateGateIndex] = value;
        }
    protected:
        ValueType mDeltaWeight[NumberOfGates];
        ValueType mPreviousDeltaWeight[NumberOfGates];
        ValueType mGradient[NumberOfGates];

    private:
        TrainableLstmConnection(const TrainableLstmConnection&) = delete;
        TrainableLstmConnection(const TrainableLstmConnection&&) = delete;
        TrainableLstmConnection& operator=(const TrainableLstmConnection&) = delete;
    };


    template<typename ValueType, hiddenLayerConfiguration_e HiddenLayerConfiguration, bool IsTrainable>
    struct ConnectionTypeSelector
    {
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, NonRecurrentHiddenLayerConfiguration, true>
    {
        typedef TrainableConnection<ValueType> ConnectionType;
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, NonRecurrentHiddenLayerConfiguration, false>
    {
        typedef Connection<ValueType> ConnectionType;
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, RecurrentHiddenLayerConfiguration, true>
    {
        typedef TrainableConnection<ValueType> ConnectionType;
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, RecurrentHiddenLayerConfiguration, false>
    {
        typedef Connection<ValueType> ConnectionType;
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, LSTMHiddenLayerConfiguration, true>
    {
        typedef TrainableLstmConnection<ValueType> ConnectionType;
    };

    template<typename ValueType>
    struct ConnectionTypeSelector<ValueType, LSTMHiddenLayerConfiguration, false>
    {
        typedef LstmConnection<ValueType> ConnectionType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct Neuron
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getOutputValue() const
        {
            return this->mOutputValue;
        }

        ValueType getOutputValueForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            const ValueType result = this->mOutputValue * pConnection->getWeight();

            return result;
        }
        
        ValueType getWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getWeight();
        }

        void initializeWeights()
        {
            for (size_t nextNeuron = 0; nextNeuron < NumberOfOutgoingConnections; ++nextNeuron)
            {
                this->setWeightForConnection(nextNeuron, TransferFunctionsPolicy::generateRandomWeight());
            }
        }

        void setWeightForConnection(const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setWeight(weight);
        }

        void setIndex(const size_t index)
        {
            this->mIndex = index;
        }

        void setOutputValue(const ValueType& value)
        {
            this->mOutputValue = value;
        }

    protected: // Don't instantiate class. Only for use by child classses
        Neuron()
        {
            size_t bufferIndex;
            for(size_t index = 0;index < NumberOfOutgoingConnections;++index)
            {
                bufferIndex = index * sizeof(ConnectionType);
                new (&this->mOutgoingConnectionsBuffer[bufferIndex]) ConnectionType();
            }

            this->mOutputValue = static_cast<ValueType>(0);
            this->mIndex = 0;
        }

        unsigned char mOutgoingConnectionsBuffer[NumberOfOutgoingConnections * sizeof(ConnectionType)];
        ValueType mOutputValue;
        size_t mIndex;
        
        static_assert(NumberOfOutgoingConnections > 0, "Invalid number of outgoing connections.");
    private:
        Neuron(const Neuron&) = delete;
        Neuron(const Neuron&&) = delete;
        Neuron& operator=(const Neuron&) = delete;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> ParentType;
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getDeltaWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getDeltaWeight();
        }

        ValueType getGradientForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getGradient();
        }

        ValueType getNodeDelta() const
        {
            return this->mNodeDelta;
        }

        ValueType getPreviousDeltaWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getPreviousDeltaWeight();
        }

        void initializeWeights()
        {
            ParentType::initializeWeights();

            for (size_t nextNeuron = 0; nextNeuron < NumberOfOutgoingConnections; ++nextNeuron)
            {
                this->setDeltaWeightForConnection(nextNeuron, TransferFunctionsPolicy::initialDeltaWeight());
            }
        }

        void setDeltaWeightForConnection(const size_t connection, const ValueType& deltaWeight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setDeltaWeight(deltaWeight);
        }

        void setGradientForConnection(const size_t connection, const ValueType& gradient)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setGradient(gradient);
        }

        void setNodeDelta(const ValueType& value)
        {
            this->mNodeDelta = value;
        }
        
    protected: // Don't instantiate class. Only for use by child classses
        TrainableNeuron()
        {
            this->mNodeDelta = static_cast<ValueType>(0);
        }
        
        ValueType mNodeDelta;

        static_assert(NumberOfOutgoingConnections > 0, "Invalid number of outgoing connections.");
    private:
        TrainableNeuron(const TrainableNeuron&) = delete;
        TrainableNeuron(const TrainableNeuron&&) = delete;
        TrainableNeuron& operator=(const TrainableNeuron&) = delete;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct LstmNeuron
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getOutputValue() const
        {
            return this->mOutputValue;
        }

        ValueType getOutputValueForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            const ValueType result = this->mOutputValue * pConnection->getWeight();

            return result;
        }

        ValueType getForgetGateWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getForgetGateWeight();
        }

        ValueType getInputGateWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getInputGateWeight();
        }

        ValueType getOutputGateWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getOutputGateWeight();
        }

        ValueType getUpdateGateWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getUpdateGateWeight();
        }

        void initializeWeights()
        {
            for (size_t nextNeuron = 0; nextNeuron < NumberOfOutgoingConnections; ++nextNeuron)
            {
                this->setForgetGateWeightForConnection(nextNeuron, TransferFunctionsPolicy::generateRandomWeight());
                this->setInputGateWeightForConnection(nextNeuron, TransferFunctionsPolicy::generateRandomWeight());
                this->setUpdateGateWeightForConnection(nextNeuron, TransferFunctionsPolicy::generateRandomWeight());
                this->setOutputGateWeightForConnection(nextNeuron, TransferFunctionsPolicy::generateRandomWeight());
            }
        }

        void setForgetGateWeightForConnection(const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setForgetGateWeight(weight);
        }

        void setInputGateWeightForConnection(const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setInputGateWeight(weight);
        }

        void setOutputGateWeightForConnection(const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setOutputGateWeight(weight);
        }

        void setUpdateGateWeightForConnection(const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setUpdateGateWeight(weight);
        }

        void setIndex(const size_t index)
        {
            this->mIndex = index;
        }

        void setOutputValue(const ValueType& value)
        {
            this->mOutputValue = value;
        }

    protected: // Don't instantiate class. Only for use by child classses
        LstmNeuron()
        {
            size_t bufferIndex;
            for(size_t index = 0;index < NumberOfOutgoingConnections;++index)
            {
                bufferIndex = index * sizeof(ConnectionType);
                new (&this->mOutgoingConnectionsBuffer[bufferIndex]) ConnectionType();
            }

            this->mOutputValue = static_cast<ValueType>(0);
            this->mIndex = 0;
        }
        
        unsigned char mOutgoingConnectionsBuffer[NumberOfOutgoingConnections * sizeof(ConnectionType)];
        ValueType mOutputValue;
        size_t mIndex;
        
        static_assert(NumberOfOutgoingConnections > 0, "Invalid number of outgoing connections.");
    private:
        LstmNeuron(const LstmNeuron&) = delete;
        LstmNeuron(const LstmNeuron&&) = delete;
        LstmNeuron& operator=(const LstmNeuron&) = delete;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableLstmNeuron : public LstmNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> ParentType;
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getDeltaWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getDeltaWeight();
        }

        ValueType getGradientForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getGradient();
        }

        ValueType getNodeDelta() const
        {
            return this->mNodeDelta;
        }

        ValueType getPreviousDeltaWeightForConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            const ConnectionType* pConnection = reinterpret_cast<const ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            return pConnection->getPreviousDeltaWeight();
        }

        void initializeWeights()
        {
            ParentType::initializeWeights();

            for (size_t nextNeuron = 0; nextNeuron < NumberOfOutgoingConnections; ++nextNeuron)
            {
                this->setDeltaWeightForConnection(nextNeuron, TransferFunctionsPolicy::initialDeltaWeight());
            }
        }

        void setDeltaWeightForConnection(const size_t connection, const ValueType& deltaWeight)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setDeltaWeight(deltaWeight);
        }

        void setGradientForConnection(const size_t connection, const ValueType& gradient)
        {
            const size_t bufferIndex = connection * sizeof(ConnectionType);
            ConnectionType* pConnection = reinterpret_cast<ConnectionType*>(&this->mOutgoingConnectionsBuffer[bufferIndex]);
            pConnection->setGradient(gradient);
        }

        void setNodeDelta(const ValueType& value)
        {
            this->mNodeDelta = value;
        }
        
    protected: // Don't instantiate class. Only for use by child classses
        TrainableLstmNeuron()
        {
            this->mNodeDelta = static_cast<ValueType>(0);
        }
        
        ValueType mNodeDelta;

        static_assert(NumberOfOutgoingConnections > 0, "Invalid number of outgoing connections.");
    private:
        TrainableLstmNeuron(const TrainableLstmNeuron&) = delete;
        TrainableLstmNeuron(const TrainableLstmNeuron&&) = delete;
        TrainableLstmNeuron& operator=(const TrainableLstmNeuron&) = delete;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct InputLayerNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableInputLayerNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy,
            bool IsTrainable
            >
    struct InputLayerNeuronTypeSelector
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct InputLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true>
    {
        typedef TrainableInputLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> InputLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct InputLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false>
    {
        typedef InputLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> InputLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableHiddenLayerNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct GruHiddenLayerNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableGruHiddenLayerNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct LstmHiddenLayerNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getCellState(void) const
        {
            return this->mState;
        }

        void setCellState(const ValueType& state)
        {
            this->mState = state;
        }
    private:
        ValueType mState;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableLstmHiddenLayerNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;

        ValueType getCellState(void) const
        {
            return this->mState;
        }

        void setCellState(const ValueType& state)
        {
            this->mState = state;
        }
    private:
        ValueType mState;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy,
            bool IsTrainable,
            hiddenLayerConfiguration_e HiddenLayerConfiguration = NonRecurrentHiddenLayerConfiguration
            >
    struct HiddenLayerNeuronTypeSelector
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true, NonRecurrentHiddenLayerConfiguration>
    {
        typedef TrainableHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false, NonRecurrentHiddenLayerConfiguration>
    {
        typedef HiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true, RecurrentHiddenLayerConfiguration>
    {
        typedef TrainableHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false, RecurrentHiddenLayerConfiguration>
    {
        typedef HiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true, GRUHiddenLayerConfiguration>
    {
        typedef TrainableGruHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false, GRUHiddenLayerConfiguration>
    {
        typedef GruHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true, LSTMHiddenLayerConfiguration>
    {
        typedef TrainableLstmHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false, LSTMHiddenLayerConfiguration>
    {
        typedef LstmHiddenLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> HiddenLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct RecurrentLayerNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableRecurrentLayerNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename NeuronTransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = NumberOfOutgoingConnections;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy,
            bool IsTrainable
            >
    struct RecurrentLayerNeuronTypeSelector
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct RecurrentLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true>
    {
        typedef TrainableRecurrentLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> RecurrentLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct RecurrentLayerNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false>
    {
        typedef RecurrentLayerNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> RecurrentLayerNeuronType;
    };

    template<
            typename ConnectionType,
            typename TransferFunctionsPolicy
            >
    struct OutputLayerNeuron : public Neuron<ConnectionType, 1, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = 1;
    };

    template<
            typename ConnectionType,
            typename TransferFunctionsPolicy
            >
    struct TrainableOutputLayerNeuron : public TrainableNeuron<ConnectionType, 1, TransferFunctionsPolicy>
    {
        typedef ConnectionType NeuronConnectionType;
        typedef TransferFunctionsPolicy NeuronTransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr size_t NumberOfOutgoingConnectionsFromNeuron = 1;
    };

    template<
            typename ConnectionType,
            typename TransferFunctionsPolicy,
            bool IsTrainable
            >
    struct OutputLayerNeuronTypeSelector
    {
    };

    template<
            typename ConnectionType,
            typename TransferFunctionsPolicy
            >
    struct OutputLayerNeuronTypeSelector<ConnectionType, TransferFunctionsPolicy, true>
    {
        typedef TrainableOutputLayerNeuron<ConnectionType, TransferFunctionsPolicy> OutputLayerNeuronType;
    };

    template<
            typename ConnectionType,
            typename TransferFunctionsPolicy
            >
    struct OutputLayerNeuronTypeSelector<ConnectionType, TransferFunctionsPolicy, false>
    {
        typedef OutputLayerNeuron<ConnectionType, TransferFunctionsPolicy> OutputLayerNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct BiasNeuron : public Neuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct TrainableBiasNeuron : public TrainableNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy>
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy,
            bool IsTrainable
            >
    struct BiasNeuronTypeSelector
    {
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct BiasNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, true>
    {
        typedef TrainableBiasNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> BiasNeuronType;
    };

    template<
            typename ConnectionType,
            size_t NumberOfOutgoingConnections,
            typename TransferFunctionsPolicy
            >
    struct BiasNeuronTypeSelector<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy, false>
    {
        typedef BiasNeuron<ConnectionType, NumberOfOutgoingConnections, TransferFunctionsPolicy> BiasNeuronType;
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct Layer
    {
        typedef typename NeuronType::ValueType ValueType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;

        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;

        ValueType getGradientForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getGradientForConnection(connection);
        }

        ValueType getDeltaWeightForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getDeltaWeightForConnection(connection);
        }

        ValueType getNodeDeltaForNeuron(const size_t neuron) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getNodeDelta();
        }

        ValueType getOutputValueForNeuron(const size_t neuron) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getOutputValue();
        }

        ValueType getOutputValueForOutgoingConnection(const size_t connection) const
        {
            size_t bufferIndex;
            ValueType sum(0);

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                sum += pNeuron->getOutputValueForConnection(connection);
            }

            return sum;
        }

        ValueType getPreviousDeltaWeightForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getPreviousDeltaWeightForConnection(connection);
        }

        ValueType getWeightForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getWeightForConnection(connection);
        }

        void initializeNeurons()
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setIndex(neuron);
            }
        }

        void initializeWeights()
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->initializeWeights();
            }
        }

        void setDeltaWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& value)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setDeltaWeightForConnection(connection, value);
        }
        
        void setGradientForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& value)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setGradientForConnection(connection, value);
        }

        void setNodeDeltaForNeuron(const size_t neuron, const ValueType& value)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setNodeDelta(value);
        }
        
        void setWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& weight)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setWeightForConnection(connection, weight);
        }
    protected:
        Layer()
        {
            size_t bufferIndex;

            for(size_t neuronIndex = 0;neuronIndex < NumberOfNeurons;++neuronIndex)
            {
                bufferIndex = neuronIndex * sizeof(NeuronType);
                new (&this->mNeuronsBuffer[bufferIndex]) NeuronType();
            }
        }
        unsigned char mNeuronsBuffer[NumberOfNeurons * sizeof(NeuronType)];
        static_assert(NumberOfNeurons > 0, "Number neurons must be > 0");
    };

    template<typename NeuronType, size_t NumberOfNeurons, size_t NumberOfBiasNeurons = 1>
    struct LayerWithBias : public Layer<NeuronType, NumberOfNeurons>
    {
        typedef Layer<NeuronType, NumberOfNeurons> ParentType;
        typedef typename NeuronType::NeuronConnectionType ConnectionType;
        typedef typename NeuronType::ValueType ValueType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename BiasNeuronTypeSelector<ConnectionType, NeuronType::NumberOfOutgoingConnectionsFromNeuron, TransferFunctionsPolicy, ConnectionType::IsTrainable>::BiasNeuronType BiasNeuronType;

        static constexpr size_t NumberOfBiasNeuronsInLayer = NumberOfBiasNeurons;

        void initializeNeurons()
        {
            ParentType::initializeNeurons();

            for (size_t index = 0;index < NumberOfBiasNeurons;++index)
            {
                this->mBiasNeuron[index].setOutputValue(TransferFunctionsPolicy::initialBiasOutputValue());
            }
        }

        void initializeWeights()
        {
            ParentType::initializeWeights();

            for (size_t index = 0;index < NumberOfBiasNeurons;++index)
            {
                this->mBiasNeuron[index].initializeWeights();
            }
        }

    protected:
        ValueType getBiasNeuronDeltaWeightForConnection(const size_t index, const size_t connection) const
        {
            return this->mBiasNeuron[index].getDeltaWeightForConnection(connection);
        }

        ValueType getBiasNeuronGradientForConnection(const size_t index, const size_t connection) const
        {
            return this->mBiasNeuron[index].getGradientForConnection(connection);
        }

        ValueType getBiasNeuronOutputValue(const size_t index) const
        {
            return this->mBiasNeuron[index].getOutputValue();
        }

        ValueType getBiasNeuronPreviousDeltaWeightForConnection(const size_t index, const size_t connection) const
        {
            return this->mBiasNeuron[index].getPreviousDeltaWeightForConnection(connection);
        }

        ValueType getBiasNeuronWeightForConnection(const size_t index, const size_t connection) const
        {
            return this->mBiasNeuron[index].getWeightForConnection(connection);
        }

        ValueType getBiasNeuronValueForOutgoingConnection(const size_t index, const size_t connection) const
        {
            return this->mBiasNeuron[index].getOutputValueForConnection(connection);
        }

        void setBiasNeuronDeltaWeightForConnection(const size_t index, const size_t connection, const ValueType deltaWeight)
        {
            this->mBiasNeuron[index].setDeltaWeightForConnection(connection, deltaWeight);
        }

        void setBiasNeuronGradientForConnection(const size_t index, const size_t connection, const ValueType value)
        {
            this->mBiasNeuron[index].setGradientForConnection(connection, value);
        }

        void setBiasNeuronWeightForConnection(const size_t index, const size_t connection, const ValueType weight)
        {
            this->mBiasNeuron[index].setWeightForConnection(connection, weight);
        }

        LayerWithBias()
        {
        }

        ~LayerWithBias()
        {
        }

        BiasNeuronType mBiasNeuron[NumberOfBiasNeurons];
    private:
        LayerWithBias(const LayerWithBias&) = delete;
        LayerWithBias(const LayerWithBias&&) = delete;
        LayerWithBias& operator=(const LayerWithBias&) = delete;
    };

    template<typename NeuronType, size_t NumberOfNeurons, size_t NumberOfBiasNeurons = 1>
    struct InputLayer : public LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons>
    {
        typedef LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons> ParentType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr layerType_e LayerType = layerType_e::InputLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = NumberOfBiasNeurons;
        static constexpr size_t BiasNeuronIndex = 0;

        /**
         * Feed forward simply latches the current value for each neuron.
         */
        void feedForward(ValueType const* const values)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(values[neuron]);
            }
        }

        ValueType getBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(BiasNeuronIndex);
        }

        ValueType getBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(BiasNeuronIndex, connection);
        }

        void setBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(BiasNeuronIndex, connection, deltaWeight);
        }

        void setBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(BiasNeuronIndex, connection, value);
        }

        void setBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(BiasNeuronIndex, connection, weight);
        }
    };

    template<typename NeuronType, size_t NumberOfNeurons, size_t NumberOfBiasNeurons = 4>
    struct LstmInputLayer : public LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons>
    {
        typedef LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons> ParentType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr layerType_e LayerType = layerType_e::LstmInputLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = NumberOfBiasNeurons;
        static constexpr size_t ForgetGateIndex = 0;
        static constexpr size_t UpdateGateIndex = 1;
        static constexpr size_t InputGateIndex = 2;
        static constexpr size_t OutputGateIndex = 3;

        /**
         * Feed forward simply latches the current value for each neuron.
         */
        void feedForward(ValueType const* const values)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(values[neuron]);
            }
        }

        ValueType getForgetGateBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(ForgetGateIndex, connection);
        }

        ValueType getUpdateGateBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(UpdateGateIndex, connection);
        }

        ValueType getInputGateBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(InputGateIndex, connection);
        }

        ValueType getOutputGateBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(OutputGateIndex, connection);
        }

        ValueType getForgetGateBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(ForgetGateIndex, connection);
        }

        ValueType getUpdateGateBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(UpdateGateIndex, connection);
        }

        ValueType getInputGateBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(InputGateIndex, connection);
        }

        ValueType getOutputGateBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(OutputGateIndex, connection);
        }

        ValueType getForgetGateBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(ForgetGateIndex);
        }

        ValueType getUpdateGateBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(UpdateGateIndex);
        }

        ValueType getInputGateBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(InputGateIndex);
        }

        ValueType getOutputGateBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(OutputGateIndex);
        }

        ValueType getForgetGateBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(ForgetGateIndex, connection);
        }

        ValueType getUpdateGateBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(UpdateGateIndex, connection);
        }

        ValueType getInputGateBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(InputGateIndex, connection);
        }

        ValueType getOutputGateBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(OutputGateIndex, connection);
        }

        ValueType getForgetGateBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(ForgetGateIndex, connection);
        }

        ValueType getUpdateGateBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(UpdateGateIndex, connection);
        }

        ValueType getInputGateBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(InputGateIndex, connection);
        }

        ValueType getOutputGateBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(OutputGateIndex, connection);
        }

        ValueType getForgetGateBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(ForgetGateIndex, connection);
        }

        ValueType getUpdateGateBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(UpdateGateIndex, connection);
        }

        ValueType getInputGateBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(InputGateIndex, connection);
        }

        ValueType getOutputGateBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(OutputGateIndex, connection);
        }

        void setForgetGateBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(ForgetGateIndex, connection, deltaWeight);
        }

        void setUpdateGateBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(UpdateGateIndex, connection, deltaWeight);
        }

        void setInputGateBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(InputGateIndex, connection, deltaWeight);
        }

        void setOutputGateBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(OutputGateIndex, connection, deltaWeight);
        }

        void setForgetGateBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(ForgetGateIndex, connection, value);
        }

        void setUpdateGateBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(UpdateGateIndex, connection, value);
        }

        void setInputGateBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(InputGateIndex, connection, value);
        }

        void setOutputGateBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(OutputGateIndex, connection, value);
        }

        void setForgetGateBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(ForgetGateIndex, connection, weight);
        }

        void setUpdateGateBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(UpdateGateIndex, connection, weight);
        }

        void setInputGateBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(InputGateIndex, connection, weight);
        }

        void setOutputGateBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(OutputGateIndex, connection, weight);
        }
    };

    template<typename NeuronType, size_t NumberOfNeurons, size_t NumberOfBiasNeurons = 1>
    struct HiddenLayer : public LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons>
    {
        typedef LayerWithBias<NeuronType, NumberOfNeurons, NumberOfBiasNeurons> ParentType;
        typedef typename NeuronType::ValueType ValueType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;

        static constexpr layerType_e LayerType = layerType_e::HiddenLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 1;
        static constexpr size_t BiasNeuronIndex = 0;

        template<typename PreviousLayerType>
        void feedForward(const PreviousLayerType& previousLayer)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            ValueType activation;
            ValueType sum;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                sum = previousLayer.getBiasNeuronValueForOutgoingConnection(neuron);
                sum += previousLayer.getOutputValueForOutgoingConnection(neuron);

                activation = TransferFunctionsPolicy::hiddenNeuronActivationFunction(sum);
                
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(activation);
            }
        }

        template<typename PreviousLayerType, typename RecurrentLayerType>
        void feedForward(const PreviousLayerType& previousLayer, RecurrentLayerType& recurrentLayer)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            ValueType activation;
            ValueType sum;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                sum = previousLayer.getBiasNeuronValueForOutgoingConnection(neuron);

                sum += previousLayer.getOutputValueForOutgoingConnection(neuron);
                
                sum += recurrentLayer.getOutputValueForOutgoingConnection(neuron);

                activation = TransferFunctionsPolicy::hiddenNeuronActivationFunction(sum);
                
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(activation);
                
                recurrentLayer.setOutputValueForOutgoingConnection(neuron, activation);
            }
        }

        ValueType getBiasNeuronDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronDeltaWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronGradientForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronGradientForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronOutputValue() const
        {
            return ParentType::getBiasNeuronOutputValue(BiasNeuronIndex);
        }

        ValueType getBiasNeuronPreviousDeltaWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronPreviousDeltaWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronWeightForConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronWeightForConnection(BiasNeuronIndex, connection);
        }

        ValueType getBiasNeuronValueForOutgoingConnection(const size_t connection) const
        {
            return ParentType::getBiasNeuronValueForOutgoingConnection(BiasNeuronIndex, connection);
        }

        ValueType getRecurrentConnectionDeltaWeightForNeuronAtDepth(const size_t neuron, const size_t depth) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getRecurrentConnectionDeltaWeightAtDepth(depth);
        }
        
        ValueType getRecurrentConnectionGradientForNeuronAtDepth(const size_t neuron, const size_t depth) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getRecurrentConnectionGradientAtDepth(depth);
        }
        
        ValueType getRecurrentConnectionWeightForNeuronAtDepth(const size_t neuron, const size_t depth) const
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getRecurrentConnectionWeightAtDepth(depth);
        }

        void setBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType deltaWeight)
        {
            ParentType::setBiasNeuronDeltaWeightForConnection(BiasNeuronIndex, connection, deltaWeight);
        }

        void setBiasNeuronGradientForConnection(const size_t connection, const ValueType value)
        {
            ParentType::setBiasNeuronGradientForConnection(BiasNeuronIndex, connection, value);
        }

        void setBiasNeuronWeightForConnection(const size_t connection, const ValueType weight)
        {
            ParentType::setBiasNeuronWeightForConnection(BiasNeuronIndex, connection, weight);
        }

        void setRecurrentConnectionDeltaWeightForNeuronAtDepth(const size_t neuron, const size_t depth, const ValueType& value)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setRecurrentConnectionDeltaWeightAtDepth(depth, value);
        }
        
        void setRecurrentConnectionWeightForNeuronAtDepth(const size_t neuron, const size_t depth, const ValueType& value)
        {
            const size_t bufferIndex = neuron * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setRecurrentConnectionWeightAtDepth(depth, value);
        }
    };

    /**
     * Hidden neural net layer with GRU neurons.
     */
    template<typename NeuronType, size_t NumberOfNeurons>
    struct GruHiddenLayer : public HiddenLayer<NeuronType, NumberOfNeurons>
    {
        typedef typename NeuronType::ValueType ValueType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef SigmoidActivationPolicy<ValueType> ResetGateActivationPolicy;
        typedef SigmoidActivationPolicy<ValueType> UpdateGateActivationPolicy;
        typedef TanhActivationPolicy<ValueType> CellStateActivationPolicy;

        static constexpr layerType_e LayerType = layerType_e::GruHiddenLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 1;

        template<typename PreviousLayerType, typename RecurrentLayerType>
        void feedForward(const PreviousLayerType& previousLayer, RecurrentLayerType& recurrentLayer)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            ValueType inputActivation;
            ValueType resetGateActivation;
            ValueType updateGateActivation;
            ValueType sum;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                sum = previousLayer.getBiasNeuronValueForOutgoingConnection(neuron);

                sum += previousLayer.getOutputValueForOutgoingConnection(neuron);

                sum += recurrentLayer.getOutputValueForOutgoingConnection(neuron);

                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setCellState(sum);

                inputActivation = TransferFunctionsPolicy::hiddenNeuronActivationFunction(sum);
                resetGateActivation = ResetGateActivationPolicy::activationFunction(sum);
                updateGateActivation = UpdateGateActivationPolicy::activationFunction(sum);
                
                pNeuron->setOutputValue(inputActivation);
                
                recurrentLayer.setOutputValueForOutgoingConnection(neuron, inputActivation);
            }
        }

        ValueType getOutputValueForOutgoingConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getOutputValueForConnection(connection);
        }
        
        void setOutputValueForOutgoingConnection(const size_t connection, const ValueType& value)
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setOutputValue(value);
        }
    };

    /**
     * Hidden neural net layer with LSTM neurons.
     */
    template<typename NeuronType, size_t NumberOfNeurons>
    struct LstmHiddenLayer : public HiddenLayer<NeuronType, NumberOfNeurons>
    {
        typedef typename NeuronType::ValueType ValueType;
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef SigmoidActivationPolicy<ValueType> ForgetGateActivationPolicy;
        typedef SigmoidActivationPolicy<ValueType> InputGateActivationPolicy;
        typedef TanhActivationPolicy<ValueType> UpdateGateActivationPolicy;
        typedef SigmoidActivationPolicy<ValueType> OutputGateActivationPolicy;
        typedef TanhActivationPolicy<ValueType> CellStateActivationPolicy;
        
        static constexpr layerType_e LayerType = layerType_e::LstmHiddenLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 1;

        template<typename PreviousLayerType, typename RecurrentLayerType>
        void feedForward(const PreviousLayerType& previousLayer, RecurrentLayerType& recurrentLayer)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            ValueType inputActivation;
            ValueType inputGateActivation;
            ValueType updateGateActivation;
            ValueType forgetGateActivation;
            ValueType outputGateActivation;
            ValueType output;
            ValueType cellStateSigmoidActivation;
            ValueType cellState;
            ValueType previousCellState;
            ValueType inputValue;
            ValueType recurrentValue;
            ValueType biasValue;
            ValueType gateInputValue;
            ValueType sum;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);

                previousCellState = pNeuron->getCellState();

                recurrentValue = recurrentLayer.getOutputValueForOutgoingConnection(neuron);
                inputValue = previousLayer.getOutputValueForOutgoingConnection(neuron);

                // Calculate forget gate activation
                biasValue = previousLayer.getForgetGateBiasNeuronValueForOutgoingConnection(neuron);
                sum = (recurrentValue + inputValue + biasValue);
                forgetGateActivation = ForgetGateActivationPolicy::activationFunction(sum);

                // Calculate input gate activation
                biasValue = previousLayer.getInputGateBiasNeuronValueForOutgoingConnection(neuron);
                sum = (recurrentValue + inputValue + biasValue);
                inputGateActivation = InputGateActivationPolicy::activationFunction(sum);

                // Calculate update gate activation
                biasValue = previousLayer.getupdateGateBiasNeuronValueForOutgoingConnection(neuron);
                sum = (recurrentValue + inputValue + biasValue);
                updateGateActivation = UpdateGateActivationPolicy::activationFunction(sum);

                // Calculate output gate activation
                biasValue = previousLayer.getoutputGateBiasNeuronValueForOutgoingConnection(neuron);
                sum = (recurrentValue + inputValue + biasValue);
                outputGateActivation = OutputGateActivationPolicy::activationFunction(sum);

                forgetGateActivation = ForgetGateActivationPolicy::activationFunction(gateInputValue);
                inputGateActivation = InputGateActivationPolicy::activationFunction(gateInputValue);
                cellStateSigmoidActivation = CellStateActivationPolicy::activationFunction(gateInputValue);
                outputGateActivation = OutputGateActivationPolicy::activationFunction(gateInputValue);

                cellState = (previousCellState * forgetGateActivation);
                cellState += (cellStateSigmoidActivation * inputActivation);
                pNeuron->setCellState(cellState);

                output = CellStateActivationPolicy::activationFunction(cellState);
                output *= outputGateActivation;

                pNeuron->setOutputValue(output);

                recurrentLayer.setOutputValueForOutgoingConnection(neuron, output);
            }
        }

        ValueType getOutputValueForOutgoingConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getOutputValueForConnection(connection);
        }
        
        void setOutputValueForOutgoingConnection(const size_t connection, const ValueType& value)
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setOutputValue(value);
        }
    };

    template<typename ValueType>
    struct NullHiddenLayer
    {
        ValueType getBiasNeuronWeightForConnection(const size_t connection) const
        {
            return 0;
        }

        ValueType getWeightForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            return 0;
        }

        void setBiasNeuronDeltaWeightForConnection(const size_t connection, const ValueType& deltaWeight)
        {
        }

        void setBiasNeuronGradientForConnection(const size_t connection, const ValueType& value)
        {
        }

        void setBiasNeuronWeightForConnection(const size_t connection, const ValueType& weight)
        {
        }

        void setWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& weight)
        {
        }
    };

    /**
     * Handle recurrent values.
     */
    template<typename NeuronType, size_t NumberOfNeurons, size_t RecurrentConnectionDepth>
    struct RecurrentLayer : public Layer<NeuronType, NumberOfNeurons>
    {
        typedef typename NeuronType::ValueType ValueType;
        
        static constexpr layerType_e LayerType = layerType_e::RecurrentLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 0;
        static constexpr size_t RecurrentLayerRecurrentConnectionDepth = RecurrentConnectionDepth;

        ValueType getOutputValueForOutgoingConnection(const size_t connection) const
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            const NeuronType* pNeuron = reinterpret_cast<const NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            return pNeuron->getOutputValueForConnection(connection);
        }
        
        void setOutputValueForOutgoingConnection(const size_t connection, const ValueType& value)
        {
            const size_t bufferIndex = connection * sizeof(NeuronType);
            NeuronType* pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
            pNeuron->setOutputValue(value);
        }
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct OutputLayer : public Layer<NeuronType, NumberOfNeurons>
    {
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr layerType_e LayerType = layerType_e::OutputLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 0;
        static constexpr outputLayerConfiguration_e OutputLayerConfiguration = FeedForwardOutputLayerConfiguration;

        template<typename PreviousLayerType>
        void feedForward(const PreviousLayerType& previousLayer)
        {
            size_t bufferIndex;
            NeuronType* pNeuron;
            ValueType activation;
            ValueType sum;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                sum = previousLayer.getBiasNeuronValueForOutgoingConnection(neuron);
                sum += previousLayer.getOutputValueForOutgoingConnection(neuron);

                activation = TransferFunctionsPolicy::outputNeuronActivationFunction(sum);

                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(activation);
            }
        }
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct ClassifierOutputLayer : public Layer<NeuronType, NumberOfNeurons>
    {
        typedef typename NeuronType::NeuronTransferFunctionsPolicy TransferFunctionsPolicy;
        typedef typename TransferFunctionsPolicy::TransferFunctionsValueType ValueType;

        static constexpr layerType_e LayerType = layerType_e::ClassifierOutputLayerType;
        static constexpr size_t NumberOfNeuronsInLayer = NumberOfNeurons;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 0;
        static constexpr outputLayerConfiguration_e OutputLayerConfiguration = ClassifierOutputLayerConfiguration;

        template<typename PreviousLayerType>
        void feedForward(const PreviousLayerType& previousLayer)
        {
            size_t bufferIndex;
            ValueType values[NumberOfNeurons];
            ValueType results[NumberOfNeurons];
            NeuronType* pNeuron;

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                values[neuron] = previousLayer.getBiasNeuronValueForOutgoingConnection(neuron);
                values[neuron] += previousLayer.getOutputValueForOutgoingConnection(neuron);
            }

            TransferFunctionsPolicy::outputNeuronActivationFunction(&values[0], &results[0], NumberOfNeurons);

            for (size_t neuron = 0; neuron < NumberOfNeurons; ++neuron)
            {
                bufferIndex = neuron * sizeof(NeuronType);
                pNeuron = reinterpret_cast<NeuronType*>(&this->mNeuronsBuffer[bufferIndex]);
                pNeuron->setOutputValue(results[neuron]);
            }
        }
    };

    template<typename NeuronType, size_t NumberOfNeurons, hiddenLayerConfiguration_e HiddenLayerConfiguration>
    struct InputLayerTypeSelector
    {
        typedef InputLayer<NeuronType, NumberOfNeurons> InputLayerType;
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct InputLayerTypeSelector<NeuronType, NumberOfNeurons, LSTMHiddenLayerConfiguration>
    {
        typedef LstmInputLayer<NeuronType, NumberOfNeurons> InputLayerType;
    };

    template<typename NeuronType, size_t NumberOfNeurons, outputLayerConfiguration_e outputLayerConfiguration>
    struct OutputLayerTypeSelector
    {
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct OutputLayerTypeSelector<NeuronType, NumberOfNeurons, FeedForwardOutputLayerConfiguration>
    {
        typedef OutputLayer<NeuronType, NumberOfNeurons> OutputLayerType;
    };

    template<typename NeuronType, size_t NumberOfNeurons>
    struct OutputLayerTypeSelector<NeuronType, NumberOfNeurons, ClassifierOutputLayerConfiguration>
    {
        typedef ClassifierOutputLayer<NeuronType, NumberOfNeurons> OutputLayerType;
    };

    template<typename InnerHiddenLayerType, size_t NumberOfInnerHiddenLayers>
    struct InnerHiddenLayerManager
    {
        InnerHiddenLayerManager()
        {
            size_t bufferIndex;
            for(size_t innerHiddenLayerIndex = 0;innerHiddenLayerIndex < NumberOfInnerHiddenLayers;++innerHiddenLayerIndex)
            {
                bufferIndex = innerHiddenLayerIndex * sizeof(InnerHiddenLayerType);
                new (&this->mInnerHiddenLayersBuffer[bufferIndex]) InnerHiddenLayerType();
            }

            this->pInnerHiddenLayers = reinterpret_cast<InnerHiddenLayerType*>(&this->mInnerHiddenLayersBuffer[0]);
        }
        
        void initializeInnerHiddenLayerWeights()
        {
            InnerHiddenLayerType* pInnerHiddenLayer;
            size_t bufferIndex;
            for (size_t hiddenLayer = 0; hiddenLayer < NumberOfInnerHiddenLayers; ++hiddenLayer)
            {
                bufferIndex = hiddenLayer * sizeof(InnerHiddenLayerType);
                pInnerHiddenLayer = reinterpret_cast<InnerHiddenLayerType*>(&this->mInnerHiddenLayersBuffer[bufferIndex]);
                pInnerHiddenLayer->initializeWeights();
            }
        }

        InnerHiddenLayerType* getPointerToInnerHiddenLayers()
        {
            return this->pInnerHiddenLayers;
        }

        void initializeInnerHiddenLayerNeurons()
        {
            InnerHiddenLayerType* pInnerHiddenLayer;
            size_t bufferIndex;
            for (size_t hiddenLayer = 0; hiddenLayer < NumberOfInnerHiddenLayers; ++hiddenLayer)
            {
                bufferIndex = hiddenLayer * sizeof(InnerHiddenLayerType);
                pInnerHiddenLayer = reinterpret_cast<InnerHiddenLayerType*>(&this->mInnerHiddenLayersBuffer[bufferIndex]);
                pInnerHiddenLayer->initializeNeurons();
            }
        }
    private:
        unsigned char mInnerHiddenLayersBuffer[NumberOfInnerHiddenLayers * sizeof(InnerHiddenLayerType)];
        InnerHiddenLayerType* pInnerHiddenLayers;
    };

    template<typename InnerHiddenLayerType>
    struct InnerHiddenLayerManager<InnerHiddenLayerType, 0>
    {
        InnerHiddenLayerManager()
        {
            this->pInnerHiddenLayers = NULL;
        }

        static void initializeInnerHiddenLayerWeights()
        {
        }

        InnerHiddenLayerType* getPointerToInnerHiddenLayers()
        {
            return this->pInnerHiddenLayers;
        }
        
        static void initializeInnerHiddenLayerNeurons()
        {
        }
    private:
        InnerHiddenLayerType* pInnerHiddenLayers;
    };

    template<typename InputLayerType, typename InnerHiddenLayerType, typename LastHiddenLayerType, size_t NumberOfInnerHiddenLayers>
    struct HiddenLayerFeedForwardManager
    {
        static void feedForward(InputLayerType& inputLayer, InnerHiddenLayerType* pInnerHiddenLayers, LastHiddenLayerType& lastHiddenLayer)
        {
            pInnerHiddenLayers[0].feedForward(inputLayer);

            for (size_t hiddenLayer = 1; hiddenLayer < NumberOfInnerHiddenLayers; ++hiddenLayer)
            {
                pInnerHiddenLayers[hiddenLayer].feedForward(pInnerHiddenLayers[hiddenLayer - 1]);
            }

            lastHiddenLayer.feedForward(pInnerHiddenLayers[NumberOfInnerHiddenLayers - 1]);
        }
    };

    template<typename InputLayerType, typename InnerHiddenLayerType, typename LastHiddenLayerType>
    struct HiddenLayerFeedForwardManager<InputLayerType, InnerHiddenLayerType, LastHiddenLayerType, 1>
    {
        static void feedForward(InputLayerType& inputLayer, InnerHiddenLayerType* pInnerHiddenLayers, LastHiddenLayerType& lastHiddenLayer)
        {
            pInnerHiddenLayers[0].feedForward(inputLayer);

            lastHiddenLayer.feedForward(pInnerHiddenLayers[0]);
        }
    };

    template<typename InputLayerType, typename InnerHiddenLayerType, typename LastHiddenLayerType>
    struct HiddenLayerFeedForwardManager<InputLayerType, InnerHiddenLayerType, LastHiddenLayerType, 0>
    {
        static void feedForward(InputLayerType& inputLayer, InnerHiddenLayerType* pInnerHiddenLayers, LastHiddenLayerType& lastHiddenLayer)
        {
            lastHiddenLayer.feedForward(inputLayer);
        }
    };

    template<typename ConnectionType, size_t NumberOfHiddenLayers, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy, hiddenLayerConfiguration_e HiddenLayerConfiguration>
    struct HiddenLayerTypeSelector
    {
    };

    template<typename ConnectionType, size_t NumberOfHiddenLayers, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, NumberOfHiddenLayers, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, NonRecurrentHiddenLayerConfiguration>
    {
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfNeuronsInHiddenLayers, TransferFunctionsPolicy, ConnectionType::IsTrainable, NonRecurrentHiddenLayerConfiguration>::HiddenLayerNeuronType InnerHiddenLayerNeuronType;
        typedef HiddenLayer<InnerHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> InnerHiddenLayerType;
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutputs, TransferFunctionsPolicy, ConnectionType::IsTrainable, NonRecurrentHiddenLayerConfiguration>::HiddenLayerNeuronType LastHiddenLayerNeuronType;
        typedef HiddenLayer<LastHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> LastHiddenLayerType;
    };

    template<typename ConnectionType, size_t NumberOfHiddenLayers, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, NumberOfHiddenLayers, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, RecurrentHiddenLayerConfiguration>
    {
        // TODO: Need to add support for recurrent NNs with > 1 hidden layer
    };

    template<typename ConnectionType, size_t NumberOfHiddenLayers, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, NumberOfHiddenLayers, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, GRUHiddenLayerConfiguration>
    {
        // TODO: Need to add support for recurrent NNs with > 1 hidden layer
    };

    template<typename ConnectionType, size_t NumberOfHiddenLayers, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, NumberOfHiddenLayers, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, LSTMHiddenLayerConfiguration>
    {
        // TODO: Need to add support for recurrent NNs with > 1 hidden layer
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, 1, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, NonRecurrentHiddenLayerConfiguration>
    {
        typedef typename ConnectionType::ConnectionValueType ValueType;
        typedef NullHiddenLayer<ValueType> InnerHiddenLayerType;
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutputs, TransferFunctionsPolicy, ConnectionType::IsTrainable, NonRecurrentHiddenLayerConfiguration>::HiddenLayerNeuronType LastHiddenLayerNeuronType;
        typedef HiddenLayer<LastHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> LastHiddenLayerType;
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, 1, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, RecurrentHiddenLayerConfiguration>
    {
        typedef typename ConnectionType::ConnectionValueType ValueType;
        typedef NullHiddenLayer<ValueType> InnerHiddenLayerType;
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutputs, TransferFunctionsPolicy, ConnectionType::IsTrainable, RecurrentHiddenLayerConfiguration>::HiddenLayerNeuronType LastHiddenLayerNeuronType;
        typedef HiddenLayer<LastHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> LastHiddenLayerType;
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, 1, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, GRUHiddenLayerConfiguration>
    {
        typedef typename ConnectionType::ConnectionValueType ValueType;
        typedef NullHiddenLayer<ValueType> InnerHiddenLayerType;
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutputs, TransferFunctionsPolicy, ConnectionType::IsTrainable, GRUHiddenLayerConfiguration>::HiddenLayerNeuronType LastHiddenLayerNeuronType;
        typedef GruHiddenLayer<LastHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> LastHiddenLayerType;
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, size_t NumberOfOutputs, typename TransferFunctionsPolicy>
    struct HiddenLayerTypeSelector<ConnectionType, 1, NumberOfNeuronsInHiddenLayers, NumberOfOutputs, TransferFunctionsPolicy, LSTMHiddenLayerConfiguration>
    {
        typedef typename ConnectionType::ConnectionValueType ValueType;
        typedef NullHiddenLayer<ValueType> InnerHiddenLayerType;
        typedef typename HiddenLayerNeuronTypeSelector<ConnectionType, NumberOfOutputs, TransferFunctionsPolicy, ConnectionType::IsTrainable, LSTMHiddenLayerConfiguration>::HiddenLayerNeuronType LastHiddenLayerNeuronType;
        typedef LstmHiddenLayer<LastHiddenLayerNeuronType, NumberOfNeuronsInHiddenLayers> LastHiddenLayerType;
    };

    struct NullRecurrentLayer
    {
        static constexpr size_t RecurrentLayerRecurrentConnectionDepth = 0;
        static constexpr size_t NumberOfBiasNeuronsInLayer = 0;
        
        void initializeNeurons()
        {
        }

        void initializeWeights()
        {
        }
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, typename TransferFunctionsPolicy, size_t RecurrentConnectionDepth, bool HasRecurrentLayer>
    struct RecurrentLayerTypeSelector
    {
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, typename TransferFunctionsPolicy, size_t RecurrentConnectionDepth>
    struct RecurrentLayerTypeSelector<ConnectionType, NumberOfNeuronsInHiddenLayers, TransferFunctionsPolicy, RecurrentConnectionDepth, false>
    {
        typedef NullRecurrentLayer RecurrentLayerType;
    };

    template<typename ConnectionType, size_t NumberOfNeuronsInHiddenLayers, typename TransferFunctionsPolicy, size_t RecurrentConnectionDepth>
    struct RecurrentLayerTypeSelector<ConnectionType, NumberOfNeuronsInHiddenLayers, TransferFunctionsPolicy, RecurrentConnectionDepth, true>
    {
        typedef typename RecurrentLayerNeuronTypeSelector<ConnectionType, NumberOfNeuronsInHiddenLayers, TransferFunctionsPolicy, ConnectionType::IsTrainable>::RecurrentLayerNeuronType RecurrentLayerNeuronType;
        typedef RecurrentLayer<RecurrentLayerNeuronType, NumberOfNeuronsInHiddenLayers, RecurrentConnectionDepth> RecurrentLayerType;
    };

    /**
     * MLP Neural Network
     */
    template<
            typename ValueType,
            size_t NumberOfInputs,
            size_t NumberOfHiddenLayers,
            size_t NumberOfNeuronsInHiddenLayers,
            size_t NumberOfOutputs,
            typename TransferFunctionsPolicy,
            bool IsTrainable = true,
            size_t BatchSize = 1,
            bool HasRecurrentLayer = false,
            hiddenLayerConfiguration_e HiddenLayerConfiguration = NonRecurrentHiddenLayerConfiguration,
            size_t RecurrentConnectionDepth = 0,
            outputLayerConfiguration_e OutputLayerConfiguration = FeedForwardOutputLayerConfiguration
            >
    class MultilayerPerceptron
    {
    public:
        typedef MultilayerPerceptron<   ValueType,
                                        NumberOfInputs,
                                        NumberOfHiddenLayers,
                                        NumberOfNeuronsInHiddenLayers,
                                        NumberOfOutputs,
                                        TransferFunctionsPolicy,
                                        IsTrainable,
                                        BatchSize,
                                        HasRecurrentLayer,
                                        HiddenLayerConfiguration,
                                        RecurrentConnectionDepth,
                                        OutputLayerConfiguration> NeuralNetworkType;

        typedef ValueType NeuralNetworkValueType;
        typedef typename ConnectionTypeSelector<ValueType, HiddenLayerConfiguration, IsTrainable>::ConnectionType ConnectionType;
        typedef HiddenLayerTypeSelector<ConnectionType,
                                        NumberOfHiddenLayers,
                                        NumberOfNeuronsInHiddenLayers,
                                        NumberOfOutputs,
                                        TransferFunctionsPolicy,
                                        HiddenLayerConfiguration> HiddenLayerTypeSelectorType;
        typedef typename HiddenLayerTypeSelectorType::InnerHiddenLayerType InnerHiddenLayerType;
        typedef typename HiddenLayerTypeSelectorType::LastHiddenLayerType LastHiddenLayerType;
        typedef RecurrentLayerTypeSelector< ConnectionType,
                                            NumberOfNeuronsInHiddenLayers,
                                            TransferFunctionsPolicy,
                                            RecurrentConnectionDepth,
                                            HasRecurrentLayer> RecurrentLayerTypeSelectorType;
        typedef typename RecurrentLayerTypeSelectorType::RecurrentLayerType NeuralNetworkRecurrentLayerType;
        typedef typename InputLayerNeuronTypeSelector<ConnectionType, NumberOfNeuronsInHiddenLayers, TransferFunctionsPolicy, IsTrainable>::InputLayerNeuronType InputLayerNeuronType;
        typedef InputLayerTypeSelector<InputLayerNeuronType, NumberOfInputs, HiddenLayerConfiguration> InputLayerTypeSelectorType;
        typedef typename OutputLayerNeuronTypeSelector<ConnectionType, TransferFunctionsPolicy, IsTrainable>::OutputLayerNeuronType OutputLayerNeuronType;
        typedef typename InputLayerTypeSelectorType::InputLayerType InputLayerType;
        typedef OutputLayerTypeSelector<OutputLayerNeuronType,
                                        NumberOfOutputs,
                                        OutputLayerConfiguration> OutputLayerTypeSelectorType;
        typedef typename OutputLayerTypeSelectorType::OutputLayerType NeuralNetworkOutputLayerType;
        typedef TransferFunctionsPolicy NeuralNetworkTransferFunctionsPolicy;
        typedef typename GradientsManagerSelector<
                                                    NeuralNetworkType,
                                                    NumberOfHiddenLayers - 1,
                                                    IsTrainable>::GradientsManagerType GradientsManagerType;
        typedef typename BackPropTrainingPolicySelector<
                                                        TransferFunctionsPolicy,
                                                        BatchSize,
                                                        HasRecurrentLayer,
                                                        HiddenLayerConfiguration,
                                                        IsTrainable,
                                                        OutputLayerConfiguration>::TrainingPolicyType TrainingPolicyType;

        static constexpr size_t NeuralNetworkNumberOfHiddenLayers = NumberOfHiddenLayers;
        static constexpr size_t NumberOfInnerHiddenLayers = NumberOfHiddenLayers - 1;
        static constexpr size_t NumberOfInputLayerNeurons = InputLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfHiddenLayerNeurons = LastHiddenLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NumberOfOutputLayerNeurons = NeuralNetworkOutputLayerType::NumberOfNeuronsInLayer;
        static constexpr size_t NeuralNetworkRecurrentConnectionDepth = NeuralNetworkRecurrentLayerType::RecurrentLayerRecurrentConnectionDepth;
        static constexpr size_t NeuralNetworkBatchSize = BatchSize;
        static constexpr hiddenLayerConfiguration_e NeuralNetworkHiddenLayerConfiguration = HiddenLayerConfiguration;
        static constexpr outputLayerConfiguration_e NeuralNetworkOutputLayerConfiguration = OutputLayerConfiguration;

        MultilayerPerceptron()
        {
            size_t bufferIndex;

            this->mInputLayer.initializeNeurons();

            this->mInnerHiddenLayerManager.initializeInnerHiddenLayerNeurons();

            this->mLastHiddenLayer.initializeNeurons();

            this->mOutputLayer.initializeNeurons();

            this->mRecurrentLayer.initializeNeurons();

            this->mTrainingPolicy.initialize();

            this->mInputLayer.initializeWeights();

            this->mInnerHiddenLayerManager.initializeInnerHiddenLayerWeights();

            this->mLastHiddenLayer.initializeWeights();

            this->mOutputLayer.initializeWeights();

            this->mRecurrentLayer.initializeWeights();

            for(size_t learnedValue = 0;learnedValue < NumberOfOutputLayerNeurons;++learnedValue)
            {
                bufferIndex = learnedValue * sizeof(ValueType);
                new (&this->mLearnedValuesBuffer[bufferIndex]) ValueType();
            }
        }

        ValueType calculateError(ValueType const* const targetValues)
        {
            ValueType* pLearnedValues = reinterpret_cast<ValueType*>(&this->mLearnedValuesBuffer[0]);

            getLearnedValues(pLearnedValues);

            return TransferFunctionsPolicy::calculateError(targetValues, pLearnedValues);
        }

        void feedForward(ValueType const* const values)
        {
            this->feedForwardInputLayer(values);
            
            this->feedForwardHiddenLayers();

            this->feedForwardOutputLayer();
        }

        ValueType getAccelerationRate() const
        {
            return this->mTrainingPolicy.getAccelerationRate();
        }
        
        ValueType getLearningRate() const
        {
            return this->mTrainingPolicy.getLearningRate();
        }

        ValueType getMomentumRate() const
        {
            return this->mTrainingPolicy.getMomentumRate();
        }

        NeuralNetworkRecurrentLayerType& getRecurrentLayer()
        {
            return this->mRecurrentLayer;
        }
        
        GradientsManagerType& getGradientsManager()
        {
            return this->mGradientsManager;
        }

        ValueType getHiddenLayerBiasNeuronWeightForConnection(const size_t hiddenLayer, const size_t connection)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                return this->mLastHiddenLayer.getBiasNeuronWeightForConnection(connection);
            }
            else
            {
                return this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].getBiasNeuronWeightForConnection(connection);
            }
        }

        ValueType getHiddenLayerWeightForNeuronAndConnection(const size_t hiddenLayer, const size_t neuron, const size_t connection)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                return this->mLastHiddenLayer.getWeightForNeuronAndConnection(neuron, connection);
            }
            else
            {
                return this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].getWeightForNeuronAndConnection(neuron, connection);
            }
        }

        InputLayerType& getInputLayer()
        {
            return this->mInputLayer;
        }

        ValueType getInputLayerBiasNeuronWeightForConnection(const size_t connection) const
        {
            return this->mInputLayer.getBiasNeuronWeightForConnection(connection);
        }

        ValueType getInputLayerWeightForNeuronAndConnection(const size_t neuron, const size_t connection) const
        {
            return this->mInputLayer.getWeightForNeuronAndConnection(neuron, connection);
        }

        LastHiddenLayerType& getLastHiddenLayer()
        {
            return this->mLastHiddenLayer;
        }

        void getLearnedValues(ValueType* output) const
        {
            for (size_t outputNeuron = 0; outputNeuron < NumberOfOutputLayerNeurons; ++outputNeuron)
            {
                output[outputNeuron] = mOutputLayer.getOutputValueForNeuron(outputNeuron);
            }
        }

        NeuralNetworkOutputLayerType& getOutputLayer()
        {
            return this->mOutputLayer;
        }

        InnerHiddenLayerType* getPointerToInnerHiddenLayers()
        {
            return this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers();
        }

        void setAccelerationRate(const ValueType& value)
        {
            this->mTrainingPolicy.setAccelerationRate(value);
        }
        
        void setLearningRate(const ValueType& value)
        {
            this->mTrainingPolicy.setLearningRate(value);
        }
        
        void setMomentumRate(const ValueType& value)
        {
            this->mTrainingPolicy.setMomentumRate(value);
        }

        void setHiddenLayerBiasNeuronWeightForConnection(const size_t hiddenLayer, const size_t connection, const ValueType& weight)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                this->mLastHiddenLayer.setBiasNeuronWeightForConnection(connection, weight);
            }
            else
            {
                this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].setBiasNeuronWeightForConnection(connection, weight);
            }
        }

        void setHiddenLayerBiasDeltaWeightForConnection(const size_t hiddenLayer, const size_t connection, const ValueType& deltaWeight)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                this->mLastHiddenLayer.setBiasNeuronDeltaWeightForConnection(connection, deltaWeight);
            }
            else
            {
                this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].setBiasNeuronDeltaWeightForConnection(connection, deltaWeight);
            }
        }
        
        void setHiddenLayerDeltaWeightForNeuronAndConnection(const size_t hiddenLayer, const size_t neuron, const size_t connection, const ValueType& deltaWeight)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                this->mLastHiddenLayer.setDeltaWeightForNeuronAndConnection(neuron, connection, deltaWeight);
            }
            else
            {
                this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].setDeltaWeightForNeuronAndConnection(neuron, connection, deltaWeight);
            }
        }
        
        void setHiddenLayerWeightForNeuronAndConnection(const size_t hiddenLayer, const size_t neuron, const size_t connection, const ValueType& weight)
        {
            if((NumberOfHiddenLayers - 1) == hiddenLayer)
            {
                this->mLastHiddenLayer.setWeightForNeuronAndConnection(neuron, connection, weight);
            }
            else
            {
                this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers()[hiddenLayer].setWeightForNeuronAndConnection(neuron, connection, weight);
            }
        }

        void setInputLayerBiasWeightForConnection(const size_t connection, const ValueType& weight)
        {
            this->mInputLayer.setBiasNeuronWeightForConnection(connection, weight);
        }
        
        void setInputLayerBiasDeltaWeightForConnection(const size_t connection, const ValueType& deltaWeight)
        {
            this->mInputLayer.setBiasNeuronDeltaWeightForConnection(connection, deltaWeight);
        }
        
        void setInputLayerDeltaWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& deltaWeight)
        {
            this->mInputLayer.setDeltaWeightForNeuronAndConnection(neuron, connection, deltaWeight);
        }

        void setInputLayerWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& weight)
        {
            this->mInputLayer.setWeightForNeuronAndConnection(neuron, connection, weight);
        }

        void setOutputLayerDeltaWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& deltaWeight)
        {
            this->mOutputLayer.setDeltaWeightForNeuronAndConnection(neuron, connection, deltaWeight);
        }

        void setOutputLayerWeightForNeuronAndConnection(const size_t neuron, const size_t connection, const ValueType& weight)
        {
            this->mOutputLayer.setWeightForNeuronAndConnection(neuron, connection, weight);
        }

        void setWeights(NeuralNetworkType& other)
        {
            ValueType weightValue;
            size_t    hiddenLayer = 0;

            for(size_t i = 0; i < NumberOfInputLayerNeurons; ++i)
            {
                for(size_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    weightValue = other.getInputLayerWeightForNeuronAndConnection(i, h);
                    this->setInputLayerWeightForNeuronAndConnection(i, h, weightValue);
                }
            }

            for(size_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
            {
                weightValue = other.getInputLayerBiasNeuronWeightForConnection(h);
                this->setInputLayerBiasWeightForConnection(h, weightValue);
            }

            for(;hiddenLayer < (NumberOfHiddenLayers - 1);++hiddenLayer)
            {
                for(size_t h = 0; h < NumberOfHiddenLayerNeurons; ++h)
                {
                    for(size_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                    {
                        weightValue = other.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, h1);
                        this->setHiddenLayerWeightForNeuronAndConnection(hiddenLayer, h, h1, weightValue);
                    }
                }

                for(size_t h1 = 0; h1 < NumberOfHiddenLayerNeurons; ++h1)
                {
                    weightValue = other.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, h1);
                    this->setHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, h1, weightValue);
                }
            }

            for(size_t hiddenNeuron = 0; hiddenNeuron < NumberOfHiddenLayerNeurons; ++hiddenNeuron)
            {
                for(size_t outputNeuron = 0; outputNeuron < NumberOfOutputLayerNeurons; ++outputNeuron)
                {
                    weightValue = other.getHiddenLayerWeightForNeuronAndConnection(hiddenLayer, hiddenNeuron, outputNeuron);
                    this->setHiddenLayerWeightForNeuronAndConnection(hiddenLayer, hiddenNeuron, outputNeuron, weightValue);
                }
            }

            for(size_t outputNeuron = 0; outputNeuron < NumberOfOutputLayerNeurons; ++outputNeuron)
            {
                weightValue = other.getHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, outputNeuron);
                this->setHiddenLayerBiasNeuronWeightForConnection(hiddenLayer, outputNeuron, weightValue);
            }
        }

        void trainNetwork(ValueType const* const targetValues)
        {
            this->mTrainingPolicy.trainNetwork(*this, targetValues);
        }
    protected:
        void feedForwardHiddenLayers()
        {
            typedef HiddenLayerFeedForwardManager<InputLayerType, InnerHiddenLayerType, LastHiddenLayerType, NumberOfInnerHiddenLayers> HiddenLayerFeedForwardManagerType;

            HiddenLayerFeedForwardManagerType::feedForward(this->mInputLayer, this->mInnerHiddenLayerManager.getPointerToInnerHiddenLayers(), this->mLastHiddenLayer);
        }
        
        void feedForwardInputLayer(ValueType const* const values)
        {
            this->mInputLayer.feedForward(values);
        }
        
        void feedForwardOutputLayer()
        {
            this->mOutputLayer.feedForward(this->mLastHiddenLayer);
        }

    protected:
        InputLayerType mInputLayer;
        InnerHiddenLayerManager<InnerHiddenLayerType, NumberOfInnerHiddenLayers> mInnerHiddenLayerManager;
        LastHiddenLayerType mLastHiddenLayer;
        NeuralNetworkOutputLayerType mOutputLayer;
        NeuralNetworkRecurrentLayerType mRecurrentLayer;
        TrainingPolicyType mTrainingPolicy;
        GradientsManagerType mGradientsManager;
    private:
        unsigned char mLearnedValuesBuffer[NumberOfOutputLayerNeurons * sizeof(ValueType)];
    private:
        MultilayerPerceptron(const MultilayerPerceptron&) = delete;
        MultilayerPerceptron(const MultilayerPerceptron&&) = delete;
        MultilayerPerceptron& operator=(const MultilayerPerceptron&) = delete;

        static_assert(NumberOfInputs > 0, "Invalid number of inputs.");
        static_assert(NumberOfHiddenLayers > 0, "Invalid number of hidden layers.");
        static_assert(NumberOfNeuronsInHiddenLayers > 0, "Invalid number of neurons in hidden layers layers.");
        static_assert(NumberOfOutputs > 0, "Invalid number of outputs.");
        static_assert(NumberOfOutputLayerNeurons == TransferFunctionsPolicy::NumberOfTransferFunctionsOutputNeurons, "TransferFunctionPolicy NumberOfOutputNeurons is incorrect.");
    };

    /**
     * Recurrent MLP
     */
    template<
            typename ValueType,
            size_t NumberOfInputs,
            size_t NumberOfHiddenLayers,
            size_t NumberOfNeuronsInHiddenLayers,
            size_t NumberOfOutputs,
            typename TransferFunctionsPolicy,
            bool IsTrainable = true,
            size_t BatchSize = 1,
            bool HasRecurrentLayer = true,
            hiddenLayerConfiguration_e HiddenLayerConfiguration = RecurrentHiddenLayerConfiguration,
            size_t RecurrentConnectionDepth = 1,
            outputLayerConfiguration_e OutputLayerConfiguration = FeedForwardOutputLayerConfiguration
            >
    class RecurrentMultilayerPerceptron : public MultilayerPerceptron<  ValueType,
                                                                        NumberOfInputs,
                                                                        NumberOfHiddenLayers,
                                                                        NumberOfNeuronsInHiddenLayers,
                                                                        NumberOfOutputs, 
                                                                        TransferFunctionsPolicy,
                                                                        IsTrainable,
                                                                        BatchSize,
                                                                        HasRecurrentLayer,
                                                                        HiddenLayerConfiguration,
                                                                        RecurrentConnectionDepth,
                                                                        OutputLayerConfiguration
                                                                        >
    {
    public:
        void feedForward(ValueType const* const values)
        {
            this->feedForwardInputLayer(values);
            
            this->mLastHiddenLayer.feedForward(this->mInputLayer, this->mRecurrentLayer);
            
            this->feedForwardOutputLayer();
        }
    private:
        static_assert(RecurrentConnectionDepth > 0, "Invalid recurrent connection depth.");
    };

    /**
     * Elman Network
     */
    template<
            typename ValueType,
            size_t NumberOfInputs,
            size_t NumberOfNeuronsInHiddenLayers,
            size_t NumberOfOutputs,
            typename TransferFunctionsPolicy,
            bool IsTrainable = true,
            size_t BatchSize = 1,
            outputLayerConfiguration_e OutputLayerConfiguration = FeedForwardOutputLayerConfiguration
            >
    class ElmanNetwork : public RecurrentMultilayerPerceptron<  ValueType,
                                                                NumberOfInputs,
                                                                1,
                                                                NumberOfNeuronsInHiddenLayers,
                                                                NumberOfOutputs,
                                                                TransferFunctionsPolicy,
                                                                IsTrainable,
                                                                BatchSize
                                                                >
    {
    };

    /**
     * LSTM Network
     */
    template<
            typename ValueType,
            size_t NumberOfInputs,
            size_t NumberOfHiddenLayers,
            size_t NumberOfNeuronsInHiddenLayers,
            size_t NumberOfOutputs,
            typename TransferFunctionsPolicy,
            bool IsTrainable = true,
            size_t BatchSize = 1,
            bool HasRecurrentLayer = true,
            hiddenLayerConfiguration_e HiddenLayerConfiguration = LSTMHiddenLayerConfiguration,
            size_t RecurrentConnectionDepth = 1,
            outputLayerConfiguration_e OutputLayerConfiguration = FeedForwardOutputLayerConfiguration
            >
    class LstmMultilayerPerceptron : public MultilayerPerceptron<   ValueType,
                                                                    NumberOfInputs,
                                                                    NumberOfHiddenLayers,
                                                                    NumberOfNeuronsInHiddenLayers,
                                                                    NumberOfOutputs, 
                                                                    TransferFunctionsPolicy,
                                                                    IsTrainable,
                                                                    BatchSize,
                                                                    HasRecurrentLayer,
                                                                    HiddenLayerConfiguration,
                                                                    RecurrentConnectionDepth,
                                                                    OutputLayerConfiguration
                                                                    >
    {
    public:
        void feedForward(ValueType const* const values)
        {
            this->feedForwardInputLayer(values);
            
            this->mLastHiddenLayer.feedForward(this->mInputLayer, this->mRecurrentLayer);
            
            this->feedForwardOutputLayer();
        }
    private:
        static_assert(RecurrentConnectionDepth > 0, "Invalid recurrent connection depth.");
    };
}
