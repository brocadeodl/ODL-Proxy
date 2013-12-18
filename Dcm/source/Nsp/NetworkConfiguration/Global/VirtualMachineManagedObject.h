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

#ifndef VIRTUALMACHINEMANAGEDOBJECT_H
#define VIRTUALMACHINEMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;

class VirtualMachineManagedObject : public WaveManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

    protected :
    public :
                            VirtualMachineManagedObject (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
        virtual            ~VirtualMachineManagedObject ();

        static  string      getClassName                ();

                ObjectId    getVirtualNetowrkInterface  () const;
                void        setVirtualNetowrkInterface  (const ObjectId &virtualNetowrkInterface);

                MacAddress  getMacAddress               () const;
                void        setMacAddress               (const MacAddress &macAddress);

                string      getTargetHost               () const;
                void        setTargetHost               (const string &targetHost);


    // Now the Data Members

    private :
        ObjectId   m_virtualNetworkInterface;
        MacAddress m_macAddress;
        string     m_targetHost;

    protected :
    public :
};

}

#endif // VIRTUALMACHINEMANAGEDOBJECT_H
