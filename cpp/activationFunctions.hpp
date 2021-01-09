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
#include "constants.hpp"
#include "sigmoid.hpp"
#include "tanh.hpp"
#include "exp.hpp"
#include "typeChooser.hpp"

namespace tinymind {
    template<typename ValueType>
    struct NullActivationPolicy
    {
        static ValueType activationFunction(const ValueType& value)
        {
            return 0;
        }

        static ValueType activationFunctionDerivative(const ValueType& value)
        {
            return 0;
        }
    };
    
    template<typename ValueType>
    struct ReluActivationPolicy
    {
        static ValueType activationFunction(const ValueType& value)
        {
            if(value <= 0)
            {
                return 0;
            }

            return value;
        }

        static ValueType activationFunctionDerivative(const ValueType& value)
        {
            if(value <= 0)
            {
                return 0;
            }

            return Constants<ValueType>::one();
        }
    private:
        static_assert(ValueType::IsSigned, "Relu activation policy requires a signed type.");
    };

    template<typename ValueType, typename FullWidthFieldTypeShim<ValueType>::FullWidthFieldType MaxValue>
    struct CappedReluActivationPolicy
    {
        typedef typename FullWidthFieldTypeShim<ValueType>::FullWidthFieldType FullWidthFieldType;
        static const FullWidthFieldType MAX_VALUE = MaxValue;

        static ValueType activationFunction(const ValueType& value)
        {
            if(value <= 0)
            {
                return 0;
            }

            if(value > MAX_VALUE)
            {
                return MAX_VALUE;
            }
            else
            {
                return value;
            }
        }

        static ValueType activationFunctionDerivative(const ValueType& value)
        {
            return ReluActivationPolicy<ValueType>::activationFunctionDerivative(value);
        }
    private:
        static_assert(ValueType::IsSigned, "Capped Relu activation policy requires a signed type.");
    };

    template<typename ValueType>
    struct SigmoidActivationPolicy
    {
        typedef LookupTable<ValueType> LookupTableType;
        typedef typename SigmoidValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::SigmoidTableType SigmoidTableType;

        static ValueType activationFunction(const ValueType& value)
        {
            static const ptrdiff_t MAX_ACTIVATION_INDEX = ((sizeof(sigmoidActivationTable.values) / sizeof(sigmoidActivationTable.values[0])) - 1);

            const ValueType result = LookupTableType::getValue(value, &sigmoidActivationTable.values[0], MAX_ACTIVATION_INDEX);

            return result;
        }

        static ValueType activationFunctionDerivative(const ValueType& value)
        {
            //Approximation for 1st derivative of sigmoid
            const ValueType oneMinusValue = (Constants<ValueType>::one() - value);

            return (value * oneMinusValue);
        }
    private:
        static const SigmoidTableType sigmoidActivationTable;
        static_assert(ValueType::IsSigned, "Sigmoid activation tables require a signed type.");
    };

    template<typename ValueType>
    const typename SigmoidValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::SigmoidTableType SigmoidActivationPolicy<ValueType>::sigmoidActivationTable;

    template<typename ValueType>
    struct TanhActivationPolicy
    {
        typedef typename ValueType::FullWidthFieldType FullWidthFieldType;
        typedef LookupTable<ValueType> LookupTableType;
        typedef typename TanhValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::TanhTableType TanhTableType;

        static ValueType activationFunction(const ValueType& value)
        {
            static const ptrdiff_t MAX_ACTIVATION_INDEX = (((sizeof(FullWidthFieldType) * NUMBER_OF_ACTIVATION_TABLE_VALUES) / sizeof(tanhActivationTable.values[0])) - 1);

            const ValueType result = LookupTableType::getValue(value, &tanhActivationTable.values[0], MAX_ACTIVATION_INDEX);

            return result;
        }

        static ValueType activationFunctionDerivative(const ValueType& value)
        {
            //Approximation for 1st derivative of tanh
            const ValueType valueSquared = (value * value);

            return (Constants<ValueType>::one() - valueSquared);
        }
    private:
        static const TanhTableType tanhActivationTable;
        static_assert(ValueType::IsSigned, "Tanh activation tables require a signed type.");
    };

    template<typename ValueType>
    const typename TanhValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::TanhTableType TanhActivationPolicy<ValueType>::tanhActivationTable;

    template<typename ValueType>
    struct SoftmaxActivationPolicy
    {
        typedef typename ValueType::FullWidthFieldType FullWidthFieldType;
        typedef LookupTable<ValueType> LookupTableType;
        typedef typename ExpValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::ExpTableType ExpTableType;

        static void activationFunction(ValueType const* const values, ValueType* results, const size_t numberOfNerons)
        {
            static const ptrdiff_t MAX_ACTIVATION_INDEX = (((sizeof(FullWidthFieldType) * NUMBER_OF_ACTIVATION_TABLE_VALUES) / sizeof(expActivationTable.values[0])) - 1);
            ValueType result;
            ValueType sum(0);

            for(size_t neuron = 0;neuron < numberOfNerons;++neuron)
            {
                result = LookupTableType::getValue(values[neuron], &expActivationTable.values[0], MAX_ACTIVATION_INDEX);
                results[neuron] = result;
                sum += result;
            }

            for(size_t neuron = 0;neuron < numberOfNerons;++neuron)
            {
                results[neuron] /= sum;
            }
        }

        static void activationFunctionDerivative(ValueType const* const values, ValueType const* const targetValues, ValueType* results, const size_t numberOfNerons)
        {
            for(size_t neuron = 0;neuron < numberOfNerons;++neuron)
            {
                if(targetValues[neuron] == Constants<ValueType>::one())
                {
                    results[neuron] = (values[neuron] - Constants<ValueType>::one());
                }
                else
                {
                    results[neuron] = values[neuron];
                }
            }
        }
    private:
        static const ExpTableType expActivationTable;
        static_assert(ValueType::IsSigned, "Exp activation tables require a signed type.");
    };

    template<typename ValueType>
    const typename ExpValuesTableSelector<ValueType::NumberOfFixedBits, ValueType::NumberOfFractionalBits, ValueType::IsSigned>::ExpTableType SoftmaxActivationPolicy<ValueType>::expActivationTable;
}