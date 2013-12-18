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

#ifndef CEEGLOBALCREATEPRIORITYGROUPMESSAGE_H
#define CEEGLOBALCREATEPRIORITYGROUPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class CeeGlobalCreatePriorityGroupMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            CeeGlobalCreatePriorityGroupMessage ();
            CeeGlobalCreatePriorityGroupMessage (const string &ceeMapName,const string &pgId,const SI32 &weight,const SI32 &pfc);
            virtual    ~CeeGlobalCreatePriorityGroupMessage ();
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setPgId(const string &pgId);
            string getPgId()  const;
            void setWeight(const SI32 &weight);
            SI32 getWeight()  const;
            void setPfc(const SI32 &pfc);
            SI32 getPfc()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ceeMapName;
            string  m_pgId;
            SI32  m_weight;
            SI32  m_pfc;
    };
}
#endif                                            //CEEGLOBALCREATEPRIORITYGROUPMESSAGE_H
