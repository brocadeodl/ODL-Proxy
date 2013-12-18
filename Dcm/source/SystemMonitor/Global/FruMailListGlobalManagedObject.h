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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef FRUMAILLISTGLOBALMANAGEDOBJECT_H
#define FRUMAILLISTGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorGlobalObjectManager;
    class FruMailListGlobalManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FruMailListGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager);
            FruMailListGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const string &email);
            virtual    ~FruMailListGlobalManagedObject ();
            static string getClassName();
            void setEmail(const string &email);
            string getEmail()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_email;
    };
}
#endif                                //FRUMAILLISTGLOBALMANAGEDOBJECT_H
