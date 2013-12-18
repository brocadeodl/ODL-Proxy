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
 *   Author : Sudha
 **************************************************************************/

#ifndef POLICERLOCALSHOWMESSAGE_H
#define POLICERLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PolicerLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PolicerLocalShowMessage ();
            PolicerLocalShowMessage(const string &Interface, const string &ifName,
			const UI32 &direction, const UI32 &cmdCode, bool isGwRequest,
			const string &policyMapName, const UI32 &locationId);

            virtual ~PolicerLocalShowMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setInterface(const string &interface);
            string getInterface() const;
			void setDirection(const UI32 &direction);
            UI32 getDirection() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;
            string getPolicyMapName () const;
            void setPolicyMapName (const string &policyMapName);
            void setLocationId(const UI32 &locationId);
            UI32 getLocationId() const;
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
            void transferOutputBuffer(PolicerLocalShowMessage *&pMsg);

        private:
        protected:
        public:
            string m_interface;
            string m_ifName;
			UI32 m_direction;
            UI32 m_cmdCode;
            bool    m_isGwRequest;
            string m_policyMapName;
            UI32 m_locationId;
    };
}
#endif
