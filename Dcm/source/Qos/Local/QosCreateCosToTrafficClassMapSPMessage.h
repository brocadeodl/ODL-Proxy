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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSCREATECOSTOTRAFFICCLASSMAPSPMESSAGE_H
#define QOSCREATECOSTOTRAFFICCLASSMAPSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreateCosToTrafficClassMapSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreateCosToTrafficClassMapSPMessage ();
            QosCreateCosToTrafficClassMapSPMessage (const string &cosToTrafficClassMapName,const SI32 &cos0MappedTrafficClassValue,const SI32 &cos1MappedTrafficClassValue,const SI32 &cos2MappedTrafficClassValue,const SI32 &cos3MappedTrafficClassValue,const SI32 &cos4MappedTrafficClassValue,const SI32 &cos5MappedTrafficClassValue,const SI32 &cos6MappedTrafficClassValue,const SI32 &cos7MappedTrafficClassValue);
            virtual    ~QosCreateCosToTrafficClassMapSPMessage ();
            void setCosToTrafficClassMapName(const string &cosToTrafficClassMapName);
            string getCosToTrafficClassMapName()  const;
            void setCos0MappedTrafficClassValue(const SI32 &cos0MappedTrafficClassValue);
            SI32 getCos0MappedTrafficClassValue()  const;
            void setCos1MappedTrafficClassValue(const SI32 &cos1MappedTrafficClassValue);
            SI32 getCos1MappedTrafficClassValue()  const;
            void setCos2MappedTrafficClassValue(const SI32 &cos2MappedTrafficClassValue);
            SI32 getCos2MappedTrafficClassValue()  const;
            void setCos3MappedTrafficClassValue(const SI32 &cos3MappedTrafficClassValue);
            SI32 getCos3MappedTrafficClassValue()  const;
            void setCos4MappedTrafficClassValue(const SI32 &cos4MappedTrafficClassValue);
            SI32 getCos4MappedTrafficClassValue()  const;
            void setCos5MappedTrafficClassValue(const SI32 &cos5MappedTrafficClassValue);
            SI32 getCos5MappedTrafficClassValue()  const;
            void setCos6MappedTrafficClassValue(const SI32 &cos6MappedTrafficClassValue);
            SI32 getCos6MappedTrafficClassValue()  const;
            void setCos7MappedTrafficClassValue(const SI32 &cos7MappedTrafficClassValue);
            SI32 getCos7MappedTrafficClassValue()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_cosToTrafficClassMapName;
            SI32  m_cos0MappedTrafficClassValue;
            SI32  m_cos1MappedTrafficClassValue;
            SI32  m_cos2MappedTrafficClassValue;
            SI32  m_cos3MappedTrafficClassValue;
            SI32  m_cos4MappedTrafficClassValue;
            SI32  m_cos5MappedTrafficClassValue;
            SI32  m_cos6MappedTrafficClassValue;
            SI32  m_cos7MappedTrafficClassValue;
    };
}
#endif                                            //QOSCREATECOSTOTRAFFICCLASSMAPSPMESSAGE_H
