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

#ifndef FCOEFIPMANAGEDOBJECT_H
#define FCOEFIPMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalObjectManager;
    class FcoeFipManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeFipManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager);
            FcoeFipManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const SI32 &fka_intvl,const bool &timeout);
            virtual    ~FcoeFipManagedObject ();
            static string getClassName();
            void setFka_intvl(const SI32 &fka_intvl);
            SI32 getFka_intvl()  const;
            void setTimeout(const bool &timeout);
            bool getTimeout()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_fka_intvl;
            bool  m_timeout;
    };
}
#endif                                            //FCOEFIPMANAGEDOBJECT_H
