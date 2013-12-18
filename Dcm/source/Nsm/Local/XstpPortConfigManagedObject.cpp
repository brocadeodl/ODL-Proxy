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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoStpConfigMessage.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/NsmUpdatePhyStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"

#include "vcs.h"
#include "Modeling/XPath/WaveXPathExpressionElement.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"

namespace DcmNs
{

    bool XstpPortConfigManagedObject::DEFAULT_SHUT_STATE = false;


    XstpPortConfigManagedObject::XstpPortConfigManagedObject (WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (XstpPortConfigManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
	    setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
	    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
	    setCost(MSTP_DCM_INVALID_COST);
	    setRestrictedRole(false);
	    setRestrictedTcn(false);
	    setAutoEdge(false);
	    setPortFast(false);
	    setEdgePort(false);
	    setBpduGuardEnable(false);
	    setBpduGuard(false);
	    setBpduFilter(false);
	    setGuard(false);
	    setLinkShared(MSTPPORTLINKTYPEP2P);
	    m_stpDisable = DEFAULT_SHUT_STATE;
	    setBpduMac(MSTPPORTPVSTBPDUMACBRCD);
        setTaggedBpduEnable(false);
        setPortUid(0);
        setBpduDropDirection(BPDU_DROP_DISABLED);
    }

    XstpPortConfigManagedObject::~XstpPortConfigManagedObject ()
    {
    }

    string  XstpPortConfigManagedObject::getClassName()
    {
        return ("XstpPortConfigManagedObject");
    }

    void  XstpPortConfigManagedObject::setupAttributesForPersistence()
    {
    	const	UI32 defaultPortxSTPCost = MSTP_DCM_INVALID_COST; /* 0*/
    	const	UI32 defaultPortMSTPHelloTime = MSTP_DCM_DEFAULT_HELLO_TIME;
    	const	UI32 defaultPortxSTPPrio = MSTP_DCM_DEFAULT_PORT_PRIORITY;
    	const 	MstpPortLinkTypeEnum defaultPortlinkType = MSTPPORTLINKTYPEP2P;
    	const 	MstpPortPvstBpDuMac defaultPortPVSTmac = MSTPPORTPVSTBPDUMACBRCD;
    	const	UI8 defaultBpduDropDirection	= BPDU_DROP_DISABLED;



	    DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_port,"port"));
        addPersistableAttribute (new AttributeUI32(&m_priority,defaultPortxSTPPrio,"priority",xstp_priority));
        addPersistableAttribute (new AttributeUI32(&m_helloTime,defaultPortMSTPHelloTime,"helloTime",xstp_hello_time));
        addPersistableAttribute (new AttributeUI32(&m_cost,defaultPortxSTPCost,"cost",xstp_cost));
        addPersistableAttribute (new AttributeBool(&m_restrictedRole,"restrictedRole",xstp_restricted_role));
        addPersistableAttribute (new AttributeBool(&m_restrictedTcn,"restrictedTcn",xstp_restricted_tcn));
        addPersistableAttribute (new AttributeBool(&m_autoEdge,"autoEdge",xstp_autoedge));
        addPersistableAttribute (new AttributeBool(&m_portFast,"portFast",xstp_portfastbasic));
        addPersistableAttribute (new AttributeBool(&m_edgePort,"edgePort",xstp_edgeportbasic));
        addPersistableAttribute (new AttributeBool(&m_bpduGuardEnable,"bpduGuardEnable",xstp_enable));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_bpduDropDirection),defaultBpduDropDirection,"bpduDropDirection",xstp_direction));
        addPersistableAttribute (new AttributeBool(&m_bpduGuard,"bpduGuard",xstp_bpdu_guard));
        addPersistableAttribute (new AttributeBool(&m_bpduFilter,"bpduFilter",xstp_bpdu_filter));
        addPersistableAttribute (new AttributeBool((&m_guard),"guard",xstp_root));
        //addPersistableAttribute (new AttributeEnum((UI32 *)(&m_guard),"guard",xstp_root));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_linkShared),defaultPortlinkType,"linkShared",xstp_link_type));
        addPersistableAttribute (new AttributeBool(&m_stpDisable,"stpDisable",xstp_shutdown));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_mac),defaultPortPVSTmac,"bpduMac",xstp_bpdu_mac));
        addPersistableAttribute (new AttributeBool(&m_taggedBpduEnable, "taggedBpduEnable", xstp_tagged_ieee_bpdu));
        addPersistableAttribute (new AttributeUI32(&m_portUid, "portUid"));
    }

    void  XstpPortConfigManagedObject::setupAttributesForCreate()
    {
	    const	UI32 defaultPortxSTPCost = MSTP_DCM_INVALID_COST; /* 0*/
	    const	UI32 defaultPortMSTPHelloTime = MSTP_DCM_DEFAULT_HELLO_TIME;
	    const	UI32 defaultPortxSTPPrio = MSTP_DCM_DEFAULT_PORT_PRIORITY;
	    const 	MstpPortLinkTypeEnum defaultPortlinkType = MSTPPORTLINKTYPEP2P;
	    const 	MstpPortPvstBpDuMac defaultPortPVSTmac = MSTPPORTPVSTBPDUMACBRCD;
    	const	UI8 defaultBpduDropDirection	= BPDU_DROP_DISABLED;

        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeString(&m_port,"port"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_priority,defaultPortxSTPPrio,"priority",xstp_priority));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_helloTime,defaultPortMSTPHelloTime,"helloTime",xstp_hello_time));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_cost,defaultPortxSTPCost,"cost",xstp_cost));
        addPersistableAttributeForCreate  (new AttributeBool(&m_restrictedRole,"restrictedRole",xstp_restricted_role));
        addPersistableAttributeForCreate  (new AttributeBool(&m_restrictedTcn,"restrictedTcn",xstp_restricted_tcn));
        addPersistableAttributeForCreate  (new AttributeBool(&m_autoEdge,"autoEdge",xstp_autoedge));
        addPersistableAttributeForCreate (new AttributeBool(&m_portFast,"portFast",xstp_portfastbasic));
        addPersistableAttributeForCreate (new AttributeBool(&m_edgePort,"edgePort",xstp_edgeportbasic));
        addPersistableAttributeForCreate  (new AttributeBool(&m_bpduGuardEnable,"bpduGuardEnable",xstp_enable));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)(&m_bpduDropDirection),defaultBpduDropDirection,"bpduDropDirection",xstp_direction));
        addPersistableAttributeForCreate  (new AttributeBool(&m_bpduGuard,"bpduGuard",xstp_bpdu_guard));
        addPersistableAttributeForCreate  (new AttributeBool(&m_bpduFilter,"bpduFilter",xstp_bpdu_filter));
        addPersistableAttributeForCreate  (new AttributeBool((&m_guard),"guard",xstp_root));
        //addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_guard),"guard",xstp_root));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_linkShared),defaultPortlinkType,"linkShared",xstp_link_type));
        addPersistableAttributeForCreate  (new AttributeBool(&m_stpDisable,"stpDisable",xstp_shutdown));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_mac),defaultPortPVSTmac,"bpduMac",xstp_bpdu_mac));
        addPersistableAttributeForCreate  (new AttributeBool(&m_taggedBpduEnable, "taggedBpduEnable", xstp_tagged_ieee_bpdu));
        addPersistableAttributeForCreate (new AttributeUI32(&m_portUid, "portUid"));
    }

    Attribute *XstpPortConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
            Attribute *pAttribute = new AttributeBool (true, "mstp");
            return (pAttribute);
    }

    void  XstpPortConfigManagedObject::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  XstpPortConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  XstpPortConfigManagedObject::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  XstpPortConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  XstpPortConfigManagedObject::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  XstpPortConfigManagedObject::getCost() const
    {
        return (m_cost);
    }

    void  XstpPortConfigManagedObject::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  XstpPortConfigManagedObject::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

    void  XstpPortConfigManagedObject::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    bool  XstpPortConfigManagedObject::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  XstpPortConfigManagedObject::setAutoEdge(const bool &autoEdge)
    {
        m_autoEdge  =  autoEdge;
    }

    bool  XstpPortConfigManagedObject::getAutoEdge() const
    {
        return (m_autoEdge);
    }

    void  XstpPortConfigManagedObject::setPortFast(const bool &portFast)
    {
        m_portFast  =  portFast;
    }

    bool  XstpPortConfigManagedObject::getPortFast() const
    {
        return (m_portFast);
    }

    void  XstpPortConfigManagedObject::setEdgePort(const bool &edgePort)
    {
        m_edgePort  =  edgePort;
    }

    bool  XstpPortConfigManagedObject::getEdgePort() const
    {
        return (m_edgePort);
    }

    void  XstpPortConfigManagedObject::setBpduGuardEnable(const bool &bpduGuardEnable)
    {
        m_bpduGuardEnable  =  bpduGuardEnable;
    }

    bool  XstpPortConfigManagedObject::getBpduGuardEnable() const
    {
        return (m_bpduGuardEnable);
    }

    void  XstpPortConfigManagedObject::setBpduGuard(const bool &bpduGuard)
    {
        m_bpduGuard  =  bpduGuard;
    }

    bool  XstpPortConfigManagedObject::getBpduGuard() const
    {
        return (m_bpduGuard);
    }

    void  XstpPortConfigManagedObject::setBpduFilter(const bool &bpduFilter)
    {
        m_bpduFilter  =  bpduFilter;
    }

    bool  XstpPortConfigManagedObject::getBpduFilter() const
    {
        return (m_bpduFilter);
    }

    void  XstpPortConfigManagedObject::setGuard(const bool &guard)
    {
        m_guard  =  guard;
    }

    bool  XstpPortConfigManagedObject::getGuard() const
    {
        return (m_guard);
    }

    void  XstpPortConfigManagedObject::setLinkShared(const MstpPortLinkTypeEnum &linkShared)
    {
        m_linkShared  =  linkShared;
    }

    MstpPortLinkTypeEnum  XstpPortConfigManagedObject::getLinkShared() const
    {
        return (m_linkShared);
    }

    void  XstpPortConfigManagedObject::setStpEnable(const bool &stpEnable)
    {
        m_stpDisable  =  !stpEnable;
    }

    bool  XstpPortConfigManagedObject::getStpEnable() const
    {
        return (!m_stpDisable);
    }

    void  XstpPortConfigManagedObject::setPort(const string &port)
    {
        m_port  =  port;
    }

    string  XstpPortConfigManagedObject::getPort() const
    {
        return (m_port);
    }

	void XstpPortConfigManagedObject::setBpduMac(const MstpPortPvstBpDuMac &mac)
	{
		m_mac = mac;
	}
	MstpPortPvstBpDuMac XstpPortConfigManagedObject::getBpduMac()  const
	{
		return m_mac;
	}

    void XstpPortConfigManagedObject::setTaggedBpduEnable (bool taggedBpduEnable)
    {
        m_taggedBpduEnable = taggedBpduEnable;
    }

    bool XstpPortConfigManagedObject::getTaggedBpduEnable () const
    {
        return m_taggedBpduEnable;
    }

    void XstpPortConfigManagedObject::setPortUid (UI32 value)
    {
        m_portUid = value;
    }

    UI32 XstpPortConfigManagedObject::getPortUid () const
    {
        return m_portUid;
    }

    UI32 XstpPortConfigManagedObject::getBpduDropDirection() const
    {
        return m_bpduDropDirection;
    }

    void XstpPortConfigManagedObject::setBpduDropDirection(const UI32 direction)
    {
        m_bpduDropDirection = direction;
    }

    void  XstpPortConfigManagedObject::setDefaultAttribute(const string &ifName, bool resetStpEnable)
    {
	    setPort(ifName);
        setDefaultAttribute(resetStpEnable, false);
    }

    void XstpPortConfigManagedObject::setDefaultAttribute (bool resetStpEnable, bool recordFieldNames)
    {
	    setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
	    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
	    setCost(MSTP_DCM_INVALID_COST);
	    setRestrictedRole(false);
	    setRestrictedTcn(false);
	    setAutoEdge(false);
	    setPortFast(false);
	    setEdgePort(false);
	    setBpduGuardEnable(false);
	    setBpduDropDirection((UI32)BPDU_DROP_DISABLED);
	    setBpduGuard(false);
	    setBpduFilter(false);
	    setGuard(false);
	    setLinkShared(MSTPPORTLINKTYPEP2P);
	    if (resetStpEnable) m_stpDisable = DEFAULT_SHUT_STATE;
	    setBpduMac(MSTPPORTPVSTBPDUMACBRCD);
        setTaggedBpduEnable(false);
        //m_portUid = 0;

        if (recordFieldNames) {
            string names[] = { "priority", "helloTime", "cost", "restrictedRole", "restrictedTcn", "autoEdge", "portFast", "edgePort", "bpduGuardEnable", "bpduGuard", "bpduFilter", "guard", "linkShared", "bpduMac", "taggedBpduEnable" };
            setAttributesToBeUpdated(vector<string>(names, names + (sizeof names / sizeof names[0])));
            if (resetStpEnable) addAttributeToBeUpdated("stpDisable");
        }
    }

    ResourceId XstpPortConfigManagedObject::setAttributeByMessage(NsmGlobalUpdatePoStpConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    MSTP_PLUG_DBG_S(string("Entering with cmd : ") + pMsg->m_cmdCode);


	    switch (pMsg->m_cmdCode)
	    {
		    case  MSTP_MSG_IF_AUTO_EDGE :
			    setAutoEdge(true);
			    break;

		    case  MSTP_MSG_IF_NO_AUTO_EDGE :
			    setAutoEdge(false);
			    break;

		    case  MSTP_MSG_IF_COST :
			    setCost(pMsg->getCost());
			    break;

		    case  MSTP_MSG_IF_NO_COST :
			    setCost(MSTP_DCM_INVALID_COST);
			    break;

		    case  MSTP_MSG_IF_EDGE :
			    setEdgePort(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE :
			    setEdgePort(false);
			    break;

		    case  MSTP_MSG_IF_EDGE_FILTER :
			    setBpduFilter(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE_FILTER :
			    setBpduFilter(false);
			    break;

		    case DCM_SSM_BPDUGUARD_ENABLE :
			    setBpduGuardEnable(true);
			    setBpduDropDirection(pMsg->getBpduDropDirection());
			    break;

		    case  DCM_SSM_BPDUGUARD_NO_ENABLE :
			    setBpduGuardEnable(false);
                setBpduDropDirection(BPDU_DROP_DISABLED);
			    break;

            case DCM_SSM_TAGGED_BPDU_ENABLE:
                setTaggedBpduEnable(true);
                break;

            case DCM_SSM_TAGGED_BPDU_NO_ENABLE:
                setTaggedBpduEnable(false);
                break;

		    case  MSTP_MSG_IF_EDGE_GUARD :
			    setBpduGuard(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE_GUARD :
			    setBpduGuard(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST :
			    setPortFast(true);
			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST :
			    setPortFast(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST_FILTER :
			    setBpduFilter(true);

			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST_FILTER :
			    setBpduFilter(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST_GUARD :
			    setBpduGuard(true);

			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST_GUARD :
			    setBpduGuard(false);
			    break;

		    case  MSTP_MSG_IF_ROOT_GUARD :
			    setGuard(true);
			    break;

		    case  MSTP_MSG_IF_NO_ROOT_GUARD :
			    setGuard(false);
			    break;

		    case  MSTP_MSG_IF_HELLO_TIME :
			    setHelloTime(pMsg->getHelloTime());
			    break;

		    case  MSTP_MSG_IF_NO_HELLO_TIME :
			    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
			    break;

		    case  MSTP_MSG_IF_LINK_SHARE :
			    setLinkShared(MSTPPORTLINKTYPESHARED);
			    break;

		    case  MSTP_MSG_IF_NO_LINK_SHARE :
			    setLinkShared(MSTPPORTLINKTYPEP2P);
			    break;

		    case  MSTP_MSG_IF_RESTRICTED_ROLE :
			    setRestrictedRole(true);
			    break;

		    case  MSTP_MSG_IF_NO_RESTRICTED_ROLE :
			    setRestrictedRole(false);
			    break;

		    case  MSTP_MSG_IF_RESTRICTED_TCN :
			    setRestrictedTcn(true);
			    break;

		    case  MSTP_MSG_IF_NO_RESTRICTED_TCN :
			    setRestrictedTcn(false);
			    break;

		    case  MSTP_MSG_IF_PROTO_ENABLE :
			    setStpEnable(true);
			    break;

		    case  MSTP_MSG_IF_NO_PROTO_ENABLE :
			    setStpEnable(false);
			    break;

		    case  MSTP_MSG_IF_PVST_MAC_CHANGE :
			    setBpduMac(MSTPPORTPVSTBPDUMACCISCO);
			    break;

		    case  MSTP_MSG_IF_NO_PVST_MAC_CHANGE :
			    setBpduMac(MSTPPORTPVSTBPDUMACBRCD);
			    break;

            case MSTP_MSG_IF_PRIORITY :
            	setPriority (pMsg->getPriority());
                break;

            case MSTP_MSG_IF_NO_PRIORITY :
            	setPriority (MSTP_DCM_DEFAULT_PORT_PRIORITY);
                break;

		    default :

			    MSTP_PLUG_DBG_ERR_S(string("cmd not supprted : ") + pMsg->m_cmdCode);
			    status = WAVE_MESSAGE_ERROR;

	    }

	    return (status);
    }

    ResourceId XstpPortConfigManagedObject::setAttributeByMessage(NsmUpdatePhyStpConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    MSTP_PLUG_DBG_S(string("Entering with cmd : ") + pMsg->m_cmdCode);


	    switch (pMsg->m_cmdCode)
	    {
		    case  MSTP_MSG_IF_AUTO_EDGE :
			    setAutoEdge(true);
			    break;

		    case  MSTP_MSG_IF_NO_AUTO_EDGE :
			    setAutoEdge(false);
			    break;

		    case  MSTP_MSG_IF_COST :
			    setCost(pMsg->getCost());
			    break;

		    case  MSTP_MSG_IF_NO_COST :
			    setCost(MSTP_DCM_INVALID_COST);
			    break;

		    case  MSTP_MSG_IF_EDGE :
			    setEdgePort(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE :
			    setEdgePort(false);
			    break;

		    case  MSTP_MSG_IF_EDGE_FILTER :
			    setBpduFilter(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE_FILTER :
			    setBpduFilter(false);
			    break;

		    case  DCM_SSM_BPDUGUARD_ENABLE :
			    setBpduGuardEnable(true);
			    setBpduDropDirection(pMsg->getBpduDropDirection());
			    break;

		    case  DCM_SSM_BPDUGUARD_NO_ENABLE :
			    setBpduGuardEnable(false);
			    setBpduDropDirection(BPDU_DROP_DISABLED);
			    break;

            case DCM_SSM_TAGGED_BPDU_ENABLE:
                setTaggedBpduEnable(true);
                break;

            case DCM_SSM_TAGGED_BPDU_NO_ENABLE:
                setTaggedBpduEnable(false);
                break;

		    case  MSTP_MSG_IF_EDGE_GUARD :
			    setBpduGuard(true);
			    break;

		    case  MSTP_MSG_IF_NO_EDGE_GUARD :
			    setBpduGuard(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST :
			    setPortFast(true);
			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST :
			    setPortFast(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST_FILTER :
			    setBpduFilter(true);

			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST_FILTER :
			    setBpduFilter(false);
			    break;

		    case  MSTP_MSG_IF_PORTFAST_GUARD :
			    setBpduGuard(true);

			    break;

		    case  MSTP_MSG_IF_NO_PORTFAST_GUARD :
			    setBpduGuard(false);
			    break;

		    case  MSTP_MSG_IF_ROOT_GUARD :
			    setGuard(true);
			    break;

		    case  MSTP_MSG_IF_NO_ROOT_GUARD :
			    setGuard(false);
			    break;

		    case  MSTP_MSG_IF_HELLO_TIME :
			    setHelloTime(pMsg->getHelloTime());
			    break;

		    case  MSTP_MSG_IF_NO_HELLO_TIME :
			    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
			    break;

		    case  MSTP_MSG_IF_LINK_SHARE :
			    setLinkShared(MSTPPORTLINKTYPESHARED);
			    break;

		    case  MSTP_MSG_IF_NO_LINK_SHARE :
			    setLinkShared(MSTPPORTLINKTYPEP2P);
			    break;

		    case  MSTP_MSG_IF_RESTRICTED_ROLE :
			    setRestrictedRole(true);
			    break;

		    case  MSTP_MSG_IF_NO_RESTRICTED_ROLE :
			    setRestrictedRole(false);
			    break;

		    case  MSTP_MSG_IF_RESTRICTED_TCN :
			    setRestrictedTcn(true);
			    break;

		    case  MSTP_MSG_IF_NO_RESTRICTED_TCN :
			    setRestrictedTcn(false);
			    break;

		    case  MSTP_MSG_IF_PROTO_ENABLE :
			    setStpEnable(true);
			    break;

		    case  MSTP_MSG_IF_NO_PROTO_ENABLE :
			    setStpEnable(false);
			    break;

		    case  MSTP_MSG_IF_PVST_MAC_CHANGE :
			    setBpduMac(MSTPPORTPVSTBPDUMACCISCO);
			    break;

		    case  MSTP_MSG_IF_NO_PVST_MAC_CHANGE :
			    setBpduMac(MSTPPORTPVSTBPDUMACBRCD);
			    break;

		    case MSTP_MSG_IF_PRIORITY :
			    setPriority (pMsg->getPriority());
			    break;

		    case MSTP_MSG_IF_NO_PRIORITY :
			    setPriority (MSTP_DCM_DEFAULT_PORT_PRIORITY);
			    break;


		    default :

			    MSTP_PLUG_DBG_ERR_S(string("cmd not supprted : ") + pMsg->m_cmdCode);
			    status = WAVE_MESSAGE_ERROR;

	    }

	    return (status);
    }

#if 0
    ResourceId XstpPortConfigManagedObject::getAttributeToMessage(NsmUpdatePhyStpConfigMessage *&pMsg)
    {
            ResourceId status = WAVE_MESSAGE_SUCCESS;
            pMsg->setPriority (getPriority());
            pMsg->setHelloTime (getHelloTime());
            pMsg->setCost (getCost());
            pMsg->setRestrictedRole (getRestrictedRole());
            pMsg->setRestrictedTcn (getRestrictedTcn());
            pMsg->setAutoEdge (getAutoEdge());
            pMsg->setPortFast (getPortFast());
            pMsg->setEdgePort (getEdgePort());
            pMsg->setBpduGuard (getBpduGuard());
            pMsg->setBpduFilter (getBpduFilter());
	    //if (getGuard() == MSTPPORTGUARDROOT)
		    pMsg->setGuard (getGuard());
//	    else 
//		    pMsg->setGuard (false);
            pMsg->setLinkShared (getLinkShared());
            pMsg->setStpEnable (getStpEnable());
            pMsg->setBpduMac (getBpduMac());
            //pMsg->setPort (getPriority());

            return (status);
    }

    ResourceId XstpPortConfigManagedObject::getAttributeToMessage(NsmUpdatePoStpConfigSPMessage *&pMsg)
    {
            ResourceId status = WAVE_MESSAGE_SUCCESS;
            pMsg->setPriority (getPriority());
            pMsg->setHelloTime (getHelloTime());
            pMsg->setCost (getCost());
            pMsg->setRestrictedRole (getRestrictedRole());
            pMsg->setRestrictedTcn (getRestrictedTcn());
            pMsg->setAutoEdge (getAutoEdge());
            pMsg->setPortFast (getPortFast());
            pMsg->setEdgePort (getEdgePort());
            pMsg->setBpduGuard (getBpduGuard());
            pMsg->setBpduFilter (getBpduFilter());
	    //if (getGuard() == MSTPPORTGUARDROOT)
		    pMsg->setGuard (getGuard());
//	    else 
//		    pMsg->setGuard (false);

            pMsg->setLinkShared (getLinkShared());
            pMsg->setStpEnable (getStpEnable());
            pMsg->setBpduMac (getBpduMac());
            //pMsg->setPort (getPriority());

            return (status);
    }
#endif

    ResourceId XstpPortConfigManagedObject::getAttributeToMessage(MstpPortConfigMessage *&pMsg)
    {
            ResourceId status = WAVE_MESSAGE_SUCCESS;
            pMsg->setPriority (getPriority());
            pMsg->setHelloTime (getHelloTime());
            pMsg->setCost (getCost());
            pMsg->setRestrictedRole (getRestrictedRole());
            pMsg->setRestrictedTcn (getRestrictedTcn());
            pMsg->setAutoEdge (getAutoEdge());
            pMsg->setPortFast (getPortFast());
            pMsg->setEdgePort (getEdgePort());
            pMsg->setBpduGuardEnable(getBpduGuardEnable());
            pMsg->setBpduDropDirection(getBpduDropDirection());
            pMsg->setBpduGuard (getBpduGuard());
            pMsg->setBpduFilter (getBpduFilter());
	    //if (getGuard() == MSTPPORTGUARDROOT)
		    pMsg->setGuard (getGuard());
//	    else 
//		    pMsg->setGuard (false);
            pMsg->setTaggedBpduEnable(m_taggedBpduEnable);
            pMsg->setLinkShared (getLinkShared());
            pMsg->setStpEnable (getStpEnable());
            pMsg->setBpduMac (getBpduMac());
            pMsg->setInterfaceInfo (NsmUtils::getIfType(getOwnerManagedObjectId()), getPort());

            return (status);
	}


    void XstpPortConfigManagedObject::initDefaults()
    {
        DEFAULT_SHUT_STATE = MstpToolKit::isVcsEnabled();
    }

    string XstpPortConfigManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
    {
        //return string("XstpPortConfigManagedObject::customConfigurationDisplay for run template ---------------- ");

       string displayString; 
        
       YangDisplayConfigurationContext *pYangDisplayConfigurationContext = waveCustomCliDisplayConfigurationContext->getYangDisplayConfigurationContext ();
       YangElement *pYangElement = waveCustomCliDisplayConfigurationContext->getYangElement();

       WaveLocationPath locationPath;


       if (true == m_stpDisable)
       {
            locationPath.setIsAbsolute(false);

            locationPath.addForwardStep("switchport","");
            locationPath.addForwardStep("basic","");
            locationPath.incrementNumberOfStepsToReachRelativeRoot();
            locationPath.incrementNumberOfStepsToReachRelativeRoot();
            locationPath.computeStringRepresentation ();

            WaveXPathExpressionValue value = pYangDisplayConfigurationContext->evaluateValueOfLocationPath(locationPath, pYangElement);

           if(value.getBoolValue())
           {

                displayString =  "spanning-tree shutdown\n" ;         
           }
           else
           {
                displayString =  "\r" ;         
           }

       }
       else
       {
           locationPath.addForwardStep("vcsmode","");
           locationPath.addForwardStep("vcs-mode","");
           locationPath.computeStringRepresentation ();

           WaveXPathExpressionValue value = pYangDisplayConfigurationContext->evaluateValueOfLocationPath(locationPath, pYangElement);
          
           if(value.getBoolValue())
           {
                displayString =  "no spanning-tree shutdown\n";
           }
           else
           {
                displayString =  "\r" ;
           }
       }
       
       return displayString;
    }
}
