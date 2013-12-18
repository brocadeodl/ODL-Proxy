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
 *   Author : Sree Shankar                                                 *
 ***************************************************************************/

#ifndef _PIMBSRCANDCLIENTMSG_H_
#define _PIMBSRCANDCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Pim/Local/BsrCandidateLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimBsrCandClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimBsrCandClientMsg();
        PimBsrCandClientMsg(const UI8 &msgType, const UI8 &intfType,
            const string &intfName, const UI32 &hashMask, const UI32 &priority);
        virtual ~PimBsrCandClientMsg();
        
        void setMsgType(const UI8 &msgType);
        void setIntfType(const UI8 &intfType);
		void setIntfName(const string &intfName);
		void setHashMask(const UI32 &hashMask);
		void setPriority(const UI32 &priority);
       
        UI8 getMsgType()  const;
        UI8 getIntfType()  const;
        string getIntfName() const;
		UI32 getHashMask() const;
		UI32 getPriority() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_msgType;
        UI8 m_intfType;
        string m_intfName;
		UI32  m_hashMask;
		UI32  m_priority;
};
} /* namespace */
#endif /* _PIMBSRCANDCLIENTMSG_H_ */
