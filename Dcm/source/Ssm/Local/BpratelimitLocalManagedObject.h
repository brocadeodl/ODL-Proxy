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

#ifndef BPRATELIMITLOCALMANAGEDOBJECT_H
#define BPRATELIMITLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#define SSM_MPRATELIMIT_MAX_SLOTS 16
using namespace WaveNs;

namespace DcmNs
{

    class SsmLocalObjectManager;
    class BpratelimitLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            BpratelimitLocalManagedObject (SsmLocalObjectManager *pBpratelimitLocalObjectManager);
            BpratelimitLocalManagedObject( SsmLocalObjectManager  *pBpratelimitLocalObjectManager,const UI32 &mode);
            virtual    ~BpratelimitLocalManagedObject ();
            static string getClassName();
            void setSlot(const UI32 &slotId);
            UI32 getSlot() ;

// Now the data members

        private:
        protected:
        public:
            UI32 m_slot;
    };
}
#endif                                            //BpratelimitLocalManagedObject_H
