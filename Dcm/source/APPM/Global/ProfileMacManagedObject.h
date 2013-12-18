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
 *   Author : bgangise                                                     *
 **************************************************************************/

#ifndef PROFILEMACMANAGEDOBJECT_H
#define PROFILEMACMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class ProfileMacManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            ProfileMacManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            ProfileMacManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &mac);
            virtual    ~ProfileMacManagedObject ();
            static string getClassName();
            void setMac(const string &mac);
            string getMac()  const;
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileId(const UI32 &profileId);
            UI32 getProfileId()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_mac;
            string  m_profileName;
            UI32  m_profileId;
    };
}
#endif                                            //PROFILEMACMANAGEDOBJECT_H
