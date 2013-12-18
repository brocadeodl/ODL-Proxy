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

#ifndef SECPROFILECFGMANAGEDOBJECT_H
#define SECPROFILECFGMANAGEDOBJECT_H

#include "APPM/Global/SubProfileManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class SecProfileCfgManagedObject : virtual public SubProfileManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            SecProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            SecProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const string &accessGroup);
            virtual    ~SecProfileCfgManagedObject ();
            static string getClassName();
            void setAccessGroup(const string &accessGroup);
			string getAccessGroup()  const;
            void setAccessIn(const bool &accessIn);
			bool getAccessIn()  const;
			Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
            UI8 sendBulkRequestToClient(const string profile_name, const UI32 profile_id);
            virtual UI32 activationValidate();
            UI32 SecurityProfileActivationValidate(bool isAllowedAll, vector<UI32> vlans);

// Now the data members

        private:
        protected:
        public:
            string  m_accessGroup;
            bool    m_accessIn;
    };
}
#endif                                            //SECPROFILECFGMANAGEDOBJECT_H
