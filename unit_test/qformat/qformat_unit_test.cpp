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

// qformat_unit_test.cpp : Defines the entry point for the Qformat template unit tests.
//
#include <iostream>
#include <cstdint>
#include <limits>

#include "qformat.hpp"

typedef tinymind::QValue<8, 8, false, tinymind::RoundUpPolicy> UnsignedQ8_8Type;
typedef tinymind::QValue<8, 8, true> SignedQ8_8Type;
typedef tinymind::QValue<7, 9, true> SignedQ7_9Type;
typedef tinymind::QValue<1, 15, true> SignedQ1_15Type;
typedef tinymind::QValue<24, 8, true> SignedQ24_8Type;
typedef tinymind::QValue<16, 16, true> SignedQ16_16Type;
typedef tinymind::QValue<24, 8, false> UnsignedQ24_8Type;
typedef tinymind::QValue<8, 24, true> SignedQ8_24Type;
typedef tinymind::QValue<8, 24, false> UnsignedQ8_24Type;
typedef tinymind::QValue<24, 8, false> UnsignedQ24_8Type;

typedef tinymind::QValue<8, 8, false> UnsignedTruncatingQType;

static_assert((std::numeric_limits<uint8_t>::max() == UnsignedQ8_8Type::MaxFixedPartValue), "Incorrect max fixed value.");
static_assert((std::numeric_limits<uint8_t>::max() == UnsignedQ8_8Type::MaxFractionalPartValue), "Incorrect max fractional value.");
static_assert((((1ULL << 24) - 1) == UnsignedQ24_8Type::MaxFixedPartValue), "Incorrect max value.");
static_assert((std::numeric_limits<uint8_t>::max() == UnsignedQ24_8Type::MaxFractionalPartValue), "Incorrect max fractional value.");
static_assert((std::numeric_limits<int8_t>::max() == SignedQ8_8Type::MaxFixedPartValue), "Incorrect fixed max value.");
static_assert((std::numeric_limits<uint8_t>::max() == SignedQ8_8Type::MaxFractionalPartValue), "Incorrect max fractional value.");
static_assert((((1ULL << 23) - 1) == SignedQ24_8Type::MaxFixedPartValue), "Incorrect max fixed value.");
static_assert((std::numeric_limits<uint8_t>::max() == SignedQ24_8Type::MaxFractionalPartValue), "Incorrect max fractional value.");
static_assert(((std::numeric_limits<int8_t>::max() >> 1) == SignedQ7_9Type::MaxFixedPartValue), "Incorrect max fixed value.");
static_assert((((1ULL << 9) - 1) == SignedQ7_9Type::MaxFractionalPartValue), "Incorrect max fractional value.");

static_assert((std::numeric_limits<uint16_t>::max() == std::numeric_limits<typename UnsignedQ8_8Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint16_t>::max() == std::numeric_limits<typename UnsignedQ8_8Type::FractionalPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<int16_t>::max() == std::numeric_limits<typename SignedQ8_8Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint16_t>::max() == std::numeric_limits<typename SignedQ8_8Type::FractionalPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<int16_t>::max() == std::numeric_limits<typename SignedQ7_9Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint16_t>::max() == std::numeric_limits<typename SignedQ7_9Type::FractionalPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<int16_t>::max() == std::numeric_limits<typename SignedQ1_15Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint16_t>::max() == std::numeric_limits<typename SignedQ1_15Type::FractionalPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint32_t>::max() == std::numeric_limits<typename UnsignedQ24_8Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint32_t>::max() == std::numeric_limits<typename UnsignedQ24_8Type::FractionalPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<int32_t>::max() == std::numeric_limits<typename SignedQ24_8Type::FixedPartFieldType>::max()), "Invalid type.");
static_assert((std::numeric_limits<uint32_t>::max() == std::numeric_limits<typename SignedQ24_8Type::FractionalPartFieldType>::max()), "Invalid type.");

#define BOOST_TEST_MODULE test module name
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_qformat)

BOOST_AUTO_TEST_CASE(test_case_construction)
{
    SignedQ8_8Type Q0(0, 0);
    SignedQ8_8Type Q1(0, 0);
    SignedQ8_8Type Q2(-1, 0);
    SignedQ8_8Type Q3(-1, 1);
    SignedQ1_15Type Q4(-1, 0);
    SignedQ1_15Type Q5(0, 1);
    SignedQ7_9Type Q6(-1, 0);
    SignedQ7_9Type Q7(1, 0);
    SignedQ24_8Type Q8(-1, 0);
    UnsignedQ8_8Type uQ0;
    UnsignedQ8_8Type uQ1(0, 0);
    UnsignedQ8_8Type uQ2(1, 0);
    SignedQ8_24Type Q9(-1, 0);
    SignedQ8_24Type Q10(1, 0);

    BOOST_TEST(static_cast<SignedQ8_8Type::FixedPartFieldType>(0) == Q0.getFixedPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FractionalPartFieldType>(0) == Q0.getFractionalPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    BOOST_TEST(static_cast<SignedQ8_8Type::FixedPartFieldType>(0) == Q1.getFixedPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FractionalPartFieldType>(0) == Q1.getFractionalPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q1.getValue());

    BOOST_TEST(static_cast<SignedQ8_8Type::FixedPartFieldType>(-1) == Q2.getFixedPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FractionalPartFieldType>(0) == Q2.getFractionalPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xFF00) == Q2.getValue());

    BOOST_TEST(static_cast<SignedQ8_8Type::FixedPartFieldType>(-1) == Q3.getFixedPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FractionalPartFieldType>(1) == Q3.getFractionalPart());
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xFF01) == Q3.getValue());

    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x8000) == Q4.getValue());
    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x0001) == Q5.getValue());

    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0xfe00) == Q6.getValue());
    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x0200) == Q7.getValue());

    BOOST_TEST(static_cast<UnsignedQ8_8Type::FixedPartFieldType>(0) == uQ0.getFixedPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FractionalPartFieldType>(0) == uQ0.getFractionalPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    BOOST_TEST(static_cast<UnsignedQ8_8Type::FixedPartFieldType>(0) == uQ1.getFixedPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FractionalPartFieldType>(0) == uQ1.getFractionalPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ1.getValue());

    BOOST_TEST(static_cast<UnsignedQ8_8Type::FixedPartFieldType>(1) == uQ2.getFixedPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FractionalPartFieldType>(0) == uQ2.getFractionalPart());
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x100) == uQ2.getValue());

    BOOST_TEST(static_cast<SignedQ24_8Type::FullWidthValueType>(0xFFFFFF00) == Q8.getValue());

    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFF000000) == Q9.getValue());
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x1000000) == Q10.getValue());

    Q0.setValue(1, 0);
    BOOST_TEST(static_cast<SignedQ8_8Type ::FullWidthValueType>(0x100) == Q0.getValue());

    Q0.setValue(-1, 0);
    BOOST_TEST(static_cast<SignedQ8_8Type ::FullWidthValueType>(0xFF00) == Q0.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_addition)
{
    UnsignedQ8_8Type uQ0(0, 0);
    UnsignedQ8_8Type uQ1(0, 1);
    UnsignedQ8_8Type uQ2(1, 1);
    UnsignedQ8_8Type uQ3;
    UnsignedQ8_8Type uQ4(-1, 0);
    SignedQ8_8Type Q0;
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_8Type Q2(1, 0);
    SignedQ8_8Type Q3;
    SignedQ1_15Type Q4(-1, 0);
    SignedQ1_15Type Q5(0, 1);
    SignedQ24_8Type Q6(-1, 0);
    SignedQ24_8Type Q7(1, 0);
    SignedQ24_8Type Q8;
    SignedQ24_8Type Q9(0,128);
    SignedQ8_24Type Q10(-1, 0);
    SignedQ8_24Type Q11(1, 0);
    SignedQ8_24Type Q12(0x800000);
    SignedQ8_24Type Q13;

    uQ0 += 0;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    uQ0 += uQ0;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    uQ0 += uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(1) == uQ0.getValue());

    uQ0 += 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(2) == uQ0.getValue());

    uQ0 += uQ2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x103) == uQ0.getValue());

    uQ3 = uQ1 + uQ2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x102) == uQ3.getValue());

    Q0 += 0;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    Q0 += Q0;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    Q0 += Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(-256) == Q0.getValue());

    Q0 += Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(-512) == Q0.getValue());

    Q0 += Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(-256) == Q0.getValue());

    Q0 += Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    Q3 = Q1 + Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0) == Q3.getValue());

    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x8000) == Q4.getValue());

    Q4 += Q5;

    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x8001) == Q4.getValue());

    Q4 = 0x8000;

    for (uint32_t i = 0; i < 4194303; ++i)
    {
        Q4 += Q5;
    }

    BOOST_TEST(static_cast<SignedQ1_15Type::FullWidthValueType>(0x7FFF) == Q4.getValue());

    Q8 = Q6 + Q7;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x0) == Q8.getValue());

    Q8 = Q7 + Q9;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x180) == Q8.getValue());

    Q13 = Q10 + Q11;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x0) == Q13.getValue());

    Q13 = Q11 + Q11;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x2000000) == Q13.getValue());

    Q13 = Q10 + Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFE000000) == Q13.getValue());

    Q13 = Q10 + Q12;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFF800000) == Q13.getValue());

    Q13 = Q12 + Q12;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x1000000) == Q13.getValue());

    Q13 = Q11 + 1;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x2000000) == Q13.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_subtraction)
{
    UnsignedQ8_8Type uQ0(1, 1);
    UnsignedQ8_8Type uQ1(0, 1);
    UnsignedQ8_8Type uQ2(0, 0xFF);
    UnsignedQ8_8Type uQ3;
    SignedQ8_8Type Q0(1, 0);
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_8Type Q2(1, 0);
    SignedQ8_8Type Q3;
    SignedQ1_15Type Q4(0x7F, 0xFF);
    SignedQ8_24Type Q10(-1, 0);
    SignedQ8_24Type Q11(1, 0);
    SignedQ8_24Type Q12(0x800000);
    SignedQ8_24Type Q13;

    uQ3 = uQ0 - uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x100) == uQ3.getValue());

    uQ3 = uQ3 - uQ2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x1) == uQ3.getValue());

    uQ0 -= 0;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ0.getValue());

    uQ0 -= uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x100) == uQ0.getValue());

    uQ0 -= 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0xFF) == uQ0.getValue());

    uQ0 -= uQ2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    Q0 -= 0;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x100) == Q0.getValue());

    Q0 -= Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x200) == Q0.getValue());

    Q0 -= Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x100) == Q0.getValue());

    Q0 -= Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x0) == Q0.getValue());

    Q0 -= Q2;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xFF00) == Q0.getValue());

    Q13 = Q10 - Q11;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFE000000) == Q13.getValue());

    Q13 = Q10 - Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x0) == Q13.getValue());

    Q13 = Q11 - Q12;
    BOOST_TEST(Q12.getValue() == Q13.getValue());

    Q13 = Q12 - Q11;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFF800000) == Q13.getValue());

    Q13 = Q11 - 1;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x0) == Q13.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_increment_decrement)
{
    UnsignedQ8_8Type uQ0(0, 0);
    UnsignedQ8_8Type uQ2(-1, 0);
    SignedQ8_8Type Q0;
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_24Type Q10(-1, 0);
    SignedQ8_24Type Q11(1, 0);
    SignedQ8_24Type Q12(0x800000);
    SignedQ8_24Type Q13;

    --uQ0;
    BOOST_TEST(uQ2.getValue() == uQ0.getValue());

    ++uQ0;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    uQ0--;
    BOOST_TEST(uQ2.getValue() == uQ0.getValue());

    uQ0++;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == uQ0.getValue());

    --Q0;
    BOOST_TEST(Q1.getValue() == Q0.getValue());

    ++Q0;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    Q0--;
    BOOST_TEST(Q1.getValue() == Q0.getValue());

    Q0++;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0) == Q0.getValue());

    Q10++;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0) == Q10.getValue());

    ++Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x1000000) == Q10.getValue());

    Q10--;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0) == Q10.getValue());

    --Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFF000000) == Q10.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_multiplication)
{
    UnsignedQ8_8Type uQ0(1, 1);
    UnsignedQ8_8Type uQ1(1, 0);
    UnsignedQ8_8Type uQ2;
    UnsignedQ8_8Type uQ3(2, 0x9F);
    UnsignedQ8_8Type uQ4(0, 0x7F);
    UnsignedQ8_8Type uQ5;
    SignedQ8_8Type Q0(1, 0);
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_8Type Q2;
    SignedQ8_8Type Q3(0, 0x80);
    SignedQ8_8Type Q4(-1, 0);
    SignedQ8_8Type Q5;
    SignedQ8_8Type Q6(0, 0x80);
    SignedQ8_8Type Q7(-1, 0x80);
    SignedQ8_8Type Q8;
    SignedQ7_9Type Q9(-1, 0);
    SignedQ7_9Type Q10(0, 0x100);
    SignedQ7_9Type Q11;
    SignedQ24_8Type Q12(1, 0);
    SignedQ24_8Type Q13;
    SignedQ24_8Type Q14(0, 128);
    SignedQ24_8Type Q15(-1, 0);
    SignedQ8_24Type Q16(-1, 0);
    SignedQ8_24Type Q17(1, 0);
    SignedQ8_24Type Q18(0x800000);
    SignedQ8_24Type Q19;
    SignedQ8_8Type Q20(-2, 0x94);
    SignedQ8_8Type Q21(0, 0x7F);

    uQ2 = uQ0 * uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ2 = uQ1 * 2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x200) == uQ2.getValue());

    uQ2 = uQ0 * 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ5 = uQ3 * uQ4;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x14D) == uQ5.getValue());

    uQ2 = uQ1;
    uQ2 *= 2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x200) == uQ2.getValue());

    Q2 = Q0 * Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xff00) == Q2.getValue());

    Q5 = Q3 * Q4;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xff80) == Q5.getValue());

    Q8 = Q6 * Q7;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xffC0) == Q8.getValue());

    Q11 = Q9 * Q10;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xff00) == Q11.getValue());

    Q13 = Q12 * Q14;
    BOOST_TEST(static_cast<SignedQ24_8Type::FullWidthValueType>(0x80) == Q13.getValue());

    Q13 = Q14 * Q15;
    BOOST_TEST(static_cast<SignedQ24_8Type::FullWidthValueType>(0xFFFFFF80) == Q13.getValue());

    Q19 = Q17 * Q18;
    BOOST_TEST(Q18.getValue() == Q19.getValue());

    Q19 = Q16 * Q18;
    BOOST_TEST(static_cast<SignedQ24_8Type::FullWidthValueType>(0xFF800000) == Q19.getValue());

    Q19 = Q16;
    Q19 *= Q18;
    BOOST_TEST(static_cast<SignedQ24_8Type::FullWidthValueType>(0xFF800000) == Q19.getValue());

    Q2 = Q20 * Q21;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xff4B) == Q2.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_multiplication_trunc)
{
    UnsignedTruncatingQType uQ0(1, 1);
    UnsignedTruncatingQType uQ1(1, 0);
    UnsignedTruncatingQType uQ2;
    UnsignedTruncatingQType uQ3(2, 0x9F);
    UnsignedTruncatingQType uQ4(0, 0x7F);
    UnsignedTruncatingQType uQ5;

    uQ2 = uQ0 * uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ2 = uQ1 * 2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x200) == uQ2.getValue());

    uQ2 = uQ0 * 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ5 = uQ3 * uQ4;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x14C) == uQ5.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_division)
{
    UnsignedQ8_8Type uQ0(1, 1);
    UnsignedQ8_8Type uQ1(1, 0);
    UnsignedQ8_8Type uQ2;
    UnsignedQ8_8Type uQ3(2, 0);
    UnsignedQ8_8Type uQ4(0, 0x80);
    UnsignedQ8_8Type uQ5(128, 0);
    UnsignedQ8_8Type uQ6(64, 0);
    SignedQ8_8Type Q0(1, 0);
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_8Type Q2;
    SignedQ8_8Type Q3(0, 0x80);
    SignedQ8_8Type Q4(-0, 0x80);
    SignedQ7_9Type Q5(-1, 0);
    SignedQ7_9Type Q6(0, 0x100);
    SignedQ7_9Type Q7;
    SignedQ8_24Type Q8(-1, 0);
    SignedQ8_24Type Q9(1, 0);
    SignedQ8_24Type Q10(0x800000);
    SignedQ8_24Type Q11;

    uQ2 = uQ0 / uQ1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ2 = uQ0 / 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x101) == uQ2.getValue());

    uQ2 = uQ1 / 2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x80) == uQ2.getValue());

    uQ2 = uQ1 / uQ3;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x80) == uQ2.getValue());

    uQ2 = uQ1 / uQ4;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x200) == uQ2.getValue());

    Q2 = Q0 / Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xff00) == Q2.getValue());

    Q2 = Q2 / Q1;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x100) == Q2.getValue());

    Q2 = Q0 / Q3;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0x200) == Q2.getValue());

    Q2 = Q1 / Q3;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xfe00) == Q2.getValue());

    Q7 = Q5 / Q6;
    BOOST_TEST(static_cast<SignedQ8_8Type::FullWidthValueType>(0xfc00) == Q7.getValue());

    Q11 = Q9 / Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x2000000) == Q11.getValue());

    Q11 = Q9;
    Q11 /= Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0x2000000) == Q11.getValue());

    Q11 = Q8 / Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFE000000) == Q11.getValue());

    Q11 = Q8;
    Q11 /= Q10;
    BOOST_TEST(static_cast<SignedQ8_24Type::FullWidthValueType>(0xFE000000) == Q11.getValue());

    uQ2 = (uQ6 / uQ5);
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x0080) == uQ2.getValue());

    uQ2 = uQ1;
    uQ2 /= 1;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x0100) == uQ2.getValue());

    uQ2 /= 2;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x0080) == uQ2.getValue());

    typename UnsignedQ8_8Type::FixedPartFieldType divisor = 1;
    uQ2 = uQ1;
    uQ2 /= divisor;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x0100) == uQ2.getValue());

    divisor = 2;
    uQ2 /= divisor;
    BOOST_TEST(static_cast<UnsignedQ8_8Type::FullWidthValueType>(0x0080) == uQ2.getValue());
}

BOOST_AUTO_TEST_CASE(test_case_comparators)
{
    UnsignedQ8_8Type uQ0(1, 1);
    UnsignedQ8_8Type uQ1(1, 0);
    UnsignedQ8_8Type uQ2(1, 0);
    SignedQ8_8Type Q0(1, 0);
    SignedQ8_8Type Q1(-1, 0);
    SignedQ8_8Type Q2(-1, 0);
    SignedQ8_8Type Q3(-2, 0);
    int one = 1;
    int negativeOne = -1;
    int bigValue = 100;
    int smallValue = -100;

    BOOST_TEST(uQ0 > uQ1);
    BOOST_TEST(uQ1 < uQ0);
    BOOST_TEST(uQ2 <= uQ1);
    BOOST_TEST(uQ2 >= uQ1);
    BOOST_TEST((uQ2 <= one));
    BOOST_TEST((uQ2 <= bigValue));
    BOOST_TEST((uQ2 < bigValue));
    BOOST_TEST((Q1 >= negativeOne));
    BOOST_TEST((Q1 >= smallValue));
    BOOST_TEST((Q1 > smallValue));
    BOOST_TEST(Q0 > Q1);
    BOOST_TEST(Q1 < Q0);
    BOOST_TEST(Q2 <= Q1);
    BOOST_TEST(Q2 >= Q1);
    BOOST_TEST(Q3 < Q2);
    BOOST_TEST(Q3 <= Q2);
}

BOOST_AUTO_TEST_CASE(test_case_assignment)
{
    SignedQ8_8Type Q0;
    SignedQ8_8Type Q1(2, 0);

    BOOST_TEST(Q0.getValue() == 0x0);
    Q0 = 0x100;
    BOOST_TEST(Q0.getValue() == 0x100);

    Q0 = 0;
    BOOST_TEST(Q0.getValue() == 0x0);
    Q0.setValue(0x100);
    BOOST_TEST(Q0.getValue() == 0x100);

    Q0 = Q1;
    BOOST_TEST(Q0.getValue() == 0x200);
}

BOOST_AUTO_TEST_CASE(test_case_conversion)
{
    SignedQ8_8Type Q0;
    SignedQ8_24Type Q1(1, 0);
    SignedQ8_24Type Q2(1, (1 << 23));
    SignedQ16_16Type Q3(1, (1 << 15));
    SignedQ16_16Type Q4(-1, (1 << 15));
    SignedQ24_8Type Q5(1, (1 << 7));
    SignedQ24_8Type Q6(-1, (1 << 7));
    UnsignedQ24_8Type UQ0(1, (1 << 7));

    BOOST_TEST(Q0.getValue() == 0x0);
    Q0.convertFromOtherQValueType(Q1);
    BOOST_TEST(Q0.getValue() == 0x100);

    Q0.convertFromOtherQValueType(Q2);
    BOOST_TEST(Q0.getValue() == 0x180);

    Q0.convertFromOtherQValueType(Q3);
    BOOST_TEST(Q0.getValue() == 0x180);

    Q0.convertFromOtherQValueType(Q4);
    BOOST_TEST(Q0.getValue() == static_cast<typename SignedQ8_8Type::FullWidthValueType>(0xFF80));

    BOOST_TEST(Q3.getValue() == static_cast<typename SignedQ16_16Type::FullWidthValueType>(0x18000));
    Q3.convertFromOtherQValueType(Q0);
    BOOST_TEST(Q3.getValue() == static_cast<typename SignedQ16_16Type::FullWidthValueType>(0xFFFF8000));

    Q0 = 0;
    BOOST_TEST(Q0.getValue() == 0x0);
    Q0.convertFromOtherQValueType(UQ0);
    BOOST_TEST(Q0.getValue() == static_cast<typename SignedQ8_8Type::FullWidthValueType>(0x180));

    Q0 = 0;
    BOOST_TEST(Q0.getValue() == 0x0);
    Q0.convertFromOtherQValueType(Q5);
    BOOST_TEST(Q0.getValue() == static_cast<typename SignedQ8_8Type::FullWidthValueType>(0x180));

    Q0 = 0;
    BOOST_TEST(Q0.getValue() == 0x0);
    Q0.convertFromOtherQValueType(Q6);
    BOOST_TEST(Q0.getValue() == static_cast<typename SignedQ8_8Type::FullWidthValueType>(0xFF80));

    Q0 = 0x180;
    UQ0 = 0;
    BOOST_TEST(UQ0.getValue() == 0x0);
    UQ0.convertFromOtherQValueType(Q0);
    BOOST_TEST(UQ0.getValue() == static_cast<typename UnsignedQ24_8Type::FullWidthValueType>(0x180));
}

BOOST_AUTO_TEST_SUITE_END()
