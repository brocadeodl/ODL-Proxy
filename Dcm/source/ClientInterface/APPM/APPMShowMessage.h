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
 *   Author : Bala                                                         *
 **************************************************************************/

#ifndef APPMSHOWMESSAGE_H
#define APPMSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMShowMessage();
            APPMShowMessage(const UI32 &cmdCode, const string &ifName, const string &profileName, const UI32 &mappedId, const string &lastProfielMac, const UI32 &numOfRecords, const UI32 &fetchModelType);
            virtual ~APPMShowMessage();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setProfileName(const string &profileName);
            string getProfileName() const;
            void setPortProfileDomainName(const string &profileDomainName);
            string getPortProfileDomainName() const;			
			void  setMappedId(const UI32 &mappedId);
			UI32  getMappedId() const;

            void setLastProfileMac(const string &lastProfileMac);
            string getLastProfileMac() const;
            void setNumberOfRecords(const UI32 &numOfRecords);
            UI32 getNumberOfRecords() const;
            void setFetchModelType(const UI32 &fetchModelType);
            UI32 getFetchModelType() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

        private:
        protected:
        public:
            UI32 m_cmdCode;
            string m_ifName;
            string m_profileName;
			string m_portProfileDomainName;

            UI32 m_mappedId;
            
            string m_lastProfileMac;
            UI32 m_numOfRecords;
            UI32 m_fetchModelType;
    };
}
#endif  //APPMSHOWMESSAGE_H
