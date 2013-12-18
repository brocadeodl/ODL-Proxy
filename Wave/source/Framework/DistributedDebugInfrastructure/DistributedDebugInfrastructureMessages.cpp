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
 *@file DistributedDebugInfrastructureMessages.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file has  the implementation  for the
 *              all the messages used by Debug Infrastructure
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */


#include "Framework/Utils/StringUtils.h"

#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureTypes.h" 
#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureMessages.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugGlobalObjectManager.h" 
#include "Framework/DistributedDebugInfrastructure/DistributedDebugLocalObjectManager.h" 

using namespace std;

namespace WaveNs
{

RunDebugScriptMessage::RunDebugScriptMessage ()
    :ManagementInterfaceMessage (DistributedDebugGlobalObjectManager::getClassName (),RUN_DEBUG_SCRIPT_MESSAGE),
    m_isNos             (true),
    m_scriptString      ("")
{

}

RunDebugScriptMessage::RunDebugScriptMessage (bool nosOrBash)
    :ManagementInterfaceMessage (DistributedDebugGlobalObjectManager::getClassName (),RUN_DEBUG_SCRIPT_MESSAGE),
    m_isNos             (nosOrBash),
    m_scriptString      ("")
{
}

RunDebugScriptMessage::~RunDebugScriptMessage ()
{
}



bool RunDebugScriptMessage::isScriptNos () const
{
   return m_isNos;
}


void   RunDebugScriptMessage::setScriptString   (const string & inputScriptString)
{
       m_scriptString = inputScriptString;
}

const string &  RunDebugScriptMessage::getScriptString   () const
{
   return m_scriptString;
}

void RunDebugScriptMessage::setOutputStringVector  (const vector<string> & outputStringVector)
{
    m_outputStringVector = outputStringVector;
}

const vector<string> & RunDebugScriptMessage::getOutputStringVector () const
{
    return (m_outputStringVector); 
}

void RunDebugScriptMessage::setLocationsToRunDebugScript (const vector<LocationId> & locationsToRunDebugScript)
{
    m_locationsToRunDebugScript = locationsToRunDebugScript;
}

const vector<LocationId> & RunDebugScriptMessage::getLocationsToRunDebugScript () const
{
    return (m_locationsToRunDebugScript);
}

void RunDebugScriptMessage::setupAttributesForSerialization ()
{

    ManagementInterfaceMessage::setupAttributesForSerialization ();


     addSerializableAttribute (new AttributeBool             (&m_isNos,                     "isNos"));
     addSerializableAttribute (new AttributeString           (&m_scriptString,              "scriptString"));
     addSerializableAttribute (new AttributeLocationIdVector (&m_locationsToRunDebugScript, "locationsToRunDebugScript"));
     addSerializableAttribute (new AttributeStringVector     (&m_outputStringVector,        "outputStringVector"));

}

RunDebugScriptOnClusterMemberMessage::RunDebugScriptOnClusterMemberMessage ()
    :PrismMessage(DistributedDebugLocalObjectManager::getPrismServiceId (),RUN_DEBUG_SCRIPT_ON_CLUSTER_MEMBER_MESSAGE),
    m_isNos             (true),
    m_scriptString      (""),
    m_outputString      ("")
{
}

RunDebugScriptOnClusterMemberMessage::RunDebugScriptOnClusterMemberMessage (bool nosOrBash)
    :PrismMessage(DistributedDebugLocalObjectManager::getPrismServiceId (),RUN_DEBUG_SCRIPT_ON_CLUSTER_MEMBER_MESSAGE), 
    m_isNos             (nosOrBash),
    m_scriptString      (""),
    m_outputString      ("")
{
}

RunDebugScriptOnClusterMemberMessage::~RunDebugScriptOnClusterMemberMessage ()
{
}

bool      RunDebugScriptOnClusterMemberMessage::isScriptNos () const
{
    return m_isNos;
}

void   RunDebugScriptOnClusterMemberMessage::setScriptString   (const string & inputScriptString)
{
       m_scriptString = inputScriptString;
}

const string &  RunDebugScriptOnClusterMemberMessage::getScriptString   () const
{
   return m_scriptString;
}

void  RunDebugScriptOnClusterMemberMessage::setOutputString   (const string & outputString)
{
      m_outputString = outputString;

}

const  string &  RunDebugScriptOnClusterMemberMessage::getOutputString   () const
{
       return m_outputString; 
}

void RunDebugScriptOnClusterMemberMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
     addSerializableAttribute (new AttributeBool       (&m_isNos,           "isNos"));
     addSerializableAttribute (new AttributeString     (&m_scriptString,    "scriptString"));
     addSerializableAttribute (new AttributeString     (&m_outputString,    "outputString"));

}

}
