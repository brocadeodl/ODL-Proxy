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
 **************************************************************************/

#ifndef OSPFAREALOCALMESSAGE_H
#define OSPFAREALOCALMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    //class OSPFAreaLocalMessage : public ManagementInterfaceMessage
    class OSPFAreaLocalMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            OSPFAreaLocalMessage ();
            virtual    ~OSPFAreaLocalMessage ();

            void setAreaId(const string &id);
            string getAreaId()  const;
            void setOpCode(const unsigned int &opCode);
            unsigned int getOpCode()  const;
            void setCmd(const unsigned int &action);
            unsigned int getCmd()  const;
            void setNssa(const unsigned int &nssa);
            unsigned int getNssa()  const;
            void setIsNssa(const bool &is_nssa);
            bool getIsNssa() const;
            void setIsNssaNoSummary(const bool &is_nssa_no_summary);
            bool getIsNssaNoSummary() const;
            void setIsNssaDefInfoOrig(const bool &is_nssa_def_info_orig);
            bool getIsNssaDefInfoOrig() const;
            void setStub(const unsigned int &stub);
            unsigned int getStub()  const;
            void setIsStubNoSummary(const bool &is_stub_no_summary);
            bool getIsStubNoSummary() const;
			void loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
			string m_areaId;
            unsigned int m_opCode;
            unsigned int m_cmd;
            unsigned int m_nssa;
            bool m_is_nssa;
            bool m_is_nssa_no_summary;
            bool m_is_nssa_def_info_orig;
            bool m_is_stub_no_summary;
            unsigned int m_stub;
        protected:
        public:
    };
}
#endif

