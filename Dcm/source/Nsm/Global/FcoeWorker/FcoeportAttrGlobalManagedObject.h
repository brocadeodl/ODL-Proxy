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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOEPORTATTRGLOBALMANAGEDOBJECT_H
#define FCOEPORTATTRGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class FcoeportAttrGlobalManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeportAttrGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            FcoeportAttrGlobalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager, const ObjectId &map);
            virtual    ~FcoeportAttrGlobalManagedObject ();
            static string getClassName();
            void setMap (const ObjectId &map);
            ObjectId getMap () const;

// Now the data members

        private:
        protected:
        public:
            ObjectId m_map;
    };
}
#endif                                            //FCOEPORTATTRGLOBALMANAGEDOBJECT_H
