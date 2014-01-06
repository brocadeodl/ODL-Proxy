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
 *   Author : bgangise, asharma                                            *
 **************************************************************************/

#ifndef APPMLOCALAPPMFCOEMESSAGE_H
#define APPMLOCALAPPMFCOEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMLocalAPPMFcoeMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMLocalAPPMFcoeMessage ();
            APPMLocalAPPMFcoeMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const bool &fcoePort);
            virtual    ~APPMLocalAPPMFcoeMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 &cmdCode);
            UI8 getCmdCode()  const;
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;
            void setFcoePort(const bool &fcoePort);
            bool getFcoePort()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string  m_profileName;
            UI32  m_profileId;
            bool  m_fcoePort;
    };
}
#endif                                            //APPMLOCALAPPMFCOEMESSAGE_H