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

#ifndef _PIMCANDRPCFGCLIENTMSG_H_
#define _PIMCANDRPCFGCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Pim/Local/RpCandidateLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimCandRpCfgClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimCandRpCfgClientMsg();
        PimCandRpCfgClientMsg(const UI8 &msgType, const UI8 &intfType, 
            const string &intfName, const string &prefixList);
        virtual ~PimCandRpCfgClientMsg();
        
        void setMsgType(const UI8 &msgType);
        void setIntfType(const UI8 &intfType);
        void setIntfName(const string &intfName);
        void setPrefixList(const string &prefixList);
       
        UI8 getMsgType()  const;
        UI8 getIntfType()  const;
        string getIntfName() const;
        string getPrefixList() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_msgType;
        UI8 m_intfType;
        string m_intfName;
        string m_prefixList;
};
} /* namespace */
#endif /* _PIMCANDRPCFGCLIENTMSG_H_ */
