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
    #if (defined(TINYMIND_USE_EXP_1_15))
    struct ExpValuesTableQ1_15
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_1_15))
    #if (defined(TINYMIND_USE_EXP_2_14))
    struct ExpValuesTableQ2_14
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_2_14))
    #if (defined(TINYMIND_USE_EXP_3_13))
    struct ExpValuesTableQ3_13
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_3_13))
    #if (defined(TINYMIND_USE_EXP_4_12))
    struct ExpValuesTableQ4_12
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_4_12))
    #if (defined(TINYMIND_USE_EXP_5_11))
    struct ExpValuesTableQ5_11
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_5_11))
    #if (defined(TINYMIND_USE_EXP_6_10))
    struct ExpValuesTableQ6_10
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_6_10))
    #if (defined(TINYMIND_USE_EXP_7_9))
    struct ExpValuesTableQ7_9
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_7_9))
    #if (defined(TINYMIND_USE_EXP_8_8))
    struct ExpValuesTableQ8_8
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_8_8))
    #if (defined(TINYMIND_USE_EXP_9_7))
    struct ExpValuesTableQ9_7
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_9_7))
    #if (defined(TINYMIND_USE_EXP_10_6))
    struct ExpValuesTableQ10_6
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_10_6))
    #if (defined(TINYMIND_USE_EXP_11_5))
    struct ExpValuesTableQ11_5
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_11_5))
    #if (defined(TINYMIND_USE_EXP_12_4))
    struct ExpValuesTableQ12_4
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_12_4))
    #if (defined(TINYMIND_USE_EXP_13_3))
    struct ExpValuesTableQ13_3
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_13_3))
    #if (defined(TINYMIND_USE_EXP_14_2))
    struct ExpValuesTableQ14_2
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_14_2))
    #if (defined(TINYMIND_USE_EXP_15_1))
    struct ExpValuesTableQ15_1
    {
        static const uint16_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_EXP_15_1))
}
