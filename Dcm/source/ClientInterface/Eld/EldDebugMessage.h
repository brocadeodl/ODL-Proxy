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

#ifndef ELDDEBUGMESSSAGE_H
#define ELDDEBUGMESSSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class EldDebugMessage: public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			EldDebugMessage ();
			EldDebugMessage (const string &interface, const string &ifName,
				const string &Mac, const UI32 &vlan_id, const UI32 &type,
				const UI32 &cmdcode, const UI32 &bridge, const UI32 &ifindex,
				const UI32 &interfaceType, const UI32 &portChannelId,const UI32 &mappedid);
			virtual ~EldDebugMessage ();
			void setCmdCode(const UI32 &cmdcode);
			UI32 getCmdCode() const;
			void setVlanid(const UI32 &vlan_id);
			UI32 getVlanid() const;
			void setRbridge(const UI32 &rbridge_id);
			UI32 getRbridge() const;
			void setInterface(const string &interface);
			string getInterface() const;
			void setDbgFlags (const UI32 &dbgFlags);
			UI32 getDbgFlags ()  const;

			virtual const void *getCStructureForInputs ();
			virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            UI32 m_cmdcode;
            UI32 m_vlanid;
            string m_interface;
			UI32 m_rbridge;
			UI32    m_dbgFlags;
    };
}
#endif  //ELDDEBUGMESSSAGE_H
