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

#include "OSPF/Local/OSPFPermitDenyRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFPermitDenyRedistributeLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "ClientInterface/OSPF/OSPFPermitDenyRedistributeMessage.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFPermitDenyRedistributeLocalManagedObject::OSPFPermitDenyRedistributeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFPermitDenyRedistributeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
		m_index = 0;
		m_permitDenyOption = OSPF_PERMIT_DENY_OPTION_NONE;
		m_routeOption = OSPF_ROUTE_OPTION_NONE;
		//m_subNet.fromString("0.0.0.0");
		//m_mask.fromString("0.0.0.0");
		m_matchMetric = 0;
		m_setMetric = 0;
		m_crDummy = false;
    }

    OSPFPermitDenyRedistributeLocalManagedObject::OSPFPermitDenyRedistributeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const UI32 &index,const OSPFPermitDenyOption &permitDenyOption,const OSPFRouteOption &routeOption,const IpV4Address &subNet,const IpV4Address &mask,const UI32 &matchMetric, const UI32 &setMetric, const bool &crDummy)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFPermitDenyRedistributeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_index    (index),
        m_permitDenyOption    (permitDenyOption),
        m_routeOption (routeOption),
        m_subNet    (subNet),
        m_mask    (mask),
        m_matchMetric    (matchMetric),
		m_setMetric (setMetric),
        m_crDummy (crDummy)
    {
    }

    OSPFPermitDenyRedistributeLocalManagedObject::~OSPFPermitDenyRedistributeLocalManagedObject ()
    {
    }

    string  OSPFPermitDenyRedistributeLocalManagedObject::getClassName()
    {
        return ("OSPFPermitDenyRedistributeLocalManagedObject");
    }

    void  OSPFPermitDenyRedistributeLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_index,"index", ospf_redist_value));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_permitDenyOption,"permitDenyOption"));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_routeOption,0,"routeOption", ospf_route_option));
        IpV4Address routerID(string("0.0.0.0"));
        addPersistableAttribute (new AttributeIpV4AddressUC(&m_subNet,false,true,routerID,"subNet",ospf_address));
        addPersistableAttribute (new AttributeIpV4AddressUC(&m_mask,false,true,routerID,"mask",ospf_mask));
        addPersistableAttribute (new AttributeUI32(&m_matchMetric,0,"matchMetric", ospf_match_metric_val));
        addPersistableAttribute (new AttributeUI32(&m_setMetric,0,"setMetric", ospf_set_metric_val));
        addPersistableAttribute (new AttributeBool(&m_crDummy,"crDummy"/*, ospf_dummy4*/));
    }

    void  OSPFPermitDenyRedistributeLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_index,"index", ospf_redist_value));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_permitDenyOption,"permitDenyOption"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_routeOption,0,"routeOption", ospf_route_option));
        IpV4Address routerID(string("0.0.0.0"));
		addPersistableAttributeForCreate (new AttributeIpV4AddressUC(&m_subNet,false,true,routerID,"subNet",ospf_address));
		addPersistableAttributeForCreate (new AttributeIpV4AddressUC(&m_mask,false,true,routerID,"mask",ospf_mask));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_matchMetric,0,"matchMetric", ospf_match_metric_val));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_setMetric,0,"setMetric", ospf_set_metric_val));
        addPersistableAttributeForCreate  (new AttributeBool(&m_crDummy,"crDummy"/*, ospf_dummy4*/));
    }

    void OSPFPermitDenyRedistributeLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
		keyName.push_back ("index");
		keyName.push_back ("routeOption");
		setUserDefinedKeyCombination (keyName);
    }
    
    void  OSPFPermitDenyRedistributeLocalManagedObject::setIndex(const UI32 &index)
    {
        m_index  =  index;
    }

    UI32  OSPFPermitDenyRedistributeLocalManagedObject::getIndex() const
    {
        return (m_index);
    }

    void OSPFPermitDenyRedistributeLocalManagedObject::setPermitDenyOption(const OSPFPermitDenyOption &permitDenyOption)
    {
        m_permitDenyOption = permitDenyOption;
    }

    OSPFPermitDenyOption OSPFPermitDenyRedistributeLocalManagedObject::getPermitDenyOption()  const
    {
        return (m_permitDenyOption);
    }


    void OSPFPermitDenyRedistributeLocalManagedObject::setRouteOption(const OSPFRouteOption &routeOption)
    {
        m_routeOption = routeOption;
    }

    OSPFRouteOption OSPFPermitDenyRedistributeLocalManagedObject::getRouteOption() const
    {
        return (m_routeOption);
    }

    void  OSPFPermitDenyRedistributeLocalManagedObject::setSubNet(const IpV4Address &subNet, const bool &set)
    {
        //m_subNet  =  subNet;
        m_subNet.setIpV4AddressValue(subNet.toString());
        m_subNet.setIsUserConfigured(set);
    }

    IpV4Address  OSPFPermitDenyRedistributeLocalManagedObject::getSubNet() const
    {
        return (IpV4Address(m_subNet.getIpV4AddressValue()));
    }

    void  OSPFPermitDenyRedistributeLocalManagedObject::setMask(const IpV4Address &mask, const bool &set)
    {
        //m_mask  =  mask;
        m_mask.setIpV4AddressValue(mask.toString());
        m_mask.setIsUserConfigured(set);
    }

    IpV4Address  OSPFPermitDenyRedistributeLocalManagedObject::getMask() const
    {
        return (IpV4Address(m_mask.getIpV4AddressValue()));
    }

    void  OSPFPermitDenyRedistributeLocalManagedObject::setMatchMetric(const UI32 &matchMetric)
    {
        m_matchMetric  =  matchMetric;
    }

    UI32  OSPFPermitDenyRedistributeLocalManagedObject::getMatchMetric() const
    {
        return (m_matchMetric);
    }

	void OSPFPermitDenyRedistributeLocalManagedObject::setSetMetric(const UI32 &setMetric)
	{
		m_setMetric = setMetric;
	}

	UI32 OSPFPermitDenyRedistributeLocalManagedObject::getSetMetric() const
	{
		return (m_setMetric);
	}

    void OSPFPermitDenyRedistributeLocalManagedObject::setCrDummy(const bool &crDummy)
    {
        m_crDummy = crDummy;
    }

    bool OSPFPermitDenyRedistributeLocalManagedObject::getCrDummy() const
    {
        return(m_crDummy);
    }

    void OSPFPermitDenyRedistributeLocalManagedObject::updateFromMessage(OSPFPermitDenyRedistributeLocalMessage *pMsg)
    {
		switch (pMsg->getOpCode())
		{
			case OSPF_CREATE:
			{
				m_index = pMsg->m_index;
				m_permitDenyOption = pMsg->m_permitDenyOption;
				m_routeOption = pMsg->m_routeOption;

				switch (pMsg->getCmdCode()) {
					case OSPF_PERMIT_DENY_CMD_ROUTE_OPTION:
						break;
					case OSPF_PERMIT_DENY_CMD_ADDRESS:
                        setSubNet(pMsg->m_subNet, true);
                        setMask(pMsg->m_mask, true);
						if (pMsg->m_matchMetric) {
							m_matchMetric = pMsg->m_matchMetric;
						}

						if (pMsg->m_setMetric) {
							m_setMetric = pMsg->m_setMetric;
						}
						break;
					case OSPF_PERMIT_DENY_CMD_METRIC:
						if (pMsg->m_matchMetric) {
							m_matchMetric = pMsg->m_matchMetric;
						}

						if (pMsg->m_setMetric) {
							m_setMetric = pMsg->m_setMetric;
						}
                        setSubNet(IpV4Address("0.0.0.0"), false);
                        setMask(IpV4Address("0.0.0.0"), true);
						break;
					default:
						break;
				}
			}
				break;
			case OSPF_SET_ELEM:
			{
				prismAssert(m_index == pMsg->m_index, __FILE__, __LINE__);
				prismAssert(m_permitDenyOption == pMsg->m_permitDenyOption, __FILE__, __LINE__);
				switch (pMsg->getCmdCode()) {
					case OSPF_PERMIT_DENY_CMD_ROUTE_OPTION:
						break;
					case OSPF_PERMIT_DENY_CMD_ADDRESS:
                        setSubNet(pMsg->m_subNet, true);
                        setMask(pMsg->m_mask, true);
						if (pMsg->m_matchMetric) {
							m_matchMetric = pMsg->m_matchMetric;
						}
						if (pMsg->m_setMetric) {
							m_setMetric = pMsg->m_setMetric;
						}
						break;
					case OSPF_PERMIT_DENY_CMD_METRIC:
						if (pMsg->m_matchMetric) {
							m_matchMetric = pMsg->m_matchMetric;
						}

						if (pMsg->m_setMetric) {
							m_setMetric = pMsg->m_setMetric;
						}
						break;
					default:
						break;
				}
			}
				break;
			case OSPF_REMOVE:
			{
				prismAssert(m_index == pMsg->m_index, __FILE__, __LINE__);
				prismAssert(m_permitDenyOption == pMsg->m_permitDenyOption, __FILE__, __LINE__);
				switch (pMsg->getCmdCode()) {
					case OSPF_PERMIT_DENY_CMD_ROUTE_OPTION:
                        setSubNet(IpV4Address("0.0.0.0"), false);
                        setSubNet(IpV4Address("0.0.0.0"), false);
						m_matchMetric = 0;
						m_setMetric = 0;
						break;
					case OSPF_PERMIT_DENY_CMD_ADDRESS:
                        setSubNet(IpV4Address("0.0.0.0"), false);
                        setSubNet(IpV4Address("0.0.0.0"), false);
						break;
					case OSPF_PERMIT_DENY_CMD_METRIC:
						m_matchMetric = 0;
						m_setMetric = 0;
						break;
					default:
						break;
				}
			}
				break;
		}
    }

    void OSPFPermitDenyRedistributeLocalManagedObject::updateClientMessageForReplay(OSPFPermitDenyRedistributeMessage *pMsg)
	{
		pMsg->setOpCode(OSPF_CREATE);
		pMsg->setIndex(m_index);
		pMsg->setPermitDenyOption(m_permitDenyOption);
		pMsg->setRouteOption(m_routeOption);

		if (m_subNet.getIsUserConfigured()) {
			pMsg->setCmdCode(OSPF_PERMIT_DENY_CMD_ADDRESS);
			pMsg->setSubNet(m_subNet.getIpV4AddressValue());
			pMsg->setMask(m_mask.getIpV4AddressValue());
			if (m_matchMetric) {
				pMsg->setMatchMetric(m_matchMetric);
			}
			if (m_setMetric) {
				pMsg->setSetMetric(m_setMetric);
			}
		} else if (m_matchMetric || m_setMetric) {
			pMsg->setCmdCode(OSPF_PERMIT_DENY_CMD_METRIC);
			pMsg->setMatchMetric(m_matchMetric);
			pMsg->setSetMetric(m_setMetric);
		} else {
			pMsg->setCmdCode(OSPF_PERMIT_DENY_CMD_ROUTE_OPTION);
		}
	}
}
