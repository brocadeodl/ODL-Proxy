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

#include "OSPF/Local/OSPFAreaRangeLocalManagedObject.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "ClientInterface/OSPF/OSPFAreaRangeMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFAreaRangeLocalManagedObject::OSPFAreaRangeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFAreaRangeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
    }

    OSPFAreaRangeLocalManagedObject::OSPFAreaRangeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &rangeAddress,const IpV4Address &rangeMask,const OSPFAreaRangeEffect &rangeAdvertise, const UI32 &rangeCost)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFAreaRangeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_rangeAddress    (rangeAddress),
        m_rangeMask    (rangeMask),
        m_rangeAdvertise    (rangeAdvertise),
        m_rangeCost    (rangeCost)
    {
    }

    OSPFAreaRangeLocalManagedObject::~OSPFAreaRangeLocalManagedObject ()
    {
    }

    string  OSPFAreaRangeLocalManagedObject::getClassName()
    {
        return ("OSPFAreaRangeLocalManagedObject");
    }

    void  OSPFAreaRangeLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_rangeAddress,"rangeAddress",ospf_range_address));
        addPersistableAttribute (new AttributeIpV4Address(&m_rangeMask,"rangeMask", ospf_range_mask));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_rangeAdvertise,"rangeAdvertise", ospf_range_effect));
        addPersistableAttribute (new AttributeUI32((UI32 *)&m_rangeCost,"rangeCost", ospf_range_cost));
    }

    void  OSPFAreaRangeLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_rangeAddress,"rangeAddress",ospf_range_address));
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_rangeMask,"rangeMask", ospf_range_mask));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_rangeAdvertise,"rangeAdvertise", ospf_range_effect));
        addPersistableAttributeForCreate  (new AttributeUI32((UI32 *)&m_rangeCost,"rangeCost", ospf_range_cost));

		OSPFAreaRangeLocalManagedObject::setupKeys();
    }

    void OSPFAreaRangeLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("rangeAddress");
        keyName.push_back("rangeMask");
        setUserDefinedKeyCombination (keyName);
    }

    void  OSPFAreaRangeLocalManagedObject::setRangeAddress(const IpV4Address &rangeAddress)
    {
        m_rangeAddress  =  rangeAddress;
    }

    IpV4Address  OSPFAreaRangeLocalManagedObject::getRangeAddress() const
    {
        return (m_rangeAddress);
    }

    void  OSPFAreaRangeLocalManagedObject::setRangeMask(const IpV4Address &rangeMask)
    {
        m_rangeMask  =  rangeMask;
    }

    IpV4Address  OSPFAreaRangeLocalManagedObject::getRangeMask() const
    {
        return (m_rangeMask);
    }

    void  OSPFAreaRangeLocalManagedObject::setRangeAdvertise(const OSPFAreaRangeEffect &rangeAdvertise)
    {
        m_rangeAdvertise  =  rangeAdvertise;
    }

    OSPFAreaRangeEffect  OSPFAreaRangeLocalManagedObject::getRangeAdvertise() const
    {
        return (m_rangeAdvertise);
    }

    void  OSPFAreaRangeLocalManagedObject::setRangeCost(const UI32 &rangeCost)
    {
        m_rangeCost  =  rangeCost;
    }

    UI32  OSPFAreaRangeLocalManagedObject::getRangeCost() const
    {
        return (m_rangeCost);
    }

	void OSPFAreaRangeLocalManagedObject::updateClientMessageForReplay(OSPFAreaRangeMessage *pMsg)
	{
		pMsg->setOpCode(OSPF_CREATE);
		pMsg->setCmdCode(OSPF_AREA_CONFIG_RANGE);
		pMsg->setIpAddr(m_rangeAddress);
		pMsg->setIpMask(m_rangeMask);
		pMsg->setRangeEffect(m_rangeAdvertise);
		pMsg->setRangeCost(m_rangeCost);
	}
}
