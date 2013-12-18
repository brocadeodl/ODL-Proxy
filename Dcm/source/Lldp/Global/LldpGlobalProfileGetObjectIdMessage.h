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
 *   Author : Mobin Mohan                                                     *
 **************************************************************************/

#ifndef LLDPGLOBALPROFILEGETOBJECTIDMESSAGE_H
#define LLDPGLOBALPROFILEGETOBJECTIDMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpGlobalProfileGetObjectIdMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LldpGlobalProfileGetObjectIdMessage ();
            LldpGlobalProfileGetObjectIdMessage (const string &profileName);
            virtual    ~LldpGlobalProfileGetObjectIdMessage ();
            void setProfileName(const string &profileName);
            string getProfileName()  const;
            void setProfileObjId(const ObjectId &profileId);
            ObjectId getProfileObjId()  const;

// Now the data members

        private:
        protected:
        public:
            string    m_profileName;
            ObjectId  m_profileObjId;
    };
}
#endif                                            //LLDPGLOBALPROFILEGETOBJECTIDMESSAGE_H
