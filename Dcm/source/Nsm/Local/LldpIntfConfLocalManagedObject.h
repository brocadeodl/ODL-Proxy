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
 *   Author : sanjeevj                                                     *
 **************************************************************************/

#ifndef LLDPINTFCONFLOCALMANAGEDOBJECT_H
#define LLDPINTFCONFLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class LldpIntfConfLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LldpIntfConfLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager);
            LldpIntfConfLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const LldpDcbxVersion &dcbxVersion,const bool &disable,const ObjectId &profileId,const UI32 &iscsiPriority);
            virtual    ~LldpIntfConfLocalManagedObject ();
            static string getClassName();
            void setDcbxVersion(const LldpDcbxVersion &dcbxVersion);
            LldpDcbxVersion getDcbxVersion()  const;
            void setDisable(const bool &disable);
            bool getDisable()  const;
            void setProfileId(const ObjectId &profileId);
            ObjectId getProfileId()  const;
            void setIscsiPriority(const UI32 &iscsiPriority);
            UI32 getIscsiPriority()  const;
			void setLldpCeeStatus(const lldp_cee_on_off_t &lldpCeeType);
			lldp_cee_on_off_t getLldpCeeStatus() const;


// Now the data members

        private:
        protected:
        public:
            LldpDcbxVersion  m_dcbxVersion;
            bool  m_disable;
            ObjectId  m_profileId;
            UI32  m_iscsiPriority;
			lldp_cee_on_off_t m_lldpCeeType;
    };
}
#endif                                            //LLDPINTFCONFLOCALMANAGEDOBJECT_H
