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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMLOCALSHOWINTFMESSAGE_H
#define NSMLOCALSHOWINTFMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Nsm/Local/PoPaginationLocalMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#define INTF_PER_PAGE 10

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalShowIntfMessage : public PoPaginationLocalMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalShowIntfMessage ();
            NsmLocalShowIntfMessage (const string &ifName,const UI32 &cmdCode,const bool isGwRequest, const SI32 locationId = -1);
            virtual    ~NsmLocalShowIntfMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setLocationId(const SI32 &locationId);
            SI32 getLocationId()  const;
            UI32 getReqNoRecords()  const;
            void setReqNoRecords(const UI32 &reqNoRecords);
            string getReqStartIndex()  const;
            void setReqStartIndex(const string &reqStartIndex);
            string getReqEndIndex()  const;
            void setReqEndIndex(const string &reqEndIndex);
            InterfaceType getReqIfType()  const;
            void setReqIfType(const InterfaceType &reqIfType);
            UI32 getReqRequestType()  const;
            void setReqRequestType(const UI32 &reqRequestType);

            //void setIsGwRequest (bool isGwRequest);
			//bool getIsGwRequest ()  const;

			void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);
			void copyAllBuffers (const PrismMessage &prismMessage);

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            UI32  m_cmdCode;
			//bool m_isGwRequest;
			SI32 m_locationId;
			UI32 m_reqNoRecords;
			string m_reqStartIndex;
			string m_reqEndIndex;
			InterfaceType m_reqIfType;
			UI32 m_reqRequestType;
    };
}
#endif                                            //NSMSHOWINTFMESSAGE_H
