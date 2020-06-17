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

#include "expValues8Bit.hpp"
#include "expValues16Bit.hpp"
#include "expValues32Bit.hpp"
#include "expValues64Bit.hpp"

namespace tinymind {
    template<unsigned FixedBits, unsigned FracBits, bool IsSigned>
    struct ExpTableValueSize
    {
    };

    #if (defined(TINYMIND_USE_EXP_1_7))
    template<>
    struct ExpTableValueSize<1, 7, true>
    {
        typedef ExpValuesTableQ1_7 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_1_7))

    #if (defined(TINYMIND_USE_EXP_2_6))
    template<>
    struct ExpTableValueSize<2, 6, true>
    {
        typedef ExpValuesTableQ2_6 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_2_6))

    #if (defined(TINYMIND_USE_EXP_3_5))
    template<>
    struct ExpTableValueSize<3, 5, true>
    {
        typedef ExpValuesTableQ3_5 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_3_5))

    #if (defined(TINYMIND_USE_EXP_4_4))
    template<>
    struct ExpTableValueSize<4, 4, true>
    {
        typedef ExpValuesTableQ4_4 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_4_4))

    #if (defined(TINYMIND_USE_EXP_5_3))
    template<>
    struct ExpTableValueSize<5, 3, true>
    {
        typedef ExpValuesTableQ5_3 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_5_3))

    #if (defined(TINYMIND_USE_EXP_6_2))
    template<>
    struct ExpTableValueSize<6, 2, true>
    {
        typedef ExpValuesTableQ6_2 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_6_2))

    #if (defined(TINYMIND_USE_EXP_7_1))
    template<>
    struct ExpTableValueSize<7, 1, true>
    {
        typedef ExpValuesTableQ7_1 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_7_1))

    #if (defined(TINYMIND_USE_EXP_1_15))
    template<>
    struct ExpTableValueSize<1, 15, true>
    {
        typedef ExpValuesTableQ1_15 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_1_15))

    #if (defined(TINYMIND_USE_EXP_2_14))
    template<>
    struct ExpTableValueSize<2, 14, true>
    {
        typedef ExpValuesTableQ2_14 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_2_14))

    #if (defined(TINYMIND_USE_EXP_3_13))
    template<>
    struct ExpTableValueSize<3, 13, true>
    {
        typedef ExpValuesTableQ3_13 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_3_13))

    #if (defined(TINYMIND_USE_EXP_4_12))
    template<>
    struct ExpTableValueSize<4, 12, true>
    {
        typedef ExpValuesTableQ4_12 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_4_12))

    #if (defined(TINYMIND_USE_EXP_5_11))
    template<>
    struct ExpTableValueSize<5, 11, true>
    {
        typedef ExpValuesTableQ5_11 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_5_11))

    #if (defined(TINYMIND_USE_EXP_6_10))
    template<>
    struct ExpTableValueSize<6, 10, true>
    {
        typedef ExpValuesTableQ6_10 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_6_10))

    #if (defined(TINYMIND_USE_EXP_7_9))
    template<>
    struct ExpTableValueSize<7, 9, true>
    {
        typedef ExpValuesTableQ7_9 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_7_9))

    #if (defined(TINYMIND_USE_EXP_8_8))
    template<>
    struct ExpTableValueSize<8, 8, true>
    {
        typedef ExpValuesTableQ8_8 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_8_8))

    #if (defined(TINYMIND_USE_EXP_9_7))
    template<>
    struct ExpTableValueSize<9, 7, true>
    {
        typedef ExpValuesTableQ9_7 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_9_7))

    #if (defined(TINYMIND_USE_EXP_10_6))
    template<>
    struct ExpTableValueSize<10, 6, true>
    {
        typedef ExpValuesTableQ10_6 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_10_6))

    #if (defined(TINYMIND_USE_EXP_11_5))
    template<>
    struct ExpTableValueSize<11, 5, true>
    {
        typedef ExpValuesTableQ11_5 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_11_5))

    #if (defined(TINYMIND_USE_EXP_12_4))
    template<>
    struct ExpTableValueSize<12, 4, true>
    {
        typedef ExpValuesTableQ12_4 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_12_4))

    #if (defined(TINYMIND_USE_EXP_13_3))
    template<>
    struct ExpTableValueSize<13, 3, true>
    {
        typedef ExpValuesTableQ13_3 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_13_3))

    #if (defined(TINYMIND_USE_EXP_14_2))
    template<>
    struct ExpTableValueSize<14, 2, true>
    {
        typedef ExpValuesTableQ14_2 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_14_2))

    #if (defined(TINYMIND_USE_EXP_15_1))
    template<>
    struct ExpTableValueSize<15, 1, true>
    {
        typedef ExpValuesTableQ15_1 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_15_1))

    #if (defined(TINYMIND_USE_EXP_1_31))
    template<>
    struct ExpTableValueSize<1, 31, true>
    {
        typedef ExpValuesTableQ1_31 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_1_31))

    #if (defined(TINYMIND_USE_EXP_2_30))
    template<>
    struct ExpTableValueSize<2, 30, true>
    {
        typedef ExpValuesTableQ2_30 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_2_30))

    #if (defined(TINYMIND_USE_EXP_3_29))
    template<>
    struct ExpTableValueSize<3, 29, true>
    {
        typedef ExpValuesTableQ3_29 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_3_29))

    #if (defined(TINYMIND_USE_EXP_4_28))
    template<>
    struct ExpTableValueSize<4, 28, true>
    {
        typedef ExpValuesTableQ4_28 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_4_28))

    #if (defined(TINYMIND_USE_EXP_5_27))
    template<>
    struct ExpTableValueSize<5, 27, true>
    {
        typedef ExpValuesTableQ5_27 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_5_27))

    #if (defined(TINYMIND_USE_EXP_6_26))
    template<>
    struct ExpTableValueSize<6, 26, true>
    {
        typedef ExpValuesTableQ6_26 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_6_26))

    #if (defined(TINYMIND_USE_EXP_7_25))
    template<>
    struct ExpTableValueSize<7, 25, true>
    {
        typedef ExpValuesTableQ7_25 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_7_25))

    #if (defined(TINYMIND_USE_EXP_8_24))
    template<>
    struct ExpTableValueSize<8, 24, true>
    {
        typedef ExpValuesTableQ8_24 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_8_24))

    #if (defined(TINYMIND_USE_EXP_9_23))
    template<>
    struct ExpTableValueSize<9, 23, true>
    {
        typedef ExpValuesTableQ9_23 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_9_23))

    #if (defined(TINYMIND_USE_EXP_10_22))
    template<>
    struct ExpTableValueSize<10, 22, true>
    {
        typedef ExpValuesTableQ10_22 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_10_22))

    #if (defined(TINYMIND_USE_EXP_11_21))
    template<>
    struct ExpTableValueSize<11, 21, true>
    {
        typedef ExpValuesTableQ11_21 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_11_21))

    #if (defined(TINYMIND_USE_EXP_12_20))
    template<>
    struct ExpTableValueSize<12, 20, true>
    {
        typedef ExpValuesTableQ12_20 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_12_20))

    #if (defined(TINYMIND_USE_EXP_13_19))
    template<>
    struct ExpTableValueSize<13, 19, true>
    {
        typedef ExpValuesTableQ13_19 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_13_19))

    #if (defined(TINYMIND_USE_EXP_14_18))
    template<>
    struct ExpTableValueSize<14, 18, true>
    {
        typedef ExpValuesTableQ14_18 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_14_18))

    #if (defined(TINYMIND_USE_EXP_15_17))
    template<>
    struct ExpTableValueSize<15, 17, true>
    {
        typedef ExpValuesTableQ15_17 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_15_17))

    #if (defined(TINYMIND_USE_EXP_16_16))
    template<>
    struct ExpTableValueSize<16, 16, true>
    {
        typedef ExpValuesTableQ16_16 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_16_16))

    #if (defined(TINYMIND_USE_EXP_17_15))
    template<>
    struct ExpTableValueSize<17, 15, true>
    {
        typedef ExpValuesTableQ17_15 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_17_15))

    #if (defined(TINYMIND_USE_EXP_18_14))
    template<>
    struct ExpTableValueSize<18, 14, true>
    {
        typedef ExpValuesTableQ18_14 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_18_14))

    #if (defined(TINYMIND_USE_EXP_19_13))
    template<>
    struct ExpTableValueSize<19, 13, true>
    {
        typedef ExpValuesTableQ19_13 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_19_13))

    #if (defined(TINYMIND_USE_EXP_20_12))
    template<>
    struct ExpTableValueSize<20, 12, true>
    {
        typedef ExpValuesTableQ20_12 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_20_12))

    #if (defined(TINYMIND_USE_EXP_21_11))
    template<>
    struct ExpTableValueSize<21, 11, true>
    {
        typedef ExpValuesTableQ21_11 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_21_11))

    #if (defined(TINYMIND_USE_EXP_22_10))
    template<>
    struct ExpTableValueSize<22, 10, true>
    {
        typedef ExpValuesTableQ22_10 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_22_10))

    #if (defined(TINYMIND_USE_EXP_23_9))
    template<>
    struct ExpTableValueSize<23, 9, true>
    {
        typedef ExpValuesTableQ23_9 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_23_9))

    #if (defined(TINYMIND_USE_EXP_24_8))
    template<>
    struct ExpTableValueSize<24, 8, true>
    {
        typedef ExpValuesTableQ24_8 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_24_8))

    #if (defined(TINYMIND_USE_EXP_25_7))
    template<>
    struct ExpTableValueSize<25, 7, true>
    {
        typedef ExpValuesTableQ25_7 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_25_7))

    #if (defined(TINYMIND_USE_EXP_26_6))
    template<>
    struct ExpTableValueSize<26, 6, true>
    {
        typedef ExpValuesTableQ26_6 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_26_6))

    #if (defined(TINYMIND_USE_EXP_27_5))
    template<>
    struct ExpTableValueSize<27, 5, true>
    {
        typedef ExpValuesTableQ27_5 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_27_5))

    #if (defined(TINYMIND_USE_EXP_28_4))
    template<>
    struct ExpTableValueSize<28, 4, true>
    {
        typedef ExpValuesTableQ28_4 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_28_4))

    #if (defined(TINYMIND_USE_EXP_29_3))
    template<>
    struct ExpTableValueSize<29, 3, true>
    {
        typedef ExpValuesTableQ29_3 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_29_3))

    #if (defined(TINYMIND_USE_EXP_30_2))
    template<>
    struct ExpTableValueSize<30, 2, true>
    {
        typedef ExpValuesTableQ30_2 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_30_2))

    #if (defined(TINYMIND_USE_EXP_31_1))
    template<>
    struct ExpTableValueSize<31, 1, true>
    {
        typedef ExpValuesTableQ31_1 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_31_1))

    #if (defined(TINYMIND_USE_EXP_1_63))
    template<>
    struct ExpTableValueSize<1, 63, true>
    {
        typedef ExpValuesTableQ1_63 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_1_63))

    #if (defined(TINYMIND_USE_EXP_2_62))
    template<>
    struct ExpTableValueSize<2, 62, true>
    {
        typedef ExpValuesTableQ2_62 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_2_62))

    #if (defined(TINYMIND_USE_EXP_3_61))
    template<>
    struct ExpTableValueSize<3, 61, true>
    {
        typedef ExpValuesTableQ3_61 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_3_61))

    #if (defined(TINYMIND_USE_EXP_4_60))
    template<>
    struct ExpTableValueSize<4, 60, true>
    {
        typedef ExpValuesTableQ4_60 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_4_60))

    #if (defined(TINYMIND_USE_EXP_5_59))
    template<>
    struct ExpTableValueSize<5, 59, true>
    {
        typedef ExpValuesTableQ5_59 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_5_59))

    #if (defined(TINYMIND_USE_EXP_6_58))
    template<>
    struct ExpTableValueSize<6, 58, true>
    {
        typedef ExpValuesTableQ6_58 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_6_58))

    #if (defined(TINYMIND_USE_EXP_7_57))
    template<>
    struct ExpTableValueSize<7, 57, true>
    {
        typedef ExpValuesTableQ7_57 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_7_57))

    #if (defined(TINYMIND_USE_EXP_8_56))
    template<>
    struct ExpTableValueSize<8, 56, true>
    {
        typedef ExpValuesTableQ8_56 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_8_56))

    #if (defined(TINYMIND_USE_EXP_9_55))
    template<>
    struct ExpTableValueSize<9, 55, true>
    {
        typedef ExpValuesTableQ9_55 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_9_55))

    #if (defined(TINYMIND_USE_EXP_10_54))
    template<>
    struct ExpTableValueSize<10, 54, true>
    {
        typedef ExpValuesTableQ10_54 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_10_54))

    #if (defined(TINYMIND_USE_EXP_11_53))
    template<>
    struct ExpTableValueSize<11, 53, true>
    {
        typedef ExpValuesTableQ11_53 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_11_53))

    #if (defined(TINYMIND_USE_EXP_12_52))
    template<>
    struct ExpTableValueSize<12, 52, true>
    {
        typedef ExpValuesTableQ12_52 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_12_52))

    #if (defined(TINYMIND_USE_EXP_13_51))
    template<>
    struct ExpTableValueSize<13, 51, true>
    {
        typedef ExpValuesTableQ13_51 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_13_51))

    #if (defined(TINYMIND_USE_EXP_14_50))
    template<>
    struct ExpTableValueSize<14, 50, true>
    {
        typedef ExpValuesTableQ14_50 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_14_50))

    #if (defined(TINYMIND_USE_EXP_15_49))
    template<>
    struct ExpTableValueSize<15, 49, true>
    {
        typedef ExpValuesTableQ15_49 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_15_49))

    #if (defined(TINYMIND_USE_EXP_16_48))
    template<>
    struct ExpTableValueSize<16, 48, true>
    {
        typedef ExpValuesTableQ16_48 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_16_48))

    #if (defined(TINYMIND_USE_EXP_17_47))
    template<>
    struct ExpTableValueSize<17, 47, true>
    {
        typedef ExpValuesTableQ17_47 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_17_47))

    #if (defined(TINYMIND_USE_EXP_18_46))
    template<>
    struct ExpTableValueSize<18, 46, true>
    {
        typedef ExpValuesTableQ18_46 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_18_46))

    #if (defined(TINYMIND_USE_EXP_19_45))
    template<>
    struct ExpTableValueSize<19, 45, true>
    {
        typedef ExpValuesTableQ19_45 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_19_45))

    #if (defined(TINYMIND_USE_EXP_20_44))
    template<>
    struct ExpTableValueSize<20, 44, true>
    {
        typedef ExpValuesTableQ20_44 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_20_44))

    #if (defined(TINYMIND_USE_EXP_21_43))
    template<>
    struct ExpTableValueSize<21, 43, true>
    {
        typedef ExpValuesTableQ21_43 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_21_43))

    #if (defined(TINYMIND_USE_EXP_22_42))
    template<>
    struct ExpTableValueSize<22, 42, true>
    {
        typedef ExpValuesTableQ22_42 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_22_42))

    #if (defined(TINYMIND_USE_EXP_23_41))
    template<>
    struct ExpTableValueSize<23, 41, true>
    {
        typedef ExpValuesTableQ23_41 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_23_41))

    #if (defined(TINYMIND_USE_EXP_24_40))
    template<>
    struct ExpTableValueSize<24, 40, true>
    {
        typedef ExpValuesTableQ24_40 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_24_40))

    #if (defined(TINYMIND_USE_EXP_25_39))
    template<>
    struct ExpTableValueSize<25, 39, true>
    {
        typedef ExpValuesTableQ25_39 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_25_39))

    #if (defined(TINYMIND_USE_EXP_26_38))
    template<>
    struct ExpTableValueSize<26, 38, true>
    {
        typedef ExpValuesTableQ26_38 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_26_38))

    #if (defined(TINYMIND_USE_EXP_27_37))
    template<>
    struct ExpTableValueSize<27, 37, true>
    {
        typedef ExpValuesTableQ27_37 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_27_37))

    #if (defined(TINYMIND_USE_EXP_28_36))
    template<>
    struct ExpTableValueSize<28, 36, true>
    {
        typedef ExpValuesTableQ28_36 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_28_36))

    #if (defined(TINYMIND_USE_EXP_29_35))
    template<>
    struct ExpTableValueSize<29, 35, true>
    {
        typedef ExpValuesTableQ29_35 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_29_35))

    #if (defined(TINYMIND_USE_EXP_30_34))
    template<>
    struct ExpTableValueSize<30, 34, true>
    {
        typedef ExpValuesTableQ30_34 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_30_34))

    #if (defined(TINYMIND_USE_EXP_31_33))
    template<>
    struct ExpTableValueSize<31, 33, true>
    {
        typedef ExpValuesTableQ31_33 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_31_33))

    #if (defined(TINYMIND_USE_EXP_32_32))
    template<>
    struct ExpTableValueSize<32, 32, true>
    {
        typedef ExpValuesTableQ32_32 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_32_32))

    #if (defined(TINYMIND_USE_EXP_33_31))
    template<>
    struct ExpTableValueSize<33, 31, true>
    {
        typedef ExpValuesTableQ33_31 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_33_31))

    #if (defined(TINYMIND_USE_EXP_34_30))
    template<>
    struct ExpTableValueSize<34, 30, true>
    {
        typedef ExpValuesTableQ34_30 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_34_30))

    #if (defined(TINYMIND_USE_EXP_35_29))
    template<>
    struct ExpTableValueSize<35, 29, true>
    {
        typedef ExpValuesTableQ35_29 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_35_29))

    #if (defined(TINYMIND_USE_EXP_36_28))
    template<>
    struct ExpTableValueSize<36, 28, true>
    {
        typedef ExpValuesTableQ36_28 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_36_28))

    #if (defined(TINYMIND_USE_EXP_37_27))
    template<>
    struct ExpTableValueSize<37, 27, true>
    {
        typedef ExpValuesTableQ37_27 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_37_27))

    #if (defined(TINYMIND_USE_EXP_38_26))
    template<>
    struct ExpTableValueSize<38, 26, true>
    {
        typedef ExpValuesTableQ38_26 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_38_26))

    #if (defined(TINYMIND_USE_EXP_39_25))
    template<>
    struct ExpTableValueSize<39, 25, true>
    {
        typedef ExpValuesTableQ39_25 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_39_25))

    #if (defined(TINYMIND_USE_EXP_40_24))
    template<>
    struct ExpTableValueSize<40, 24, true>
    {
        typedef ExpValuesTableQ40_24 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_40_24))

    #if (defined(TINYMIND_USE_EXP_41_23))
    template<>
    struct ExpTableValueSize<41, 23, true>
    {
        typedef ExpValuesTableQ41_23 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_41_23))

    #if (defined(TINYMIND_USE_EXP_42_22))
    template<>
    struct ExpTableValueSize<42, 22, true>
    {
        typedef ExpValuesTableQ42_22 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_42_22))

    #if (defined(TINYMIND_USE_EXP_43_21))
    template<>
    struct ExpTableValueSize<43, 21, true>
    {
        typedef ExpValuesTableQ43_21 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_43_21))

    #if (defined(TINYMIND_USE_EXP_44_20))
    template<>
    struct ExpTableValueSize<44, 20, true>
    {
        typedef ExpValuesTableQ44_20 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_44_20))

    #if (defined(TINYMIND_USE_EXP_45_19))
    template<>
    struct ExpTableValueSize<45, 19, true>
    {
        typedef ExpValuesTableQ45_19 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_45_19))

    #if (defined(TINYMIND_USE_EXP_46_18))
    template<>
    struct ExpTableValueSize<46, 18, true>
    {
        typedef ExpValuesTableQ46_18 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_46_18))

    #if (defined(TINYMIND_USE_EXP_47_17))
    template<>
    struct ExpTableValueSize<47, 17, true>
    {
        typedef ExpValuesTableQ47_17 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_47_17))

    #if (defined(TINYMIND_USE_EXP_48_16))
    template<>
    struct ExpTableValueSize<48, 16, true>
    {
        typedef ExpValuesTableQ48_16 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_48_16))

    #if (defined(TINYMIND_USE_EXP_49_15))
    template<>
    struct ExpTableValueSize<49, 15, true>
    {
        typedef ExpValuesTableQ49_15 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_49_15))

    #if (defined(TINYMIND_USE_EXP_50_14))
    template<>
    struct ExpTableValueSize<50, 14, true>
    {
        typedef ExpValuesTableQ50_14 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_50_14))

    #if (defined(TINYMIND_USE_EXP_51_13))
    template<>
    struct ExpTableValueSize<51, 13, true>
    {
        typedef ExpValuesTableQ51_13 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_51_13))

    #if (defined(TINYMIND_USE_EXP_52_12))
    template<>
    struct ExpTableValueSize<52, 12, true>
    {
        typedef ExpValuesTableQ52_12 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_52_12))

    #if (defined(TINYMIND_USE_EXP_53_11))
    template<>
    struct ExpTableValueSize<53, 11, true>
    {
        typedef ExpValuesTableQ53_11 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_53_11))

    #if (defined(TINYMIND_USE_EXP_54_10))
    template<>
    struct ExpTableValueSize<54, 10, true>
    {
        typedef ExpValuesTableQ54_10 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_54_10))

    #if (defined(TINYMIND_USE_EXP_55_9))
    template<>
    struct ExpTableValueSize<55, 9, true>
    {
        typedef ExpValuesTableQ55_9 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_55_9))

    #if (defined(TINYMIND_USE_EXP_56_8))
    template<>
    struct ExpTableValueSize<56, 8, true>
    {
        typedef ExpValuesTableQ56_8 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_56_8))

    #if (defined(TINYMIND_USE_EXP_57_7))
    template<>
    struct ExpTableValueSize<57, 7, true>
    {
        typedef ExpValuesTableQ57_7 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_57_7))

    #if (defined(TINYMIND_USE_EXP_58_6))
    template<>
    struct ExpTableValueSize<58, 6, true>
    {
        typedef ExpValuesTableQ58_6 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_58_6))

    #if (defined(TINYMIND_USE_EXP_59_5))
    template<>
    struct ExpTableValueSize<59, 5, true>
    {
        typedef ExpValuesTableQ59_5 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_59_5))

    #if (defined(TINYMIND_USE_EXP_60_4))
    template<>
    struct ExpTableValueSize<60, 4, true>
    {
        typedef ExpValuesTableQ60_4 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_60_4))

    #if (defined(TINYMIND_USE_EXP_61_3))
    template<>
    struct ExpTableValueSize<61, 3, true>
    {
        typedef ExpValuesTableQ61_3 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_61_3))

    #if (defined(TINYMIND_USE_EXP_62_2))
    template<>
    struct ExpTableValueSize<62, 2, true>
    {
        typedef ExpValuesTableQ62_2 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_62_2))

    #if (defined(TINYMIND_USE_EXP_63_1))
    template<>
    struct ExpTableValueSize<63, 1, true>
    {
        typedef ExpValuesTableQ63_1 ExpTableType;
    };
    #endif // (defined(TINYMIND_USE_EXP_63_1))

    template<unsigned FixedBits,unsigned FracBits, bool IsSigned>
    struct ExpValuesTableSelector
    {
        typedef typename ExpTableValueSize<FixedBits, FracBits, IsSigned>::ExpTableType ExpTableType;
    };
}
