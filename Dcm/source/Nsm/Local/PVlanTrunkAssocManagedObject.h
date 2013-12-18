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
#ifndef PVLANTRUNKASSOCMANAGEDOBJECT_H
#define PVLANTRUNKASSOCMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class PVlanTrunkAssocManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PVlanTrunkAssocManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            PVlanTrunkAssocManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            PVlanTrunkAssocManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &primaryVlanId,const UI32 &secondaryVlanId);
            PVlanTrunkAssocManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &primaryVlanId,const UI32 &secondaryVlanId);

            virtual    ~PVlanTrunkAssocManagedObject ();
            static string getClassName();
            void setPrimaryVlanId(const UI32 &primaryVlanId);
            UI32 getPrimaryVlanId()  const;
            void setSecondaryVlanId(const UI32 &secondaryVlanId);
            UI32 getSecondaryVlanId()  const;


        private:
        protected:
        public:
            UI32  m_primaryVlanId;
            UI32  m_secondaryVlanId;
    };
}
#endif

