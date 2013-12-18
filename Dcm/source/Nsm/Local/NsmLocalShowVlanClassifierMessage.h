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
 *   Author : Raghuprem                                                     *
 **************************************************************************/

#ifndef NSMLOCALSHOWVLANCLASSIFIERMESSAGE_H
#define NSMLOCALSHOWVLANCLASSIFIERMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalShowVlanClassifierMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalShowVlanClassifierMessage ();
            NsmLocalShowVlanClassifierMessage (const string &backEndData,const UI32 &cmdCode,const bool isGwRequest, const SI32 locationId = -1);
            virtual    ~NsmLocalShowVlanClassifierMessage ();
            void setBackEndData(const string &backEndData);
            string getBackEndData()  const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

			SI32 getLocationId() const;
			void setLocationId(const SI32 &locationId);

            void setIsGwRequest (bool isGwRequest);
			bool getIsGwRequest ()  const;
            void setLastIndex(const UI32 &lastIndex);
            UI32 getLastIndex()  const;

			void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);
			void copyAllBuffers (const PrismMessage &prismMessage);

// Now the data members

        private:
        protected:
        public:
            string  m_backEndData;
            UI32  m_cmdCode;
			bool m_isGwRequest;
			SI32 m_locationId;
			UI32 m_lastIndex;
    };
}
#endif                                            //NSMSHOWINTFMESSAGE_H
