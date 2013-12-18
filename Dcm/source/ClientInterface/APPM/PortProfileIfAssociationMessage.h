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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : statpatt														*
 ****************************************************************************/

#ifndef PORTPROFILEIFASSOCIATIONMESSAGE_H
#define PORTPROFILEIFASSOCIATIONMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs {

    class PortProfileInterfaceAssociationMessage : public DcmManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortProfileInterfaceAssociationMessage ();
            virtual ~PortProfileInterfaceAssociationMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType() const;
            void setIfName(const string &ifName);
            string getIfName() const;
            void setAssociatedPortProfiles(const vector<string> &associatedPP);
            vector<string> getAssociatedPortProfiles() const;
            void setAssociatedPortProfileDomain(const string &associatedPPDomain);
            string getAssociatedPortProfileDomain() const;

            virtual const void *getCStructureForInputs();
            virtual void loadOutputsFromCStructure(const void *pOutputCStructure);

            // Now the data members

        private:
        protected:
        public:
            UI32                m_cmdCode;
            UI32                m_ifType;
            string              m_ifName;
            vector <string>     m_associatedPortProfiles;
            string              m_associatedPortProfileDomain;
    };
}
#endif //PORTPROFILEIFASSOCIATIONMESSAGE_H

