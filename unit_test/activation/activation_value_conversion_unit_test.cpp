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

#define BOOST_TEST_MODULE test module name
#include <boost/test/included/unit_test.hpp>

#include <cstdint>

#include "range.hpp"
#include "qformat.hpp"
#include "constants.hpp"
#include "activationFunctions.hpp"

typedef tinymind::QValue<21, 11, true, tinymind::RoundUpPolicy> QValueType;

BOOST_AUTO_TEST_SUITE(test_suite_activation_value_conversion)

BOOST_AUTO_TEST_CASE(test_case_activation_value_input_conversion_0_100)
{
    typedef tinymind::RangeConverter<QValueType> RangeConverterType;
    static const QValueType minInput(0);
    static const QValueType maxInput(100, 0);
    static const QValueType minOutput(0);
    static const QValueType maxOutput(10, 0);
    static const QValueType oneHalf(0,(1 << (QValueType::NumberOfFractionalBits - 1)));
    QValueType input(minInput);

    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(tinymind::Constants<QValueType>::zero() == input);

    input = maxInput;
    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(tinymind::Constants<QValueType>::one() == input);

    input = QValueType(50,0);
    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(oneHalf == input);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_input_conversion_1_100)
{
    typedef tinymind::RangeConverter<QValueType> RangeConverterType;
    static const QValueType minInput(1,0);
    static const QValueType maxInput(100, 0);
    QValueType input(minInput);

    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(tinymind::Constants<QValueType>::zero() == input);

    input = maxInput;
    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(tinymind::Constants<QValueType>::one() == input);

    input = QValueType(50,0);
    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(1013 == input.getValue()); // ~ 0.494949495 * 2^11
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_output_sigmoid_conversion_0_10)
{
    typedef tinymind::ActivationOutputRangeConverter<QValueType, QValueType, tinymind::SigmoidActivationPolicy<QValueType> > ActivationOutputRangeConverterSigmoidType;
    static const QValueType minOutput(0);
    static const QValueType maxOutput(10, 0);
    static const QValueType oneHalf(0,(1 << (QValueType::NumberOfFractionalBits - 1)));
    static const QValueType midPoint = (maxOutput / 2);
    QValueType output(minOutput);

    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(tinymind::Constants<QValueType>::zero() == output);

    output = oneHalf;
    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(midPoint == output);

    output = tinymind::Constants<QValueType>::one();
    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(maxOutput == output);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_output_tanh_conversion_0_10)
{
    typedef tinymind::ActivationOutputRangeConverter<QValueType, QValueType, tinymind::TanhActivationPolicy<QValueType> > ActivationOutputRangeConverterTanhType;
    static const QValueType minOutput(0);
    static const QValueType maxOutput(10, 0);
    static const QValueType oneHalf(0,(1 << (QValueType::NumberOfFractionalBits - 1)));
    static const QValueType midPoint = (maxOutput / 2);
    QValueType output(tinymind::Constants<QValueType>::negativeOne());

    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(tinymind::Constants<QValueType>::zero() == output);

    output = tinymind::Constants<QValueType>::zero();
    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(midPoint == output);

    output = tinymind::Constants<QValueType>::one();
    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(maxOutput == output);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_output_sigmoid_conversion_2_10)
{
    typedef tinymind::ActivationOutputRangeConverter<QValueType, QValueType, tinymind::SigmoidActivationPolicy<QValueType> > ActivationOutputRangeConverterSigmoidType;
    static const QValueType minOutput(2,0);
    static const QValueType maxOutput(10, 0);
    static const QValueType oneFourth = (tinymind::Constants<QValueType>::one() / QValueType(4, 0));
    QValueType output(0);

    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(minOutput == output);

    output = oneFourth;
    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(QValueType(4, 0) == output);

    output = tinymind::Constants<QValueType>::one();
    ActivationOutputRangeConverterSigmoidType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(maxOutput == output);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_output_tanh_conversion_2_10)
{
    typedef tinymind::ActivationOutputRangeConverter<QValueType, QValueType, tinymind::TanhActivationPolicy<QValueType> > ActivationOutputRangeConverterTanhType;
    static const QValueType minOutput(2,0);
    static const QValueType maxOutput(10, 0);
    static const QValueType negativeOneHalf = (tinymind::Constants<QValueType>::negativeOne() * (tinymind::Constants<QValueType>::one() / QValueType(2, 0)));
    static const QValueType six(6, 0);
    QValueType output(tinymind::Constants<QValueType>::negativeOne());

    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(minOutput == output);

    output = negativeOneHalf;
    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(QValueType(4, 0) == output);

    output = tinymind::Constants<QValueType>::zero();
    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(six == output);

    output = tinymind::Constants<QValueType>::one();
    ActivationOutputRangeConverterTanhType::convertToOutputValue(output, minOutput, maxOutput);
    BOOST_TEST(maxOutput == output);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_input_conversion_range_equals_0)
{
    typedef tinymind::RangeConverter<QValueType> RangeConverterType;
    static const QValueType minInput(100, 0);
    static const QValueType maxInput(100, 0);
    QValueType input(minInput);

    RangeConverterType::convertToFractionOfRange(input, minInput, maxInput);
    BOOST_TEST(minInput == input);
}

BOOST_AUTO_TEST_CASE(test_case_activation_value_input_conversion_range_invalid_min_max)
{
    typedef tinymind::RangeConverter<QValueType> RangeConverterType;
    static const QValueType minInput(100, 0);
    static const QValueType maxInput(101, 0);
    QValueType input(minInput);

    RangeConverterType::convertToFractionOfRange(input, maxInput, minInput);
    BOOST_TEST(minInput == input);
}

BOOST_AUTO_TEST_SUITE_END()
