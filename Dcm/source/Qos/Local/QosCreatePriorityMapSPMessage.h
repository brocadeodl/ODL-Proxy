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

#ifndef QOSCREATEPRIORITYMAPSPMESSAGE_H
#define QOSCREATEPRIORITYMAPSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosCreatePriorityMapSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosCreatePriorityMapSPMessage ();
            QosCreatePriorityMapSPMessage (const string &ceeMapName,const string &cos0ToPriorityGrouMap,const string &cos1ToPriorityGrouMap,const string &cos2ToPriorityGrouMap,const string &cos3ToPriorityGrouMap,const string &cos4ToPriorityGrouMap,const string &cos5ToPriorityGrouMap,const string &cos6ToPriorityGrouMap,const string &cos7ToPriorityGrouMap);
            virtual    ~QosCreatePriorityMapSPMessage ();
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setCos0ToPriorityGrouMap(const string &cos0ToPriorityGrouMap);
            string getCos0ToPriorityGrouMap()  const;
            void setCos1ToPriorityGrouMap(const string &cos1ToPriorityGrouMap);
            string getCos1ToPriorityGrouMap()  const;
            void setCos2ToPriorityGrouMap(const string &cos2ToPriorityGrouMap);
            string getCos2ToPriorityGrouMap()  const;
            void setCos3ToPriorityGrouMap(const string &cos3ToPriorityGrouMap);
            string getCos3ToPriorityGrouMap()  const;
            void setCos4ToPriorityGrouMap(const string &cos4ToPriorityGrouMap);
            string getCos4ToPriorityGrouMap()  const;
            void setCos5ToPriorityGrouMap(const string &cos5ToPriorityGrouMap);
            string getCos5ToPriorityGrouMap()  const;
            void setCos6ToPriorityGrouMap(const string &cos6ToPriorityGrouMap);
            string getCos6ToPriorityGrouMap()  const;
            void setCos7ToPriorityGrouMap(const string &cos7ToPriorityGrouMap);
            string getCos7ToPriorityGrouMap()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ceeMapName;
            string  m_cos0ToPriorityGrouMap;
            string  m_cos1ToPriorityGrouMap;
            string  m_cos2ToPriorityGrouMap;
            string  m_cos3ToPriorityGrouMap;
            string  m_cos4ToPriorityGrouMap;
            string  m_cos5ToPriorityGrouMap;
            string  m_cos6ToPriorityGrouMap;
            string  m_cos7ToPriorityGrouMap;
    };
}
#endif                                            //QOSCREATEPRIORITYMAPSPMESSAGE_H
