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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : agidwani                                                     *
 **************************************************************************/

#ifndef IGMPINTFLOCALCONFIGMESSAGE_H
#define IGMPINTFLOCALCONFIGMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class IgmpPoSviIntfLocalConfigMessage : public PrismMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        IgmpPoSviIntfLocalConfigMessage ();
        IgmpPoSviIntfLocalConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval,const string &ifName,const UI64 &ifIndex, const UI32 &ifType, const UI32 &opCode,const bool &negation);
        virtual    ~IgmpPoSviIntfLocalConfigMessage ();
        void setLmqi(const UI32 &lmqi);
        UI32 getLmqi()  const;
        void setQmrt(const UI32 &qmrt);
        UI32 getQmrt()  const;
        void setImmediateLeave(const bool &immediateLeave);
        bool getImmediateLeave()  const;
        void setQueryInterval(const UI32 &queryInterval);
        UI32 getQueryInterval()  const;
        void setIfName(const string &ifName);
        string getIfName()  const;
        void setIfIndex(const UI64 &ifIndex);
        UI64 getIfIndex()  const;
        void setIfType(const UI32 &ifType);
        UI32 getIfType() const;
        void setOpCode(const UI32 &opCode);
        UI32 getOpCode()  const;
        void setNegation(const bool &negation);
        bool getNegation()  const;

// Now the data members

    private:
    protected:
    public:
        UI32  m_lmqi;
        UI32  m_qmrt;
        bool  m_immediateLeave;
        UI32  m_queryInterval;
        string  m_ifName;
        UI64  m_ifIndex;
        UI32  m_ifType;
        UI32  m_opCode;
        bool  m_negation;
};
}
#endif                                            //IGMPINTFLOCALCONFIGMESSAGE_H
