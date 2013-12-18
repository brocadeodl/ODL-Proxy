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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#ifndef SFLOWGLOBALCONFIGMANAGEDOBJECT_H
#define SFLOWGLOBALCONFIGMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Sflow/Global/SflowCollectorManagedObject.h"

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class SflowGlobalObjectManager;
    class SflowGlobalConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            SflowGlobalConfigManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager);
            SflowGlobalConfigManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const bool &enable,const UI32 &pollingInterval,const UI32 &sampleRate,const vector<WaveManagedObjectPointer<SflowCollectorManagedObject> > &collectorList);
            virtual    ~SflowGlobalConfigManagedObject ();
            static string getClassName();
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setPollingInterval(const UI32 &pollingInterval);
            UI32 getPollingInterval()  const;
            void setSampleRate(const UI32 &sampleRate);
            UI32 getSampleRate()  const;

            void setCollectorList(const vector<WaveManagedObjectPointer<SflowCollectorManagedObject> > &collectorList);
            vector<WaveManagedObjectPointer<SflowCollectorManagedObject> > getCollectorList()  const;
            void addSflowCollectorManagedObject(SflowCollectorManagedObject *pSflowCollectorMO);
            void deleteSflowCollectorManagedObject(SflowCollectorManagedObject *pSflowCollectorMO);

// Now the data members

        private:
        protected:
        public:
            bool  m_enable;
            UI32  m_pollingInterval;
            UI32  m_sampleRate;
            vector<WaveManagedObjectPointer<SflowCollectorManagedObject> >  m_collectorList;
    };
}
#endif                                            //SFLOWGLOBALCONFIGMANAGEDOBJECT_H
