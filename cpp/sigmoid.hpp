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

#include "sigmoidValues8Bit.hpp"
#include "sigmoidValues16Bit.hpp"
#include "sigmoidValues32Bit.hpp"
#include "sigmoidValues64Bit.hpp"

namespace tinymind {
    template<unsigned FixedBits, unsigned FracBits, bool IsSigned>
    struct SigmoidTableValueSize
    {
    };

    #if (defined(TINYMIND_USE_SIGMOID_1_7))
    template<>
    struct SigmoidTableValueSize<1, 7, true>
    {
        typedef SigmoidValuesTableQ1_7 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_7))

    #if (defined(TINYMIND_USE_SIGMOID_2_6))
    template<>
    struct SigmoidTableValueSize<2, 6, true>
    {
        typedef SigmoidValuesTableQ2_6 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_6))

    #if (defined(TINYMIND_USE_SIGMOID_3_5))
    template<>
    struct SigmoidTableValueSize<3, 5, true>
    {
        typedef SigmoidValuesTableQ3_5 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_5))

    #if (defined(TINYMIND_USE_SIGMOID_4_4))
    template<>
    struct SigmoidTableValueSize<4, 4, true>
    {
        typedef SigmoidValuesTableQ4_4 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_4))

    #if (defined(TINYMIND_USE_SIGMOID_5_3))
    template<>
    struct SigmoidTableValueSize<5, 3, true>
    {
        typedef SigmoidValuesTableQ5_3 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_3))

    #if (defined(TINYMIND_USE_SIGMOID_6_2))
    template<>
    struct SigmoidTableValueSize<6, 2, true>
    {
        typedef SigmoidValuesTableQ6_2 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_2))

    #if (defined(TINYMIND_USE_SIGMOID_7_1))
    template<>
    struct SigmoidTableValueSize<7, 1, true>
    {
        typedef SigmoidValuesTableQ7_1 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_1))

    #if (defined(TINYMIND_USE_SIGMOID_1_15))
    template<>
    struct SigmoidTableValueSize<1, 15, true>
    {
        typedef SigmoidValuesTableQ1_15 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_15))

    #if (defined(TINYMIND_USE_SIGMOID_2_14))
    template<>
    struct SigmoidTableValueSize<2, 14, true>
    {
        typedef SigmoidValuesTableQ2_14 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_14))

    #if (defined(TINYMIND_USE_SIGMOID_3_13))
    template<>
    struct SigmoidTableValueSize<3, 13, true>
    {
        typedef SigmoidValuesTableQ3_13 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_13))

    #if (defined(TINYMIND_USE_SIGMOID_4_12))
    template<>
    struct SigmoidTableValueSize<4, 12, true>
    {
        typedef SigmoidValuesTableQ4_12 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_12))

    #if (defined(TINYMIND_USE_SIGMOID_5_11))
    template<>
    struct SigmoidTableValueSize<5, 11, true>
    {
        typedef SigmoidValuesTableQ5_11 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_11))

    #if (defined(TINYMIND_USE_SIGMOID_6_10))
    template<>
    struct SigmoidTableValueSize<6, 10, true>
    {
        typedef SigmoidValuesTableQ6_10 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_10))

    #if (defined(TINYMIND_USE_SIGMOID_7_9))
    template<>
    struct SigmoidTableValueSize<7, 9, true>
    {
        typedef SigmoidValuesTableQ7_9 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_9))

    #if (defined(TINYMIND_USE_SIGMOID_8_8))
    template<>
    struct SigmoidTableValueSize<8, 8, true>
    {
        typedef SigmoidValuesTableQ8_8 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_8))

    #if (defined(TINYMIND_USE_SIGMOID_9_7))
    template<>
    struct SigmoidTableValueSize<9, 7, true>
    {
        typedef SigmoidValuesTableQ9_7 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_7))

    #if (defined(TINYMIND_USE_SIGMOID_10_6))
    template<>
    struct SigmoidTableValueSize<10, 6, true>
    {
        typedef SigmoidValuesTableQ10_6 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_6))

    #if (defined(TINYMIND_USE_SIGMOID_11_5))
    template<>
    struct SigmoidTableValueSize<11, 5, true>
    {
        typedef SigmoidValuesTableQ11_5 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_5))

    #if (defined(TINYMIND_USE_SIGMOID_12_4))
    template<>
    struct SigmoidTableValueSize<12, 4, true>
    {
        typedef SigmoidValuesTableQ12_4 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_4))

    #if (defined(TINYMIND_USE_SIGMOID_13_3))
    template<>
    struct SigmoidTableValueSize<13, 3, true>
    {
        typedef SigmoidValuesTableQ13_3 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_3))

    #if (defined(TINYMIND_USE_SIGMOID_14_2))
    template<>
    struct SigmoidTableValueSize<14, 2, true>
    {
        typedef SigmoidValuesTableQ14_2 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_2))

    #if (defined(TINYMIND_USE_SIGMOID_15_1))
    template<>
    struct SigmoidTableValueSize<15, 1, true>
    {
        typedef SigmoidValuesTableQ15_1 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_1))

    #if (defined(TINYMIND_USE_SIGMOID_1_31))
    template<>
    struct SigmoidTableValueSize<1, 31, true>
    {
        typedef SigmoidValuesTableQ1_31 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_31))

    #if (defined(TINYMIND_USE_SIGMOID_2_30))
    template<>
    struct SigmoidTableValueSize<2, 30, true>
    {
        typedef SigmoidValuesTableQ2_30 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_30))

    #if (defined(TINYMIND_USE_SIGMOID_3_29))
    template<>
    struct SigmoidTableValueSize<3, 29, true>
    {
        typedef SigmoidValuesTableQ3_29 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_29))

    #if (defined(TINYMIND_USE_SIGMOID_4_28))
    template<>
    struct SigmoidTableValueSize<4, 28, true>
    {
        typedef SigmoidValuesTableQ4_28 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_28))

    #if (defined(TINYMIND_USE_SIGMOID_5_27))
    template<>
    struct SigmoidTableValueSize<5, 27, true>
    {
        typedef SigmoidValuesTableQ5_27 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_27))

    #if (defined(TINYMIND_USE_SIGMOID_6_26))
    template<>
    struct SigmoidTableValueSize<6, 26, true>
    {
        typedef SigmoidValuesTableQ6_26 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_26))

    #if (defined(TINYMIND_USE_SIGMOID_7_25))
    template<>
    struct SigmoidTableValueSize<7, 25, true>
    {
        typedef SigmoidValuesTableQ7_25 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_25))

    #if (defined(TINYMIND_USE_SIGMOID_8_24))
    template<>
    struct SigmoidTableValueSize<8, 24, true>
    {
        typedef SigmoidValuesTableQ8_24 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_24))

    #if (defined(TINYMIND_USE_SIGMOID_9_23))
    template<>
    struct SigmoidTableValueSize<9, 23, true>
    {
        typedef SigmoidValuesTableQ9_23 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_23))

    #if (defined(TINYMIND_USE_SIGMOID_10_22))
    template<>
    struct SigmoidTableValueSize<10, 22, true>
    {
        typedef SigmoidValuesTableQ10_22 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_22))

    #if (defined(TINYMIND_USE_SIGMOID_11_21))
    template<>
    struct SigmoidTableValueSize<11, 21, true>
    {
        typedef SigmoidValuesTableQ11_21 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_21))

    #if (defined(TINYMIND_USE_SIGMOID_12_20))
    template<>
    struct SigmoidTableValueSize<12, 20, true>
    {
        typedef SigmoidValuesTableQ12_20 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_20))

    #if (defined(TINYMIND_USE_SIGMOID_13_19))
    template<>
    struct SigmoidTableValueSize<13, 19, true>
    {
        typedef SigmoidValuesTableQ13_19 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_19))

    #if (defined(TINYMIND_USE_SIGMOID_14_18))
    template<>
    struct SigmoidTableValueSize<14, 18, true>
    {
        typedef SigmoidValuesTableQ14_18 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_18))

    #if (defined(TINYMIND_USE_SIGMOID_15_17))
    template<>
    struct SigmoidTableValueSize<15, 17, true>
    {
        typedef SigmoidValuesTableQ15_17 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_17))

    #if (defined(TINYMIND_USE_SIGMOID_16_16))
    template<>
    struct SigmoidTableValueSize<16, 16, true>
    {
        typedef SigmoidValuesTableQ16_16 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_16_16))

    #if (defined(TINYMIND_USE_SIGMOID_17_15))
    template<>
    struct SigmoidTableValueSize<17, 15, true>
    {
        typedef SigmoidValuesTableQ17_15 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_17_15))

    #if (defined(TINYMIND_USE_SIGMOID_18_14))
    template<>
    struct SigmoidTableValueSize<18, 14, true>
    {
        typedef SigmoidValuesTableQ18_14 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_18_14))

    #if (defined(TINYMIND_USE_SIGMOID_19_13))
    template<>
    struct SigmoidTableValueSize<19, 13, true>
    {
        typedef SigmoidValuesTableQ19_13 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_19_13))

    #if (defined(TINYMIND_USE_SIGMOID_20_12))
    template<>
    struct SigmoidTableValueSize<20, 12, true>
    {
        typedef SigmoidValuesTableQ20_12 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_20_12))

    #if (defined(TINYMIND_USE_SIGMOID_21_11))
    template<>
    struct SigmoidTableValueSize<21, 11, true>
    {
        typedef SigmoidValuesTableQ21_11 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_21_11))

    #if (defined(TINYMIND_USE_SIGMOID_22_10))
    template<>
    struct SigmoidTableValueSize<22, 10, true>
    {
        typedef SigmoidValuesTableQ22_10 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_22_10))

    #if (defined(TINYMIND_USE_SIGMOID_23_9))
    template<>
    struct SigmoidTableValueSize<23, 9, true>
    {
        typedef SigmoidValuesTableQ23_9 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_23_9))

    #if (defined(TINYMIND_USE_SIGMOID_24_8))
    template<>
    struct SigmoidTableValueSize<24, 8, true>
    {
        typedef SigmoidValuesTableQ24_8 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_24_8))

    #if (defined(TINYMIND_USE_SIGMOID_25_7))
    template<>
    struct SigmoidTableValueSize<25, 7, true>
    {
        typedef SigmoidValuesTableQ25_7 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_25_7))

    #if (defined(TINYMIND_USE_SIGMOID_26_6))
    template<>
    struct SigmoidTableValueSize<26, 6, true>
    {
        typedef SigmoidValuesTableQ26_6 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_26_6))

    #if (defined(TINYMIND_USE_SIGMOID_27_5))
    template<>
    struct SigmoidTableValueSize<27, 5, true>
    {
        typedef SigmoidValuesTableQ27_5 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_27_5))

    #if (defined(TINYMIND_USE_SIGMOID_28_4))
    template<>
    struct SigmoidTableValueSize<28, 4, true>
    {
        typedef SigmoidValuesTableQ28_4 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_28_4))

    #if (defined(TINYMIND_USE_SIGMOID_29_3))
    template<>
    struct SigmoidTableValueSize<29, 3, true>
    {
        typedef SigmoidValuesTableQ29_3 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_29_3))

    #if (defined(TINYMIND_USE_SIGMOID_30_2))
    template<>
    struct SigmoidTableValueSize<30, 2, true>
    {
        typedef SigmoidValuesTableQ30_2 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_30_2))

    #if (defined(TINYMIND_USE_SIGMOID_31_1))
    template<>
    struct SigmoidTableValueSize<31, 1, true>
    {
        typedef SigmoidValuesTableQ31_1 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_31_1))

    #if (defined(TINYMIND_USE_SIGMOID_1_63))
    template<>
    struct SigmoidTableValueSize<1, 63, true>
    {
        typedef SigmoidValuesTableQ1_63 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_1_63))

    #if (defined(TINYMIND_USE_SIGMOID_2_62))
    template<>
    struct SigmoidTableValueSize<2, 62, true>
    {
        typedef SigmoidValuesTableQ2_62 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_2_62))

    #if (defined(TINYMIND_USE_SIGMOID_3_61))
    template<>
    struct SigmoidTableValueSize<3, 61, true>
    {
        typedef SigmoidValuesTableQ3_61 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_3_61))

    #if (defined(TINYMIND_USE_SIGMOID_4_60))
    template<>
    struct SigmoidTableValueSize<4, 60, true>
    {
        typedef SigmoidValuesTableQ4_60 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_4_60))

    #if (defined(TINYMIND_USE_SIGMOID_5_59))
    template<>
    struct SigmoidTableValueSize<5, 59, true>
    {
        typedef SigmoidValuesTableQ5_59 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_5_59))

    #if (defined(TINYMIND_USE_SIGMOID_6_58))
    template<>
    struct SigmoidTableValueSize<6, 58, true>
    {
        typedef SigmoidValuesTableQ6_58 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_6_58))

    #if (defined(TINYMIND_USE_SIGMOID_7_57))
    template<>
    struct SigmoidTableValueSize<7, 57, true>
    {
        typedef SigmoidValuesTableQ7_57 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_7_57))

    #if (defined(TINYMIND_USE_SIGMOID_8_56))
    template<>
    struct SigmoidTableValueSize<8, 56, true>
    {
        typedef SigmoidValuesTableQ8_56 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_8_56))

    #if (defined(TINYMIND_USE_SIGMOID_9_55))
    template<>
    struct SigmoidTableValueSize<9, 55, true>
    {
        typedef SigmoidValuesTableQ9_55 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_9_55))

    #if (defined(TINYMIND_USE_SIGMOID_10_54))
    template<>
    struct SigmoidTableValueSize<10, 54, true>
    {
        typedef SigmoidValuesTableQ10_54 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_10_54))

    #if (defined(TINYMIND_USE_SIGMOID_11_53))
    template<>
    struct SigmoidTableValueSize<11, 53, true>
    {
        typedef SigmoidValuesTableQ11_53 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_11_53))

    #if (defined(TINYMIND_USE_SIGMOID_12_52))
    template<>
    struct SigmoidTableValueSize<12, 52, true>
    {
        typedef SigmoidValuesTableQ12_52 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_12_52))

    #if (defined(TINYMIND_USE_SIGMOID_13_51))
    template<>
    struct SigmoidTableValueSize<13, 51, true>
    {
        typedef SigmoidValuesTableQ13_51 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_13_51))

    #if (defined(TINYMIND_USE_SIGMOID_14_50))
    template<>
    struct SigmoidTableValueSize<14, 50, true>
    {
        typedef SigmoidValuesTableQ14_50 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_14_50))

    #if (defined(TINYMIND_USE_SIGMOID_15_49))
    template<>
    struct SigmoidTableValueSize<15, 49, true>
    {
        typedef SigmoidValuesTableQ15_49 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_15_49))

    #if (defined(TINYMIND_USE_SIGMOID_16_48))
    template<>
    struct SigmoidTableValueSize<16, 48, true>
    {
        typedef SigmoidValuesTableQ16_48 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_16_48))

    #if (defined(TINYMIND_USE_SIGMOID_17_47))
    template<>
    struct SigmoidTableValueSize<17, 47, true>
    {
        typedef SigmoidValuesTableQ17_47 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_17_47))

    #if (defined(TINYMIND_USE_SIGMOID_18_46))
    template<>
    struct SigmoidTableValueSize<18, 46, true>
    {
        typedef SigmoidValuesTableQ18_46 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_18_46))

    #if (defined(TINYMIND_USE_SIGMOID_19_45))
    template<>
    struct SigmoidTableValueSize<19, 45, true>
    {
        typedef SigmoidValuesTableQ19_45 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_19_45))

    #if (defined(TINYMIND_USE_SIGMOID_20_44))
    template<>
    struct SigmoidTableValueSize<20, 44, true>
    {
        typedef SigmoidValuesTableQ20_44 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_20_44))

    #if (defined(TINYMIND_USE_SIGMOID_21_43))
    template<>
    struct SigmoidTableValueSize<21, 43, true>
    {
        typedef SigmoidValuesTableQ21_43 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_21_43))

    #if (defined(TINYMIND_USE_SIGMOID_22_42))
    template<>
    struct SigmoidTableValueSize<22, 42, true>
    {
        typedef SigmoidValuesTableQ22_42 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_22_42))

    #if (defined(TINYMIND_USE_SIGMOID_23_41))
    template<>
    struct SigmoidTableValueSize<23, 41, true>
    {
        typedef SigmoidValuesTableQ23_41 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_23_41))

    #if (defined(TINYMIND_USE_SIGMOID_24_40))
    template<>
    struct SigmoidTableValueSize<24, 40, true>
    {
        typedef SigmoidValuesTableQ24_40 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_24_40))

    #if (defined(TINYMIND_USE_SIGMOID_25_39))
    template<>
    struct SigmoidTableValueSize<25, 39, true>
    {
        typedef SigmoidValuesTableQ25_39 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_25_39))

    #if (defined(TINYMIND_USE_SIGMOID_26_38))
    template<>
    struct SigmoidTableValueSize<26, 38, true>
    {
        typedef SigmoidValuesTableQ26_38 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_26_38))

    #if (defined(TINYMIND_USE_SIGMOID_27_37))
    template<>
    struct SigmoidTableValueSize<27, 37, true>
    {
        typedef SigmoidValuesTableQ27_37 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_27_37))

    #if (defined(TINYMIND_USE_SIGMOID_28_36))
    template<>
    struct SigmoidTableValueSize<28, 36, true>
    {
        typedef SigmoidValuesTableQ28_36 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_28_36))

    #if (defined(TINYMIND_USE_SIGMOID_29_35))
    template<>
    struct SigmoidTableValueSize<29, 35, true>
    {
        typedef SigmoidValuesTableQ29_35 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_29_35))

    #if (defined(TINYMIND_USE_SIGMOID_30_34))
    template<>
    struct SigmoidTableValueSize<30, 34, true>
    {
        typedef SigmoidValuesTableQ30_34 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_30_34))

    #if (defined(TINYMIND_USE_SIGMOID_31_33))
    template<>
    struct SigmoidTableValueSize<31, 33, true>
    {
        typedef SigmoidValuesTableQ31_33 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_31_33))

    #if (defined(TINYMIND_USE_SIGMOID_32_32))
    template<>
    struct SigmoidTableValueSize<32, 32, true>
    {
        typedef SigmoidValuesTableQ32_32 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_32_32))

    #if (defined(TINYMIND_USE_SIGMOID_33_31))
    template<>
    struct SigmoidTableValueSize<33, 31, true>
    {
        typedef SigmoidValuesTableQ33_31 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_33_31))

    #if (defined(TINYMIND_USE_SIGMOID_34_30))
    template<>
    struct SigmoidTableValueSize<34, 30, true>
    {
        typedef SigmoidValuesTableQ34_30 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_34_30))

    #if (defined(TINYMIND_USE_SIGMOID_35_29))
    template<>
    struct SigmoidTableValueSize<35, 29, true>
    {
        typedef SigmoidValuesTableQ35_29 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_35_29))

    #if (defined(TINYMIND_USE_SIGMOID_36_28))
    template<>
    struct SigmoidTableValueSize<36, 28, true>
    {
        typedef SigmoidValuesTableQ36_28 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_36_28))

    #if (defined(TINYMIND_USE_SIGMOID_37_27))
    template<>
    struct SigmoidTableValueSize<37, 27, true>
    {
        typedef SigmoidValuesTableQ37_27 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_37_27))

    #if (defined(TINYMIND_USE_SIGMOID_38_26))
    template<>
    struct SigmoidTableValueSize<38, 26, true>
    {
        typedef SigmoidValuesTableQ38_26 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_38_26))

    #if (defined(TINYMIND_USE_SIGMOID_39_25))
    template<>
    struct SigmoidTableValueSize<39, 25, true>
    {
        typedef SigmoidValuesTableQ39_25 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_39_25))

    #if (defined(TINYMIND_USE_SIGMOID_40_24))
    template<>
    struct SigmoidTableValueSize<40, 24, true>
    {
        typedef SigmoidValuesTableQ40_24 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_40_24))

    #if (defined(TINYMIND_USE_SIGMOID_41_23))
    template<>
    struct SigmoidTableValueSize<41, 23, true>
    {
        typedef SigmoidValuesTableQ41_23 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_41_23))

    #if (defined(TINYMIND_USE_SIGMOID_42_22))
    template<>
    struct SigmoidTableValueSize<42, 22, true>
    {
        typedef SigmoidValuesTableQ42_22 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_42_22))

    #if (defined(TINYMIND_USE_SIGMOID_43_21))
    template<>
    struct SigmoidTableValueSize<43, 21, true>
    {
        typedef SigmoidValuesTableQ43_21 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_43_21))

    #if (defined(TINYMIND_USE_SIGMOID_44_20))
    template<>
    struct SigmoidTableValueSize<44, 20, true>
    {
        typedef SigmoidValuesTableQ44_20 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_44_20))

    #if (defined(TINYMIND_USE_SIGMOID_45_19))
    template<>
    struct SigmoidTableValueSize<45, 19, true>
    {
        typedef SigmoidValuesTableQ45_19 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_45_19))

    #if (defined(TINYMIND_USE_SIGMOID_46_18))
    template<>
    struct SigmoidTableValueSize<46, 18, true>
    {
        typedef SigmoidValuesTableQ46_18 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_46_18))

    #if (defined(TINYMIND_USE_SIGMOID_47_17))
    template<>
    struct SigmoidTableValueSize<47, 17, true>
    {
        typedef SigmoidValuesTableQ47_17 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_47_17))

    #if (defined(TINYMIND_USE_SIGMOID_48_16))
    template<>
    struct SigmoidTableValueSize<48, 16, true>
    {
        typedef SigmoidValuesTableQ48_16 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_48_16))

    #if (defined(TINYMIND_USE_SIGMOID_49_15))
    template<>
    struct SigmoidTableValueSize<49, 15, true>
    {
        typedef SigmoidValuesTableQ49_15 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_49_15))

    #if (defined(TINYMIND_USE_SIGMOID_50_14))
    template<>
    struct SigmoidTableValueSize<50, 14, true>
    {
        typedef SigmoidValuesTableQ50_14 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_50_14))

    #if (defined(TINYMIND_USE_SIGMOID_51_13))
    template<>
    struct SigmoidTableValueSize<51, 13, true>
    {
        typedef SigmoidValuesTableQ51_13 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_51_13))

    #if (defined(TINYMIND_USE_SIGMOID_52_12))
    template<>
    struct SigmoidTableValueSize<52, 12, true>
    {
        typedef SigmoidValuesTableQ52_12 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_52_12))

    #if (defined(TINYMIND_USE_SIGMOID_53_11))
    template<>
    struct SigmoidTableValueSize<53, 11, true>
    {
        typedef SigmoidValuesTableQ53_11 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_53_11))

    #if (defined(TINYMIND_USE_SIGMOID_54_10))
    template<>
    struct SigmoidTableValueSize<54, 10, true>
    {
        typedef SigmoidValuesTableQ54_10 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_54_10))

    #if (defined(TINYMIND_USE_SIGMOID_55_9))
    template<>
    struct SigmoidTableValueSize<55, 9, true>
    {
        typedef SigmoidValuesTableQ55_9 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_55_9))

    #if (defined(TINYMIND_USE_SIGMOID_56_8))
    template<>
    struct SigmoidTableValueSize<56, 8, true>
    {
        typedef SigmoidValuesTableQ56_8 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_56_8))

    #if (defined(TINYMIND_USE_SIGMOID_57_7))
    template<>
    struct SigmoidTableValueSize<57, 7, true>
    {
        typedef SigmoidValuesTableQ57_7 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_57_7))

    #if (defined(TINYMIND_USE_SIGMOID_58_6))
    template<>
    struct SigmoidTableValueSize<58, 6, true>
    {
        typedef SigmoidValuesTableQ58_6 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_58_6))

    #if (defined(TINYMIND_USE_SIGMOID_59_5))
    template<>
    struct SigmoidTableValueSize<59, 5, true>
    {
        typedef SigmoidValuesTableQ59_5 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_59_5))

    #if (defined(TINYMIND_USE_SIGMOID_60_4))
    template<>
    struct SigmoidTableValueSize<60, 4, true>
    {
        typedef SigmoidValuesTableQ60_4 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_60_4))

    #if (defined(TINYMIND_USE_SIGMOID_61_3))
    template<>
    struct SigmoidTableValueSize<61, 3, true>
    {
        typedef SigmoidValuesTableQ61_3 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_61_3))

    #if (defined(TINYMIND_USE_SIGMOID_62_2))
    template<>
    struct SigmoidTableValueSize<62, 2, true>
    {
        typedef SigmoidValuesTableQ62_2 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_62_2))

    #if (defined(TINYMIND_USE_SIGMOID_63_1))
    template<>
    struct SigmoidTableValueSize<63, 1, true>
    {
        typedef SigmoidValuesTableQ63_1 SigmoidTableType;
    };
    #endif // (defined(TINYMIND_USE_SIGMOID_63_1))

    template<unsigned FixedBits,unsigned FracBits, bool IsSigned>
    struct SigmoidValuesTableSelector
    {
        typedef typename SigmoidTableValueSize<FixedBits, FracBits, IsSigned>::SigmoidTableType SigmoidTableType;
    };
}
