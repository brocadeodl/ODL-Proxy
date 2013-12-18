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
 *   Author : Ankur Sharma                                                 *
 **************************************************************************/

#ifndef APPMQOSPROFILEMESSAGE_H
#define APPMQOSPROFILEMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class APPMQosProfileMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            APPMQosProfileMessage ();
            APPMQosProfileMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const string &qosMapName,const UI32 &qosCos,const UI32 &tx,const UI32 &rx ,const UI32 &profileId);
            virtual    ~APPMQosProfileMessage ();
            void setOpCode(const UI8 &opCode);
            UI8 getOpCode()  const;
            void setCmdCode(const UI8 cmdCode);
            UI8 getCmdCode()  const;
            void setQosProfileName(const string &profileName);
            string getQosProfileName()  const;
            void setQosMapName(const string &qosMapName);
            string getQosMapName()  const;
            void setQosCos(const UI32 qosCos);
            UI32 getQosCos() const;
            void setQosTx(const UI32 tx);
            UI32 getQosTx() const;
            void setQosRx(const UI32 tx);
            UI32 getQosRx() const;
            
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;
            
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            UI8  m_opCode;
            UI8  m_cmdCode;
            string  m_profileName;
            string m_qosMapName;
            UI32  m_qosCos;
            UI32 m_tx;
            UI32 m_rx;
            UI32  m_profileId;
    };
}
#endif //APPMPROFILEMESSAGE_H
