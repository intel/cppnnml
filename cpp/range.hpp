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
#include "constants.hpp"

namespace tinymind {
    template<typename ValueType>
    struct RangeConverter
    {
        static void convertToFractionOfRange(ValueType& value, const ValueType& minValue, const ValueType& maxValue)
        {
            ValueType range;

            if(maxValue > minValue)
            {
                range = (maxValue - minValue);
                value -= minValue;
                value /= range;
            }
        }
    };

    template<typename OutputValueType, typename ActivationTableValueType, typename ActivationFunctionPolicyType>
    struct ActivationOutputRangeConverter
    {
        static void convertToOutputValue(OutputValueType& value, const OutputValueType& minValue, const OutputValueType& maxValue)
        {
        }
    };

    template<typename OutputValueType, typename ActivationTableValueType>
    struct ActivationOutputRangeConverter<OutputValueType, ActivationTableValueType, tinymind::TanhActivationPolicy<ActivationTableValueType> >
    {
        // The tanh activation function outputs values from -1..1 so we must scale the output such that -1 == min value, 1 == max value.
        static void convertToOutputValue(OutputValueType& value, const OutputValueType& minValue, const OutputValueType& maxValue)
        {
            const OutputValueType range = (maxValue - minValue);

            value += Constants<OutputValueType>::one();
            value *= range;
            value /= 2;
            value += minValue;
        }
    };

    template<typename OutputValueType, typename ActivationTableValueType>
    struct ActivationOutputRangeConverter<OutputValueType, ActivationTableValueType, tinymind::SigmoidActivationPolicy<ActivationTableValueType> >
    {
        static void convertToOutputValue(OutputValueType& value, const OutputValueType& minValue, const OutputValueType& maxValue)
        {
            const OutputValueType range = (maxValue - minValue);

            value *= range;
            value += minValue;
        }
    };
}