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
 **************************************************************************/

#ifndef PimShowMcacheMsg_H
#define PimShowMcacheMsg_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimShowMcacheMsg : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PimShowMcacheMsg ();
            PimShowMcacheMsg (const UI8 &cmdCode, const UI8 &firstPg, 
                const IpV4Address &ipAddr1, const IpV4Address &ipAddr2,
                const UI32 &nxtSrc, const UI32 &nxtGrp);
            virtual ~PimShowMcacheMsg ();

            void setCmdCode(const UI8 &cmdCode);
            void setFirstPg(const UI8 &firstPg);
            void setIpAddr1(const IpV4Address &ipAddr);
            void setIpAddr2(const IpV4Address &ipAddr);
            void setNxtSrc(const UI32 &nxtSrc);
            void setNxtGrp(const UI32 &nxtGrp);

            UI8 getCmdCode()  const;
            UI8 getFirstPg()  const;
            IpV4Address getIpAddr1() const;
            IpV4Address getIpAddr2() const;
            UI32 getNxtSrc()  const;
            UI32 getNxtGrp()  const;

            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        /* Data member declarations */
        private:
        protected:
        public:
            UI8 m_cmd;
            UI8 m_firstPg;
            IpV4Address m_ipAddr1;
            IpV4Address m_ipAddr2;
            UI32 m_nxtSrc;
            UI32 m_nxtGrp;
    };
}
#endif /* PimShowMcacheMsg_H */
