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

#ifndef QOSCREATECEEMAPSPMESSAGE_H
#define QOSCREATECEEMAPSPMESSAGE_H

//#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreateCeeMapSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreateCeeMapSPMessage ();
            QosCreateCeeMapSPMessage (const string &ceeMapName,const SI32 &precedence);
            virtual    ~QosCreateCeeMapSPMessage ();
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setPrecedence(const SI32 &precedence);
            SI32 getPrecedence()  const;
			void setConfigReplay(const SI32 &configReplay);
			SI32 getConfigReplay() const;

// Now the data members

        private:
			SI32 m_configReplay;
        protected:
        public:
            string  m_ceeMapName;
            SI32  m_precedence;
    };
}
#endif                                            //QOSCREATECEEMAPSPMESSAGE_H
