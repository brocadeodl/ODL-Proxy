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

#ifndef COSTOCOSMUTATIONMAPMANAGEDOBJECT_H
#define COSTOCOSMUTATIONMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class CosToCosMutationMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CosToCosMutationMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            CosToCosMutationMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &cosToCosMutationMapName,const SI32 &cos0MutatedCosValue,const SI32 &cos1MutatedCosValue,const SI32 &cos2MutatedCosValue,const SI32 &cos3MutatedCosValue,const SI32 &cos4MutatedCosValue,const SI32 &cos5MutatedCosValue,const SI32 &cos6MutatedCosValue,const SI32 &cos7MutatedCosValue);
            virtual    ~CosToCosMutationMapManagedObject ();
            static string getClassName();
            void setCosToCosMutationMapName(const string &cosToCosMutationMapName);
            string getCosToCosMutationMapName()  const;
            void setCos0MutatedCosValue(const SI32 &cos0MutatedCosValue);
            SI32 getCos0MutatedCosValue()  const;
            void setCos1MutatedCosValue(const SI32 &cos1MutatedCosValue);
            SI32 getCos1MutatedCosValue()  const;
            void setCos2MutatedCosValue(const SI32 &cos2MutatedCosValue);
            SI32 getCos2MutatedCosValue()  const;
            void setCos3MutatedCosValue(const SI32 &cos3MutatedCosValue);
            SI32 getCos3MutatedCosValue()  const;
            void setCos4MutatedCosValue(const SI32 &cos4MutatedCosValue);
            SI32 getCos4MutatedCosValue()  const;
            void setCos5MutatedCosValue(const SI32 &cos5MutatedCosValue);
            SI32 getCos5MutatedCosValue()  const;
            void setCos6MutatedCosValue(const SI32 &cos6MutatedCosValue);
            SI32 getCos6MutatedCosValue()  const;
            void setCos7MutatedCosValue(const SI32 &cos7MutatedCosValue);
            SI32 getCos7MutatedCosValue()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_cosToCosMutationMapName;
            SI32  m_cos0MutatedCosValue;
            SI32  m_cos1MutatedCosValue;
            SI32  m_cos2MutatedCosValue;
            SI32  m_cos3MutatedCosValue;
            SI32  m_cos4MutatedCosValue;
            SI32  m_cos5MutatedCosValue;
            SI32  m_cos6MutatedCosValue;
            SI32  m_cos7MutatedCosValue;
    };
}
#endif                                            //COSTOCOSMUTATIONMAPMANAGEDOBJECT_H
