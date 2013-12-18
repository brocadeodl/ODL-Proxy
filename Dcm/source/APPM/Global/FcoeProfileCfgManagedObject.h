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
 *   Author : bgangise, asharma                                            *
 **************************************************************************/

#ifndef FCOEPROFILECFGMANAGEDOBJECT_H
#define FCOEPROFILECFGMANAGEDOBJECT_H

#include "APPM/Global/SubProfileManagedObject.h"
#include "vcs.h"
using namespace WaveNs;

namespace DcmNs
{

    class APPMGlobalObjectManager;
    class FcoeProfileCfgManagedObject : virtual public SubProfileManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FcoeProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager);
            FcoeProfileCfgManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const bool &fcoePort);
            virtual    ~FcoeProfileCfgManagedObject ();
            static string getClassName();
            void setFcoePort(const bool &fcoePort);
            bool getFcoePort()  const;
	    void setMapName (const string &map_name);
	    string getMapName() const;

			Attribute *getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined);
            UI8 sendBulkRequestToClient(const string profile_name, const UI32 profile_id);

// Now the data members

        private:
        protected:
        public:
            bool  m_fcoePort;
	   string m_mapName;
    };
}
#endif                                            //FCOEPROFILECFGMANAGEDOBJECT_H
