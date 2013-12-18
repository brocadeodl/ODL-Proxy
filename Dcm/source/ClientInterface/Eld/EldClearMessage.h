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
 *   Author : Amitayu Das
 **************************************************************************/

#ifndef ELDCLEARMESSSAGE_H
#define ELDCLEARMESSSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/Eld/EldLocalClearMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class EldClearMessage: public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			EldClearMessage ();
                        EldClearMessage (EldLocalClearMessage *pMsg);
			EldClearMessage (const string &interface, const string &ifName,
				const string &Mac, const UI32 &vlan_id, const UI32 &type,
				const UI32 &cmdcode, const UI32 &bridge, const UI32 &ifindex,
				const UI32 &interfaceType, const UI32 &portChannelId,const UI32 &mappedid);
			virtual ~EldClearMessage ();
			void setCmdCode(const UI32 &cmdcode);
			UI32 getCmdCode() const;
			void setIfName(const string &ifName);
			string getIfName() const;
			void setIfType(const UI32 &ifName);
			UI32 getIfType() const;


			virtual const void *getCStructureForInputs ();
			virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            UI32 m_cmdcode;
            string m_ifName;
            UI32 m_ifType;
    };
}
#endif  //ELDCLEARMESSSAGE_H
