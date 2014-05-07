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

#include "Mixer_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

Mixer_base::Mixer_base(const char *uuid, const char *label) :
    Resource_impl(uuid, label),
    ThreadedComponent()
{
    loadProperties();

    dataFloat_in_data1 = new bulkio::InFloatPort("dataFloat_in_data1");
    addPort("dataFloat_in_data1", dataFloat_in_data1);
    dataFloat_in_data2 = new bulkio::InFloatPort("dataFloat_in_data2");
    addPort("dataFloat_in_data2", dataFloat_in_data2);
    dataFloat_in_data3 = new bulkio::InFloatPort("dataFloat_in_data3");
    addPort("dataFloat_in_data3", dataFloat_in_data3);
    dataFloat_in_data4 = new bulkio::InFloatPort("dataFloat_in_data4");
    addPort("dataFloat_in_data4", dataFloat_in_data4);
    dataFloat_out = new bulkio::OutFloatPort("dataFloat_out");
    addPort("dataFloat_out", dataFloat_out);
}

Mixer_base::~Mixer_base()
{
    delete dataFloat_in_data1;
    dataFloat_in_data1 = 0;
    delete dataFloat_in_data2;
    dataFloat_in_data2 = 0;
    delete dataFloat_in_data3;
    dataFloat_in_data3 = 0;
    delete dataFloat_in_data4;
    dataFloat_in_data4 = 0;
    delete dataFloat_out;
    dataFloat_out = 0;
}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void Mixer_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    Resource_impl::start();
    ThreadedComponent::startThread();
}

void Mixer_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    Resource_impl::stop();
    if (!ThreadedComponent::stopThread()) {
        throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
    }
}

void Mixer_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the component running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    Resource_impl::releaseObject();
}

void Mixer_base::loadProperties()
{
}


