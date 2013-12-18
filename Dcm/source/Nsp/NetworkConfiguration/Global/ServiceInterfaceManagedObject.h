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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef SERVICEINTERFACEMANAGEDOBJECT_H
#define SERVICEINTERFACEMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Nsp/NetworkConfiguration/Global/NetworkInterfaceManagedObject.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;
    
class ServiceInterfaceManagedObject : public NetworkInterfaceManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
                        ServiceInterfaceManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
        virtual        ~ServiceInterfaceManagedObject ();

        static  string  getClassName                  ();

    // Now the Data Members

    private :
    protected :
    public :
};

}

#endif // SERVICEINTERFACEMANAGEDOBJECT_H