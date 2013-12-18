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

#ifndef IGMPSCLIENTCLEARCONFIG_H 
#define IGMPSCLIENTCLEARCONFIG_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class IgmpsClientClearMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IgmpsClientClearMessage();
            IgmpsClientClearMessage(const string &grpName, const string &ifName, const UI32 &cmdCode);
            virtual ~IgmpsClientClearMessage();
            void setGrpName(const string &grpName);
            string getGrpName() const;
            void setIfName(const string &ifName);
            string getIfName() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_grp_name;
            string m_if_name;
            UI32 m_cmdCode;
    };
}
#endif  //IGMPSCLIENTCLEARCONFIG_H
