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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMCLUSTER_H
#define PRISMCLUSTER_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

#include <vector>

using namespace WaveNs;

namespace WaveNs
{

class PrismCluster : public WaveManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                                 PrismCluster           (WaveObjectManager *pWaveObjectManager);
        virtual                 ~PrismCluster           ();

        static string            getClassName           ();

               LocationId        getPrimaryLocationId   () const;
               void              setPrimaryLocationId   (const LocationId &primaryLocationId);
               string            getPrimaryIpAddress    () const;
               void              setPrimaryIpAddress    (const string &primaryIpAddress);
               UI32              getPrimaryPort         () const;
               void              setPrimaryPort         (const UI32 &port);
               void              removeAllSecondryNodes ();
               bool              isAKnownSecondaryNode  (const ObjectId &secondaryNodeObjectId);
               void              addSecondaryNode       (const ObjectId &secondaryNodeObjectId);
               void              removeSecondaryNode    (const ObjectId &secondaryNodeObjectId);
               void              setSecondaryNodes      (vector<ObjectId> &secondaryNodes);
               vector<ObjectId>  getSecondaryNodes      () const;

    // Now the data members

    private :
        LocationId       m_primaryLocationId;
        string           m_primaryIpAddress;
        UI32             m_primaryPort;
        vector<ObjectId> m_secondaryNodes;

    protected :
    public :
};

}

#endif // PRISMCLUSTER_H
