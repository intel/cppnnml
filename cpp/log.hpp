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

#include "logValues8Bit.hpp"
#include "logValues16Bit.hpp"
#include "logValues32Bit.hpp"
#include "logValues64Bit.hpp"

namespace tinymind {
    template<unsigned FixedBits, unsigned FracBits, bool IsSigned>
    struct LogTableValueSize
    {
    };

    #if (defined(TINYMIND_USE_LOG_1_7))
    template<>
    struct LogTableValueSize<1, 7, true>
    {
        typedef LogValuesTableQ1_7 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_1_7))

    #if (defined(TINYMIND_USE_LOG_2_6))
    template<>
    struct LogTableValueSize<2, 6, true>
    {
        typedef LogValuesTableQ2_6 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_2_6))

    #if (defined(TINYMIND_USE_LOG_3_5))
    template<>
    struct LogTableValueSize<3, 5, true>
    {
        typedef LogValuesTableQ3_5 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_3_5))

    #if (defined(TINYMIND_USE_LOG_4_4))
    template<>
    struct LogTableValueSize<4, 4, true>
    {
        typedef LogValuesTableQ4_4 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_4_4))

    #if (defined(TINYMIND_USE_LOG_5_3))
    template<>
    struct LogTableValueSize<5, 3, true>
    {
        typedef LogValuesTableQ5_3 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_5_3))

    #if (defined(TINYMIND_USE_LOG_6_2))
    template<>
    struct LogTableValueSize<6, 2, true>
    {
        typedef LogValuesTableQ6_2 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_6_2))

    #if (defined(TINYMIND_USE_LOG_7_1))
    template<>
    struct LogTableValueSize<7, 1, true>
    {
        typedef LogValuesTableQ7_1 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_7_1))

    #if (defined(TINYMIND_USE_LOG_1_15))
    template<>
    struct LogTableValueSize<1, 15, true>
    {
        typedef LogValuesTableQ1_15 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_1_15))

    #if (defined(TINYMIND_USE_LOG_2_14))
    template<>
    struct LogTableValueSize<2, 14, true>
    {
        typedef LogValuesTableQ2_14 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_2_14))

    #if (defined(TINYMIND_USE_LOG_3_13))
    template<>
    struct LogTableValueSize<3, 13, true>
    {
        typedef LogValuesTableQ3_13 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_3_13))

    #if (defined(TINYMIND_USE_LOG_4_12))
    template<>
    struct LogTableValueSize<4, 12, true>
    {
        typedef LogValuesTableQ4_12 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_4_12))

    #if (defined(TINYMIND_USE_LOG_5_11))
    template<>
    struct LogTableValueSize<5, 11, true>
    {
        typedef LogValuesTableQ5_11 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_5_11))

    #if (defined(TINYMIND_USE_LOG_6_10))
    template<>
    struct LogTableValueSize<6, 10, true>
    {
        typedef LogValuesTableQ6_10 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_6_10))

    #if (defined(TINYMIND_USE_LOG_7_9))
    template<>
    struct LogTableValueSize<7, 9, true>
    {
        typedef LogValuesTableQ7_9 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_7_9))

    #if (defined(TINYMIND_USE_LOG_8_8))
    template<>
    struct LogTableValueSize<8, 8, true>
    {
        typedef LogValuesTableQ8_8 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_8_8))

    #if (defined(TINYMIND_USE_LOG_9_7))
    template<>
    struct LogTableValueSize<9, 7, true>
    {
        typedef LogValuesTableQ9_7 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_9_7))

    #if (defined(TINYMIND_USE_LOG_10_6))
    template<>
    struct LogTableValueSize<10, 6, true>
    {
        typedef LogValuesTableQ10_6 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_10_6))

    #if (defined(TINYMIND_USE_LOG_11_5))
    template<>
    struct LogTableValueSize<11, 5, true>
    {
        typedef LogValuesTableQ11_5 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_11_5))

    #if (defined(TINYMIND_USE_LOG_12_4))
    template<>
    struct LogTableValueSize<12, 4, true>
    {
        typedef LogValuesTableQ12_4 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_12_4))

    #if (defined(TINYMIND_USE_LOG_13_3))
    template<>
    struct LogTableValueSize<13, 3, true>
    {
        typedef LogValuesTableQ13_3 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_13_3))

    #if (defined(TINYMIND_USE_LOG_14_2))
    template<>
    struct LogTableValueSize<14, 2, true>
    {
        typedef LogValuesTableQ14_2 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_14_2))

    #if (defined(TINYMIND_USE_LOG_15_1))
    template<>
    struct LogTableValueSize<15, 1, true>
    {
        typedef LogValuesTableQ15_1 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_15_1))

    #if (defined(TINYMIND_USE_LOG_1_31))
    template<>
    struct LogTableValueSize<1, 31, true>
    {
        typedef LogValuesTableQ1_31 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_1_31))

    #if (defined(TINYMIND_USE_LOG_2_30))
    template<>
    struct LogTableValueSize<2, 30, true>
    {
        typedef LogValuesTableQ2_30 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_2_30))

    #if (defined(TINYMIND_USE_LOG_3_29))
    template<>
    struct LogTableValueSize<3, 29, true>
    {
        typedef LogValuesTableQ3_29 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_3_29))

    #if (defined(TINYMIND_USE_LOG_4_28))
    template<>
    struct LogTableValueSize<4, 28, true>
    {
        typedef LogValuesTableQ4_28 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_4_28))

    #if (defined(TINYMIND_USE_LOG_5_27))
    template<>
    struct LogTableValueSize<5, 27, true>
    {
        typedef LogValuesTableQ5_27 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_5_27))

    #if (defined(TINYMIND_USE_LOG_6_26))
    template<>
    struct LogTableValueSize<6, 26, true>
    {
        typedef LogValuesTableQ6_26 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_6_26))

    #if (defined(TINYMIND_USE_LOG_7_25))
    template<>
    struct LogTableValueSize<7, 25, true>
    {
        typedef LogValuesTableQ7_25 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_7_25))

    #if (defined(TINYMIND_USE_LOG_8_24))
    template<>
    struct LogTableValueSize<8, 24, true>
    {
        typedef LogValuesTableQ8_24 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_8_24))

    #if (defined(TINYMIND_USE_LOG_9_23))
    template<>
    struct LogTableValueSize<9, 23, true>
    {
        typedef LogValuesTableQ9_23 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_9_23))

    #if (defined(TINYMIND_USE_LOG_10_22))
    template<>
    struct LogTableValueSize<10, 22, true>
    {
        typedef LogValuesTableQ10_22 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_10_22))

    #if (defined(TINYMIND_USE_LOG_11_21))
    template<>
    struct LogTableValueSize<11, 21, true>
    {
        typedef LogValuesTableQ11_21 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_11_21))

    #if (defined(TINYMIND_USE_LOG_12_20))
    template<>
    struct LogTableValueSize<12, 20, true>
    {
        typedef LogValuesTableQ12_20 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_12_20))

    #if (defined(TINYMIND_USE_LOG_13_19))
    template<>
    struct LogTableValueSize<13, 19, true>
    {
        typedef LogValuesTableQ13_19 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_13_19))

    #if (defined(TINYMIND_USE_LOG_14_18))
    template<>
    struct LogTableValueSize<14, 18, true>
    {
        typedef LogValuesTableQ14_18 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_14_18))

    #if (defined(TINYMIND_USE_LOG_15_17))
    template<>
    struct LogTableValueSize<15, 17, true>
    {
        typedef LogValuesTableQ15_17 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_15_17))

    #if (defined(TINYMIND_USE_LOG_16_16))
    template<>
    struct LogTableValueSize<16, 16, true>
    {
        typedef LogValuesTableQ16_16 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_16_16))

    #if (defined(TINYMIND_USE_LOG_17_15))
    template<>
    struct LogTableValueSize<17, 15, true>
    {
        typedef LogValuesTableQ17_15 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_17_15))

    #if (defined(TINYMIND_USE_LOG_18_14))
    template<>
    struct LogTableValueSize<18, 14, true>
    {
        typedef LogValuesTableQ18_14 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_18_14))

    #if (defined(TINYMIND_USE_LOG_19_13))
    template<>
    struct LogTableValueSize<19, 13, true>
    {
        typedef LogValuesTableQ19_13 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_19_13))

    #if (defined(TINYMIND_USE_LOG_20_12))
    template<>
    struct LogTableValueSize<20, 12, true>
    {
        typedef LogValuesTableQ20_12 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_20_12))

    #if (defined(TINYMIND_USE_LOG_21_11))
    template<>
    struct LogTableValueSize<21, 11, true>
    {
        typedef LogValuesTableQ21_11 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_21_11))

    #if (defined(TINYMIND_USE_LOG_22_10))
    template<>
    struct LogTableValueSize<22, 10, true>
    {
        typedef LogValuesTableQ22_10 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_22_10))

    #if (defined(TINYMIND_USE_LOG_23_9))
    template<>
    struct LogTableValueSize<23, 9, true>
    {
        typedef LogValuesTableQ23_9 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_23_9))

    #if (defined(TINYMIND_USE_LOG_24_8))
    template<>
    struct LogTableValueSize<24, 8, true>
    {
        typedef LogValuesTableQ24_8 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_24_8))

    #if (defined(TINYMIND_USE_LOG_25_7))
    template<>
    struct LogTableValueSize<25, 7, true>
    {
        typedef LogValuesTableQ25_7 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_25_7))

    #if (defined(TINYMIND_USE_LOG_26_6))
    template<>
    struct LogTableValueSize<26, 6, true>
    {
        typedef LogValuesTableQ26_6 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_26_6))

    #if (defined(TINYMIND_USE_LOG_27_5))
    template<>
    struct LogTableValueSize<27, 5, true>
    {
        typedef LogValuesTableQ27_5 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_27_5))

    #if (defined(TINYMIND_USE_LOG_28_4))
    template<>
    struct LogTableValueSize<28, 4, true>
    {
        typedef LogValuesTableQ28_4 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_28_4))

    #if (defined(TINYMIND_USE_LOG_29_3))
    template<>
    struct LogTableValueSize<29, 3, true>
    {
        typedef LogValuesTableQ29_3 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_29_3))

    #if (defined(TINYMIND_USE_LOG_30_2))
    template<>
    struct LogTableValueSize<30, 2, true>
    {
        typedef LogValuesTableQ30_2 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_30_2))

    #if (defined(TINYMIND_USE_LOG_31_1))
    template<>
    struct LogTableValueSize<31, 1, true>
    {
        typedef LogValuesTableQ31_1 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_31_1))

    #if (defined(TINYMIND_USE_LOG_1_63))
    template<>
    struct LogTableValueSize<1, 63, true>
    {
        typedef LogValuesTableQ1_63 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_1_63))

    #if (defined(TINYMIND_USE_LOG_2_62))
    template<>
    struct LogTableValueSize<2, 62, true>
    {
        typedef LogValuesTableQ2_62 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_2_62))

    #if (defined(TINYMIND_USE_LOG_3_61))
    template<>
    struct LogTableValueSize<3, 61, true>
    {
        typedef LogValuesTableQ3_61 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_3_61))

    #if (defined(TINYMIND_USE_LOG_4_60))
    template<>
    struct LogTableValueSize<4, 60, true>
    {
        typedef LogValuesTableQ4_60 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_4_60))

    #if (defined(TINYMIND_USE_LOG_5_59))
    template<>
    struct LogTableValueSize<5, 59, true>
    {
        typedef LogValuesTableQ5_59 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_5_59))

    #if (defined(TINYMIND_USE_LOG_6_58))
    template<>
    struct LogTableValueSize<6, 58, true>
    {
        typedef LogValuesTableQ6_58 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_6_58))

    #if (defined(TINYMIND_USE_LOG_7_57))
    template<>
    struct LogTableValueSize<7, 57, true>
    {
        typedef LogValuesTableQ7_57 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_7_57))

    #if (defined(TINYMIND_USE_LOG_8_56))
    template<>
    struct LogTableValueSize<8, 56, true>
    {
        typedef LogValuesTableQ8_56 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_8_56))

    #if (defined(TINYMIND_USE_LOG_9_55))
    template<>
    struct LogTableValueSize<9, 55, true>
    {
        typedef LogValuesTableQ9_55 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_9_55))

    #if (defined(TINYMIND_USE_LOG_10_54))
    template<>
    struct LogTableValueSize<10, 54, true>
    {
        typedef LogValuesTableQ10_54 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_10_54))

    #if (defined(TINYMIND_USE_LOG_11_53))
    template<>
    struct LogTableValueSize<11, 53, true>
    {
        typedef LogValuesTableQ11_53 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_11_53))

    #if (defined(TINYMIND_USE_LOG_12_52))
    template<>
    struct LogTableValueSize<12, 52, true>
    {
        typedef LogValuesTableQ12_52 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_12_52))

    #if (defined(TINYMIND_USE_LOG_13_51))
    template<>
    struct LogTableValueSize<13, 51, true>
    {
        typedef LogValuesTableQ13_51 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_13_51))

    #if (defined(TINYMIND_USE_LOG_14_50))
    template<>
    struct LogTableValueSize<14, 50, true>
    {
        typedef LogValuesTableQ14_50 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_14_50))

    #if (defined(TINYMIND_USE_LOG_15_49))
    template<>
    struct LogTableValueSize<15, 49, true>
    {
        typedef LogValuesTableQ15_49 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_15_49))

    #if (defined(TINYMIND_USE_LOG_16_48))
    template<>
    struct LogTableValueSize<16, 48, true>
    {
        typedef LogValuesTableQ16_48 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_16_48))

    #if (defined(TINYMIND_USE_LOG_17_47))
    template<>
    struct LogTableValueSize<17, 47, true>
    {
        typedef LogValuesTableQ17_47 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_17_47))

    #if (defined(TINYMIND_USE_LOG_18_46))
    template<>
    struct LogTableValueSize<18, 46, true>
    {
        typedef LogValuesTableQ18_46 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_18_46))

    #if (defined(TINYMIND_USE_LOG_19_45))
    template<>
    struct LogTableValueSize<19, 45, true>
    {
        typedef LogValuesTableQ19_45 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_19_45))

    #if (defined(TINYMIND_USE_LOG_20_44))
    template<>
    struct LogTableValueSize<20, 44, true>
    {
        typedef LogValuesTableQ20_44 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_20_44))

    #if (defined(TINYMIND_USE_LOG_21_43))
    template<>
    struct LogTableValueSize<21, 43, true>
    {
        typedef LogValuesTableQ21_43 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_21_43))

    #if (defined(TINYMIND_USE_LOG_22_42))
    template<>
    struct LogTableValueSize<22, 42, true>
    {
        typedef LogValuesTableQ22_42 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_22_42))

    #if (defined(TINYMIND_USE_LOG_23_41))
    template<>
    struct LogTableValueSize<23, 41, true>
    {
        typedef LogValuesTableQ23_41 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_23_41))

    #if (defined(TINYMIND_USE_LOG_24_40))
    template<>
    struct LogTableValueSize<24, 40, true>
    {
        typedef LogValuesTableQ24_40 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_24_40))

    #if (defined(TINYMIND_USE_LOG_25_39))
    template<>
    struct LogTableValueSize<25, 39, true>
    {
        typedef LogValuesTableQ25_39 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_25_39))

    #if (defined(TINYMIND_USE_LOG_26_38))
    template<>
    struct LogTableValueSize<26, 38, true>
    {
        typedef LogValuesTableQ26_38 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_26_38))

    #if (defined(TINYMIND_USE_LOG_27_37))
    template<>
    struct LogTableValueSize<27, 37, true>
    {
        typedef LogValuesTableQ27_37 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_27_37))

    #if (defined(TINYMIND_USE_LOG_28_36))
    template<>
    struct LogTableValueSize<28, 36, true>
    {
        typedef LogValuesTableQ28_36 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_28_36))

    #if (defined(TINYMIND_USE_LOG_29_35))
    template<>
    struct LogTableValueSize<29, 35, true>
    {
        typedef LogValuesTableQ29_35 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_29_35))

    #if (defined(TINYMIND_USE_LOG_30_34))
    template<>
    struct LogTableValueSize<30, 34, true>
    {
        typedef LogValuesTableQ30_34 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_30_34))

    #if (defined(TINYMIND_USE_LOG_31_33))
    template<>
    struct LogTableValueSize<31, 33, true>
    {
        typedef LogValuesTableQ31_33 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_31_33))

    #if (defined(TINYMIND_USE_LOG_32_32))
    template<>
    struct LogTableValueSize<32, 32, true>
    {
        typedef LogValuesTableQ32_32 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_32_32))

    #if (defined(TINYMIND_USE_LOG_33_31))
    template<>
    struct LogTableValueSize<33, 31, true>
    {
        typedef LogValuesTableQ33_31 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_33_31))

    #if (defined(TINYMIND_USE_LOG_34_30))
    template<>
    struct LogTableValueSize<34, 30, true>
    {
        typedef LogValuesTableQ34_30 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_34_30))

    #if (defined(TINYMIND_USE_LOG_35_29))
    template<>
    struct LogTableValueSize<35, 29, true>
    {
        typedef LogValuesTableQ35_29 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_35_29))

    #if (defined(TINYMIND_USE_LOG_36_28))
    template<>
    struct LogTableValueSize<36, 28, true>
    {
        typedef LogValuesTableQ36_28 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_36_28))

    #if (defined(TINYMIND_USE_LOG_37_27))
    template<>
    struct LogTableValueSize<37, 27, true>
    {
        typedef LogValuesTableQ37_27 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_37_27))

    #if (defined(TINYMIND_USE_LOG_38_26))
    template<>
    struct LogTableValueSize<38, 26, true>
    {
        typedef LogValuesTableQ38_26 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_38_26))

    #if (defined(TINYMIND_USE_LOG_39_25))
    template<>
    struct LogTableValueSize<39, 25, true>
    {
        typedef LogValuesTableQ39_25 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_39_25))

    #if (defined(TINYMIND_USE_LOG_40_24))
    template<>
    struct LogTableValueSize<40, 24, true>
    {
        typedef LogValuesTableQ40_24 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_40_24))

    #if (defined(TINYMIND_USE_LOG_41_23))
    template<>
    struct LogTableValueSize<41, 23, true>
    {
        typedef LogValuesTableQ41_23 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_41_23))

    #if (defined(TINYMIND_USE_LOG_42_22))
    template<>
    struct LogTableValueSize<42, 22, true>
    {
        typedef LogValuesTableQ42_22 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_42_22))

    #if (defined(TINYMIND_USE_LOG_43_21))
    template<>
    struct LogTableValueSize<43, 21, true>
    {
        typedef LogValuesTableQ43_21 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_43_21))

    #if (defined(TINYMIND_USE_LOG_44_20))
    template<>
    struct LogTableValueSize<44, 20, true>
    {
        typedef LogValuesTableQ44_20 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_44_20))

    #if (defined(TINYMIND_USE_LOG_45_19))
    template<>
    struct LogTableValueSize<45, 19, true>
    {
        typedef LogValuesTableQ45_19 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_45_19))

    #if (defined(TINYMIND_USE_LOG_46_18))
    template<>
    struct LogTableValueSize<46, 18, true>
    {
        typedef LogValuesTableQ46_18 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_46_18))

    #if (defined(TINYMIND_USE_LOG_47_17))
    template<>
    struct LogTableValueSize<47, 17, true>
    {
        typedef LogValuesTableQ47_17 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_47_17))

    #if (defined(TINYMIND_USE_LOG_48_16))
    template<>
    struct LogTableValueSize<48, 16, true>
    {
        typedef LogValuesTableQ48_16 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_48_16))

    #if (defined(TINYMIND_USE_LOG_49_15))
    template<>
    struct LogTableValueSize<49, 15, true>
    {
        typedef LogValuesTableQ49_15 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_49_15))

    #if (defined(TINYMIND_USE_LOG_50_14))
    template<>
    struct LogTableValueSize<50, 14, true>
    {
        typedef LogValuesTableQ50_14 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_50_14))

    #if (defined(TINYMIND_USE_LOG_51_13))
    template<>
    struct LogTableValueSize<51, 13, true>
    {
        typedef LogValuesTableQ51_13 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_51_13))

    #if (defined(TINYMIND_USE_LOG_52_12))
    template<>
    struct LogTableValueSize<52, 12, true>
    {
        typedef LogValuesTableQ52_12 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_52_12))

    #if (defined(TINYMIND_USE_LOG_53_11))
    template<>
    struct LogTableValueSize<53, 11, true>
    {
        typedef LogValuesTableQ53_11 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_53_11))

    #if (defined(TINYMIND_USE_LOG_54_10))
    template<>
    struct LogTableValueSize<54, 10, true>
    {
        typedef LogValuesTableQ54_10 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_54_10))

    #if (defined(TINYMIND_USE_LOG_55_9))
    template<>
    struct LogTableValueSize<55, 9, true>
    {
        typedef LogValuesTableQ55_9 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_55_9))

    #if (defined(TINYMIND_USE_LOG_56_8))
    template<>
    struct LogTableValueSize<56, 8, true>
    {
        typedef LogValuesTableQ56_8 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_56_8))

    #if (defined(TINYMIND_USE_LOG_57_7))
    template<>
    struct LogTableValueSize<57, 7, true>
    {
        typedef LogValuesTableQ57_7 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_57_7))

    #if (defined(TINYMIND_USE_LOG_58_6))
    template<>
    struct LogTableValueSize<58, 6, true>
    {
        typedef LogValuesTableQ58_6 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_58_6))

    #if (defined(TINYMIND_USE_LOG_59_5))
    template<>
    struct LogTableValueSize<59, 5, true>
    {
        typedef LogValuesTableQ59_5 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_59_5))

    #if (defined(TINYMIND_USE_LOG_60_4))
    template<>
    struct LogTableValueSize<60, 4, true>
    {
        typedef LogValuesTableQ60_4 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_60_4))

    #if (defined(TINYMIND_USE_LOG_61_3))
    template<>
    struct LogTableValueSize<61, 3, true>
    {
        typedef LogValuesTableQ61_3 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_61_3))

    #if (defined(TINYMIND_USE_LOG_62_2))
    template<>
    struct LogTableValueSize<62, 2, true>
    {
        typedef LogValuesTableQ62_2 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_62_2))

    #if (defined(TINYMIND_USE_LOG_63_1))
    template<>
    struct LogTableValueSize<63, 1, true>
    {
        typedef LogValuesTableQ63_1 LogTableType;
    };
    #endif // (defined(TINYMIND_USE_LOG_63_1))

    template<unsigned FixedBits,unsigned FracBits, bool IsSigned>
    struct LogValuesTableSelector
    {
        typedef typename LogTableValueSize<FixedBits, FracBits, IsSigned>::LogTableType LogTableType;
    };
}
