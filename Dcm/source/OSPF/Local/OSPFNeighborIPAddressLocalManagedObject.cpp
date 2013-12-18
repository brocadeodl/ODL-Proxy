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

#include "OSPF/Local/OSPFNeighborIPAddressLocalManagedObject.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFNeighborIPAddressLocalManagedObject::OSPFNeighborIPAddressLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFNeighborIPAddressLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
    }

    OSPFNeighborIPAddressLocalManagedObject::OSPFNeighborIPAddressLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &neighborAddress)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFNeighborIPAddressLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_neighborAddress    (neighborAddress)
    {
    }

    OSPFNeighborIPAddressLocalManagedObject::~OSPFNeighborIPAddressLocalManagedObject ()
    {
    }

    string  OSPFNeighborIPAddressLocalManagedObject::getClassName()
    {
        return ("OSPFNeighborIPAddressLocalManagedObject");
    }

    void  OSPFNeighborIPAddressLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_neighborAddress,"neighborAddress", ospf_neighbor_addr));
    }

    void  OSPFNeighborIPAddressLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_neighborAddress,"neighborAddress", ospf_neighbor_addr));
		OSPFNeighborIPAddressLocalManagedObject::setupKeys();
    }

    void OSPFNeighborIPAddressLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back ("neighborAddress");
        setUserDefinedKeyCombination (keyName);
    }

    void  OSPFNeighborIPAddressLocalManagedObject::setNeighborAddress(const IpV4Address &neighborAddress)
    {
        m_neighborAddress  =  neighborAddress;
    }

    IpV4Address  OSPFNeighborIPAddressLocalManagedObject::getNeighborAddress() const
    {
        return (m_neighborAddress);
    }

	void OSPFNeighborIPAddressLocalManagedObject::updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg)
	{
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			case OSPF_SET_ELEM:
			{
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_NEIGHBOR_ADDRESS:
						setNeighborAddress(pMsg->getNeighborAddress());
				}
			}
				break;

			case OSPF_REMOVE:
			{
				IpV4Address addr;
				addr.fromString("0.0.0.0");
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_NEIGHBOR_ADDRESS:
						setNeighborAddress(addr);
				}
			}
		}
		return ;
	}

}
