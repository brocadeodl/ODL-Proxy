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
 *   Author : sriramr                                                     *
 **************************************************************************/

#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalStaticMacSPMessage.h"
#include "L2sys/Local/L2sysTypes.h"
//#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"
#include  "L2sys/Local/L2sysLocalShowMacProfileMessage.h"
#include  "ClientInterface/L2sys/L2sysMessageDef.h"
#include  "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    L2sysLocalShowMacProfileMessage::L2sysLocalShowMacProfileMessage ()
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWMACPROFILE),
        m_rbridgeid    (0),
        m_interface_type    (0),
        m_ifindex    (0),
        m_vlan    (0),
        m_vlanid    (0),
        m_cmdcode    (0),
        m_type      (0),
        m_isGwRequest(0),
        m_lastRbridge_id(0),
        m_lastIf_name("0"),
        m_lastMac("0"),
        m_lastType("0"),
        m_lastState("0"),
        m_lastVlan_id(0),
        m_lastAppmFlag(0),
        m_lastIs_last(0),
        m_lastIsprofiled("0"),
        m_isFirstRequest(0)
    {
    }

    L2sysLocalShowMacProfileMessage::L2sysLocalShowMacProfileMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &interface_type,const string &ifname,const UI32 &ifindex,const UI32 &vlan,const UI32 &vlanid,const UI32 &Opcode,const string &Profiled,const UI32 &type,bool isGwRequest)
        : PrismMessage (L2sysLocalObjectManager::getPrismServiceId (),L2SYSSHOWMACPROFILE),
        m_rbridgeid    (rbridgeid),
        m_Mac    (Mac),
        m_interface_type    (interface_type),
        m_ifname    (ifname),
        m_ifindex    (ifindex),
        m_vlan    (vlan),
        m_vlanid    (vlanid),

        m_cmdcode    (Opcode),
	m_Profiled  (Profiled),
	m_type      (type),
	m_isGwRequest(isGwRequest)
    {
        m_lastRbridge_id = 0;
        m_lastIf_name = "0";
        m_lastMac = "0";
        m_lastType = "0";
        m_lastState = "0";
        m_lastVlan_id = 0;
        m_lastAppmFlag = 0;
        m_lastIs_last = 0;
        m_lastIsprofiled = "0";
        m_isFirstRequest = 0;
    }

    L2sysLocalShowMacProfileMessage::~L2sysLocalShowMacProfileMessage ()
    {
    }

    void  L2sysLocalShowMacProfileMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rbridgeid,"rbridgeid"));
        addSerializableAttribute (new AttributeString(&m_Mac,"Mac"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_interface_type,"interface_type"));
        addSerializableAttribute (new AttributeString(&m_ifname,"ifname"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_ifindex,"ifindex"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_vlan,"vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vlanid,"vlanid"));
	addSerializableAttribute (new AttributeUI32(&m_type,"type"));
	addSerializableAttribute (new AttributeString(&m_Profiled,"Profiled"));

	addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
	addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));

        // Last Mac Record information
        addSerializableAttribute (new AttributeUI32(&m_lastRbridge_id, "lastRbridge_id"));
        addSerializableAttribute (new AttributeString(&m_lastIf_name,"lastIf_name"));
        addSerializableAttribute (new AttributeString(&m_lastMac,"lastMac"));
        addSerializableAttribute (new AttributeString(&m_lastType,"lastType"));
        addSerializableAttribute (new AttributeString(&m_lastState,"lastState"));
        addSerializableAttribute (new AttributeUI32(&m_lastVlan_id, "lastVlan_id"));
        addSerializableAttribute (new AttributeUI32(&m_lastAppmFlag, "lastAppmFlag"));
        addSerializableAttribute (new AttributeUI32(&m_lastIs_last, "lastIs_last"));
        addSerializableAttribute (new AttributeString(&m_lastIsprofiled, "lastIsprofiled"));
        addSerializableAttribute (new AttributeUI32(&m_isFirstRequest, "isFirstRequest"));
    }

    void  L2sysLocalShowMacProfileMessage::setRbridgeid(const UI32 &rbridgeid)
    {
        m_rbridgeid  =  rbridgeid;
    }

    UI32  L2sysLocalShowMacProfileMessage::getRbridgeid() const
    {
        return (m_rbridgeid);
    }

    void  L2sysLocalShowMacProfileMessage::setMac(const string &Mac)
    {
        m_Mac  =  Mac;
    }

    string  L2sysLocalShowMacProfileMessage::getMac() const
    {
        return (m_Mac);
    }


    void  L2sysLocalShowMacProfileMessage::setInterface_type(const UI32 &interface_type)
    {
        m_interface_type  =  interface_type;
    }

    UI32  L2sysLocalShowMacProfileMessage::getInterface_type() const
    {
        return (m_interface_type);
    }

    void  L2sysLocalShowMacProfileMessage::setIfname(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  L2sysLocalShowMacProfileMessage::getIfname() const
    {
        return (m_ifname);
    }

    void  L2sysLocalShowMacProfileMessage::setIfindex(const UI32 &ifindex)
    {
        m_ifindex  =  ifindex;
    }

    UI32  L2sysLocalShowMacProfileMessage::getIfindex() const
    {
        return (m_ifindex);
    }

    void  L2sysLocalShowMacProfileMessage::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  L2sysLocalShowMacProfileMessage::getVlan() const
    {
        return (m_vlan);
    }

    void  L2sysLocalShowMacProfileMessage::setVlanId(const UI32 &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    UI32  L2sysLocalShowMacProfileMessage::getVlanId() const
    {
        return (m_vlanid);
    }

    void  L2sysLocalShowMacProfileMessage::setCmdcode(const UI32 &cmdcode)
    {

        m_cmdcode  =  cmdcode;
    }

    UI32  L2sysLocalShowMacProfileMessage::getCmdcode() const
    {
        return (m_cmdcode);
   }	
   void  L2sysLocalShowMacProfileMessage::setProfiled(const string &Profiled)
   {
	m_Profiled = Profiled;
	}
	string L2sysLocalShowMacProfileMessage::getProfiled()const
	{
	return (m_Profiled);
	}
	void L2sysLocalShowMacProfileMessage::setType(const UI32 &type)
	{
	m_type = type;
	}
	UI32 L2sysLocalShowMacProfileMessage::getType() const
	{
	return(m_type);
	}
	void L2sysLocalShowMacProfileMessage::setIsGwRequest(bool isGwRequest)
	    {
	     m_isGwRequest = isGwRequest;
	    }

	 bool L2sysLocalShowMacProfileMessage::getIsGwRequest() const
	     {
	      return (m_isGwRequest);
	    }
void  L2sysLocalShowMacProfileMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }

    void L2sysLocalShowMacProfileMessage::transferOutputBuffer(L2sysShowMessage *&pMsg)
    {
                UI32 size = 0;
                UI32 bufferNum = pMsg->getCmdCode();
                void *pBuffer = NULL;

                trace(TRACE_LEVEL_DEVEL, string("L2sysLocalShowMacProfileMessage::transferOutputBuffer bufNum is ") + bufferNum);
                pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
                if (pBuffer == NULL) {
                        trace(TRACE_LEVEL_DEVEL, "L2sysLocalShowMacProfileMessage::transferOutputBuffer received empty buffer");
                        return;
                }
                addBuffer(bufferNum, size, pBuffer);
    }
void L2sysLocalShowMacProfileMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
    trace(TRACE_LEVEL_INFO, string("L2sysLocalShowMacProfileMessage::copyAllBuffers "));
            this->copyBuffersFrom(prismMessage);
	        }

    void L2sysLocalShowMacProfileMessage::setLastMacRecord(const l2sys_show_mac_output_msg_t *lastMacRecord)
    {
        m_lastRbridge_id = lastMacRecord->rbridge_id;
        m_lastIf_name = lastMacRecord->if_name;
        m_lastMac = lastMacRecord->Mac;
        m_lastType = lastMacRecord->type;
        m_lastState = lastMacRecord->state;
        m_lastVlan_id = lastMacRecord->vlan_id;
        m_lastAppmFlag = lastMacRecord->appmFlag;
        m_lastIs_last = lastMacRecord->is_last;
        m_lastIsprofiled = lastMacRecord->isprofiled;
    }

    void L2sysLocalShowMacProfileMessage::getLastMacRecord(l2sys_show_mac_output_msg_t *lastMacRecord)
    {
        lastMacRecord->rbridge_id = m_lastRbridge_id;
        strncpy(lastMacRecord->if_name, m_lastIf_name.c_str(), 256);
        strncpy(lastMacRecord->Mac, m_lastMac.c_str(), 16);
        strncpy(lastMacRecord->type, m_lastType.c_str(), 64);
        strncpy(lastMacRecord->state, m_lastState.c_str(), 64);
        lastMacRecord->vlan_id = m_lastVlan_id;
        lastMacRecord->appmFlag = m_lastAppmFlag;
        lastMacRecord->is_last = m_lastIs_last;
        strncpy(lastMacRecord->isprofiled, m_lastIsprofiled.c_str(), 20);
    }

    void L2sysLocalShowMacProfileMessage::setIsFirstRequest(const UI32 &isFirstRequest)
    {
        m_isFirstRequest = isFirstRequest;
    }

    UI32 L2sysLocalShowMacProfileMessage::getIsFirstRequest() const
    {
        return (m_isFirstRequest);
    }

}
