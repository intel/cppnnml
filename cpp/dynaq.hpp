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

namespace tinymind {
    template<typename NeuralNetworkType>
    struct DefaultSimulatedExperiencePolicy
    {
        typedef typename NeuralNetworkType::NeuralNetworkValueType ValueType;

        DefaultSimulatedExperiencePolicy() : mAverageError(0), mNumberOfSamplesPerAverage(10), mAverageErrorCalculated(false) {}
        DefaultSimulatedExperiencePolicy(const int numberOfSamplesPerAverage) : mAverageError(0), mNumberOfSamplesPerAverage(numberOfSamplesPerAverage), mAverageErrorCalculated(false) {}

        bool readyToSimulateExperience() const
        {
            if(this->mAverageErrorCalculated && NeuralNetworkType::NeuralNetworkTransferFunctionsPolicy::isWithinZeroTolerance(this->mAverageError))
            {
                return true;
            }

            return false;
        }

        void updateModel()
        {
            static ValueType accumulatedError(0);
            static int sampleNumber = 0;
            const ValueType error = this->mNeuralNetwork.getError();

            ++sampleNumber;
            accumulatedError += error;
            if(sampleNumber == this->mNumberOfSamplesPerAverage)
            {
                this->mAverageErrorCalculated = true;
                accumulatedError /= this->mNumberOfSamplesPerAverage;
                this->mAverageError = accumulatedError;
                sampleNumber = 0;
                accumulatedError = 0;
            }
        }
    private:
        ValueType mAverageError;
        int mNumberOfSamplesPerAverage;
        bool mAverageErrorCalculated;
        NeuralNetworkType mNeuralNetwork;
    };

    template<typename QLearnerType, typename SimulatedExperiencePolicy>
    class DynaQLearner
    {
    public:
        typedef typename QLearnerType::ValueType ValueType;

        DynaQLearner(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint) : 
            mQLearner(learningRate, discountFactor, randomActionDecisionPoint)
        {
        }
    private:
        QLearnerType mQLearner;
        SimulatedExperiencePolicy mSimulatedExperiencePolicy;
    };
}