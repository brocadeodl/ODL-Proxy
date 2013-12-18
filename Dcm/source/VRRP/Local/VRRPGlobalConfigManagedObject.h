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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef VRRPGLOBALCONFIGMANAGEDOBJECT_H
#define VRRPGLOBALCONFIGMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class VRRPLocalObjectManager;
    class VRRPGlobalConfigManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            VRRPGlobalConfigManagedObject (VRRPLocalObjectManager *pVRRPLocalObjectManager);
            VRRPGlobalConfigManagedObject (VRRPLocalObjectManager *pVRRPLocalObjectManager,const bool &vrrp_config,const bool &vrrpe_config);
            virtual    ~VRRPGlobalConfigManagedObject ();
            static string getClassName();
            void setVrrpEnable(const bool &vrrp_config);
            bool getVrrpEnable()  const;
            void setVrrpeEnable(const bool &vrrpe_config);
            bool getVrrpeEnable()  const;

// Now the data members

        private:
        protected:
        public:
            bool  m_vrrp_enable;
            bool  m_vrrpe_enable;
    };
}
#endif                                            //VRRPGLOBALCONFIGMANAGEDOBJECT_H
