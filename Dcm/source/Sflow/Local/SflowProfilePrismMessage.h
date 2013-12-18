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

/*
 * SflowProfilePrismMessage.h
 *
 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Sep 17, 2012
 *      Author: sunniram
 */

#ifndef SFLOWPROFILEPRISMMESSAGE_H_
#define SFLOWPROFILEPRISMMESSAGE_H_

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowProfilePrismMessage : public PrismMessage
    {
    private:
    protected:
        virtual void setupAttributesForSerialization();

    public:
        SflowProfilePrismMessage ();
        SflowProfilePrismMessage (const string &name,const UI32 &samplingRate,const UI32 &opCode);
        virtual    ~SflowProfilePrismMessage ();

        void setProfileName(const string &name);
        string getProfileName()  const;
        void setSamplingRate(const UI32 &samplingRate);
        UI32 getSamplingRate()  const;

        void setOpCode(const UI32 &opCode);
        UI32 getOpCode()  const;


    private:
    protected:
    public:
        string  m_name;
        UI32  m_samplingRate;
        UI32  m_opCode;
    };
}


#endif /* SFLOWPROFILEPRISMMESSAGE_H_ */
