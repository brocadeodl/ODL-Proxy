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
 *   Author : nbellari*
 **************************************************************************/

#ifndef QOSCLIENTCREATECONFIGREPLAYDSCPCOSMAPMESSAGE_H
#define QOSCLIENTCREATECONFIGREPLAYDSCPCOSMAPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientConfigReplayDscpCosMapMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosClientConfigReplayDscpCosMapMessage();
            QosClientConfigReplayDscpCosMapMessage(const string &dscpMapName);
            virtual    ~QosClientConfigReplayDscpCosMapMessage();
			void initialize();
            void setDscpCosMapName(const string &dscpCosMapName);
            string getDscpCosMapName()  const;
			void setDscpMarkListValues(const string &dscpMarkList, const SI32 &toValue);
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        protected:
        public:
            string  m_dscpCosMapName;
			vector<UI32> m_map;
    };
}
#endif /* QOSCLIENTCREATECONFIGREPLAYDSCPMUTATIONMAPMESSAGE_H */
