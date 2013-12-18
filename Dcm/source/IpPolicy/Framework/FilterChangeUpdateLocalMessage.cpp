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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/Framework/FilterChangeUpdateLocalMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    FilterChangeUpdateLocalMessage::FilterChangeUpdateLocalMessage ()
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYFILTERCHANGEUPDATEDELAY)
    {
        m_filterUpdateDelayFlag = false;
    }

    FilterChangeUpdateLocalMessage::FilterChangeUpdateLocalMessage (const UI32 &filterUpdateDelay)
        : PrismMessage (IpPolicyLocalObjectManager::getPrismServiceId (), IPPOLICYFILTERCHANGEUPDATEDELAY),
          m_filterUpdateDelay    (filterUpdateDelay)
    {
    	m_filterUpdateDelayFlag = true;
    }

    FilterChangeUpdateLocalMessage::~FilterChangeUpdateLocalMessage ()
    {
    }

    void  FilterChangeUpdateLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeEnum((UI32*)&m_op_code,"op_code"));
        addSerializableAttribute (new AttributeUI32(&m_filterUpdateDelay,"filterUpdateDelay"));
        addSerializableAttribute (new AttributeBool(&m_filterUpdateDelayFlag,"filterUpdateDelayFlag"));
        addSerializableAttribute (new AttributeString(&m_backend_err,"backend_err"));
    }

	void FilterChangeUpdateLocalMessage::setFilterUpdateDelay(const UI32 &filterUpdateDelay) {
		m_filterUpdateDelay = filterUpdateDelay;
	}

	UI32 FilterChangeUpdateLocalMessage::getFilterUpdateDelay()  const {
		return m_filterUpdateDelay;
	}

	bool FilterChangeUpdateLocalMessage::getFilterUpdateDelayFlag()  const {
		return m_filterUpdateDelayFlag;
	}

}
