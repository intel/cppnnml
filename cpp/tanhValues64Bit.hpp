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
    #if (defined(TINYMIND_USE_TANH_1_63))
    struct TanhValuesTableQ1_63
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_1_63))
    #if (defined(TINYMIND_USE_TANH_2_62))
    struct TanhValuesTableQ2_62
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_2_62))
    #if (defined(TINYMIND_USE_TANH_3_61))
    struct TanhValuesTableQ3_61
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_3_61))
    #if (defined(TINYMIND_USE_TANH_4_60))
    struct TanhValuesTableQ4_60
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_4_60))
    #if (defined(TINYMIND_USE_TANH_5_59))
    struct TanhValuesTableQ5_59
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_5_59))
    #if (defined(TINYMIND_USE_TANH_6_58))
    struct TanhValuesTableQ6_58
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_6_58))
    #if (defined(TINYMIND_USE_TANH_7_57))
    struct TanhValuesTableQ7_57
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_7_57))
    #if (defined(TINYMIND_USE_TANH_8_56))
    struct TanhValuesTableQ8_56
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_8_56))
    #if (defined(TINYMIND_USE_TANH_9_55))
    struct TanhValuesTableQ9_55
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_9_55))
    #if (defined(TINYMIND_USE_TANH_10_54))
    struct TanhValuesTableQ10_54
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_10_54))
    #if (defined(TINYMIND_USE_TANH_11_53))
    struct TanhValuesTableQ11_53
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_11_53))
    #if (defined(TINYMIND_USE_TANH_12_52))
    struct TanhValuesTableQ12_52
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_12_52))
    #if (defined(TINYMIND_USE_TANH_13_51))
    struct TanhValuesTableQ13_51
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_13_51))
    #if (defined(TINYMIND_USE_TANH_14_50))
    struct TanhValuesTableQ14_50
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_14_50))
    #if (defined(TINYMIND_USE_TANH_15_49))
    struct TanhValuesTableQ15_49
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_15_49))
    #if (defined(TINYMIND_USE_TANH_16_48))
    struct TanhValuesTableQ16_48
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_16_48))
    #if (defined(TINYMIND_USE_TANH_17_47))
    struct TanhValuesTableQ17_47
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_17_47))
    #if (defined(TINYMIND_USE_TANH_18_46))
    struct TanhValuesTableQ18_46
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_18_46))
    #if (defined(TINYMIND_USE_TANH_19_45))
    struct TanhValuesTableQ19_45
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_19_45))
    #if (defined(TINYMIND_USE_TANH_20_44))
    struct TanhValuesTableQ20_44
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_20_44))
    #if (defined(TINYMIND_USE_TANH_21_43))
    struct TanhValuesTableQ21_43
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_21_43))
    #if (defined(TINYMIND_USE_TANH_22_42))
    struct TanhValuesTableQ22_42
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_22_42))
    #if (defined(TINYMIND_USE_TANH_23_41))
    struct TanhValuesTableQ23_41
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_23_41))
    #if (defined(TINYMIND_USE_TANH_24_40))
    struct TanhValuesTableQ24_40
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_24_40))
    #if (defined(TINYMIND_USE_TANH_25_39))
    struct TanhValuesTableQ25_39
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_25_39))
    #if (defined(TINYMIND_USE_TANH_26_38))
    struct TanhValuesTableQ26_38
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_26_38))
    #if (defined(TINYMIND_USE_TANH_27_37))
    struct TanhValuesTableQ27_37
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_27_37))
    #if (defined(TINYMIND_USE_TANH_28_36))
    struct TanhValuesTableQ28_36
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_28_36))
    #if (defined(TINYMIND_USE_TANH_29_35))
    struct TanhValuesTableQ29_35
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_29_35))
    #if (defined(TINYMIND_USE_TANH_30_34))
    struct TanhValuesTableQ30_34
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_30_34))
    #if (defined(TINYMIND_USE_TANH_31_33))
    struct TanhValuesTableQ31_33
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_31_33))
    #if (defined(TINYMIND_USE_TANH_32_32))
    struct TanhValuesTableQ32_32
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_32_32))
    #if (defined(TINYMIND_USE_TANH_33_31))
    struct TanhValuesTableQ33_31
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_33_31))
    #if (defined(TINYMIND_USE_TANH_34_30))
    struct TanhValuesTableQ34_30
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_34_30))
    #if (defined(TINYMIND_USE_TANH_35_29))
    struct TanhValuesTableQ35_29
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_35_29))
    #if (defined(TINYMIND_USE_TANH_36_28))
    struct TanhValuesTableQ36_28
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_36_28))
    #if (defined(TINYMIND_USE_TANH_37_27))
    struct TanhValuesTableQ37_27
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_37_27))
    #if (defined(TINYMIND_USE_TANH_38_26))
    struct TanhValuesTableQ38_26
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_38_26))
    #if (defined(TINYMIND_USE_TANH_39_25))
    struct TanhValuesTableQ39_25
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_39_25))
    #if (defined(TINYMIND_USE_TANH_40_24))
    struct TanhValuesTableQ40_24
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_40_24))
    #if (defined(TINYMIND_USE_TANH_41_23))
    struct TanhValuesTableQ41_23
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_41_23))
    #if (defined(TINYMIND_USE_TANH_42_22))
    struct TanhValuesTableQ42_22
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_42_22))
    #if (defined(TINYMIND_USE_TANH_43_21))
    struct TanhValuesTableQ43_21
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_43_21))
    #if (defined(TINYMIND_USE_TANH_44_20))
    struct TanhValuesTableQ44_20
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_44_20))
    #if (defined(TINYMIND_USE_TANH_45_19))
    struct TanhValuesTableQ45_19
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_45_19))
    #if (defined(TINYMIND_USE_TANH_46_18))
    struct TanhValuesTableQ46_18
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_46_18))
    #if (defined(TINYMIND_USE_TANH_47_17))
    struct TanhValuesTableQ47_17
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_47_17))
    #if (defined(TINYMIND_USE_TANH_48_16))
    struct TanhValuesTableQ48_16
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_48_16))
    #if (defined(TINYMIND_USE_TANH_49_15))
    struct TanhValuesTableQ49_15
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_49_15))
    #if (defined(TINYMIND_USE_TANH_50_14))
    struct TanhValuesTableQ50_14
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_50_14))
    #if (defined(TINYMIND_USE_TANH_51_13))
    struct TanhValuesTableQ51_13
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_51_13))
    #if (defined(TINYMIND_USE_TANH_52_12))
    struct TanhValuesTableQ52_12
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_52_12))
    #if (defined(TINYMIND_USE_TANH_53_11))
    struct TanhValuesTableQ53_11
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_53_11))
    #if (defined(TINYMIND_USE_TANH_54_10))
    struct TanhValuesTableQ54_10
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_54_10))
    #if (defined(TINYMIND_USE_TANH_55_9))
    struct TanhValuesTableQ55_9
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_55_9))
    #if (defined(TINYMIND_USE_TANH_56_8))
    struct TanhValuesTableQ56_8
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_56_8))
    #if (defined(TINYMIND_USE_TANH_57_7))
    struct TanhValuesTableQ57_7
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_57_7))
    #if (defined(TINYMIND_USE_TANH_58_6))
    struct TanhValuesTableQ58_6
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_58_6))
    #if (defined(TINYMIND_USE_TANH_59_5))
    struct TanhValuesTableQ59_5
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_59_5))
    #if (defined(TINYMIND_USE_TANH_60_4))
    struct TanhValuesTableQ60_4
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_60_4))
    #if (defined(TINYMIND_USE_TANH_61_3))
    struct TanhValuesTableQ61_3
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_61_3))
    #if (defined(TINYMIND_USE_TANH_62_2))
    struct TanhValuesTableQ62_2
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_62_2))
    #if (defined(TINYMIND_USE_TANH_63_1))
    struct TanhValuesTableQ63_1
    {
        static const uint64_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];
    };
    #endif // (defined(TINYMIND_USE_TANH_63_1))
}
