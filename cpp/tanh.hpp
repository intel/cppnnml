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

#include "tanhValues8Bit.hpp"
#include "tanhValues16Bit.hpp"
#include "tanhValues32Bit.hpp"
#include "tanhValues64Bit.hpp"

namespace tinymind {
    template<unsigned FixedBits, unsigned FracBits, bool IsSigned>
    struct TanhTableValueSize
    {
        static_assert(IsSigned, "Tanh activation tables require a signed type.");
    };

    #if (defined(TINYMIND_USE_TANH_1_7))
    template<>
    struct TanhTableValueSize<1, 7, true>
    {
        typedef TanhValuesTableQ1_7 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_1_7))

    #if (defined(TINYMIND_USE_TANH_2_6))
    template<>
    struct TanhTableValueSize<2, 6, true>
    {
        typedef TanhValuesTableQ2_6 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_2_6))

    #if (defined(TINYMIND_USE_TANH_3_5))
    template<>
    struct TanhTableValueSize<3, 5, true>
    {
        typedef TanhValuesTableQ3_5 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_3_5))

    #if (defined(TINYMIND_USE_TANH_4_4))
    template<>
    struct TanhTableValueSize<4, 4, true>
    {
        typedef TanhValuesTableQ4_4 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_4_4))

    #if (defined(TINYMIND_USE_TANH_5_3))
    template<>
    struct TanhTableValueSize<5, 3, true>
    {
        typedef TanhValuesTableQ5_3 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_5_3))

    #if (defined(TINYMIND_USE_TANH_6_2))
    template<>
    struct TanhTableValueSize<6, 2, true>
    {
        typedef TanhValuesTableQ6_2 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_6_2))

    #if (defined(TINYMIND_USE_TANH_7_1))
    template<>
    struct TanhTableValueSize<7, 1, true>
    {
        typedef TanhValuesTableQ7_1 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_7_1))

    #if (defined(TINYMIND_USE_TANH_1_15))
    template<>
    struct TanhTableValueSize<1, 15, true>
    {
        typedef TanhValuesTableQ1_15 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_1_15))

    #if (defined(TINYMIND_USE_TANH_2_14))
    template<>
    struct TanhTableValueSize<2, 14, true>
    {
        typedef TanhValuesTableQ2_14 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_2_14))

    #if (defined(TINYMIND_USE_TANH_3_13))
    template<>
    struct TanhTableValueSize<3, 13, true>
    {
        typedef TanhValuesTableQ3_13 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_3_13))

    #if (defined(TINYMIND_USE_TANH_4_12))
    template<>
    struct TanhTableValueSize<4, 12, true>
    {
        typedef TanhValuesTableQ4_12 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_4_12))

    #if (defined(TINYMIND_USE_TANH_5_11))
    template<>
    struct TanhTableValueSize<5, 11, true>
    {
        typedef TanhValuesTableQ5_11 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_5_11))

    #if (defined(TINYMIND_USE_TANH_6_10))
    template<>
    struct TanhTableValueSize<6, 10, true>
    {
        typedef TanhValuesTableQ6_10 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_6_10))

    #if (defined(TINYMIND_USE_TANH_7_9))
    template<>
    struct TanhTableValueSize<7, 9, true>
    {
        typedef TanhValuesTableQ7_9 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_7_9))

    #if (defined(TINYMIND_USE_TANH_8_8))
    template<>
    struct TanhTableValueSize<8, 8, true>
    {
        typedef TanhValuesTableQ8_8 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_8_8))

    #if (defined(TINYMIND_USE_TANH_9_7))
    template<>
    struct TanhTableValueSize<9, 7, true>
    {
        typedef TanhValuesTableQ9_7 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_9_7))

    #if (defined(TINYMIND_USE_TANH_10_6))
    template<>
    struct TanhTableValueSize<10, 6, true>
    {
        typedef TanhValuesTableQ10_6 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_10_6))

    #if (defined(TINYMIND_USE_TANH_11_5))
    template<>
    struct TanhTableValueSize<11, 5, true>
    {
        typedef TanhValuesTableQ11_5 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_11_5))

    #if (defined(TINYMIND_USE_TANH_12_4))
    template<>
    struct TanhTableValueSize<12, 4, true>
    {
        typedef TanhValuesTableQ12_4 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_12_4))

    #if (defined(TINYMIND_USE_TANH_13_3))
    template<>
    struct TanhTableValueSize<13, 3, true>
    {
        typedef TanhValuesTableQ13_3 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_13_3))

    #if (defined(TINYMIND_USE_TANH_14_2))
    template<>
    struct TanhTableValueSize<14, 2, true>
    {
        typedef TanhValuesTableQ14_2 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_14_2))

    #if (defined(TINYMIND_USE_TANH_15_1))
    template<>
    struct TanhTableValueSize<15, 1, true>
    {
        typedef TanhValuesTableQ15_1 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_15_1))

    #if (defined(TINYMIND_USE_TANH_1_31))
    template<>
    struct TanhTableValueSize<1, 31, true>
    {
        typedef TanhValuesTableQ1_31 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_1_31))

    #if (defined(TINYMIND_USE_TANH_2_30))
    template<>
    struct TanhTableValueSize<2, 30, true>
    {
        typedef TanhValuesTableQ2_30 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_2_30))

    #if (defined(TINYMIND_USE_TANH_3_29))
    template<>
    struct TanhTableValueSize<3, 29, true>
    {
        typedef TanhValuesTableQ3_29 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_3_29))

    #if (defined(TINYMIND_USE_TANH_4_28))
    template<>
    struct TanhTableValueSize<4, 28, true>
    {
        typedef TanhValuesTableQ4_28 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_4_28))

    #if (defined(TINYMIND_USE_TANH_5_27))
    template<>
    struct TanhTableValueSize<5, 27, true>
    {
        typedef TanhValuesTableQ5_27 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_5_27))

    #if (defined(TINYMIND_USE_TANH_6_26))
    template<>
    struct TanhTableValueSize<6, 26, true>
    {
        typedef TanhValuesTableQ6_26 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_6_26))

    #if (defined(TINYMIND_USE_TANH_7_25))
    template<>
    struct TanhTableValueSize<7, 25, true>
    {
        typedef TanhValuesTableQ7_25 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_7_25))

    #if (defined(TINYMIND_USE_TANH_8_24))
    template<>
    struct TanhTableValueSize<8, 24, true>
    {
        typedef TanhValuesTableQ8_24 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_8_24))

    #if (defined(TINYMIND_USE_TANH_9_23))
    template<>
    struct TanhTableValueSize<9, 23, true>
    {
        typedef TanhValuesTableQ9_23 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_9_23))

    #if (defined(TINYMIND_USE_TANH_10_22))
    template<>
    struct TanhTableValueSize<10, 22, true>
    {
        typedef TanhValuesTableQ10_22 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_10_22))

    #if (defined(TINYMIND_USE_TANH_11_21))
    template<>
    struct TanhTableValueSize<11, 21, true>
    {
        typedef TanhValuesTableQ11_21 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_11_21))

    #if (defined(TINYMIND_USE_TANH_12_20))
    template<>
    struct TanhTableValueSize<12, 20, true>
    {
        typedef TanhValuesTableQ12_20 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_12_20))

    #if (defined(TINYMIND_USE_TANH_13_19))
    template<>
    struct TanhTableValueSize<13, 19, true>
    {
        typedef TanhValuesTableQ13_19 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_13_19))

    #if (defined(TINYMIND_USE_TANH_14_18))
    template<>
    struct TanhTableValueSize<14, 18, true>
    {
        typedef TanhValuesTableQ14_18 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_14_18))

    #if (defined(TINYMIND_USE_TANH_15_17))
    template<>
    struct TanhTableValueSize<15, 17, true>
    {
        typedef TanhValuesTableQ15_17 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_15_17))

    #if (defined(TINYMIND_USE_TANH_16_16))
    template<>
    struct TanhTableValueSize<16, 16, true>
    {
        typedef TanhValuesTableQ16_16 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_16_16))

    #if (defined(TINYMIND_USE_TANH_17_15))
    template<>
    struct TanhTableValueSize<17, 15, true>
    {
        typedef TanhValuesTableQ17_15 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_17_15))

    #if (defined(TINYMIND_USE_TANH_18_14))
    template<>
    struct TanhTableValueSize<18, 14, true>
    {
        typedef TanhValuesTableQ18_14 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_18_14))

    #if (defined(TINYMIND_USE_TANH_19_13))
    template<>
    struct TanhTableValueSize<19, 13, true>
    {
        typedef TanhValuesTableQ19_13 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_19_13))

    #if (defined(TINYMIND_USE_TANH_20_12))
    template<>
    struct TanhTableValueSize<20, 12, true>
    {
        typedef TanhValuesTableQ20_12 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_20_12))

    #if (defined(TINYMIND_USE_TANH_21_11))
    template<>
    struct TanhTableValueSize<21, 11, true>
    {
        typedef TanhValuesTableQ21_11 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_21_11))

    #if (defined(TINYMIND_USE_TANH_22_10))
    template<>
    struct TanhTableValueSize<22, 10, true>
    {
        typedef TanhValuesTableQ22_10 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_22_10))

    #if (defined(TINYMIND_USE_TANH_23_9))
    template<>
    struct TanhTableValueSize<23, 9, true>
    {
        typedef TanhValuesTableQ23_9 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_23_9))

    #if (defined(TINYMIND_USE_TANH_24_8))
    template<>
    struct TanhTableValueSize<24, 8, true>
    {
        typedef TanhValuesTableQ24_8 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_24_8))

    #if (defined(TINYMIND_USE_TANH_25_7))
    template<>
    struct TanhTableValueSize<25, 7, true>
    {
        typedef TanhValuesTableQ25_7 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_25_7))

    #if (defined(TINYMIND_USE_TANH_26_6))
    template<>
    struct TanhTableValueSize<26, 6, true>
    {
        typedef TanhValuesTableQ26_6 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_26_6))

    #if (defined(TINYMIND_USE_TANH_27_5))
    template<>
    struct TanhTableValueSize<27, 5, true>
    {
        typedef TanhValuesTableQ27_5 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_27_5))

    #if (defined(TINYMIND_USE_TANH_28_4))
    template<>
    struct TanhTableValueSize<28, 4, true>
    {
        typedef TanhValuesTableQ28_4 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_28_4))

    #if (defined(TINYMIND_USE_TANH_29_3))
    template<>
    struct TanhTableValueSize<29, 3, true>
    {
        typedef TanhValuesTableQ29_3 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_29_3))

    #if (defined(TINYMIND_USE_TANH_30_2))
    template<>
    struct TanhTableValueSize<30, 2, true>
    {
        typedef TanhValuesTableQ30_2 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_30_2))

    #if (defined(TINYMIND_USE_TANH_31_1))
    template<>
    struct TanhTableValueSize<31, 1, true>
    {
        typedef TanhValuesTableQ31_1 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_31_1))

    #if (defined(TINYMIND_USE_TANH_1_63))
    template<>
    struct TanhTableValueSize<1, 63, true>
    {
        typedef TanhValuesTableQ1_63 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_1_63))

    #if (defined(TINYMIND_USE_TANH_2_62))
    template<>
    struct TanhTableValueSize<2, 62, true>
    {
        typedef TanhValuesTableQ2_62 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_2_62))

    #if (defined(TINYMIND_USE_TANH_3_61))
    template<>
    struct TanhTableValueSize<3, 61, true>
    {
        typedef TanhValuesTableQ3_61 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_3_61))

    #if (defined(TINYMIND_USE_TANH_4_60))
    template<>
    struct TanhTableValueSize<4, 60, true>
    {
        typedef TanhValuesTableQ4_60 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_4_60))

    #if (defined(TINYMIND_USE_TANH_5_59))
    template<>
    struct TanhTableValueSize<5, 59, true>
    {
        typedef TanhValuesTableQ5_59 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_5_59))

    #if (defined(TINYMIND_USE_TANH_6_58))
    template<>
    struct TanhTableValueSize<6, 58, true>
    {
        typedef TanhValuesTableQ6_58 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_6_58))

    #if (defined(TINYMIND_USE_TANH_7_57))
    template<>
    struct TanhTableValueSize<7, 57, true>
    {
        typedef TanhValuesTableQ7_57 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_7_57))

    #if (defined(TINYMIND_USE_TANH_8_56))
    template<>
    struct TanhTableValueSize<8, 56, true>
    {
        typedef TanhValuesTableQ8_56 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_8_56))

    #if (defined(TINYMIND_USE_TANH_9_55))
    template<>
    struct TanhTableValueSize<9, 55, true>
    {
        typedef TanhValuesTableQ9_55 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_9_55))

    #if (defined(TINYMIND_USE_TANH_10_54))
    template<>
    struct TanhTableValueSize<10, 54, true>
    {
        typedef TanhValuesTableQ10_54 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_10_54))

    #if (defined(TINYMIND_USE_TANH_11_53))
    template<>
    struct TanhTableValueSize<11, 53, true>
    {
        typedef TanhValuesTableQ11_53 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_11_53))

    #if (defined(TINYMIND_USE_TANH_12_52))
    template<>
    struct TanhTableValueSize<12, 52, true>
    {
        typedef TanhValuesTableQ12_52 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_12_52))

    #if (defined(TINYMIND_USE_TANH_13_51))
    template<>
    struct TanhTableValueSize<13, 51, true>
    {
        typedef TanhValuesTableQ13_51 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_13_51))

    #if (defined(TINYMIND_USE_TANH_14_50))
    template<>
    struct TanhTableValueSize<14, 50, true>
    {
        typedef TanhValuesTableQ14_50 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_14_50))

    #if (defined(TINYMIND_USE_TANH_15_49))
    template<>
    struct TanhTableValueSize<15, 49, true>
    {
        typedef TanhValuesTableQ15_49 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_15_49))

    #if (defined(TINYMIND_USE_TANH_16_48))
    template<>
    struct TanhTableValueSize<16, 48, true>
    {
        typedef TanhValuesTableQ16_48 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_16_48))

    #if (defined(TINYMIND_USE_TANH_17_47))
    template<>
    struct TanhTableValueSize<17, 47, true>
    {
        typedef TanhValuesTableQ17_47 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_17_47))

    #if (defined(TINYMIND_USE_TANH_18_46))
    template<>
    struct TanhTableValueSize<18, 46, true>
    {
        typedef TanhValuesTableQ18_46 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_18_46))

    #if (defined(TINYMIND_USE_TANH_19_45))
    template<>
    struct TanhTableValueSize<19, 45, true>
    {
        typedef TanhValuesTableQ19_45 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_19_45))

    #if (defined(TINYMIND_USE_TANH_20_44))
    template<>
    struct TanhTableValueSize<20, 44, true>
    {
        typedef TanhValuesTableQ20_44 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_20_44))

    #if (defined(TINYMIND_USE_TANH_21_43))
    template<>
    struct TanhTableValueSize<21, 43, true>
    {
        typedef TanhValuesTableQ21_43 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_21_43))

    #if (defined(TINYMIND_USE_TANH_22_42))
    template<>
    struct TanhTableValueSize<22, 42, true>
    {
        typedef TanhValuesTableQ22_42 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_22_42))

    #if (defined(TINYMIND_USE_TANH_23_41))
    template<>
    struct TanhTableValueSize<23, 41, true>
    {
        typedef TanhValuesTableQ23_41 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_23_41))

    #if (defined(TINYMIND_USE_TANH_24_40))
    template<>
    struct TanhTableValueSize<24, 40, true>
    {
        typedef TanhValuesTableQ24_40 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_24_40))

    #if (defined(TINYMIND_USE_TANH_25_39))
    template<>
    struct TanhTableValueSize<25, 39, true>
    {
        typedef TanhValuesTableQ25_39 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_25_39))

    #if (defined(TINYMIND_USE_TANH_26_38))
    template<>
    struct TanhTableValueSize<26, 38, true>
    {
        typedef TanhValuesTableQ26_38 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_26_38))

    #if (defined(TINYMIND_USE_TANH_27_37))
    template<>
    struct TanhTableValueSize<27, 37, true>
    {
        typedef TanhValuesTableQ27_37 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_27_37))

    #if (defined(TINYMIND_USE_TANH_28_36))
    template<>
    struct TanhTableValueSize<28, 36, true>
    {
        typedef TanhValuesTableQ28_36 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_28_36))

    #if (defined(TINYMIND_USE_TANH_29_35))
    template<>
    struct TanhTableValueSize<29, 35, true>
    {
        typedef TanhValuesTableQ29_35 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_29_35))

    #if (defined(TINYMIND_USE_TANH_30_34))
    template<>
    struct TanhTableValueSize<30, 34, true>
    {
        typedef TanhValuesTableQ30_34 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_30_34))

    #if (defined(TINYMIND_USE_TANH_31_33))
    template<>
    struct TanhTableValueSize<31, 33, true>
    {
        typedef TanhValuesTableQ31_33 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_31_33))

    #if (defined(TINYMIND_USE_TANH_32_32))
    template<>
    struct TanhTableValueSize<32, 32, true>
    {
        typedef TanhValuesTableQ32_32 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_32_32))

    #if (defined(TINYMIND_USE_TANH_33_31))
    template<>
    struct TanhTableValueSize<33, 31, true>
    {
        typedef TanhValuesTableQ33_31 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_33_31))

    #if (defined(TINYMIND_USE_TANH_34_30))
    template<>
    struct TanhTableValueSize<34, 30, true>
    {
        typedef TanhValuesTableQ34_30 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_34_30))

    #if (defined(TINYMIND_USE_TANH_35_29))
    template<>
    struct TanhTableValueSize<35, 29, true>
    {
        typedef TanhValuesTableQ35_29 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_35_29))

    #if (defined(TINYMIND_USE_TANH_36_28))
    template<>
    struct TanhTableValueSize<36, 28, true>
    {
        typedef TanhValuesTableQ36_28 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_36_28))

    #if (defined(TINYMIND_USE_TANH_37_27))
    template<>
    struct TanhTableValueSize<37, 27, true>
    {
        typedef TanhValuesTableQ37_27 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_37_27))

    #if (defined(TINYMIND_USE_TANH_38_26))
    template<>
    struct TanhTableValueSize<38, 26, true>
    {
        typedef TanhValuesTableQ38_26 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_38_26))

    #if (defined(TINYMIND_USE_TANH_39_25))
    template<>
    struct TanhTableValueSize<39, 25, true>
    {
        typedef TanhValuesTableQ39_25 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_39_25))

    #if (defined(TINYMIND_USE_TANH_40_24))
    template<>
    struct TanhTableValueSize<40, 24, true>
    {
        typedef TanhValuesTableQ40_24 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_40_24))

    #if (defined(TINYMIND_USE_TANH_41_23))
    template<>
    struct TanhTableValueSize<41, 23, true>
    {
        typedef TanhValuesTableQ41_23 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_41_23))

    #if (defined(TINYMIND_USE_TANH_42_22))
    template<>
    struct TanhTableValueSize<42, 22, true>
    {
        typedef TanhValuesTableQ42_22 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_42_22))

    #if (defined(TINYMIND_USE_TANH_43_21))
    template<>
    struct TanhTableValueSize<43, 21, true>
    {
        typedef TanhValuesTableQ43_21 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_43_21))

    #if (defined(TINYMIND_USE_TANH_44_20))
    template<>
    struct TanhTableValueSize<44, 20, true>
    {
        typedef TanhValuesTableQ44_20 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_44_20))

    #if (defined(TINYMIND_USE_TANH_45_19))
    template<>
    struct TanhTableValueSize<45, 19, true>
    {
        typedef TanhValuesTableQ45_19 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_45_19))

    #if (defined(TINYMIND_USE_TANH_46_18))
    template<>
    struct TanhTableValueSize<46, 18, true>
    {
        typedef TanhValuesTableQ46_18 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_46_18))

    #if (defined(TINYMIND_USE_TANH_47_17))
    template<>
    struct TanhTableValueSize<47, 17, true>
    {
        typedef TanhValuesTableQ47_17 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_47_17))

    #if (defined(TINYMIND_USE_TANH_48_16))
    template<>
    struct TanhTableValueSize<48, 16, true>
    {
        typedef TanhValuesTableQ48_16 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_48_16))

    #if (defined(TINYMIND_USE_TANH_49_15))
    template<>
    struct TanhTableValueSize<49, 15, true>
    {
        typedef TanhValuesTableQ49_15 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_49_15))

    #if (defined(TINYMIND_USE_TANH_50_14))
    template<>
    struct TanhTableValueSize<50, 14, true>
    {
        typedef TanhValuesTableQ50_14 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_50_14))

    #if (defined(TINYMIND_USE_TANH_51_13))
    template<>
    struct TanhTableValueSize<51, 13, true>
    {
        typedef TanhValuesTableQ51_13 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_51_13))

    #if (defined(TINYMIND_USE_TANH_52_12))
    template<>
    struct TanhTableValueSize<52, 12, true>
    {
        typedef TanhValuesTableQ52_12 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_52_12))

    #if (defined(TINYMIND_USE_TANH_53_11))
    template<>
    struct TanhTableValueSize<53, 11, true>
    {
        typedef TanhValuesTableQ53_11 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_53_11))

    #if (defined(TINYMIND_USE_TANH_54_10))
    template<>
    struct TanhTableValueSize<54, 10, true>
    {
        typedef TanhValuesTableQ54_10 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_54_10))

    #if (defined(TINYMIND_USE_TANH_55_9))
    template<>
    struct TanhTableValueSize<55, 9, true>
    {
        typedef TanhValuesTableQ55_9 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_55_9))

    #if (defined(TINYMIND_USE_TANH_56_8))
    template<>
    struct TanhTableValueSize<56, 8, true>
    {
        typedef TanhValuesTableQ56_8 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_56_8))

    #if (defined(TINYMIND_USE_TANH_57_7))
    template<>
    struct TanhTableValueSize<57, 7, true>
    {
        typedef TanhValuesTableQ57_7 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_57_7))

    #if (defined(TINYMIND_USE_TANH_58_6))
    template<>
    struct TanhTableValueSize<58, 6, true>
    {
        typedef TanhValuesTableQ58_6 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_58_6))

    #if (defined(TINYMIND_USE_TANH_59_5))
    template<>
    struct TanhTableValueSize<59, 5, true>
    {
        typedef TanhValuesTableQ59_5 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_59_5))

    #if (defined(TINYMIND_USE_TANH_60_4))
    template<>
    struct TanhTableValueSize<60, 4, true>
    {
        typedef TanhValuesTableQ60_4 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_60_4))

    #if (defined(TINYMIND_USE_TANH_61_3))
    template<>
    struct TanhTableValueSize<61, 3, true>
    {
        typedef TanhValuesTableQ61_3 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_61_3))

    #if (defined(TINYMIND_USE_TANH_62_2))
    template<>
    struct TanhTableValueSize<62, 2, true>
    {
        typedef TanhValuesTableQ62_2 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_62_2))

    #if (defined(TINYMIND_USE_TANH_63_1))
    template<>
    struct TanhTableValueSize<63, 1, true>
    {
        typedef TanhValuesTableQ63_1 TanhTableType;
    };
    #endif // (defined(TINYMIND_USE_TANH_63_1))

    template<unsigned FixedBits,unsigned FracBits, bool IsSigned>
    struct TanhValuesTableSelector
    {
        typedef typename TanhTableValueSize<FixedBits, FracBits, IsSigned>::TanhTableType TanhTableType;
    };
}
