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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSMNODEPOMANAGEDOBJECT_H
#define NSMNODEPOMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Global/IntfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class IntfManagedObject;

    class NsmNodePoManagedObject : virtual public IntfManagedObject, 
        virtual public DcmLocalManagedObjectBase
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();

        public:
            NsmNodePoManagedObject(WaveObjectManager *pObjectManager);
            virtual ~NsmNodePoManagedObject ();
            static string getClassName();

            void setIfId(const UI32 &if_id);
            UI32 getIfId() const;

        private:
            UI32 m_ifId;
        protected:
        public:

    };
}
#endif                                            //NSMNODEPOMANAGEDOBJECT_H
