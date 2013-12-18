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

#ifndef PRIORITYGROUPMANAGEDOBJECT_H
#define PRIORITYGROUPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class PriorityGroupManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PriorityGroupManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            PriorityGroupManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &pgId,const SI32 &weight,const SI32 &pfc, const string &ceeMapName);
            virtual    ~PriorityGroupManagedObject ();
            static string getClassName();
            void setPgId(const string &pgId);
            string getPgId()  const;
            void setWeight(const SI32 &weight);
            SI32 getWeight()  const;
            void setPfc(const SI32 &pfc);
            SI32 getPfc()  const;
			void setCeeMapName(const string &ceeMapName);
			string getCeeMapName()	const;

// Now the data members

        private:
        protected:
        public:
            string  m_pgId;
            SI32  m_weight;
            SI32  m_pfc;
			string  m_ceeMapName;
    };
}
#endif                                            //PRIORITYGROUPMANAGEDOBJECT_H
