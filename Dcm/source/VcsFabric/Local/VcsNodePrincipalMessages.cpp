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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodePrincipalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{

VcsNodeSetIsPrincipalMessage::VcsNodeSetIsPrincipalMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_ISPRINCIPAL)
{
    m_isPrincipal = false;
}

VcsNodeSetIsPrincipalMessage::VcsNodeSetIsPrincipalMessage (const bool &isPrincipal)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_ISPRINCIPAL),
      m_isPrincipal              (isPrincipal)
{
}

VcsNodeSetIsPrincipalMessage::~VcsNodeSetIsPrincipalMessage ()
{
}

void VcsNodeSetIsPrincipalMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeBool (&m_isPrincipal, "isPrincipal"));
}

bool VcsNodeSetIsPrincipalMessage::getIsPrincipal () const
{
    return (m_isPrincipal);
}

/************************************ 
* GET ISPRINCIPAL 
*************************************/

VcsNodeGetIsPrincipalMessage::VcsNodeGetIsPrincipalMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_ISPRINCIPAL)
{
    m_isPrincipal = false;
    m_mappedId = 0;
}

VcsNodeGetIsPrincipalMessage::VcsNodeGetIsPrincipalMessage (const UI32 &mappedId)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_ISPRINCIPAL),
      m_mappedId                 (mappedId)
{
    m_isPrincipal = false;
}

VcsNodeGetIsPrincipalMessage::~VcsNodeGetIsPrincipalMessage ()
{
}

void VcsNodeGetIsPrincipalMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_mappedId,      "mappedId"));
     addSerializableAttribute (new AttributeBool (&m_isPrincipal,   "isPrincipal"));
}

void VcsNodeGetIsPrincipalMessage::setMappedId (const UI32 &mappedId)
{
    m_mappedId = mappedId;
}

void VcsNodeGetIsPrincipalMessage::setIsPrincipal (const bool &isPrincipal)
{
    m_isPrincipal = isPrincipal;
}

bool VcsNodeGetIsPrincipalMessage::getIsPrincipal () const
{
    return (m_isPrincipal);
}

UI32 VcsNodeGetIsPrincipalMessage::getMappedId () const
{
    return (m_mappedId);
}

/************************************
* SET PRINCIPAL PRIORITY
*************************************/

VcsNodeSetPrincipalPriorityMessage::VcsNodeSetPrincipalPriorityMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_PRINCIPAL_PRIORITY),
      m_principalPriority        (0)
{

}

VcsNodeSetPrincipalPriorityMessage::VcsNodeSetPrincipalPriorityMessage (const UI32 principalPriority)
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_SET_PRINCIPAL_PRIORITY),
      m_principalPriority        (principalPriority)
{
}

VcsNodeSetPrincipalPriorityMessage::~VcsNodeSetPrincipalPriorityMessage ()
{
}

void VcsNodeSetPrincipalPriorityMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_principalPriority, "principalPriority"));
}

UI32 VcsNodeSetPrincipalPriorityMessage::getPrincipalPriority () const
{
    return (m_principalPriority);
}

/************************************
* GET PRINCIPAL PRIORITY
*************************************/

VcsNodeGetPrincipalPriorityMessage::VcsNodeGetPrincipalPriorityMessage ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_GET_PRINCIPAL_PRIORITY),
      m_principalPriority        (0)
{

}

VcsNodeGetPrincipalPriorityMessage::~VcsNodeGetPrincipalPriorityMessage ()
{
}

void VcsNodeGetPrincipalPriorityMessage::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_principalPriority, "principalPriority"));
}

UI32 VcsNodeGetPrincipalPriorityMessage::getPrincipalPriority () const
{
    return (m_principalPriority);
}

void VcsNodeGetPrincipalPriorityMessage::setPrincipalPriority (const UI32 principalPriority)
{
    m_principalPriority = principalPriority;
}

/************************************
* TRIGGER PRINCIPAL FAILOVER
*************************************/

VcsNodePrincipalTriggerFailover::VcsNodePrincipalTriggerFailover ()
    : ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_PRINCIPAL_TRIGGER_FAILOVER)
{
}

VcsNodePrincipalTriggerFailover::~VcsNodePrincipalTriggerFailover ()
{
}

void VcsNodePrincipalTriggerFailover::setupAttributesForSerialization ()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
}

}
