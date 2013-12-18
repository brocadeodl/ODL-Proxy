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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                     *
 **************************************************************************/

#include "OSPF/Local/OSPFSummaryAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFSummaryAddressLocalManagedObject::OSPFSummaryAddressLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFSummaryAddressLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
    }

    OSPFSummaryAddressLocalManagedObject::OSPFSummaryAddressLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &summaryAddress,const IpV4Address &summaryMask)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFSummaryAddressLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_summaryAddress    (summaryAddress),
        m_summaryMask    (summaryMask)
    {
    }

    OSPFSummaryAddressLocalManagedObject::~OSPFSummaryAddressLocalManagedObject ()
    {
    }

    string  OSPFSummaryAddressLocalManagedObject::getClassName()
    {
        return ("OSPFSummaryAddressLocalManagedObject");
    }

    void  OSPFSummaryAddressLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_summaryAddress,"summaryAddress", ospf_sum_address));
        addPersistableAttribute (new AttributeIpV4Address(&m_summaryMask,"summaryMask", ospf_sum_address_mask));
    }

    void  OSPFSummaryAddressLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_summaryAddress,"summaryAddress", ospf_sum_address));
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_summaryMask,"summaryMask", ospf_sum_address_mask));
    }

    void OSPFSummaryAddressLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
		keyName.push_back ("summaryAddress");
		keyName.push_back ("summaryMask");
		setUserDefinedKeyCombination (keyName);
    }

    void  OSPFSummaryAddressLocalManagedObject::setSummaryAddress(const IpV4Address &summaryAddress)
    {
        m_summaryAddress  =  summaryAddress;
    }

    IpV4Address  OSPFSummaryAddressLocalManagedObject::getSummaryAddress() const
    {
        return (m_summaryAddress);
    }

    void  OSPFSummaryAddressLocalManagedObject::setSummaryMask(const IpV4Address &summaryMask)
    {
        m_summaryMask  =  summaryMask;
    }

    IpV4Address  OSPFSummaryAddressLocalManagedObject::getSummaryMask() const
    {
        return (m_summaryMask);
    }

	void OSPFSummaryAddressLocalManagedObject::updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg)
	{
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			case OSPF_SET_ELEM:
			{
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_SUMMARY_ADDRESS:
						setSummaryAddress(pMsg->getSummaryAddress());
						setSummaryMask(pMsg->getSummaryMask());
				}
			}
				break;

			case OSPF_REMOVE:
			{
				IpV4Address addr;
				addr.fromString("0.0.0.0");
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_SUMMARY_ADDRESS:
						setSummaryAddress(addr);
						setSummaryMask(addr);
				}
			}
		}
		return ;
	}

}
