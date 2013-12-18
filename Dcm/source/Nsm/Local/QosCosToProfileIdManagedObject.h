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
 *   Copyright (C) 2011-2015 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudha Balodia          			                                       *
 **************************************************************************/

#ifndef QOSCOSTOPROFILEIDMANAGEDOBJECT_H
#define QOSCOSTOPROFILEIDMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmGlobalObjectManager;
    class QosCosToProfileIdManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            QosCosToProfileIdManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            QosCosToProfileIdManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager);
            QosCosToProfileIdManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &cos,const UI32 &red_profile);
            QosCosToProfileIdManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &cos,const UI32 &red_profile);
			virtual    ~QosCosToProfileIdManagedObject ();
            static string getClassName();
            void setRedProfile(const UI32 &red_profile);
            UI32 getRedProfile()  const;
            void setCos(const UI32 &cos);
            UI32 getCos()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cos;
            UI32  m_red_profile;
    };
}
#endif                                            //QOSCOSTOPROFILEIDMANAGEDOBJECT_H
