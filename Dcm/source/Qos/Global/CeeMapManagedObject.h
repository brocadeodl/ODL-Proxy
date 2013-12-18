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

#ifndef CEEMAPMANAGEDOBJECT_H
#define CEEMAPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

using namespace WaveNs;

namespace DcmNs
{

    class QosGlobalObjectManager;
    class PriorityGroupManagedObject;
    class PriorityMapManagedObject;
    class CeeMapManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            CeeMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager);
            CeeMapManagedObject (QosGlobalObjectManager *pQosGlobalObjectManager,const string &ceeMapName,const SI32 &precedence,const vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &priorityGroup,const WaveManagedObjectPointer<PriorityMapManagedObject> &priorityMap);
            virtual    ~CeeMapManagedObject ();
            static string getClassName();
            void setCeeMapName(const string &ceeMapName);
            string getCeeMapName()  const;
            void setPrecedence(const SI32 &precedence);
            SI32 getPrecedence()  const;
            void setPriorityGroup(const vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > &priorityGroup);
            vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > getPriorityGroup()  const;
            void setPriorityMap(const WaveManagedObjectPointer<PriorityMapManagedObject> &priorityMap);
            WaveManagedObjectPointer<PriorityMapManagedObject> getPriorityMap()  const;
            void setRemapFabricPriority (const SI32 &remapFabricPriority);
            SI32 getRemapFabricPriority ()  const;
            void setRemapLosslessPriority (const SI32 &remapLosslessPriority);
            SI32 getRemapLosslessPriority ()    const;
			int delPriorityGroup(const string &pgid);
			void addPriorityGroup(PriorityGroupManagedObject *pPriorityGroupManagedObject); 
			void updatePriorityMap(const int &cos, const ObjectId &PriorityGroup);
			WaveManagedObjectPointer<PriorityGroupManagedObject>* findPriorityGroupByName(const string &pgId);
// Now the data members

        private:
        protected:
        public:
            string  m_ceeMapName;
            SI32  m_precedence;
            vector<WaveManagedObjectPointer<PriorityGroupManagedObject> >  m_priorityGroup;
            WaveManagedObjectPointer<PriorityMapManagedObject>  m_priorityMap;
			SI32  m_remapFabricPriority;
			SI32  m_remapLosslessPriority;
    };
}
#endif                                            //CEEMAPMANAGEDOBJECT_H
