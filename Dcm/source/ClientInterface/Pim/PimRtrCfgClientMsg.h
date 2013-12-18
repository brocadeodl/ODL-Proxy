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

#ifndef _PIMRTRCFGCLIENTMSG_H_
#define _PIMRTRCFGCLIENTMSG_H_

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Pim/Local/PimRtrCfgLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

class PimRtrCfgClientMsg : public DcmManagementInterfaceMessage
{
    /* Function declarations */
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        PimRtrCfgClientMsg();
        PimRtrCfgClientMsg(const UI32 msgType);
        virtual ~PimRtrCfgClientMsg();
        
        void setMsgType(const UI8 &msgType);
        void setCmdCode(const UI8 &cmdCode);
        void setEnable(const bool &enable);
        void setVrfId(const string &vrfId);
        void setMaxMcache(const UI32 &maxMcache);
        void setHelloTimer(const SI16 &helloTimer);
        void setNbrTo(const SI16 &nbrTo);
        void setInactTimer(const SI16 &inactTimer);
        void setMsgIntv(const SI32 &msgIntv);
        void setSptThold(const UI32 &sptThold);
       
        UI8 getMsgType()  const;
        UI8 getCmdCode() const;
        bool getEnable() const;
        string getVrfId() const;
        UI32 getMaxMcache() const;
        SI16 getHelloTimer() const;
        SI16 getNbrTo() const;
        SI16 getInactTimer() const;
        SI32 getMsgIntv() const;
        UI32 getSptThold() const;
        
        virtual const void *getCStructureForInputs ();
        virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
        
    /* Data member declarations */
    private:
    protected:
    public:
        UI8 m_msgType;
        UI8 m_cmdCode;
        string m_vrfId;
        bool m_enable;
        UI32 m_maxMcache;
        SI16 m_helloTimer;
        SI16 m_nbrTo;
        SI16 m_inactTimer;
        SI32 m_msgIntv;
        UI32 m_sptThold;
};
} /* namespace */
#endif /* _PIMRTRCFGCLIENTMSG_H_ */
