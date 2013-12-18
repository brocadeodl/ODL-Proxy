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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author Debojyoti                                                      *
 **************************************************************************/

#ifndef PBRPHYINTFMESSAGE_H
#define PBRPHYINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ssmAcl_dcmd_public.h"

using namespace WaveNs;

namespace DcmNs
{

    class PBRPhyIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PBRPhyIntfMessage ();
            PBRPhyIntfMessage (const string &ifName, const UI8 &ifType, const string &routeMapName, const UI32 &cmdCode);
            virtual    ~PBRPhyIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfType(const UI8 &ifType);
            UI8 getIfType()  const;
            void setRouteMapName(const string &aclName);
            string getRouteMapName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

// Now the data members

        private:
        protected:
        public:
			string                      m_ifName;
            UI8                         m_ifType;
            string                      m_routeMapName;
            UI32                        m_cmdCode;
    };
}
#endif                                            //PBRPHYINTFMESSAGE_H

