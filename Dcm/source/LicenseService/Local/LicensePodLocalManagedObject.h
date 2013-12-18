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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef LICENSEPODLOCALMANAGEDOBJECT_H
#define LICENSEPODLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#define DPOD_BASE_SET   "Base"
#define DPOD_SINGLE_SET "Single"
#define DPOD_DOUBLE_SET "Double"
#define DPOD_UNASSIGNED "None"
#define DPOD_RELEASE    "release"
#define DPOD_RESERVE    "reserve"
using namespace WaveNs;

namespace DcmNs
{

    class LicenseServiceLocalObjectManager;
    class LicensePodLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LicensePodLocalManagedObject (LicenseServiceLocalObjectManager *pLicenseServiceLocalObjectManager);
            LicensePodLocalManagedObject (LicenseServiceLocalObjectManager *pLicenseServiceLocalObjectManager,const string &portId,const string &assignedPodset,const string &operation);
            virtual    ~LicensePodLocalManagedObject ();
            static string getClassName();
            void setPortId(const string &portId);
            string getPortId()  const;
            void setAssignedPodset(const string &assignedPodset);
            string getAssignedPodset()  const;
            void setOperation(const string &operation);
            string getOperation()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_portId;
            string  m_assignedPodset;
            string  m_operation;
    };
}
#endif                                            //LICENSEPODLOCALMANAGEDOBJECT_H
