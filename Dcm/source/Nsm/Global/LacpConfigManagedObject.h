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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef LACPCONFIGMANAGEDOBJECT_H
#define LACPCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class LacpConfigManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LacpConfigManagedObject (NsmGlobalObjectManager 
                                                    *pNsmGlobalObjectManager);
            LacpConfigManagedObject (NsmGlobalObjectManager 
                         *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable);
            LacpConfigManagedObject (NsmGlobalObjectManager 
                         *pNsmGlobalObjectManager,const UI32 &systemPriority, bool dot1xenable, bool nativevlanDisable, UI32 dot1xtesttimeout);
            virtual    ~LacpConfigManagedObject ();
            static string getClassName();
            void setSystemPriority(const UI32 &systemPriority);
            UI32 getSystemPriority()  const;
			void setDot1xenable(const bool &dot1xenable);
			bool getDot1xenable() const;
			void setNativevlanDisable(const bool &nativevlanDisable);
			bool getNativevlanDisable() const;
			void setDot1xtesttimeout(const UI32 &dot1xtesttimeout);
			UI32 getDot1xtesttimeout() const;

        private:
        protected:
        public:
            UI32  m_systemPriority;
            bool  m_dot1xenable;
            bool  m_nativevlanDisable;
			UI32  m_dot1xtesttimeout;
    };
}
#endif  //LACPCONFIGMANAGEDOBJECT_H
