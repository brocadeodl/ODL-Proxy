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
 *   Author : mmohan                                                     *
 **************************************************************************/

#ifndef LLDPGLOBALSCALARSCONFMANAGEDOBJECT_H
#define LLDPGLOBALSCALARSCONFMANAGEDOBJECT_H

#include "Lldp/Global/LldpAdvertiseParamsManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class LldpGlobalObjectManager;
    class LldpGlobalScalarsConfManagedObject : virtual public LldpAdvertiseParamsManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            LldpGlobalScalarsConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager);
            LldpGlobalScalarsConfManagedObject (LldpGlobalObjectManager *pLldpGlobalObjectManager,const string &sysName,const string &sysDescr,const bool &disable, const bool &protocolFlag);
            virtual    ~LldpGlobalScalarsConfManagedObject ();
            static string getClassName();
            void setSysName(const string &sysName);
            string getSysName()  const;
            void setSysDescr(const string &sysDescr);
            string getSysDescr()  const;
            void setDisable(const bool &disable);
            bool getDisable()  const;
            void setProtocolFlag(const bool &protocolFlag);
            bool getProtocolFlag()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_sysName;
            string  m_sysDescr;
            bool  m_disable;
            bool  m_protocolFlag;
    };
}
#endif                                            //LLDPGLOBALSCALARSCONFMANAGEDOBJECT_H
