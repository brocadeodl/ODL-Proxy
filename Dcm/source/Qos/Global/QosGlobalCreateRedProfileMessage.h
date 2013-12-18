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

#ifndef QOSGLOBALCREATEREDPROFILEMESSAGE_H
#define QOSGLOBALCREATEREDPROFILEMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalCreateRedProfileMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            QosGlobalCreateRedProfileMessage ();
            QosGlobalCreateRedProfileMessage (const SI32 &profileId,const SI32 &minThreshold,const SI32 &maxThreshold,const SI32 &dropProbability);
            virtual    ~QosGlobalCreateRedProfileMessage ();
            void setProfileId(const SI32 &profileId);
            SI32 getProfileId()  const;
            void setMinThreshold(const SI32 &minThreshold);
            SI32 getMinThreshold()  const;
            void setMaxThreshold(const SI32 &maxThreshold);
            SI32 getMaxThreshold()  const;
            void setDropProbability(const SI32 &dropProbability);
            SI32 getDropProbability()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_profileId;
            SI32  m_minThreshold;
            SI32  m_maxThreshold;
            SI32  m_dropProbability;
    };
}
#endif                                            //QOSGLOBALCREATEREDPROFILEMESSAGE_H
