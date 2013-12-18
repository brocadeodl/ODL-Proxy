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
 *   Author : pahuja                                                       *
 **************************************************************************/

#ifndef SECMAILCFGGLOBALMANAGEDOBJECT_H
#define SECMAILCFGGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class SystemMonitorGlobalObjectManager;
    class SecMailCfgGlobalManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            SecMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager);
            SecMailCfgGlobalManagedObject (SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager,const bool &enableFlag);
            virtual    ~SecMailCfgGlobalManagedObject ();
            static string getClassName();
            void setEnableFlag(const bool &enableFlag);
            bool getEnableFlag()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enableFlag;
    };
}
#endif                                    //SECMAILCFGGLOBALMANAGEDOBJECT_H
