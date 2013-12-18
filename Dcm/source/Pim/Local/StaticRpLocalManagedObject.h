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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef STATICRPLOCALMANAGEDOBJECT_H
#define STATICRPLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Attributes/AttributeStringUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class PimLocalObjectManager;
    class StaticRpLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            StaticRpLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager);
            virtual    ~StaticRpLocalManagedObject ();
            static string getClassName();
            void setRpAddr(const IpV4Address &rpAddr);
            IpV4Address getRpAddr()  const;
            void setPrefixList(const string &prefixList, bool userConfig = false);
            string getPrefixList()  const;

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_rpAddr;
            StringUC  m_prefixList;
    };
}
#endif                                            //STATICRPLOCALMANAGEDOBJECT_H
