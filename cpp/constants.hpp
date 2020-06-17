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
    template<typename ValueType>
    struct Constants
    {
        static ValueType one()
        {
            static const ValueType result(1,0);

            return result;
        }

        static ValueType negativeOne()
        {
            static const ValueType result(-1, 0);

            return result;
        }

        static ValueType zero()
        {
            static const ValueType result(0);

            return result;
        }
    };
#ifndef __ghs__
    template<>
    struct Constants<float>
    {
        static float one()
        {
            return 1.0f;
        }

        static float negativeOne()
        {
            return -1.0f;
        }

        static float zero()
        {
            return 0.0f;
        }
    };

    template<>
    struct Constants<double>
    {
        static double one()
        {
            return 1.0;
        }

        static double negativeOne()
        {
            return -1.0;
        }

        static double zero()
        {
            return 0.0;
        }
    };
#endif // __ghs__
}
