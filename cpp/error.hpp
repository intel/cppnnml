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

#include "lookupTable.hpp"
#include "log.hpp"
#include "constants.hpp"

namespace tinymind {
    template<typename ValueType, unsigned NumberOfOutputNeurons>
    struct MeanSquaredErrorCalculator
    {
        /**
        * Mean-squared error
        */
        static ValueType calculateError(ValueType const* const targetValues, ValueType const* const outputValues)
        {
            ValueType error(0);

            for (uint32_t neuron = 0; neuron < NumberOfOutputNeurons; neuron++)
            {
                const ValueType delta = (targetValues[neuron] - outputValues[neuron]);
                const ValueType deltaSquared = (delta * delta);
                error += deltaSquared;
            }

            if (NumberOfOutputNeurons > 1)
            {
                error /= NumberOfOutputNeurons;
            }

            return error;
        }
    };

    template<typename ValueType, unsigned NumberOfOutputNeurons>
    struct CrossEntropyLossCalculator
    {
        typedef typename ValueType::FullWidthFieldType FullWidthFieldType;
        typedef LookupTable<ValueType> LookupTableType;
        typedef typename LogValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::LogTableType LogTableType;

        /**
         * Cross-entropy loss
         */
        static ValueType calculateError(ValueType const* const targetValues, ValueType const* const outputValues)
        {
            static const ptrdiff_t MAX_ACTIVATION_INDEX = (((sizeof(FullWidthFieldType) * NUMBER_OF_ACTIVATION_TABLE_VALUES) / sizeof(logActivationTable.values[0])) - 1);
            ValueType error(0);

            for (uint32_t neuron = 0; neuron < NumberOfOutputNeurons; neuron++)
            {
                error += (targetValues[neuron] * LookupTableType::getValue(outputValues[neuron], &logActivationTable.values[0], MAX_ACTIVATION_INDEX));
            }

            error *= Constants<ValueType>::negativeOne();

            return error;
        }
    private:
        static const LogTableType logActivationTable;
    };

    template<typename ValueType, unsigned NumberOfOutputNeurons>
    const typename LogValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::LogTableType CrossEntropyLossCalculator<ValueType, NumberOfOutputNeurons>::logActivationTable;
}
