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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#ifndef NSMSTORMCONTROLCLIENTINTFCONFSETMESSAGE_H
#define NSMSTORMCONTROLCLIENTINTFCONFSETMESSAGE_H

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmStormControlClientIntfConfSetMsg : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmStormControlClientIntfConfSetMsg ();
            NsmStormControlClientIntfConfSetMsg (const UI32 &opCode,
                const string &ifName, const UI32 &protocolType, const UI32 &rateFormat,
                const UI64 &bps, const UI32 &percentage, const UI32 &action);

            virtual    ~NsmStormControlClientIntfConfSetMsg ();

            void    setOpCode(const UI32 &opCode);
            UI32    getOpCode()  const;

            void    setIfName(const string &ifName);
            string  getIfName()  const;

            void    setProtocolType(const UI32 &protocolType);
            UI32    getProtocolType()  const;

            void    setRateFormat (const UI32 &rateFormat);
            UI32    getRateFormat()  const;

            void    setBps(const UI64 &bps);
            UI64    getBps()  const;

            void    setPercentage(const UI32 &percentage);
            UI32    getPercentage()  const;

            void    setAction(const UI32 &action);
            UI32    getAction()  const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);
			
// Now the data members
        private:
        protected:
        public:
            UI32    m_opCode;
            string  m_ifName;
            UI32    m_protocolType;
            UI32    m_rateFormat;
            UI64    m_bps;
            UI32    m_percentage;
            UI32    m_action;
    };
}
#endif                                            //NSMSTORMCONTROLINTFCONFSETMESSAGE_H
