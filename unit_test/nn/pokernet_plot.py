'''
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
'''

import os
import sys
import itertools

import matplotlib.pyplot as pyplot

if __name__ == '__main__':
    assert(len(sys.argv) == 2)

    dataPath = sys.argv[1]
    assert(os.path.exists(dataPath))

    data = []
    with open(dataPath) as f:
        line = f.readline()
        while(line):
            s = line.strip(' \r\n').split(',')
            data.extend([int(value) for value in s])
            line = f.readline()

    colors=itertools.cycle('bcmk').next
    shapes=itertools.cycle('*xov^>8sp<pP').next
    fig, axs = pyplot.subplots(2,1,sharex=True)
    axs[0].plot(data[0::13], colors() + '-' + shapes(), label = 'Training data')
    axs[0].plot(data[1::13], colors() + '-' + shapes(), label = 'Network result')
    axs[0].legend()
    axs[1].plot(data[12::13], 'r-o', label = 'Error')
    axs[1].legend()
    pyplot.title("PokerNet")
    pyplot.show()
