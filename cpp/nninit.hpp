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

#include "constants.hpp"

namespace tinymind {
    template<typename ValueType>
    struct DefaultNetworkInitializer
    {
        static ValueType initialAccelerationRate()
        {
            static const ValueType rate(0, (1 << (ValueType::NumberOfFractionalBits - 4)));

            return rate;
        }

        static ValueType initialBiasOutputValue()
        {
            return Constants<ValueType>::one();
        }

        static ValueType initialDeltaWeight()
        {
            return Constants<ValueType>::zero();
        }

        static ValueType initialGradientValue()
        {
            return Constants<ValueType>::zero();
        }

        static ValueType initialLearningRate()
        {
            static const ValueType rate(0, (1 << (ValueType::NumberOfFractionalBits - 2)));

            return rate;
        }

        static ValueType initialMomentumRate()
        {
            static const ValueType rate(0, (1 << (ValueType::NumberOfFractionalBits - 1)));

            return rate;
        }

        static ValueType initialOutputValue()
        {
            return Constants<ValueType>::zero();
        }

        static ValueType noOpDeltaWeight()
        {
            return Constants<ValueType>::one();
        }

        static ValueType noOpWeight()
        {
            return Constants<ValueType>::one();
        }
    };
}
