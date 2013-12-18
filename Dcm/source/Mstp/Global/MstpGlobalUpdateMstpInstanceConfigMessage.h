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

#ifndef MSTPGLOBALUPDATEMSTPINSTANCECONFIGMESSAGE_H
#define MSTPGLOBALUPDATEMSTPINSTANCECONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/UI32Range.h"


using namespace WaveNs;

namespace DcmNs
{

    class MstpGlobalUpdateMstpInstanceConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpGlobalUpdateMstpInstanceConfigMessage ();
            MstpGlobalUpdateMstpInstanceConfigMessage (const UI32 &mgmtFlag,const UI32 &msgType,const UI32 &cmdCode,const vector<ObjectId> &vlanid,const UI8 &instanceId,const UI32 &priority);
            virtual    ~MstpGlobalUpdateMstpInstanceConfigMessage ();
            void setMgmtFlag(const UI32 &mgmtFlag);
            UI32 getMgmtFlag()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setVlanid(const vector<ObjectId> &vlanid);
            vector<ObjectId> getVlanid()  const;
            void setVlanidList(const vector<UI32> &vlanidList);
            vector<UI32> getVlanidList()  const;
            void setInstanceId(const UI8 &instanceId);
            UI8 getInstanceId()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setVlanIdRange(const UI32Range &vlanIdRange);
            UI32Range getVlanIdRange()  const;
	    void printMsg();
// Now the data members

        private:
        protected:
        public:
            UI32  m_mgmtFlag;
            UI32  m_msgType;
            UI32  m_cmdCode;
            vector<ObjectId>  m_vlanid;
            vector<UI32>  m_vlanidList;
	    UI32Range     m_vlanIdRange;
            UI8  m_instanceId;
            UI32  m_priority;
    };
}
#endif                                            //MSTPGLOBALUPDATEMSTPINSTANCECONFIGMESSAGE_H
