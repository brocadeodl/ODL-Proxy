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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTCFGUPDATEFCINFOMESSAGE_H
#define PORTCFGUPDATEFCINFOMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgUpdateFCInfoMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgUpdateFCInfoMessage ();
            PortCfgUpdateFCInfoMessage (const string &id,const UI32 &speed,const UI32 &fillWord,const UI32 &longDistance,const UI32 &vcLinkInit,const bool &islRRdyMode,const UI32 &distance,const UI32 &dbEntryType, const bool &status, const UI32 &respCode,const UI32 &lock_port);
            virtual    ~PortCfgUpdateFCInfoMessage ();
            void setId(const string &id);
            string getId()  const;
            void setSpeed(const UI32 &speed);
            UI32 getSpeed()  const;
            void setFillWord(const UI32 &fillWord);
            UI32 getFillWord()  const;
            void setLongDistance(const UI32 &longDistance);
            UI32 getLongDistance()  const;
            void setVcLinkInit(const UI32 &vcLinkInit);
            UI32 getVcLinkInit()  const;
            void setIslRRdyMode(const bool &islRRdyMode);
            bool getIslRRdyMode()  const;
            void setDistance(const UI32 &distance);
            UI32 getDistance()  const;
            void setDbEntryType(const UI32 &dbEntryType);
            UI32 getDbEntryType()  const;
            void setStatus(const bool &status);
            bool getStatus()  const;
            void setRespCode(const UI32 &respCode);
            UI32 getRespCode()  const;
            void setLock_port(const UI32 &lock_port);
            UI32 getLock_port()  const;
// Now the data members

        private:
        protected:
        public:
            string  m_id;
            UI32  m_speed;
            UI32  m_fillWord;
            UI32  m_longDistance;
            UI32  m_vcLinkInit;
            bool  m_islRRdyMode;
            UI32  m_distance;
            UI32  m_dbEntryType;
            bool  m_status;
            UI32  m_respCode;
            UI32  m_lock_port;
    };
}
#endif                                            //PORTCFGUPDATEFCINFOMESSAGE_H
