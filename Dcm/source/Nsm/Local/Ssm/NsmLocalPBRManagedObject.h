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
 *   Author : Debojyoti                                                    *
 **************************************************************************/

#ifndef NSMLOCALPBRMANAGEDOBJECT_H
#define NSMLOCALPBRMANAGEDOBJECT_H 

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmLocalPBRManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmLocalPBRManagedObject(WaveObjectManager *pNsmLocalObjectManager);
            NsmLocalPBRManagedObject(WaveObjectManager *pNsmLocalObjectManager,
                const string &routeMapName);
            virtual ~NsmLocalPBRManagedObject ();
            static string getClassName();
			//void  setupKeys();

			void setRouteMapName(const string &routeMapName);
			string getRouteMapName()  const;

        private:
        protected:
        public:
            string     m_routeMapName;
    };
}
#endif                                            // NSMLOCALPBRMANAGEDOBJECT_H

