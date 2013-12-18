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

/***************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Venkat                                                       *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/ConnectorConfigLocalMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

ConnectorConfigLocalMessage::ConnectorConfigLocalMessage ()
  : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), CONNECTOR_CONFIG_LOCAL_MESSAGE),
    m_cmdCode (0),
    m_name ("")
{
}
    
void ConnectorConfigLocalMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32 (&m_cmdCode, "connectorConfigCmdCode"));
    addSerializableAttribute (new AttributeString (&m_name, "connectorName"));
    addSerializableAttribute (new AttributeStringVector (&m_bulkConfigs, "bulkConfigVectorForUpdate"));
}

ConnectorConfigLocalMessage::~ConnectorConfigLocalMessage ()
{
}

UI32 ConnectorConfigLocalMessage::getCmdCode ()
{
	return m_cmdCode;
}

void ConnectorConfigLocalMessage::setCmdCode (const  UI32 &cmdCode)
{
	m_cmdCode = cmdCode;
}

string ConnectorConfigLocalMessage::getName ()
{
	return m_name;
}

void ConnectorConfigLocalMessage::setName (const  string &name)
{
	m_name = name;
}

void ConnectorConfigLocalMessage::setBulkConfigsVector (const vector<string> &configs)
{
    m_bulkConfigs = configs;
}

vector<string> ConnectorConfigLocalMessage::getBulkConfigsVector () const
{
    return m_bulkConfigs;
}

void ConnectorConfigLocalMessage::addConfigToBulkConfigsvector (const string &config)
{
    m_bulkConfigs.push_back (config);
}

}
