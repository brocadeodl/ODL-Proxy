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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : justinh                                                      *
 **************************************************************************/

#ifndef NSMIPADDRMANAGEDOBJECT_H
#define NSMIPADDRMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmIpAddrManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();

        public:
            NsmIpAddrManagedObject (WaveObjectManager *pNsmLocalObjectManager);
            virtual ~NsmIpAddrManagedObject();
            static string getClassName();

            void setIpv4AddrMask(const string &ipaddr);
            string getIpv4AddrMask() const;
            void setSecond(const bool &sec);
            bool isSecond() const;
            void setOspfPassive(const bool &ospf);
            bool isOspfPassive() const;
            void setOspfIgnore(const bool &ospf);
            bool isOspfIgnore() const;

// Now the data members

        private:
        protected:
        public:
            string m_ipv4AddrMask;
            bool m_secondary;
            bool m_ospfPassive;
			bool m_ospfIgnore;
    };
}

#endif                                            //NSMIPADDRMANAGEDOBJECT_H
