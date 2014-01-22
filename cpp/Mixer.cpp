/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK.
 *
 * REDHAWK is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

 	Source: Mixer.spd.xml
 	Generated on: Wed Jul 31 09:23:40 EDT 2013
 	REDHAWK IDE
 	Version: 1.8.4
 	Build id: R201305151907

**************************************************************************/

#include "Mixer.h"
#include <algorithm>

PREPARE_LOGGING(Mixer_i)

Mixer_i::Mixer_i(const char *uuid, const char *label) : 
    Mixer_base(uuid, label)
{
	m_output = std::vector<float>();
	m_lastSize = -1;
}

Mixer_i::~Mixer_i()
{
}

int Mixer_i::serviceFunction()
{
	//Read in data from input ports
    bulkio::InFloatPort::dataTransfer *inputData1 = dataFloat_in_data1->getPacket(-1);
    bulkio::InFloatPort::dataTransfer *inputData2 = dataFloat_in_data2->getPacket(-1);
    bulkio::InFloatPort::dataTransfer *inputData3 = dataFloat_in_data3->getPacket(-1);
    bulkio::InFloatPort::dataTransfer *inputData4 = dataFloat_in_data4->getPacket(-1);

    //Check for valid data
    if (not inputData1 || not inputData2 || not inputData3 || not inputData4) {
    	if (inputData1) {
    		delete inputData1;
    	}

    	if (inputData2) {
    		delete inputData2;
    	}

    	if (inputData3) {
    		delete inputData3;
    	}

    	if (inputData4) {
    		delete inputData4;
    	}

    	return NOOP;
    }

    //Check for changes in SRI
    if (inputData1->sriChanged) {
    	dataFloat_out->pushSRI(inputData1->SRI);
    }

    if (inputData2->sriChanged) {
    	dataFloat_out->pushSRI(inputData2->SRI);
    }

    if (inputData3->sriChanged) {
    	dataFloat_out->pushSRI(inputData3->SRI);
    }

    if (inputData4->sriChanged) {
    	dataFloat_out->pushSRI(inputData4->SRI);
    }

    //Get size of smallest input to use as output size
    unsigned int currentSize = std::min(std::min(inputData1->dataBuffer.size(), inputData2->dataBuffer.size()),
    		std::min(inputData3->dataBuffer.size(), inputData4->dataBuffer.size()));

    //Check that the size isn't 0
    if (currentSize == 0) {
    	return NOOP;
    }

    //Resize output vector only when necessary
    if (m_lastSize != currentSize){
    	sizeOutput(currentSize);
    }

    //Multiply 4 inputs together
    for (unsigned int i=0; i < currentSize; ++i) {
    	m_output[i] = inputData1->dataBuffer[i] * inputData2->dataBuffer[i] * inputData3->dataBuffer[i] * inputData4->dataBuffer[i];
    }

    //push output data, set the previous size
    dataFloat_out->pushPacket(m_output, inputData4->T, inputData4->EOS, inputData4->streamID);
    m_lastSize = currentSize;

    //IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCKS
    delete inputData1;
    delete inputData2;
    delete inputData3;
    delete inputData4;

    return NORMAL;
}

void Mixer_i::sizeOutput(unsigned int currentSize)
{
	//Resize output vector if input size changes
	m_output.resize(currentSize);
}
