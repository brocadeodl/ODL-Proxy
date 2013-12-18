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
 **************************************************************************/

#ifndef SSMPBRSHOWCLIENTMESSAGE_H
#define SSMPBRSHOWCLIENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class SsmPbrShowClientMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            SsmPbrShowClientMessage();
            virtual ~SsmPbrShowClientMessage();

            void   setRouteMapName  (const string &RouteMapName);
            string getRouteMapName  ()  const;

            void   setIfType      (const UI16 &ifType);
            UI16   getIfType      () const;

            void   setIfName      (const string &ifName);
            string getIfName      () const;

            void   setOpCode      (const UI32 &opCode);
            UI32   getOpCode      ()  const;

            void   setMappedId    (const UI32 &mappedId);
            UI32   getMappedId    ()  const;

            void   setLastStanza (const UI16 &lastStannza);
			UI16   getLastStanza ()  const;

			virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

        private:
        protected:
        public:
            UI32    m_opCode;
            string  m_routeMapName;
            string  m_ifName;
            UI16    m_ifType;
            UI32    m_mappedId;
			UI16    m_lastStanza;
    };
}
#endif  //SSMPBRSHOWCLIENTMESSAGE_H

