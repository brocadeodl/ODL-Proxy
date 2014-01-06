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

#ifndef QOSCLIENTDELETEPRIORITYGROUPMESSAGE_H
#define QOSCLIENTDELETEPRIORITYGROUPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientDeletePriorityGroupMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosClientDeletePriorityGroupMessage ();
            QosClientDeletePriorityGroupMessage (const string &ceeMapName,const string &pgId);
            virtual    ~QosClientDeletePriorityGroupMessage ();
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setPgId(const string &pgId);
            string getPgId()  const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            string  m_ceeMapName;
            string  m_pgId;
    };
}
#endif                                            //QOSCLIENTDELETEPRIORITYGROUPMESSAGE_H
