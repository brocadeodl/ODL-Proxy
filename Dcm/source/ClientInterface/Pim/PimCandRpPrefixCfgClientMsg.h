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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Chirag Taunk                                                 *
 ***************************************************************************/

#ifndef PIMCANDRPPREFIXCFGCLIENTMSG_H_
#define PIMCANDRPPREFIXCFGCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Pim/Local/RpCandidatePrefixLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimCandRpPrefixCfgClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimCandRpPrefixCfgClientMsg();
        PimCandRpPrefixCfgClientMsg(const UI8 &msgType, 
			const UI8 &prefixAction, const IpV4Address &prefixIP, 
            const UI16 &prefixLen);
        virtual ~PimCandRpPrefixCfgClientMsg();
        
        void setMsgType(const UI8 &msgType);
		void setAction(const UI8 &prefixAction);
        void setPrefixIP(const IpV4Address &prefixIP);
        void setPrefixLen(const UI16 &prefixLen);
       
        UI8 getMsgType()  const;
		UI8 getAction()  const;
        IpV4Address getPrefixIP() const;
        UI16 getPrefixLen() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void 
			*pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_msgType;
		UI8 m_prefixAction;
        IpV4Address m_prefixIP;
        UI16 m_prefixLen;
};
} /* namespace */
#endif /* PIMCANDRPPREFIXCFGCLIENTMSG_H_ */

