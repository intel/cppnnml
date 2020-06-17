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
    #if (defined(TINYMIND_USE_TANH_1_15))
    struct TanhValuesTableQ1_15
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_1_15))
    #if (defined(TINYMIND_USE_TANH_2_14))
    struct TanhValuesTableQ2_14
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_2_14))
    #if (defined(TINYMIND_USE_TANH_3_13))
    struct TanhValuesTableQ3_13
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_3_13))
    #if (defined(TINYMIND_USE_TANH_4_12))
    struct TanhValuesTableQ4_12
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_4_12))
    #if (defined(TINYMIND_USE_TANH_5_11))
    struct TanhValuesTableQ5_11
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_5_11))
    #if (defined(TINYMIND_USE_TANH_6_10))
    struct TanhValuesTableQ6_10
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_6_10))
    #if (defined(TINYMIND_USE_TANH_7_9))
    struct TanhValuesTableQ7_9
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_7_9))
    #if (defined(TINYMIND_USE_TANH_8_8))
    struct TanhValuesTableQ8_8
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_8_8))
    #if (defined(TINYMIND_USE_TANH_9_7))
    struct TanhValuesTableQ9_7
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_9_7))
    #if (defined(TINYMIND_USE_TANH_10_6))
    struct TanhValuesTableQ10_6
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_10_6))
    #if (defined(TINYMIND_USE_TANH_11_5))
    struct TanhValuesTableQ11_5
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_11_5))
    #if (defined(TINYMIND_USE_TANH_12_4))
    struct TanhValuesTableQ12_4
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_12_4))
    #if (defined(TINYMIND_USE_TANH_13_3))
    struct TanhValuesTableQ13_3
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_13_3))
    #if (defined(TINYMIND_USE_TANH_14_2))
    struct TanhValuesTableQ14_2
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_14_2))
    #if (defined(TINYMIND_USE_TANH_15_1))
    struct TanhValuesTableQ15_1
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_15_1))
}
