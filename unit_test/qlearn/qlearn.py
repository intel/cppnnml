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
import numpy

from matplotlib import pyplot

if __name__ == '__main__':
    if(len(sys.argv) < 2):
        print "Error: must provide folder rewardPath."
        sys.exit(-1)
        
    folderPath = sys.argv[1]
    if not folderPath.endswith(os.sep):
        folderPath += os.sep
        
    if not os.path.exists(folderPath):
        print "Error: Folder rewardPath %s does not exist." % folderPath
        sys.exit(-1)
        
    rewardPath = folderPath + 'qlearn_accum_reward.txt'
    if not os.path.exists(rewardPath):
        print "Error: File %s does not exist." % rewardPath
        sys.exit(-1)
    
    qtablePath = folderPath + 'qtable.txt'
    if not os.path.exists(qtablePath):
        print "Error: File %s does not exist." % qtablePath
        sys.exit(-1)

    reward = numpy.fromfile(rewardPath, dtype=numpy.uint32, sep=os.linesep)
    qtable = numpy.loadtxt(fname=qtablePath, dtype=numpy.uint32, delimiter=",")
    
    print qtable
    
    pyplot.plot(reward, 'b-*')
    pyplot.show()