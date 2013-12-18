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

/*
 *  SflowCollectorManagedObject.h

 *  Copyright (C) 2005-2012 Brocade Communications Systems, Inc.
 *  All rights reserved. 

 *  Created on: Aug 14, 2012
 *  Author: sunniram
 */

#ifndef SFLOWCOLLECTORMANAGEDOBJECT_H_
#define SFLOWCOLLECTORMANAGEDOBJECT_H_

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs {


class SflowGlobalObjectManager;
class SflowCollectorManagedObject : virtual public DcmManagedObject
{
    private:
    protected:
        virtual void setupAttributesForPersistence();
        virtual void setupAttributesForCreate();
        virtual void setupKeys();
    public:
        SflowCollectorManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager);
        SflowCollectorManagedObject (SflowGlobalObjectManager *pSflowGlobalObjectManager,const IpVxAddress &collector,const UI32 &portNumber);
        virtual    ~SflowCollectorManagedObject ();
        static string getClassName();

        void setCollectorIP(const IpVxAddress &collector);
        IpVxAddress getCollectorIP()  const;
        void setCollectorPort(const UI32 &portNumber);
        UI32 getCollectorPort()  const;

    private:
    protected:
    public:
        IpVxAddress  m_collector;
        UI32  m_portNumber;

};

}


#endif /* SFLOWCOLLECTORMANAGEDOBJECT_H_ */
