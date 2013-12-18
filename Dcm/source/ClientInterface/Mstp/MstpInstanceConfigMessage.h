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

#ifndef MSTPINSTANCECONFIGMESSAGE_H
#define MSTPINSTANCECONFIGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

//#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class MstpUpdateMstpInstanceConfigMessage;

    class MstpInstanceConfigMessage:  public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();

        public:
            MstpInstanceConfigMessage ();
            MstpInstanceConfigMessage ( MstpUpdateMstpInstanceConfigMessage &pGlobalMsg);
            MstpInstanceConfigMessage (const UI32 &mgmtFlag,const UI32 &msgType,const UI32 &cmdCode,const vector<ObjectId> &vlanid,const UI8 &instanceId,const UI32 &priority);
            virtual    ~MstpInstanceConfigMessage ();
            void setMgmtFlag(const UI32 &mgmtFlag);
            UI32 getMgmtFlag()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setVlanid(const vector<ObjectId> &vlanid);
            vector<ObjectId> getVlanid()  const;
            void setInstanceId(const UI8 &instanceId);
            UI8 getInstanceId()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
			void setVlanidList(vector<UI32> & vlanidList);

	    void  printMsg();
	    virtual const void *getCStructureForInputs    ();
	    virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure) {
		    return ;
	    }



// Now the data members
        private:
            UI32  m_mgmtFlag;
            UI32  m_msgType;
            UI32  m_cmdCode;
            UI32  m_vlanNum;
            vector<UI32>  m_vlanidList;
            vector<ObjectId>  m_vlanid;
            UI8   m_instanceId;
            UI32  m_priority;
    };
}
#endif                                            //MSTPINSTANCECONFIGMESSAGE_H
