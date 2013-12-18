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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef NETWORKSEGMENTMANAGEDOBJECT_H
#define NETWORKSEGMENTMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsp/NetworkConfiguration/Global/NetworkInterfaceManagedObject.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;

class NetworkSegmentManagedObject : public WaveManagedObject
{
    private :
                void setupAttributesForPersistence    ();
                void setupAttributesForCreate         ();

        virtual void getAttributeNamesForHtmlTable    (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getHtmlTableHeaderNames          (vector<string> &htmlTableHeaderNames,       const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getRestHeaderNames               (vector<string> &restHeaderNames,            const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getManagedObjectClassNameForRest (string         &managedObjectClassNameForRest) const;


    protected :
    public :
                                               NetworkSegmentManagedObject  (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
        virtual                               ~NetworkSegmentManagedObject  ();

        static  string                         getClassName                 ();

                UI32                           getNumberOfNetworkInterfaces () const;
                NetworkInterfaceManagedObject *getNetworkInterfaceAtIndex   (const UI32 &index);

                void                           addNetworkInterface          (NetworkInterfaceManagedObject *pNetworkInterfaceManagedObject);

    // Now the Data Members

    private :
        vector<WaveManagedObjectPointer<NetworkInterfaceManagedObject> > m_networkInterfaces;

    protected :
    public :
};

}

#endif // NETWORKSEGMENTMANAGEDOBJECT_H
