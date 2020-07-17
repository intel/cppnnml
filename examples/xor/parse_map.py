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

tinymindSize = 0
foundTinymind = False
lastAddress = 0

def parseLine(line):
    global tinymindSize
    global foundTinymind
    global lastAddress

    s = line.split(' ')
    if 'tinymind' in line or foundTinymind:
        s = s[1].split(':')
        address = int("0x%s" % s[1], 16)
        if foundTinymind:
            tinymindSize += (address - lastAddress)
            foundTinymind = False 
        else:
            lastAddress = address
            foundTinymind = True

if __name__ == '__main__':
    assert(len(sys.argv) == 2)

    mapPath = sys.argv[1]
    assert(os.path.exists(mapPath))

    fileName = mapPath.split(os.path.sep)[-1]

    with open(mapPath) as f:
        line = f.readline()
        parseLine(line)
        line = f.readline()
        while(line):
            parseLine(line)
            line = f.readline()

    print('tinymind bytes = %d' % tinymindSize)
    print('total KB = %d' % (tinymindSize / 1024))
