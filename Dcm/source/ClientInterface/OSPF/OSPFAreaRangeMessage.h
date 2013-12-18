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
 **************************************************************************/

#ifndef OSPFAreaRangeMessage_H
#define OSPFAreaRangeMessage_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{
    class OSPFAreaRangeMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFAreaRangeMessage ();
            virtual    ~OSPFAreaRangeMessage ();

            void setAreaId(const string &id);
            string getAreaId()  const;

            void setIpAddr(const IpV4Address &ip);
            IpV4Address getIpAddr()  const;

            void setIpMask(const IpV4Address &ip);
            IpV4Address getIpMask()  const;

            void setRangeEffect(const UI32 &effect);
            UI32 getRangeEffect()  const;

            void setRangeCost(const UI32 &cost);
            UI32 getRangeCost()  const;

			void setCmdCode(unsigned int code);
			unsigned int getCmdCode();

			void setOpCode(const OSPFOpCode &opCode);
			OSPFOpCode getOpCode() const;

			string getVRF() const;
			void setVRF(const string &vrfName);

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
			string m_areaId;
			IpV4Address m_ipAddr;
			IpV4Address m_ipMask;
            UI32 m_rangeEffect;
            UI32 m_rangeCost;
			UI32 m_cmdCode;
			OSPFOpCode m_opCode;
			string m_vrf;

        protected:
        public:
    };
}
#endif                                            //OSPFAreaRangeMessage_H
