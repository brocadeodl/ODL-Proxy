/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 *@file DistributedDebugParameters.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file has  the implementation  for the
 *              all the messages used by Debug Infrastructure
 *              
 * 
 * Author :     Brian Adaniya
 * Date :       07/26/2011 
 */


#include "Framework/DistributedDebugInfrastructure/DistributedDebugParameters.h"

using namespace std;

namespace WaveNs
{

DistributedDebugParameters::DistributedDebugParameters ()
    : m_inputStringFromScript   (""),
    m_nosOrBash                 (""),
    m_commaDelimitedLocationIds ("")
{
}

DistributedDebugParameters::DistributedDebugParameters (const string & inputStringFromScript, const string & nosOrBash, const string & commaDelimitedLocations)
    : m_inputStringFromScript   (inputStringFromScript),
    m_nosOrBash                 (nosOrBash),
    m_commaDelimitedLocationIds (commaDelimitedLocations)
{
}

DistributedDebugParameters::~DistributedDebugParameters ()
{
}


void DistributedDebugParameters::setInputStringFromScript (const string & inputStringFromScript)
{
    m_inputStringFromScript = inputStringFromScript;
}

const string & DistributedDebugParameters::getInputStringFromScript () const
{
    return (m_inputStringFromScript);
}

void DistributedDebugParameters::setNosOrBash (const string & nosOrBash)
{
    m_nosOrBash = nosOrBash;
}

const string & DistributedDebugParameters::getNosOrBash () const
{
    return (m_nosOrBash);
}

void DistributedDebugParameters::setCommaDelimitedLocationIds (const string & commaDelimitedLocationIds)
{
    m_commaDelimitedLocationIds = commaDelimitedLocationIds;
}

const string & DistributedDebugParameters::getCommaDelimitedLocationIds () const
{
    return (m_commaDelimitedLocationIds);
}

void DistributedDebugParameters::setOutputStringVector (const vector<string> & outputStringVector)
{
    m_outputStringVector = outputStringVector;
}

const vector<string> & DistributedDebugParameters::getOutputStringVector () const
{
    return (m_outputStringVector);
}

void DistributedDebugParameters::setLocationsToRunDebugScript (const vector<LocationId> & locationsToRunDebugScript)
{
    m_locationsToRunDebugScript = locationsToRunDebugScript;
}

const vector<LocationId> & DistributedDebugParameters::getLocationsToRunDebugScript () const
{
    return (m_locationsToRunDebugScript);
}

}
