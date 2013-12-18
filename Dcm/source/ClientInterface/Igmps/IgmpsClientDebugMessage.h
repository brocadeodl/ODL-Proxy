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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#ifndef IGMPSCLIENTDEBUGMESSAGE_H 
#define IGMPSCLIENTDEBUGMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsClientDebugMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IgmpsClientDebugMessage();
            IgmpsClientDebugMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, const bool &isNegation);
            virtual ~IgmpsClientDebugMessage();
            void setGrpName(const string &grpName);
            string getGrpName() const;
            void setIfName (const string &ifName);
            string getIfName ()  const;
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode ()  const;
            void setNegation (const bool &isNegation);
            bool getNegation ()  const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

        private:
        protected:
        public:
            string  m_grpName;
            string  m_ifName;
            UI32    m_cmdCode;
            bool    m_isNegation;
    };
}
#endif  //IGMPSCLIENTDEBUGMESSAGE_H

