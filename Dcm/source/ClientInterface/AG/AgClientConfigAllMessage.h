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
 *   Author : rhkumar                                                      *
 **************************************************************************/

#ifndef AG_CLIENT_CONFIG_ALL_MESSAGE_H
#define AG_CLIENT_CONFIG_ALL_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "agd/public.h"

using namespace WaveNs;

namespace DcmNs
{
    class AgClientConfigAllMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            AgClientConfigAllMessage ();
            AgClientConfigAllMessage (const vector<string> &policy, const bool &fportTrunkEnable, const UI32 &fnmtov, const UI32 &relcountervalue);
            virtual ~AgClientConfigAllMessage ();
            void addNportMap(const string &nPort, const vector<string> &fPortTopo, const bool &failover, const bool &failback);
            void addPortGroup(const UI32 &pgId, const bool &pgEnable, const UI32 &pgFlags, const string &pgName,
                       const vector<string> &pgNPortTopo, const vector<string> &pgFPortTopo);
            void updatePortBitmap(BitMap_t bm, vector<string> strVec, UI32 offsetBegin, UI32 offsetEnd);

            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            // AG Global info
            vector<string> m_policy;
            bool m_fportTrunkEnable;  
            UI32 m_fnmtov;
            UI32 m_relcountervalue;

            // N Portmaps
            vector<string> m_nPort;  
            vector<UI32>   m_offset;  // Offset indicates the number of Fports for a given Nport in the FportTopo vector
            vector<string> m_fPortTopo;
            vector<bool>   m_failover; 
            vector<bool>   m_failback; 

            // Portgroups
            vector<UI32>   m_pgId;
            vector<bool>   m_pgEnable;
            vector<UI32>   m_pgFlags;
//            vector<string>   m_pgFlags;
            vector<string> m_pgName;
            vector<UI32>   m_pgNPortOffset;
            vector<string> m_pgNPortTopo;
            vector<UI32>   m_pgFPortOffset;
            vector<string> m_pgFPortTopo;
    };
}
#endif
