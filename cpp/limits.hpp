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
    template<typename T>
    struct limits
    {
    };

    template<>
    struct limits<int8_t>
    {
        static constexpr int8_t max = 127;
    };

    template<>
    struct limits<uint8_t>
    {
        static constexpr uint8_t max = 255;
    };

    template<>
    struct limits<int16_t>
    {
        static constexpr int16_t max = 32767;
    };

    template<>
    struct limits<uint16_t>
    {
        static constexpr uint16_t max = 65535;
    };

    template<>
    struct limits<int32_t>
    {
        static constexpr int32_t max = 2147483647;
    };

    template<>
    struct limits<uint32_t>
    {
        static constexpr uint32_t max = 4294967295;
    };

    template<>
    struct limits<int64_t>
    {
        static constexpr int64_t max = 9223372036854775807ULL;
    };

    template<>
    struct limits<uint64_t>
    {
        static constexpr uint64_t max = 18446744073709551615ULL;
    };
}