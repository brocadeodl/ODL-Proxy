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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef DISCMANAGEDOBJECTS_H
#define DISCMANAGEDOBJECTS_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class DiscoverObjectManager;

    class DiscoverManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            DiscoverManagedObject (DiscoverObjectManager *pDiscoverObjectManager);
            virtual    ~DiscoverManagedObject ();
            static string getClassName();
            void setId(const string &id);
            string getId()  const;
            void setState(const UI32 &state);
            UI32 getState()  const;
            void setStartTS(const UI32 &startTS);
            UI32 getStartTS()  const;
            void setDiscStartTS(const UI32 &discStartTS);
            UI32 getDiscStartTS()  const;
            void setDiscEndTS(const UI32 &discEndTS);
            UI32 getDiscEndTS()  const;
            void setPpStartTS(const UI32 &ppStartTS);
            UI32 getPpStartTS()  const;
            void setPpEndTS(const UI32 &ppEndTS);
            UI32 getPpEndTS()  const;
            void setExitCode(const UI32 &exitCode);
            UI32 getExitCode()  const;
            void setEndTS(const UI32 &endTS);
            UI32 getEndTS()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_id;
            UI32  m_state;
            UI32  m_startTS;
            UI32  m_discStartTS;
            UI32  m_discEndTS;
            UI32  m_ppStartTS;
            UI32  m_ppEndTS;
            UI32  m_exitCode;
            UI32  m_endTS;
    };
}
#endif                                            // DISCMANAGEDOBJECTS_H
