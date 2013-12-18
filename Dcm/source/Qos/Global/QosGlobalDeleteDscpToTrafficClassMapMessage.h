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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nbellari                                                     *
 **************************************************************************/

#ifndef QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAPMESSAGE_H
#define QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalDeleteDscpToTrafficClassMapMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosGlobalDeleteDscpToTrafficClassMapMessage ();
            QosGlobalDeleteDscpToTrafficClassMapMessage (const string &dscpToTrafficClassMapName);
            virtual    ~QosGlobalDeleteDscpToTrafficClassMapMessage ();
            void setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName);
            string getDscpToTrafficClassMapName()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_dscpToTrafficClassMapName;
    };
}
#endif                                            //QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAPMESSAGE_H