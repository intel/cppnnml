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
    #if (defined(TINYMIND_USE_SIGMOID_1_15))
    struct SigmoidValuesTableQ1_15
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_15))
    #if (defined(TINYMIND_USE_SIGMOID_2_14))
    struct SigmoidValuesTableQ2_14
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_14))
    #if (defined(TINYMIND_USE_SIGMOID_3_13))
    struct SigmoidValuesTableQ3_13
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_13))
    #if (defined(TINYMIND_USE_SIGMOID_4_12))
    struct SigmoidValuesTableQ4_12
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_12))
    #if (defined(TINYMIND_USE_SIGMOID_5_11))
    struct SigmoidValuesTableQ5_11
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_11))
    #if (defined(TINYMIND_USE_SIGMOID_6_10))
    struct SigmoidValuesTableQ6_10
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_10))
    #if (defined(TINYMIND_USE_SIGMOID_7_9))
    struct SigmoidValuesTableQ7_9
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_9))
    #if (defined(TINYMIND_USE_SIGMOID_8_8))
    struct SigmoidValuesTableQ8_8
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_8))
    #if (defined(TINYMIND_USE_SIGMOID_9_7))
    struct SigmoidValuesTableQ9_7
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_7))
    #if (defined(TINYMIND_USE_SIGMOID_10_6))
    struct SigmoidValuesTableQ10_6
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_6))
    #if (defined(TINYMIND_USE_SIGMOID_11_5))
    struct SigmoidValuesTableQ11_5
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_5))
    #if (defined(TINYMIND_USE_SIGMOID_12_4))
    struct SigmoidValuesTableQ12_4
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_4))
    #if (defined(TINYMIND_USE_SIGMOID_13_3))
    struct SigmoidValuesTableQ13_3
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_3))
    #if (defined(TINYMIND_USE_SIGMOID_14_2))
    struct SigmoidValuesTableQ14_2
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_2))
    #if (defined(TINYMIND_USE_SIGMOID_15_1))
    struct SigmoidValuesTableQ15_1
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_1))
}
