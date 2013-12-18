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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#include "VcsFabric/Local/VcsFabricDebugClientMessage.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "ClientInterface/VcsFabric/vcs_fabric_c_structures.h"

namespace DcmNs
{

VcsFabricDebugClientMessage::VcsFabricDebugClientMessage ()
    : DcmClientManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_FABRIC_DEBUG_CLIENT_MSG)
{
}

VcsFabricDebugClientMessage::~VcsFabricDebugClientMessage ()
{
}

void VcsFabricDebugClientMessage::setupAttributesForSerialization ()
{
    DcmClientManagementInterfaceMessage::setupAttributesForSerialization();

    addSerializableAttribute(new AttributeUI32Vector (&m_domainIds,"domainIds"));
}

void VcsFabricDebugClientMessage::loadInputFromCStructure (const void *pInput)
{
//    vcs_fabric_debug_client_struct_t *vcs_fabric_req_t = (vcs_fabric_debug_client_struct_t *) pInput;
}

void *VcsFabricDebugClientMessage::getOutputForCStructure ()
{
#if 1
    vcs_fabric_debug_client_struct_t *vcs_fabric_resp_t     = (vcs_fabric_debug_client_struct_t *) malloc (sizeof (vcs_fabric_debug_client_struct_t));
    UI32                              numberOfNodeInCluster = m_domainIds.size ();

    trace (TRACE_LEVEL_INFO, string ("number of Nodes : ") + numberOfNodeInCluster);

    vcs_fabric_resp_t-> number_of_domain_in_cluster = numberOfNodeInCluster;
    vcs_fabric_resp_t->domains_in_cluster           = (uint32_t *) malloc (numberOfNodeInCluster * sizeof (uint32_t));

    for (UI32 i = 0; i < numberOfNodeInCluster; i++)
    {
        vcs_fabric_resp_t->domains_in_cluster [i] = m_domainIds [i];
    }

    return (vcs_fabric_resp_t);
#else
    return (NULL);
#endif
}

vector<UI32> VcsFabricDebugClientMessage::getListOfDomainInCluster ()
{
    return (m_domainIds);
}

void VcsFabricDebugClientMessage::setListOfDomainInCluster (const vector<UI32> &domainIds)
{
    m_domainIds = domainIds;
}

}

