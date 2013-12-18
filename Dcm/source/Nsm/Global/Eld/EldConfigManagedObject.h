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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#ifndef ELDCONFIGMANAGEDOBJECT_H
#define ELDCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class EldConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            EldConfigManagedObject (NsmGlobalObjectManager 
                                                    *pNsmGlobalObjectManager);
            EldConfigManagedObject (NsmGlobalObjectManager 
                         *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable);
            virtual    ~EldConfigManagedObject ();
            static string getClassName();
            void setRxlim(const UI32 &rxlim);
            UI32 getRxlim()  const;
            void setHello(const UI32 &hello);
            UI32 getHello()  const;
            void setShutdown(const UI32 &shutdown);
            UI32 getShutdown()  const;

        private:
        protected:
        public:
            UI32  m_systemPriority;
            UI32  m_rxlim;
            UI32  m_hello;
            UI32  m_shutdown;
            bool  m_dot1xenable;
            bool  m_nativevlanDisable;
    };
}
#endif  //ELDCONFIGMANAGEDOBJECT_H