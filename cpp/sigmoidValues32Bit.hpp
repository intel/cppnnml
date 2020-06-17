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

namespace tinymind {
    #if (defined(TINYMIND_USE_SIGMOID_1_31))
    struct SigmoidValuesTableQ1_31
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_31))
    #if (defined(TINYMIND_USE_SIGMOID_2_30))
    struct SigmoidValuesTableQ2_30
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_30))
    #if (defined(TINYMIND_USE_SIGMOID_3_29))
    struct SigmoidValuesTableQ3_29
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_29))
    #if (defined(TINYMIND_USE_SIGMOID_4_28))
    struct SigmoidValuesTableQ4_28
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_28))
    #if (defined(TINYMIND_USE_SIGMOID_5_27))
    struct SigmoidValuesTableQ5_27
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_27))
    #if (defined(TINYMIND_USE_SIGMOID_6_26))
    struct SigmoidValuesTableQ6_26
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_26))
    #if (defined(TINYMIND_USE_SIGMOID_7_25))
    struct SigmoidValuesTableQ7_25
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_25))
    #if (defined(TINYMIND_USE_SIGMOID_8_24))
    struct SigmoidValuesTableQ8_24
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_24))
    #if (defined(TINYMIND_USE_SIGMOID_9_23))
    struct SigmoidValuesTableQ9_23
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_23))
    #if (defined(TINYMIND_USE_SIGMOID_10_22))
    struct SigmoidValuesTableQ10_22
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_22))
    #if (defined(TINYMIND_USE_SIGMOID_11_21))
    struct SigmoidValuesTableQ11_21
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_21))
    #if (defined(TINYMIND_USE_SIGMOID_12_20))
    struct SigmoidValuesTableQ12_20
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_20))
    #if (defined(TINYMIND_USE_SIGMOID_13_19))
    struct SigmoidValuesTableQ13_19
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_19))
    #if (defined(TINYMIND_USE_SIGMOID_14_18))
    struct SigmoidValuesTableQ14_18
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_18))
    #if (defined(TINYMIND_USE_SIGMOID_15_17))
    struct SigmoidValuesTableQ15_17
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_17))
    #if (defined(TINYMIND_USE_SIGMOID_16_16))
    struct SigmoidValuesTableQ16_16
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_16_16))
    #if (defined(TINYMIND_USE_SIGMOID_17_15))
    struct SigmoidValuesTableQ17_15
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_17_15))
    #if (defined(TINYMIND_USE_SIGMOID_18_14))
    struct SigmoidValuesTableQ18_14
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_18_14))
    #if (defined(TINYMIND_USE_SIGMOID_19_13))
    struct SigmoidValuesTableQ19_13
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_19_13))
    #if (defined(TINYMIND_USE_SIGMOID_20_12))
    struct SigmoidValuesTableQ20_12
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_20_12))
    #if (defined(TINYMIND_USE_SIGMOID_21_11))
    struct SigmoidValuesTableQ21_11
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_21_11))
    #if (defined(TINYMIND_USE_SIGMOID_22_10))
    struct SigmoidValuesTableQ22_10
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_22_10))
    #if (defined(TINYMIND_USE_SIGMOID_23_9))
    struct SigmoidValuesTableQ23_9
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_23_9))
    #if (defined(TINYMIND_USE_SIGMOID_24_8))
    struct SigmoidValuesTableQ24_8
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_24_8))
    #if (defined(TINYMIND_USE_SIGMOID_25_7))
    struct SigmoidValuesTableQ25_7
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_25_7))
    #if (defined(TINYMIND_USE_SIGMOID_26_6))
    struct SigmoidValuesTableQ26_6
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_26_6))
    #if (defined(TINYMIND_USE_SIGMOID_27_5))
    struct SigmoidValuesTableQ27_5
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_27_5))
    #if (defined(TINYMIND_USE_SIGMOID_28_4))
    struct SigmoidValuesTableQ28_4
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_28_4))
    #if (defined(TINYMIND_USE_SIGMOID_29_3))
    struct SigmoidValuesTableQ29_3
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_29_3))
    #if (defined(TINYMIND_USE_SIGMOID_30_2))
    struct SigmoidValuesTableQ30_2
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_30_2))
    #if (defined(TINYMIND_USE_SIGMOID_31_1))
    struct SigmoidValuesTableQ31_1
    {
        static const uint32_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_31_1))
}
