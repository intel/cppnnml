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
#include "interpolate.hpp"
#include <cstddef>

namespace tinymind {
    template<typename ValueType>
    struct LookupTable
    {
        typedef typename ValueType::FullWidthFieldType FullWidthFieldType;
        typedef typename ValueType::FullWidthValueType FullWidthValueType;

        static ValueType getValue(const ValueType& value, typename ValueType::FullWidthFieldType const* const tableValues, const ptrdiff_t maxActivationIndex)
        {
            static constexpr FullWidthValueType MIN_X_ACTIVATION_VALUE = static_cast<FullWidthValueType>(MIN_X_TABLE_VALUE * (1 << ValueType::NumberOfFractionalBits));
            static constexpr FullWidthValueType MAX_X_ACTIVATION_VALUE = static_cast<FullWidthValueType>(MAX_X_TABLE_VALUE * (1 << ValueType::NumberOfFractionalBits));
            static constexpr FullWidthValueType ACTIVATION_DELTA_X = static_cast<FullWidthValueType>((1 << (ValueType::NumberOfFractionalBits - ACTIVATION_DELTA_SHIFT)));

            ValueType result;

            if (value.getValue() <= MIN_X_ACTIVATION_VALUE)
            {
                result = tableValues[0];
            }
            else if (value.getValue() >= MAX_X_ACTIVATION_VALUE)
            {
                result = tableValues[maxActivationIndex];
            }
            else
            {
                const ptrdiff_t lowerIndex = static_cast<ptrdiff_t>((value.getValue() - static_cast<FullWidthValueType>(MIN_X_ACTIVATION_VALUE)) / static_cast<FullWidthValueType>(ACTIVATION_DELTA_X));
                if (lowerIndex >= maxActivationIndex)
                {
                    result = tableValues[maxActivationIndex];
                }
                else
                {
                    const ptrdiff_t upperIndex = lowerIndex + 1;
                    const FullWidthValueType lowerX = static_cast<FullWidthValueType>(MIN_X_ACTIVATION_VALUE + (lowerIndex * ACTIVATION_DELTA_X));
                    const FullWidthValueType upperX = static_cast<FullWidthValueType>(MIN_X_ACTIVATION_VALUE + (upperIndex * ACTIVATION_DELTA_X));

                    if (tableValues[lowerIndex] == tableValues[upperIndex])
                    {
                        result = tableValues[upperIndex];
                    }
                    else if (tableValues[lowerIndex] == value.getValue())
                    {
                        result = tableValues[lowerIndex];
                    }
                    else if (tableValues[upperIndex] == value.getValue())
                    {
                        result = tableValues[upperIndex];
                    }
                    else
                    {
                        result = linearInterpolation<ValueType>(
                                                                value,
                                                                static_cast<ValueType>(lowerX),
                                                                static_cast<ValueType>(upperX),
                                                                static_cast<ValueType>(tableValues[lowerIndex]),
                                                                static_cast<ValueType>(tableValues[upperIndex]));
                    }
                }
            }

            return result;
        }
    };
}
