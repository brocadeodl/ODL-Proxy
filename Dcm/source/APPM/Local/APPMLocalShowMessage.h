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

#ifndef APPMLOCALSHOWMESSAGE_H
#define APPMLOCALSHOWMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class APPMShowMessage;
    class APPMLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMLocalShowMessage();
            APPMLocalShowMessage(const UI32 &cmdCode, const string &ifName, const string &profileName, bool isGwRequest, const string &lastProfileMac, const UI32 &numOfRecords, const UI32 &fetchModelType);
            virtual ~APPMLocalShowMessage();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setProfileName(const string &profileName);
            string getProfileName() const;
            void setPortProfileDomainName(const string &profileDomainName);
            string getPortProfileDomainName() const;			
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
			void setIsGwRequest(bool isGwRequest);
			bool getIsGwRequest() const;
			void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
			void transferOutputBuffer(APPMShowMessage *&pMsg);
			void copyAllBuffers (const PrismMessage &prismMessage);
			void setInterface (const string &interface);

            void setLastProfileMac(const string &lastProfileMac);
            string getLastProfileMac() const;

            void setNumberOfRecords(const UI32 &numOfRecords);
            UI32 getNumberOfRecords() const;

            void setFetchModelType(const UI32 &typeOfFetchModel);
            UI32 getFetchModelType() const;

            void setLocationId(const UI32 &locationId);
            UI32 getLocationId() const;

			string getInterface ()  const;

        private:
        protected:
        public:
            UI32 m_cmdCode;
            string m_ifName;
            string m_profileName;
			string m_portProfileDomainName;
            bool m_isGwRequest;
			string  m_interface;
            string m_lastProfileMac;
            UI32 m_numOfRecords;
            UI32 m_fetchModelType;
            UI32 m_locationId;
    };
}
#endif  //APPMLOCALSHOWMESSAGE_H
