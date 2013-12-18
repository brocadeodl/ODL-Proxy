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

#ifndef QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAPMESSAGE_H
#define QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAPMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosClientCreateDscpToTrafficClassMapMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosClientCreateDscpToTrafficClassMapMessage ();
            QosClientCreateDscpToTrafficClassMapMessage (const string &ceeMapName);
            virtual    ~QosClientCreateDscpToTrafficClassMapMessage ();
            void setDscpTrafficClassMapName(const string &dscpTrafficClassMapName);
            string getDscpTrafficClassMapName()  const;
			void setConfigReplay(const SI32 &configReplay);
			SI32 getConfigReplay() const;
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
			SI32 m_configReplay;
        protected:
        public:
            string  m_dscpTrafficClassMapName;
    };
}
#endif                                          //QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAPMESSAGE_H
