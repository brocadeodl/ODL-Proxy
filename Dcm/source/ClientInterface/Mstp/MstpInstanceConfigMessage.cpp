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
 *   Author : sudheend                                                     *
 **************************************************************************/

//#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
//#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
//#include "Mstp/Local/MstpTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"


namespace DcmNs
{

    MstpInstanceConfigMessage::MstpInstanceConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPBRIDGEINSTCONFIG)
    {
	m_mgmtFlag    = 0;
        m_msgType     = 0;
        m_cmdCode     = 0;
        m_vlanNum      = 0;
	m_vlanid.clear();
	m_vlanidList.clear();
	m_instanceId  = 0;
        m_priority    = 0;

    }

    MstpInstanceConfigMessage::MstpInstanceConfigMessage (MstpUpdateMstpInstanceConfigMessage &local)
        : DcmManagementInterfaceMessage (MSTPBRIDGEINSTCONFIG)
    {
	m_mgmtFlag    =  local.m_mgmtFlag  ;
        m_msgType     =  local.m_msgType   ;
        m_cmdCode     =  local.m_cmdCode   ;
        m_vlanNum      =  local.m_vlanidList.size();
	m_vlanid = local.m_vlanid;
	m_vlanidList = local.m_vlanidList;
	m_instanceId  =  local.m_instanceId;
        m_priority    =  local.m_priority  ;

    }
    MstpInstanceConfigMessage::MstpInstanceConfigMessage (const UI32 &mgmtFlag,const UI32 &msgType,const UI32 &cmdCode,const vector<ObjectId> &vlanid,const UI8 &instanceId,const UI32 &priority)
        : DcmManagementInterfaceMessage (MSTPBRIDGEINSTCONFIG),
        m_mgmtFlag    (mgmtFlag),
        m_msgType    (msgType),
        m_cmdCode    (cmdCode),
        m_vlanNum   (0),
        m_vlanid    (vlanid),
        m_instanceId    (instanceId),
        m_priority    (priority)
    {
    }

    MstpInstanceConfigMessage::~MstpInstanceConfigMessage ()
    {
    }

    void  MstpInstanceConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlag,"mgmtFlag"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeUI32(&m_vlanNum,"vlanNum"));
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
    }

    void  MstpInstanceConfigMessage::setMgmtFlag(const UI32 &mgmtFlag)
    {
        m_mgmtFlag  =  mgmtFlag;
    }

    UI32  MstpInstanceConfigMessage::getMgmtFlag() const
    {
        return (m_mgmtFlag);
    }

    void  MstpInstanceConfigMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  MstpInstanceConfigMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  MstpInstanceConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpInstanceConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpInstanceConfigMessage::setVlanidList(vector<UI32> & vlanidList)
    {
        m_vlanidList  =  vlanidList;
    }

    void  MstpInstanceConfigMessage::setVlanid(const vector<ObjectId> &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    vector<ObjectId>  MstpInstanceConfigMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  MstpInstanceConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  MstpInstanceConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  MstpInstanceConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpInstanceConfigMessage::getPriority() const
    {
        return (m_priority);
    }


    void  MstpInstanceConfigMessage::printMsg()
    {
	    trace(TRACE_LEVEL_DEBUG, string("MstpInstanceConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_mgmtFlag:                 ")+toString(m_mgmtFlag));
	    trace(TRACE_LEVEL_DEBUG, string("m_msgType:                 	")+toString(m_msgType));
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:              	")+toString(m_cmdCode));

	    trace(TRACE_LEVEL_DEBUG, string("m_instanceId				")+toString(UI32 (m_instanceId)));
	    trace(TRACE_LEVEL_DEBUG, string("m_priority:             	")+toString(m_priority));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanNum:             	")+toString(m_vlanNum));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanidList.size:             	")+toString(m_vlanidList.size()));


#if 0
	    vector<UI32>::iterator vlanid;
            for (vlanid = m_vlanidList.begin(); vlanid != m_vlanidList.end(); vlanid++)
            {
                    trace(TRACE_LEVEL_DEBUG, string("vlanid:            ")+toString(*vlanid));
            }
#else
	    if (m_vlanidList.size() >= 1) {
		    trace (TRACE_LEVEL_DEBUG, string("First vlaue ") + m_vlanidList[0] + string(" "));
		    trace (TRACE_LEVEL_DEBUG, string("Last vlaue   ") + m_vlanidList[m_vlanidList.size() - 1] + string(" "));
	    }
#endif


    }

    const void * MstpInstanceConfigMessage::getCStructureForInputs() 
    {
	    unsigned int i = 0;
	    unsigned int size = 0;
	    mstp_msg_br_inst_config *br_inst_config = NULL;
	    unsigned char *ptr = NULL;

	    size = sizeof(mstp_msg_br_inst_config) + (m_vlanidList.size() * sizeof(unsigned int));

	    /* should it be malloc  */
	    //mstp_msg_br_inst_config *br_inst_config = new mstp_msg_br_inst_config;
	    ptr = (unsigned char *)calloc(1, size);
	    if (ptr == NULL) {
		    trace (TRACE_LEVEL_FATAL, string(" Calloc failed for  size :") + size);
		    return (br_inst_config);
	    }
	    br_inst_config = (mstp_msg_br_inst_config *)&ptr[0];
	    br_inst_config->vid = (unsigned int *) &ptr[(sizeof(mstp_msg_br_inst_config))];

	    br_inst_config->cmd = m_cmdCode;

	    br_inst_config->inst = m_instanceId;
	    br_inst_config->priority = m_priority;
	    br_inst_config->vlan_num = m_vlanidList.size(); //m_vlanNum

	    if (br_inst_config->vlan_num) {
		    for (i = 0; i< br_inst_config->vlan_num; i++) 
		    {
			    br_inst_config->vid[i] = m_vlanidList[i];
		    }

	    }
	    return (br_inst_config);
    }

}
