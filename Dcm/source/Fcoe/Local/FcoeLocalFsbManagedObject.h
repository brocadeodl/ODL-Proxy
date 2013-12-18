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

#if 0
/***************************************************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#ifndef FCOELOCALFSBMANAGEDOBJECT_H
#define FCOELOCALFSBMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{   

    class FcoeLocalObjectManager;
    class FcoeLocalFsbManagedObject : virtual public DcmManagedObject
    {   
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeLocalFsbManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager);
            FcoeLocalFsbManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager,const bool &enable);
            virtual    ~FcoeLocalFsbManagedObject ();
            static string getClassName();
            void setMode(const bool &enable);
            bool getMode()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_enable;
    };
}
#endif                                            //FCOELOCALFSBMANAGEDOBJECT_H
#endif
