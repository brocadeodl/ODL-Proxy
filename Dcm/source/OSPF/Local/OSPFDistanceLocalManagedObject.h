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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFDISTANCELOCALMANAGEDOBJECT_H
#define OSPFDISTANCELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
	class OSPFRouterConfigLocalMessage;

    class OSPFDistanceLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFDistanceLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFDistanceLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const OSPFRouteType &distanceType,const UI32 &distanceValue);
            virtual    ~OSPFDistanceLocalManagedObject ();
            static string getClassName();
            void setDistanceType(const OSPFRouteType &distanceType);
            OSPFRouteType  getDistanceType()  const;
            void setDistanceValue(const UI32 &distanceValue);
            UI32 getDistanceValue()  const;
			void updateMOFromLocalMessage(OSPFRouterConfigLocalMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
            OSPFRouteType  m_distanceType;
            UI32  m_distanceValue;
    };
}
#endif                                            //OSPFDISTANCELOCALMANAGEDOBJECT_H
