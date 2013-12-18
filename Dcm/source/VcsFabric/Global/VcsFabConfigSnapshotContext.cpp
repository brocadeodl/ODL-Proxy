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

/**
 *@file VcsFabConfigSnapshotContext.cpp 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              asynchronous context used during
 *              Create and Restore Snapshot request
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */
#include "VcsFabric/Global/VcsFabConfigSnapshotContext.h"

namespace DcmNs
{

VcsFabConfigSnapshotContext::VcsFabConfigSnapshotContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_snapshotId  (""),
      m_rbridgeId   (0),
      m_SWBDModel   (-1),
      m_isNodePrimary(false),
      m_nodeLocationId (0),
      m_restoreOnDisconnectedNode (false)
{
}

VcsFabConfigSnapshotContext::VcsFabConfigSnapshotContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, const string &snapshotId, const UI32 &rbridgeId, const UI32 &opcode )
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_snapshotId  (snapshotId),
      m_rbridgeId   (rbridgeId),
      m_SWBDModel   (-1),
      m_isNodePrimary(false),
      m_nodeLocationId (0),
      m_restoreOnDisconnectedNode (false)
{
}

VcsFabConfigSnapshotContext::~VcsFabConfigSnapshotContext ()
{
}

void VcsFabConfigSnapshotContext::setSnapshotId  (const string &snapshotId )
{
    m_snapshotId = snapshotId ;
}

string VcsFabConfigSnapshotContext::getSnapshotId  () const
{
    return ( m_snapshotId );
}

void VcsFabConfigSnapshotContext::setRbridgeId (const UI32 &rbridgeId )
{
    m_rbridgeId = rbridgeId;
}

UI32 VcsFabConfigSnapshotContext::getRbridgeId () const
{
    return ( m_rbridgeId );
}

void VcsFabConfigSnapshotContext::setSWBDModel ( const SI32 &SWBDModel)
{
    m_SWBDModel = SWBDModel;
}

SI32 VcsFabConfigSnapshotContext::getSWBDModel () const
{
    return ( m_SWBDModel) ;
}

void VcsFabConfigSnapshotContext::setNodeLocationId ( const LocationId &nodeLocationId )
{
    m_nodeLocationId = nodeLocationId;
}

LocationId VcsFabConfigSnapshotContext::getNodeLocationId () const
{
    return ( m_nodeLocationId );
}

void VcsFabConfigSnapshotContext::setIsNodePrimary ( const bool &isNodePrimary )
{
    m_isNodePrimary = isNodePrimary;
}

bool VcsFabConfigSnapshotContext::getIsNodePrimary () const
{
    return (m_isNodePrimary);
}

void VcsFabConfigSnapshotContext::setSnapshotCreatedTime ( const DateTime &snapshotCreatedTime )
{   
    m_snapshotCreatedTime = snapshotCreatedTime;
}

DateTime VcsFabConfigSnapshotContext::getSnapshotCreatedTime () const
{   
    return ( m_snapshotCreatedTime );
}

void VcsFabConfigSnapshotContext::setRestoreOnDisconnectedNode ( const bool &restoreOnDisconnectedNode )
{   
    m_restoreOnDisconnectedNode = restoreOnDisconnectedNode;
}

bool VcsFabConfigSnapshotContext::getRestoreOnDisconnectedNode () const
{   
    return ( m_restoreOnDisconnectedNode );
}

}
