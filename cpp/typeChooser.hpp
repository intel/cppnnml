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

namespace tinymind {
    template<unsigned NumBits, bool IsSigned>
    struct FullWidthType
    {};

    template<>
    struct FullWidthType<8, false>
    {
        typedef uint8_t FractionalPartFieldType;
        typedef uint8_t FixedPartFieldType;
        typedef uint8_t FullWidthFieldType;
        typedef uint8_t FullWidthValueType;
    };

    template<>
    struct FullWidthType<8, true>
    {
        typedef uint8_t FractionalPartFieldType;
        typedef int8_t  FixedPartFieldType;
        typedef uint8_t FullWidthFieldType;
        typedef int8_t  FullWidthValueType;
    };

    template<>
    struct FullWidthType<16, false>
    {
        typedef uint16_t FractionalPartFieldType;
        typedef uint16_t FixedPartFieldType;
        typedef uint16_t FullWidthFieldType;
        typedef uint16_t FullWidthValueType;
    };

    template<>
    struct FullWidthType<16, true>
    {
        typedef uint16_t FractionalPartFieldType;
        typedef int16_t  FixedPartFieldType;
        typedef uint16_t FullWidthFieldType;
        typedef int16_t  FullWidthValueType;
    };

    template<>
    struct FullWidthType<32, false>
    {
        typedef uint32_t FractionalPartFieldType;
        typedef uint32_t FixedPartFieldType;
        typedef uint32_t FullWidthFieldType;
        typedef uint32_t FullWidthValueType;
    };

    template<>
    struct FullWidthType<32, true>
    {
        typedef uint32_t FractionalPartFieldType;
        typedef int32_t  FixedPartFieldType;
        typedef uint32_t FullWidthFieldType;
        typedef int32_t  FullWidthValueType;
    };

    template<>
    struct FullWidthType<64, false>
    {
        typedef uint64_t FractionalPartFieldType;
        typedef uint64_t FixedPartFieldType;
        typedef uint64_t FullWidthFieldType;
        typedef uint64_t FullWidthValueType;
    };

    template<>
    struct FullWidthType<64, true>
    {
        typedef uint64_t FractionalPartFieldType;
        typedef int64_t  FixedPartFieldType;
        typedef uint64_t FullWidthFieldType;
        typedef int64_t  FullWidthValueType;
    };

    template<unsigned NumBits, bool IsSigned>
    struct FullWidthFieldTypeChooser
    {
        static_assert(NumBits <= 64, "NumBits must be <= 64.");
        static constexpr unsigned Result = (NumBits <= 8) ? 8 : (NumBits <= 16) ? 16 : (NumBits <= 32) ? 32 : 64;
        typedef typename FullWidthType<Result, IsSigned>::FractionalPartFieldType FractionalPartFieldType;
        typedef typename FullWidthType<Result, IsSigned>::FixedPartFieldType      FixedPartFieldType;
        typedef typename FullWidthType<Result, IsSigned>::FullWidthFieldType      FullWidthFieldType;
        typedef typename FullWidthType<Result, IsSigned>::FullWidthValueType      FullWidthValueType;
    };

    // Meant to be overloaded by client code not using tinymind Q format value types
    template<typename ValueType>
    struct FullWidthFieldTypeShim
    {
        typedef typename ValueType::FullWidthFieldType FullWidthFieldType;
    };
}