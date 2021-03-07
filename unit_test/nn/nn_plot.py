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

    names = []
    data = []
    with open(dataPath) as f:
        line = f.readline()
        names = line.strip(' \r\n').split(',')
        line = f.readline()
        while(line):
            s = line.strip(' \r\n').split(',')
            data.extend([float(value) for value in s])
            line = f.readline()

    pyplot.figure()
    pyplot.suptitle("%s\n%s" % ("NN Plot", fileName))
    index = 0
    rows = int(len(names) / 4)
    cols = int((len(names) / rows) + (len(names) % rows))
    xax = None
    for row in range(rows):
        for col in range(cols):
            if(index < len(names)):
                values = data[index::len(names)]
                if xax:
                    ax = pyplot.subplot2grid((rows,cols), (row,col), sharex=xax)
                else:
                    ax = pyplot.subplot2grid((rows,cols), (row,col))
                    xax = ax
                ax.plot(range(len(values)), values, label = names[index])
                ax.grid(True)
                ax.set_title(names[index], fontsize=7)
                ax.tick_params(labelsize=6)
                index += 1
    pyplot.show()
