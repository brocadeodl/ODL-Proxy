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

#include "Framework/Utils/TraceUtils.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    MstpUpdateMstpInstanceConfigMessage::MstpUpdateMstpInstanceConfigMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEMSTPINSTANCECONFIG)
    {
	      m_mgmtFlag    = 0;
        m_msgType     = 0;
        m_cmdCode     = 0;
        m_vlanid.clear();
        m_vlanidList.clear();
      	m_instanceId  = 0;
        m_priority    = 0;
        m_vlanNum     = 0;
    }

    MstpUpdateMstpInstanceConfigMessage::MstpUpdateMstpInstanceConfigMessage (MstpGlobalUpdateMstpInstanceConfigMessage & global)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEMSTPINSTANCECONFIG)
    {
        m_mgmtFlag    =  global.m_mgmtFlag  ;
        m_msgType     =  global.m_msgType   ;
        m_cmdCode     =  global.m_cmdCode   ;
        m_vlanNum	  =  global.m_vlanidList.size();
        m_vlanid      =  global.m_vlanid    ;
        m_vlanidList  =  global.m_vlanidList    ;
        m_instanceId  =  global.m_instanceId;
        m_priority    =  global.m_priority  ;
    }
    MstpUpdateMstpInstanceConfigMessage::MstpUpdateMstpInstanceConfigMessage (const UI32 &mgmtFlag,const UI32 &msgType,const UI32 &cmdCode,const vector<ObjectId> &vlanid,const UI8 &instanceId,const UI32 &priority)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEMSTPINSTANCECONFIG),
        m_mgmtFlag    (mgmtFlag),
        m_msgType    (msgType),
        m_cmdCode    (cmdCode),
        m_vlanid    (vlanid),
/*        m_vlanidList    (vlanidList),*/
        m_instanceId    (instanceId),
        m_priority    (priority)
    {
      m_vlanNum=0;
    }

    MstpUpdateMstpInstanceConfigMessage::~MstpUpdateMstpInstanceConfigMessage ()
    {
    }

    void  MstpUpdateMstpInstanceConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlag,"mgmtFlag"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeObjectIdVector(&m_vlanid,"vlanid"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vlanidList,"vlanidList"));
        addSerializableAttribute (new AttributeUI32(&m_vlanNum,"vlanNum"));
        addSerializableAttribute (new AttributeUI8(&m_instanceId,"instanceId"));
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
    }

    void  MstpUpdateMstpInstanceConfigMessage::setMgmtFlag(const UI32 &mgmtFlag)
    {
        m_mgmtFlag  =  mgmtFlag;
    }

    UI32  MstpUpdateMstpInstanceConfigMessage::getMgmtFlag() const
    {
        return (m_mgmtFlag);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  MstpUpdateMstpInstanceConfigMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpUpdateMstpInstanceConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setVlanidList(const vector<UI32> &vlanidList)
    {
        m_vlanidList  =  vlanidList;
    }

    vector<UI32>  MstpUpdateMstpInstanceConfigMessage::getVlanidList() const
    {
        return (m_vlanidList);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setVlanid(const vector<ObjectId> &vlanid)
    {
        m_vlanid  =  vlanid;
    }

    vector<ObjectId>  MstpUpdateMstpInstanceConfigMessage::getVlanid() const
    {
        return (m_vlanid);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setInstanceId(const UI8 &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    UI8  MstpUpdateMstpInstanceConfigMessage::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  MstpUpdateMstpInstanceConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpUpdateMstpInstanceConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void   MstpUpdateMstpInstanceConfigMessage::copyGlobal2LocalMsg(MstpUpdateMstpInstanceConfigMessage *pLocalMsg, const MstpGlobalUpdateMstpInstanceConfigMessage *pGlobalMsg)
    {
	    pLocalMsg->setMgmtFlag(pGlobalMsg->getMgmtFlag());
	    pLocalMsg->setMsgType(pGlobalMsg->getMsgType());
	    pLocalMsg->setCmdCode(pGlobalMsg->getCmdCode());

	    pLocalMsg->setPriority(pGlobalMsg->getPriority());
	    pLocalMsg->setInstanceId(pGlobalMsg->getInstanceId());
	    /* FIXME*/
	    pLocalMsg->setVlanid(pGlobalMsg->getVlanid());

    }

    void  MstpUpdateMstpInstanceConfigMessage::printMsg()
    {
	    trace(TRACE_LEVEL_DEBUG, string("MstpUpdateMstpInstanceConfigMessage::printMsg()"));
	    trace(TRACE_LEVEL_DEBUG, string("m_mgmtFlag:                 ")+m_mgmtFlag);
	    trace(TRACE_LEVEL_DEBUG, string("m_msgType:                 	")+m_msgType);
	    trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:              	")+m_cmdCode);

	    trace(TRACE_LEVEL_DEBUG, string("m_instanceId:            	")+m_instanceId);
	    trace(TRACE_LEVEL_DEBUG, string("m_priority:             	")+m_priority);
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanid.size:		")+toString(m_vlanid.size()));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanidList.size:		")+toString(m_vlanidList.size()));
	    trace(TRACE_LEVEL_DEBUG, string("m_vlanNum:		")+toString(m_vlanNum));


#if 0
	    vector<UI32>::iterator vlanid;
	    for (vlanid = m_vlanidList.begin(); vlanid != m_vlanidList.end(); vlanid++)
	    {
		    trace(TRACE_LEVEL_DEBUG, string("vlanid:            ")+(*vlanid));
	    }
#else 
	    if (m_vlanidList.size() >= 1)
	    {
		    trace (TRACE_LEVEL_DEBUG, string("First vlaue ") + m_vlanidList[0] + string(" "));
		    trace (TRACE_LEVEL_DEBUG, string("Last vlaue   ") + m_vlanidList[m_vlanidList.size() - 1] + string(" "));
	    }
#endif

    }

}
