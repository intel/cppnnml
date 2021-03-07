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

import matplotlib.pyplot as pyplot

if __name__ == '__main__':
    assert(len(sys.argv) == 2)

    dataPath = sys.argv[1]
    assert(os.path.exists(dataPath))

    fileName = dataPath.split(os.path.sep)[-1]

    startStateToDataDict = {}

    with open(dataPath) as f:
        line = f.readline()
        while(line):
            s = line.strip().split(',')
            startState = int(s[0], 10)
            if not startState in startStateToDataDict.keys():
                startStateToDataDict[startState] = []
            moves = []
            for i in range(1, len(s) - 1):
                moves.append(int(s[i], 10))

            startStateToDataDict[startState].append(moves)
            line = f.readline()

    for startState in startStateToDataDict.keys():
        pyplot.figure()
        pyplot.title("%s\nState state = %d" % (fileName, startState))
        for line in startStateToDataDict[startState]:
            data = [startState] + line
            pyplot.plot(range(len(data)), data, '-D')

    pyplot.show()

