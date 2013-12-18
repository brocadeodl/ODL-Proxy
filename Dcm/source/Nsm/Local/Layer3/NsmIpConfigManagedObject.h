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
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSMIPCONFIGMANAGEDOBJECT_H
#define NSMIPCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmIpConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmIpConfigManagedObject(WaveObjectManager *pNsmLocalObjectManager);
            virtual ~NsmIpConfigManagedObject ();
            static string getClassName();

            void setMtu(const UI32 &mtu);
            UI32 getMtu() const;
            void setProxyArp(const bool &flag);
            bool getProxyArp() const;
            void setDirectedBroadcast(const bool &flag);
            bool getDirectedBroadcast() const;
            void setArpAgingTimeout(const UI32 &timeout);
            UI32 getArpAgingTimeout() const;
            bool hasNonDefaultConfig ();

        private:
        protected:
        public:
            UI32  m_mtu;
            bool  m_directed_broadcast;
            bool  m_proxy_arp;
            UI32  m_arp_aging_timeout;
    };
}
#endif                                    //NSMIPCONFIGMANAGEDOBJECT_H
