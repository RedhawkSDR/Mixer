#!/usr/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#


import time
from ossie.utils import sb
import numpy as np

print '*****************************************'
print '************ Mixer Unit Test ************'
print '*****************************************'
 
#Use sb helpers for producing and receiving data from component
src1 = sb.DataSource()
src2 = sb.DataSource()
src3 = sb.DataSource()
src4 = sb.DataSource()
sink = sb.DataSink()
print '\n********* Creating Components ***********'
mixer = sb.launch('Mixer', execparams={'DEBUG_LEVEL':5})

#Make connections
print '\n********* Creating Connections **********'
src1.connect(mixer,providesPortName='dataFloat_in_data1')
src2.connect(mixer,providesPortName='dataFloat_in_data2')
src3.connect(mixer,providesPortName='dataFloat_in_data3')
src4.connect(mixer,providesPortName='dataFloat_in_data4')
mixer.connect(sink)
print 'Connections Created'
  
print '\n*********** Generating Data *************'
#Generate data to be pushed
fs = 32000.0   # Sample Rate
fm1 = 250.0    # Signal 1 Frequency (Hz)  
fm2 = 125.0    # Signal 2 Frequency (Hz)
fm3 = 300.0    # Signal 3 Frequency (Hz)
fm4 = 200.0    # Signal 4 Frequency (Hz)

print 'Same size packets...'
#In this case all four packets will be of same size
# Generate a time signal
t = np.arange(0, 1, 1.0 / fs)  

# Generate the message signals for modulation
data1 = np.sin(2 * np.pi * fm1 * t)
data2 = np.sin(2 * np.pi * fm2 * t)
data3 = np.sin(2 * np.pi * fm3 * t)
data4 = np.sin(2 * np.pi * fm4 * t)
   
print 'Different size packets...'   
#In this case one packet will be of different size
# Generate a time signal
t = np.arange(0, 1, 1.0 / fs)  
t2 = np.arange(0, 2, 1.0 / fs)  #twice as long
# Generate the message signals for modulation
data5 = np.sin(2 * np.pi * fm1 * t)
data6 = np.sin(2 * np.pi * fm2 * t)
data7 = np.sin(2 * np.pi * fm3 * t)
data8 = np.sin(2 * np.pi * fm4 * t2)

#Start sandbox env
print '\n*********** Starting Sandbox ************'
sb.start()
print 'Components Started'
    
#Send data across the wave
print 'Pushing data for test 1...'
src1.push(data1.tolist())
src2.push(data2.tolist())
src3.push(data3.tolist())
src4.push(data4.tolist())
time.sleep(1)
output1 = sink.getData()

print 'Pushing data for test 2...'
src1.push(data5.tolist())
src2.push(data6.tolist())
src3.push(data7.tolist())
src4.push(data8.tolist())
time.sleep(1)
output2 = sink.getData()

#Stop sandbox env
print '\n*********** Stopping Sandbox ************'
sb.stop()
print 'Components stopped'
        
#Validate results
expected1 = (data1*data2*data3*data4).tolist()
expected2 = (data5*data6*data7*data8[0:len(data5)]).tolist()
assert len(output1) == len(expected1), 'Length of unit test 1 output does not equal length of expected result'
assert len(output2) == len(expected2), 'Length of unit test 2 output does not equal length of expected result'
sumError1 = sum([abs(x-y) for x,y in zip(expected1,output1)])
meanError1 = sumError1/len(output1)
#print 'meanError1 =',meanError1
sumError2 = sum([abs(x-y) for x,y in zip(expected2,output2)])
meanError2 = sumError2/len(output2)
#print 'meanError2 =',meanError2

passed_1 = True
if meanError1 > .001:
    passed_1 = False
passed_2 = True
if meanError2 > .001:
    passed_2 = False

print '\n************** Results ****************'            
if passed_1:
    print "Unit Test 1 .........................",u'\u2714'
else:
    print "Unit Test 1 .........................",u'\u2718'
if passed_2:
    print "Unit Test 2 .........................",u'\u2714'
else:
    print "Unit Test 2 .........................",u'\u2718'
    
print '\nTest Complete\n'