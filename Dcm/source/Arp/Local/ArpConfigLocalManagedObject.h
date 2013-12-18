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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : vkarnati                                                     *
 **************************************************************************/

#ifndef ARPCONFIGLOCALMANAGEDOBJECT_H
#define ARPCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpLocalObjectManager;
    class ArpConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ArpConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager);
            ArpConfigLocalManagedObject (ArpLocalObjectManager *pArpLocalObjectManager,const UI32 &SystemMaxArp);
            virtual    ~ArpConfigLocalManagedObject ();
            static string getClassName();
            void setSystemMaxArp(const UI32 &SystemMaxArp);
            UI32 getSystemMaxArp()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_SystemMaxArp;
    };
}
#endif                                            //ARPCONFIGLOCALMANAGEDOBJECT_H
