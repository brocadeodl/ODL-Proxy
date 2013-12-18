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
 *   Author : Rahul Jain                                                   *
 ***************************************************************************/

#ifndef _PIMSTATICRPCFGCLIENTMSG_H_
#define _PIMSTATICRPCFGCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Pim/Local/StaticRpLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimStaticRpCfgClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimStaticRpCfgClientMsg();
        PimStaticRpCfgClientMsg(const UI8 &msgType, const IpV4Address &rpAddr, 
            const string &prefixList);
        virtual ~PimStaticRpCfgClientMsg();
        
        void setMsgType(const UI8 &msgType);
        void setRpAddr(const IpV4Address &rpAddr);
        void setPrefixList(const string &prefixList);
       
        UI8 getMsgType()  const;
        IpV4Address getRpAddr() const;
        string getPrefixList() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_msgType;
        IpV4Address m_rpAddr;
        string m_prefixList;
};
} /* namespace */
#endif /* _PIMSTATICRPCFGCLIENTMSG_H_ */

