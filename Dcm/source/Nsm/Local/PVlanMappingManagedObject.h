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
 *   Author : rprashar                                                     *
 **************************************************************************/
#ifndef PVLANMAPPINGMANAGEDOBJECT_H
#define PVLANMAPPINGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Nsm/Local/NsmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PVlanMappingManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PVlanMappingManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PVlanMappingManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PVlanMappingManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const  PVlanMappingOper &oper);
            PVlanMappingManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &primaryVlanId,const UI32Range &secVlanRange, const PVlanMappingOper  &oper);

            virtual    ~PVlanMappingManagedObject ();
            static string getClassName();
            void setPrimaryVlanId(const UI32 &primaryVlanId);
            UI32 getPrimaryVlanId()  const;
            void setSecPvlanRange (const UI32Range &psecVlanRange);
            UI32Range getSecPvlanRange ()  const;
            void setoper(const PVlanMappingOper  &oper);
            PVlanMappingOper  getoper()  const;


        private:
        protected:
        public:
            UI32  m_primaryVlanId;
            UI32Range  m_secVlanRange;
            PVlanMappingOper m_oper;
    };
}
#endif

