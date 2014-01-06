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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aditya Munjal                                                *
 **************************************************************************/

#ifndef FIRMWAREMANAGEDOBJECT_H
#define FIRMWAREMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class FirmwareObjectManager;
    class FirmwareManagedObject : virtual public DcmManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            FirmwareManagedObject (FirmwareObjectManager *pFirmwareObjectManager);
            FirmwareManagedObject(FirmwareObjectManager *pFirmwareObjectManager,
				const UI32 &rbridge_id, const UI32 &state);
            virtual    ~FirmwareManagedObject ();
            static string getClassName();
			void	setRbridge_Id(const UI32 &rbridge_id);
			UI32	getRbridge_Id()  const;
            void 	setState(const UI32 &state);
            UI32	getState()  const;
            void 	setCliState(const UI32 &state);
            UI32	getCliState()  const;

// Now the data members

        private:
        protected:
        public:
			UI32	m_rbridge_id;
            UI32	m_state;
            UI32	m_cli_state;
    };
}
#endif                                            //FIRMWAREMANAGEDOBJECT_H