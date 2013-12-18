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

#include "Framework/Attributes/AttributeUI32Range.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    MstpGlobalUpdateMstpInstanceConfigMessage::MstpGlobalUpdateMstpInstanceConfigMessage ()
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEMSTPINSTANCECONFIG)
    {
	m_mgmtFlag = 0;
	m_msgType = 0;
	m_cmdCode = 0;
        m_vlanid.clear();
        m_vlanidList.clear();
	m_instanceId = 0;
        m_priority = 0;
	m_vlanIdRange = UI32Range("");
    }

    MstpGlobalUpdateMstpInstanceConfigMessage::MstpGlobalUpdateMstpInstanceConfigMessage (const UI32 &mgmtFlag,const UI32 &msgType,const UI32 &cmdCode,const vector<ObjectId> &vlanid,const UI8 &instanceId,const UI32 &priority)
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEMSTPINSTANCECONFIG),
        m_mgmtFlag    (mgmtFlag),
        m_msgType    (msgType),
        m_cmdCode    (cmdCode),
        m_vlanid    (vlanid),
/*        m_vlanidList    (vlanidList),*/
        m_instanceId    (instanceId),
        m_priority    (priority)
    {
    }

    MstpGlobalUpdateMstpInstanceConfigMessage::~MstpGlobalUpdateMstpInstanceConfigMessage ()
    {
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlag,"mgmtFlag"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32Range(&m_vlanIdRange,"vlanIdRange"));

    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setMgmtFlag(const UI32 &mgmtFlag)
    {
        m_mgmtFlag  =  mgmtFlag;
    }

    UI32  MstpGlobalUpdateMstpInstanceConfigMessage::getMgmtFlag() const
    {
        return (m_mgmtFlag);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  MstpGlobalUpdateMstpInstanceConfigMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpGlobalUpdateMstpInstanceConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setVlanid(const vector<ObjectId> &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    vector<ObjectId>  MstpGlobalUpdateMstpInstanceConfigMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setVlanidList(const vector<UI32> &vlanidList)
    {
        m_vlanidList  =  vlanidList;
    }

    vector<UI32>  MstpGlobalUpdateMstpInstanceConfigMessage::getVlanidList() const
    {
        return (m_vlanidList);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  MstpGlobalUpdateMstpInstanceConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpGlobalUpdateMstpInstanceConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpGlobalUpdateMstpInstanceConfigMessage::setVlanIdRange(const UI32Range &vlanRange)
    {
	    m_vlanIdRange  =  vlanRange;
    }

    UI32Range MstpGlobalUpdateMstpInstanceConfigMessage::getVlanIdRange() const
    {
	    return (m_vlanIdRange);
    }


    void  MstpGlobalUpdateMstpInstanceConfigMessage::printMsg()
    {

	    trace(TRACE_LEVEL_DEBUG, string("MstpGlobalUpdateMstpInstanceConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_mgmtFlag:		")+toString(m_mgmtFlag));
	    trace(TRACE_LEVEL_DEBUG, string("m_msgType:		")+toString(m_msgType));
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:		")+toString(m_cmdCode));

	    trace(TRACE_LEVEL_DEBUG, string("m_instanceId:		")+toString(UI32 (m_instanceId)));
	    trace(TRACE_LEVEL_DEBUG, string("m_priority:		")+toString(m_priority));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanidList.size:		")+toString(m_vlanidList.size()));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanid.size:		")+toString(m_vlanid.size()));
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

}
