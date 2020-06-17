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
    #if (defined(TINYMIND_USE_SIGMOID_1_63))
    struct SigmoidValuesTableQ1_63
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_63))
    #if (defined(TINYMIND_USE_SIGMOID_2_62))
    struct SigmoidValuesTableQ2_62
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_62))
    #if (defined(TINYMIND_USE_SIGMOID_3_61))
    struct SigmoidValuesTableQ3_61
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_61))
    #if (defined(TINYMIND_USE_SIGMOID_4_60))
    struct SigmoidValuesTableQ4_60
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_60))
    #if (defined(TINYMIND_USE_SIGMOID_5_59))
    struct SigmoidValuesTableQ5_59
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_59))
    #if (defined(TINYMIND_USE_SIGMOID_6_58))
    struct SigmoidValuesTableQ6_58
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_58))
    #if (defined(TINYMIND_USE_SIGMOID_7_57))
    struct SigmoidValuesTableQ7_57
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_57))
    #if (defined(TINYMIND_USE_SIGMOID_8_56))
    struct SigmoidValuesTableQ8_56
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_56))
    #if (defined(TINYMIND_USE_SIGMOID_9_55))
    struct SigmoidValuesTableQ9_55
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_55))
    #if (defined(TINYMIND_USE_SIGMOID_10_54))
    struct SigmoidValuesTableQ10_54
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_54))
    #if (defined(TINYMIND_USE_SIGMOID_11_53))
    struct SigmoidValuesTableQ11_53
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_53))
    #if (defined(TINYMIND_USE_SIGMOID_12_52))
    struct SigmoidValuesTableQ12_52
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_52))
    #if (defined(TINYMIND_USE_SIGMOID_13_51))
    struct SigmoidValuesTableQ13_51
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_51))
    #if (defined(TINYMIND_USE_SIGMOID_14_50))
    struct SigmoidValuesTableQ14_50
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_50))
    #if (defined(TINYMIND_USE_SIGMOID_15_49))
    struct SigmoidValuesTableQ15_49
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_49))
    #if (defined(TINYMIND_USE_SIGMOID_16_48))
    struct SigmoidValuesTableQ16_48
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_16_48))
    #if (defined(TINYMIND_USE_SIGMOID_17_47))
    struct SigmoidValuesTableQ17_47
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_17_47))
    #if (defined(TINYMIND_USE_SIGMOID_18_46))
    struct SigmoidValuesTableQ18_46
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_18_46))
    #if (defined(TINYMIND_USE_SIGMOID_19_45))
    struct SigmoidValuesTableQ19_45
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_19_45))
    #if (defined(TINYMIND_USE_SIGMOID_20_44))
    struct SigmoidValuesTableQ20_44
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_20_44))
    #if (defined(TINYMIND_USE_SIGMOID_21_43))
    struct SigmoidValuesTableQ21_43
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_21_43))
    #if (defined(TINYMIND_USE_SIGMOID_22_42))
    struct SigmoidValuesTableQ22_42
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_22_42))
    #if (defined(TINYMIND_USE_SIGMOID_23_41))
    struct SigmoidValuesTableQ23_41
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_23_41))
    #if (defined(TINYMIND_USE_SIGMOID_24_40))
    struct SigmoidValuesTableQ24_40
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_24_40))
    #if (defined(TINYMIND_USE_SIGMOID_25_39))
    struct SigmoidValuesTableQ25_39
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_25_39))
    #if (defined(TINYMIND_USE_SIGMOID_26_38))
    struct SigmoidValuesTableQ26_38
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_26_38))
    #if (defined(TINYMIND_USE_SIGMOID_27_37))
    struct SigmoidValuesTableQ27_37
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_27_37))
    #if (defined(TINYMIND_USE_SIGMOID_28_36))
    struct SigmoidValuesTableQ28_36
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_28_36))
    #if (defined(TINYMIND_USE_SIGMOID_29_35))
    struct SigmoidValuesTableQ29_35
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_29_35))
    #if (defined(TINYMIND_USE_SIGMOID_30_34))
    struct SigmoidValuesTableQ30_34
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_30_34))
    #if (defined(TINYMIND_USE_SIGMOID_31_33))
    struct SigmoidValuesTableQ31_33
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_31_33))
    #if (defined(TINYMIND_USE_SIGMOID_32_32))
    struct SigmoidValuesTableQ32_32
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_32_32))
    #if (defined(TINYMIND_USE_SIGMOID_33_31))
    struct SigmoidValuesTableQ33_31
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_33_31))
    #if (defined(TINYMIND_USE_SIGMOID_34_30))
    struct SigmoidValuesTableQ34_30
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_34_30))
    #if (defined(TINYMIND_USE_SIGMOID_35_29))
    struct SigmoidValuesTableQ35_29
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_35_29))
    #if (defined(TINYMIND_USE_SIGMOID_36_28))
    struct SigmoidValuesTableQ36_28
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_36_28))
    #if (defined(TINYMIND_USE_SIGMOID_37_27))
    struct SigmoidValuesTableQ37_27
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_37_27))
    #if (defined(TINYMIND_USE_SIGMOID_38_26))
    struct SigmoidValuesTableQ38_26
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_38_26))
    #if (defined(TINYMIND_USE_SIGMOID_39_25))
    struct SigmoidValuesTableQ39_25
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_39_25))
    #if (defined(TINYMIND_USE_SIGMOID_40_24))
    struct SigmoidValuesTableQ40_24
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_40_24))
    #if (defined(TINYMIND_USE_SIGMOID_41_23))
    struct SigmoidValuesTableQ41_23
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_41_23))
    #if (defined(TINYMIND_USE_SIGMOID_42_22))
    struct SigmoidValuesTableQ42_22
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_42_22))
    #if (defined(TINYMIND_USE_SIGMOID_43_21))
    struct SigmoidValuesTableQ43_21
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_43_21))
    #if (defined(TINYMIND_USE_SIGMOID_44_20))
    struct SigmoidValuesTableQ44_20
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_44_20))
    #if (defined(TINYMIND_USE_SIGMOID_45_19))
    struct SigmoidValuesTableQ45_19
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_45_19))
    #if (defined(TINYMIND_USE_SIGMOID_46_18))
    struct SigmoidValuesTableQ46_18
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_46_18))
    #if (defined(TINYMIND_USE_SIGMOID_47_17))
    struct SigmoidValuesTableQ47_17
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_47_17))
    #if (defined(TINYMIND_USE_SIGMOID_48_16))
    struct SigmoidValuesTableQ48_16
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_48_16))
    #if (defined(TINYMIND_USE_SIGMOID_49_15))
    struct SigmoidValuesTableQ49_15
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_49_15))
    #if (defined(TINYMIND_USE_SIGMOID_50_14))
    struct SigmoidValuesTableQ50_14
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_50_14))
    #if (defined(TINYMIND_USE_SIGMOID_51_13))
    struct SigmoidValuesTableQ51_13
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_51_13))
    #if (defined(TINYMIND_USE_SIGMOID_52_12))
    struct SigmoidValuesTableQ52_12
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_52_12))
    #if (defined(TINYMIND_USE_SIGMOID_53_11))
    struct SigmoidValuesTableQ53_11
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_53_11))
    #if (defined(TINYMIND_USE_SIGMOID_54_10))
    struct SigmoidValuesTableQ54_10
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_54_10))
    #if (defined(TINYMIND_USE_SIGMOID_55_9))
    struct SigmoidValuesTableQ55_9
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_55_9))
    #if (defined(TINYMIND_USE_SIGMOID_56_8))
    struct SigmoidValuesTableQ56_8
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_56_8))
    #if (defined(TINYMIND_USE_SIGMOID_57_7))
    struct SigmoidValuesTableQ57_7
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_57_7))
    #if (defined(TINYMIND_USE_SIGMOID_58_6))
    struct SigmoidValuesTableQ58_6
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_58_6))
    #if (defined(TINYMIND_USE_SIGMOID_59_5))
    struct SigmoidValuesTableQ59_5
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_59_5))
    #if (defined(TINYMIND_USE_SIGMOID_60_4))
    struct SigmoidValuesTableQ60_4
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_60_4))
    #if (defined(TINYMIND_USE_SIGMOID_61_3))
    struct SigmoidValuesTableQ61_3
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_61_3))
    #if (defined(TINYMIND_USE_SIGMOID_62_2))
    struct SigmoidValuesTableQ62_2
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_62_2))
    #if (defined(TINYMIND_USE_SIGMOID_63_1))
    struct SigmoidValuesTableQ63_1
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_63_1))
}
