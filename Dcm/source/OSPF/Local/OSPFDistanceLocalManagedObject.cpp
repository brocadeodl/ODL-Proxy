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

#include "OSPF/Local/OSPFDistanceLocalManagedObject.h"
#include "OSPF/Local/OSPFRouterConfigLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFDistanceLocalManagedObject::OSPFDistanceLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFDistanceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
    }

    OSPFDistanceLocalManagedObject::OSPFDistanceLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const OSPFRouteType &distanceType,const UI32 &distanceValue)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFDistanceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_distanceType    (distanceType),
        m_distanceValue    (distanceValue)
    {
    }

    OSPFDistanceLocalManagedObject::~OSPFDistanceLocalManagedObject ()
    {
    }

    string  OSPFDistanceLocalManagedObject::getClassName()
    {
        return ("OSPFDistanceLocalManagedObject");
    }

    void  OSPFDistanceLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_distanceType,"distanceType", ospf_route_type));
        addPersistableAttribute (new AttributeUI32(&m_distanceValue,"distanceValue", ospf_dist_value));
    }

    void  OSPFDistanceLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_distanceType,"distanceType", ospf_route_type));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_distanceValue,"distanceValue", ospf_dist_value));
    }

    void OSPFDistanceLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back ("distanceType");
        setUserDefinedKeyCombination (keyName);
    }

    void  OSPFDistanceLocalManagedObject::setDistanceType(const OSPFRouteType &distanceType)
    {
        m_distanceType  =  distanceType;
    }

    OSPFRouteType OSPFDistanceLocalManagedObject::getDistanceType() const
    {
        return (m_distanceType);
    }

    void  OSPFDistanceLocalManagedObject::setDistanceValue(const UI32 &distanceValue)
    {
        m_distanceValue  =  distanceValue;
    }

    UI32  OSPFDistanceLocalManagedObject::getDistanceValue() const
    {
        return (m_distanceValue);
    }

	void OSPFDistanceLocalManagedObject::updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg)
	{
		setUserSpecifiedPartitionName(pMsg->getPartitionName());
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			case OSPF_SET_ELEM:
			{
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_DISTANCE_EXTERNAL:
						setDistanceType(OSPF_ROUTE_TYPE_EXTERNAL);
						setDistanceValue(pMsg->getDistanceExternal());
						//setVRF(pMsg->getVRF());
						break;
					case OSPF_ROUTER_CMD_DISTANCE_INTER_AREA:
						setDistanceType(OSPF_ROUTE_TYPE_INTER_AREA);
						setDistanceValue(pMsg->getDistanceInterArea());
						//setVRF(pMsg->getVRF());
						break;
					case OSPF_ROUTER_CMD_DISTANCE_INTRA_AREA:
						setDistanceType(OSPF_ROUTE_TYPE_INTRA_AREA);
						setDistanceValue(pMsg->getDistanceIntraArea());
						//setVRF(pMsg->getVRF());
						break;
				}
			}
				break;

			case OSPF_REMOVE:
			{
				switch (pMsg->getCmdCode())
				{
					case OSPF_ROUTER_CMD_DISTANCE_EXTERNAL:
					case OSPF_ROUTER_CMD_DISTANCE_INTER_AREA:
					case OSPF_ROUTER_CMD_DISTANCE_INTRA_AREA:
						setDistanceType(OSPF_ROUTE_TYPE_NONE);
						setDistanceValue(0);
						break;
				}
			}
		}
		return ;
	}

}
