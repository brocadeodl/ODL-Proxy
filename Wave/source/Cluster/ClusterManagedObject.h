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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef CLUSTERMANAGEDOBJECT_H
#define CLUSTERMANAGEDOBJECT_H

#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Cluster/NodeManagedObject.h"
#include "Cluster/ClusterMessages.h"
#include <string>
#include <vector>


namespace WaveNs
{


class ClusterManagedObject : public  WaveManagedObject
{
    private :
    protected :
    public :
                            ClusterManagedObject        (WaveObjectManager *pWaveObjectManager);
                           ~ClusterManagedObject        ();

       static string        getClassName                ();

       void                 showStatistic               ();
       NodeManagedObject   *getSelfNode                 ();
       NodeManagedObject   *getPrimaryNode              ();
       UI32                 getNSecondaryNodes          ();
       void                 setNSecondaryNodes          (UI32 nSecondaryNodes);
       NodeManagedObject   *getSecondaryNode            (UI32 nodeIndex);
       void                 setSecondaryNode            (UI32 nodeIndex, NodeManagedObject &secondaryNode);
       void                 addSecondaryNode            (NodeManagedObject &secondaryNode);
       UI32                 getThisNodeRole             ();
//       UI32                 getClusterCreated ();
//       UI32                 getTimeout ();
//       UI32                 getNLostHeartBeatToNotify ();

    // Now the data members

    private :
        NodeManagedObject                       m_selfNode;
        NodeManagedObject                       m_primaryNode;
        UI32                                    m_nSecondaryNodes;
        vector<NodeManagedObject>               m_nodeList;
        UI32                                    m_clusterCreated;

    protected :
    public :
        friend class CentralClusterConfigObjectManager;
        friend class LocalClusterConfigObjectManager;
//        WaveObjectManager                       *m_pWaveObjectManager;
//        UI32                                    m_timeout;
//        UI32                                    m_nLostHeartBeatToNotify;


};


}
#endif //CLUSTERMANAGEDOBJECT_H

