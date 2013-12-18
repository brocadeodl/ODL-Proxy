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

#ifndef NSMIPADDRCONFIGMANAGEDOBJECT_H
#define NSMIPADDRCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

#include "Framework/Attributes/AttributeManagedObjectComposition.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#
using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmIpAddrManagedObject;
    class NsmIpAddrConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            NsmIpAddrConfigManagedObject(WaveObjectManager *pNsmLocalObjectManager);
            virtual ~NsmIpAddrConfigManagedObject ();
            static string getClassName();

            void setIpAddr(
                const vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> > 
                &ipaddr);
            vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> > 
                getIpAddr() const;
            int findIpAddrIndex(string ipaddr);
            void findOverlapIpAddr(vector<string> &overlapIpAddr, string ipaddr);
            int addIpAddrMO(NsmIpAddrManagedObject *pMO);
            int deleteIpAddr(int index);
            int deleteIpAddr(string ipaddr);
            int updateIpAddr(string ipaddr);
            bool searchIpAddr(string ipaddr);
            bool isPrimaryIpAddr(string ipaddr);
            UI32 matchIPSubnet (string ipAddrStr);
            NsmIpAddrManagedObject* getPrimaryIpAddrMO();

        private:
        protected:
        public:
            vector<WaveManagedObjectPointer<NsmIpAddrManagedObject> > m_ipAddr;
    };
}
#endif                                    //NSMIPADDRCONFIGMANAGEDOBJECT_H
