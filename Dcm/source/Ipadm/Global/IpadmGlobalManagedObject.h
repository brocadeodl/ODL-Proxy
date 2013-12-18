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
 *   Author : Arun kumar viswanathan                                       *
 ***************************************************************************/

#ifndef IPADMGLOBALMANAGEDOBJECT_H
#define IPADMGLOBALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Ipadm/Global/IpadmGlobalObjectManager.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{
class IpadmGlobalObjectManager;

class IpadmGlobalDNManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
        IpadmGlobalDNManagedObject (IpadmGlobalObjectManager *pIpadmGlobalObjectManager);
        virtual          ~IpadmGlobalDNManagedObject ();
        static            string getClassName              ();
    // Now the data members
            string  getDomainName() const;
            void setDomainName (const string &domain_name);
    private :
            string m_domain_name;
    protected :
    public :
};

class IpadmGlobalNSManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
        virtual void setupKeys();
    public :
       IpadmGlobalNSManagedObject (IpadmGlobalObjectManager *pIpadmGlobalObjectManager);
        virtual          ~IpadmGlobalNSManagedObject ();
        static            string getClassName              ();
    // Now the data members
            IpVxAddress  getNameSrvIp() const;
            void setNameSrvIp (const IpVxAddress &nameSrvIp);
        void setIndex(const UI32 &index);
        UI32 getIndex()  const;
    private :
            IpVxAddress m_nameSrvIp;
	    UI32 m_index;
    protected :
    public :
};

}

#endif
