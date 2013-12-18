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

/******************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.             *
 *   All rights reserved.                                                     *
 *   Author : Raghuprem M													  *
 *****************************************************************************/

#ifndef APPMNATIVEVLANMANAGEDOBJECT_H
#define APPMNATIVEVLANMANAGEDOBJECT_H 

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmObjectModel/DcmManagedObject.h"


using namespace WaveNs;

namespace DcmNs
{

    //class WaveObjectManager;
    class APPMNativeVlanManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            APPMNativeVlanManagedObject (WaveObjectManager 
													*pWaveObjectManager);
            APPMNativeVlanManagedObject (WaveObjectManager 
							*pWaveObjectManager,const UI32 &nativevlanId);
            virtual    ~APPMNativeVlanManagedObject ();
            static string getClassName();
            void setNativeVlanId(const UI32 &nativevlanId);
            UI32 getNativeVlanId()  const;
            void setCtag(const UI32 &ctag);
            UI32 getCtag() const;

        private:
        protected:
        public:
            UI32  m_nativevlanId;
            UI32  m_ctag;
    };
}
#endif //NATIVEVLANINTFCONFLOCALMANAGEDOBJECT_H
