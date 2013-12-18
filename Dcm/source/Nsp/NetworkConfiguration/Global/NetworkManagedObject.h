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

#ifndef NETWORKMANAGEDOBJECT_H
#define NETWORKMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsp/NetworkConfiguration/Global/NetworkSegmentManagedObject.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;

class NetworkManagedObject : public WaveManagedObject
{
    private :
                void setupAttributesForCreate      ();
                void setupAttributesForPersistence ();

        virtual void getAttributeNamesForHtmlTable    (vector<string> &attributeNamesForHtmlTable, const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getHtmlTableHeaderNames          (vector<string> &htmlTableHeaderNames,       const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getRestHeaderNames               (vector<string> &restHeaderNames,            const WaveManagedObjectShowType &showType = WAVE_MANAGED_OBJECT_SHOW_ALL);
        virtual void getManagedObjectClassNameForRest (string         &managedObjectClassNameForRest) const;

    protected :
    public :
                                         NetworkManagedObject      (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
    virtual                             ~NetworkManagedObject      ();

    static  string                       getClassName              ();

            UI32                         getNetworkId              () const;
            void                         setNetworkId              (const UI32 &networkId);

            UI32                         getNumberOfAvailablePorts () const;
            void                         setNumberOfAvailablePorts (const UI32 &numberOfAvailablePorts);

            UI32                         getNumberOfAttachedPorts  () const;
            void                         setNumberOfAttachedPorts  (const UI32 &numberOfAttachedPorts);

            void                         addNetworkSegment         (NetworkSegmentManagedObject *pNetworkSegmentManagedObject);

            NetworkSegmentManagedObject *getNetworkSegementAtIndex (const UI32 &index);

            string                       getUuid                   () const;
            void                         setUuid                   (const string &uuid);

    // Now the data members

    private :
        vector<WaveManagedObjectPointer<NetworkSegmentManagedObject> > m_networkSegments;

        UI32   m_networkId;
        UI32   m_numberOfAvailablePorts;
        UI32   m_numberOfAttachedPorts;
        string m_uuid;

    protected :
    public :
};

}

#endif // NETWORKMANAGEDOBJECT_H
