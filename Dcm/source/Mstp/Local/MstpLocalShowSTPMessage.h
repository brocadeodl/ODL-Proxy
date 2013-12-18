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
 *   Author : Sudheendra                                                   *
 **************************************************************************/

#ifndef MSTPLOCALSHOWSTPMESSAGE_H
#define MSTPLOCALSHOWSTPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class MstpLocalShowSTPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MstpLocalShowSTPMessage ();
            MstpLocalShowSTPMessage (MstpLocalShowSTPMessage *other);
            virtual    ~MstpLocalShowSTPMessage ();

            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            string getIfName()  const;
            InterfaceType getIfType() const;
            void setInterfaceInfo (InterfaceType ifType, const string &ifName);

            void setLastIndex (UI32 lastInstId);

            LocationId getLocationId() const;
            void setLocationId(LocationId locationId);

            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;

            void getOutputBufferRef (UI32   bufferNum,UI32 &size, void *&pBuffer);

            // Now the data members

        private:
        protected:
        public:
            bool m_isGwRequest;
            LocationId m_locationId;

            UI32  m_cmdCode;
            InterfaceType m_ifType;
            string  m_ifName;
            UI32 m_instId;

            UI32 m_reqType;
            UI32 m_lastInstId;

    };
}
#endif                                            //MSTPLOCALSHOWSTPMESSAGE_H
