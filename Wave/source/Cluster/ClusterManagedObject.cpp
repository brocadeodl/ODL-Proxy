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

#include "Cluster/CentralClusterConfigObjectManager.h"
#include "Cluster/ClusterManagedObject.h"
#include "Cluster/Local/HeartBeat/HeartBeatDebug.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{


ClusterManagedObject::ClusterManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement (pWaveObjectManager),
      PrismPersistableObject (ClusterManagedObject::getClassName (), WaveManagedObject::getClassName ()),
      WaveManagedObject (pWaveObjectManager),
      m_selfNode (pWaveObjectManager),
      m_primaryNode (pWaveObjectManager)
{
//    m_pWaveObjectManager         = pWaveObjectManager;
    m_selfNode.m_nodeName       = FrameworkToolKit::getThisLocationIpAddress ();
    m_selfNode.m_nodePort       = FrameworkToolKit::getThisLocationPort ();
    m_primaryNode.m_nodeName    = FrameworkToolKit::getThisLocationIpAddress ();
    m_primaryNode.m_nodePort    = FrameworkToolKit::getThisLocationPort ();
    m_nSecondaryNodes           = 0;
    m_clusterCreated            = 0;
}

ClusterManagedObject::~ClusterManagedObject ()
{
}

string ClusterManagedObject::getClassName ()
{
    return ("ClusterManagedObject");
}

UI32 ClusterManagedObject::getThisNodeRole ()
{
    UI32    role;

    if ((m_selfNode.m_nodeName == m_primaryNode.m_nodeName) && (m_selfNode.m_nodePort == m_primaryNode.m_nodePort))
    {
        if (0 == m_clusterCreated)
        {
            role = STAND_ALONE_NODE;
        }
        else
        {
            role = PRIMARY_NODE;
        }
    }
    else
    {
        role = SECONDARY_NODE;
    }
    return role;
}

void ClusterManagedObject::showStatistic()
{
    UI32            i;
    UI32            role;


    role    =   getThisNodeRole ();
    cout << " - Node Name: " << m_selfNode.m_nodeName << "  Port: "<< m_selfNode.m_nodePort << ", Role: ";
    if (STAND_ALONE_NODE == role)
    {
        cout << "STAND_ALONE_NODE" << endl << endl;
    }
    else
    {
        if (PRIMARY_NODE == role)
        {
            cout << "PRIMARY_NODE" << endl << endl;
        }
        else
        {
            cout << "SECONDARY_NODE,  Primary Node:" << m_primaryNode.m_nodeName << " Port:" << m_primaryNode.m_nodePort << endl << endl;
        }
    }

    if (PRIMARY_NODE == role)
    {
        cout << " - Secondary Nodes " << endl;
        for( i = 0; i < m_nSecondaryNodes; i++)
        {
            ShowHeartBeatStatMessage   *pMessage;

            pMessage = new ShowHeartBeatStatMessage ();
            pMessage->setDstIpAddress (m_nodeList[i].m_nodeName);
            pMessage->setDstPortNumber (m_nodeList[i].m_nodePort);
            sendSynchronously (pMessage);
            delete (pMessage);

            //m_nodeList[i].showStatistic();
        }
    }
    else
    {
        if (SECONDARY_NODE == role)
        {
            ShowHeartBeatStatMessage   *pMessage;

            pMessage = new ShowHeartBeatStatMessage ();
            pMessage->setDstIpAddress (m_primaryNode.m_nodeName);
            pMessage->setDstPortNumber (m_primaryNode.m_nodePort);
            sendSynchronously (pMessage);
            delete (pMessage);

            //m_primaryNode.showStatistic ();
        }
    }
}


NodeManagedObject *ClusterManagedObject::getSelfNode ()
{
    return (&m_selfNode);
}

NodeManagedObject *ClusterManagedObject::getPrimaryNode ()
{
    return (&m_primaryNode);
}

UI32 ClusterManagedObject::getNSecondaryNodes ()
{
    return (m_nSecondaryNodes);
}

void ClusterManagedObject::setNSecondaryNodes (UI32 nSecondaryNodes)
{
    m_nSecondaryNodes   =   nSecondaryNodes;
}

NodeManagedObject *ClusterManagedObject::getSecondaryNode (UI32 nodeIndex)
{
    return (&m_nodeList[nodeIndex]);
}

void ClusterManagedObject::setSecondaryNode (UI32 nodeIndex, NodeManagedObject &secondaryNode)
{
    if (m_nodeList.size () > nodeIndex)
    {
        m_nodeList[nodeIndex]   =   secondaryNode;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Setting node that does not exit.");
    }
}

void ClusterManagedObject::addSecondaryNode (NodeManagedObject &secondaryNode)
{
    m_nodeList.push_back (secondaryNode);
}

}
