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

#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalClientInterface.h"
#include "ValClientInterface/DcmClientSynchronousConnection.h"
#include "VcsFabric/Local/VcsClientLocalParams.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "sys/fabos/bitmap.h"

using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

int vcs_set_local_fcf_mac_interface (const char p_fcf_mac[6])
{
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsNodeLocalSetFcfMac (p_fcf_mac);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (0);
    }
    else if (VAL_CLIENT_SWITCH_WWN_NOT_FOUND == status)
    {
        return (-2);
    }
    else
    {
        return (-1);
    }
}

int vcs_get_local_mapped_id_interface (vcs_local_node_params *local_params)
{
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    VcsClientLocalParams            vLocalParams;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsNodeLocalGetMappedId (vLocalParams);

    local_params->mapped_id = vLocalParams.getMappedId();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

int vcs_get_local_mapped_id_cached_interface (vcs_local_node_params *local_params)
{
    static unsigned int rbridgeId = 0;

    int returnStatus = 0;

    if (0 == rbridgeId)
    {
        vcs_local_node_params vcsNodeLocalParameters = {0};

        returnStatus = vcs_get_local_mapped_id_interface (&vcsNodeLocalParameters);

        if (0 == returnStatus)
        {
            rbridgeId = vcsNodeLocalParameters.mapped_id;
        }
    }

    local_params->mapped_id = rbridgeId;

    return returnStatus;
}

/**
 * Name:         vcs_get_rbridge_id_interface 
 * Description:  C API to get the rbridge id from the given WWN.  User must check for success status before using rbridge_id pointer.
 * @param1:      rbridge_id (output):  rbridge_id of switch corresponding to given wwn.
 * @param2:      wwn (input):  wwn used to lookup a corresponding rbridge_id
 *
 * @return:      int: 0 for success
 *                   -1 for framework error
 *                   -2 rbridge_id could not be found from given wwn
 */ 
int vcs_get_rbridge_id_interface (unsigned int *rbridge_id, const wwn_t *wwn)
{   
    ResourceId      status          = WAVE_MESSAGE_SUCCESS;
    UI32            rBridgeId       = 0;
    

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    WorldWideName worldWideName ((UI8 *) wwn->chars);

    status = pDcmClientSynchronousConnection->vcsNodeGetRBridgeIdFromWWN (rBridgeId, worldWideName);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (0 != rBridgeId)
        {
            *rbridge_id = static_cast<unsigned int> (rBridgeId);
            return (0);
        }
        else
        {
            return (-1);
        }
    }
    else if (VAL_CLIENT_RBRIDGE_ID_NOT_FOUND == status)
    {    
        tracePrintf (TRACE_LEVEL_WARN, true, false, "VcsNodeLocalClientInterface::vcs_get_rbridge_id_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-2);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "VcsNodeLocalClientInterface::vcs_get_rbridge_id_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-1);
    }
}

/**
 * Name:         vcs_get_wwn_interface
 * Description:  C API to get the WWN from the given RBridge-Id.  User much check for success status before using wwn pointer.
 * @param1:      wwn (output):  wwn of switch corresponding to given rbridge_id.
 * @param2:      rbridge_id (input):  rbridge_id used to lookup a corresponding wwn.
 *
 * @return:      int: 0 for success
 *                   -1 for framework error
 *                   -2 wwn could not be found from given rbridge_id
 */
int vcs_get_wwn_interface (wwn_t *wwn, const unsigned int *rbridge_id)
{
    ResourceId      status          = WAVE_MESSAGE_SUCCESS;
    UI32            rBridgeId       = (*rbridge_id); 
    WorldWideName   worldWideName;


    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsNodeGetWWNFromRBridgeId (worldWideName, rBridgeId);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (WorldWideName::isValidWorldWideNameString (worldWideName.toString ()))
        {
            for (UI32 i = 0; i < 8; i++)
            {
                tracePrintf (TRACE_LEVEL_DEVEL, true, false, "VcsNodeLocalClientInterface::vcs_get_wwn_interface : WWN[%d] : %02X", i, worldWideName[i]);

                wwn->chars[i] = worldWideName[i];
            }

            return (0);
        }
        else
        {
            return (-1);
        }
    }
    else if (VAL_CLIENT_SWITCH_WWN_NOT_FOUND == status)
    {
        tracePrintf (TRACE_LEVEL_WARN, true, false, "VcsNodeLocalClientInterface::vcs_get_wwn_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-2);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "VcsNodeLocalClientInterface::vcs_get_wwn_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-1);
    }

}

/**
 * Name:         vcs_get_ip_interface
 * Description:  C API to get the IP from the given RBridge-Id.  User must check for success status before using data pointer.
 * @param1:      ip (output):  ip of switch corresponding to given rbridge_id.
 * @param2:      rbridge_id (input):  rbridge_id used to lookup a corresponding ip.
 *
 * @return:      int: 0 for success
 *                   -1 for framework error
 *                   -2 ip could not be found from given rbridge_id
 */
int vcs_get_ip_interface (char **ip, const unsigned int *rbridge_id)
{
    ResourceId      status          = WAVE_MESSAGE_SUCCESS;
    UI32            rBridgeId       = (*rbridge_id); 
    string          ipAddress;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsNodeGetIPFromRBridgeId (rBridgeId, ipAddress);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        *ip = (char *) calloc(sizeof(char), ipAddress.length() + 1);
        if ( NULL == *ip )
        {
            tracePrintf (TRACE_LEVEL_ERROR, true, false, "VcsNodeLocalClientInterface::vcs_get_ip_interface : Memory callocation failed for ip address\n");
            return (-1);
        }
        strncpy(*ip, ipAddress.c_str(), ipAddress.length());
        tracePrintf (TRACE_LEVEL_DEVEL, true, false, "VcsNodeLocalClientInterface::vcs_get_ip_interface : ip: %s\n", *ip);
        return (0);
    }
    else if (VAL_CLIENT_SWITCH_IP_NOT_FOUND == status)
    {
        tracePrintf (TRACE_LEVEL_WARN, true, false, "VcsNodeLocalClientInterface::vcs_get_ip_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-2);
    }
    else
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, false, "VcsNodeLocalClientInterface::vcs_get_ip_interface : Status : %s", FrameworkToolKit::localize (status).c_str ());

        return (-1);
    }


}

int vcs_get_cluster_rbridge_id_from_wwn (const wwn_t *wwn, domainBitMap_t *rbridgeIdBitmap)
{
    UI32          maxRbridgeIds  = 239;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    WorldWideName                   switchWWN ((UI8 *) wwn->chars);

    BitMap rbridgeIdVector (maxRbridgeIds);

    BM_Clear (*rbridgeIdBitmap, maxRbridgeIds);

    status = pDcmClientSynchronousConnection->vcsGetClusterRbridgeIdFromWWN (switchWWN, rbridgeIdVector);

    if (WAVE_MESSAGE_SUCCESS == status)
    {

        for (UI32 i = 0; i < maxRbridgeIds; i++)
        {
            if (rbridgeIdVector [i])
            {
                BM_SetBitN (i+1, *rbridgeIdBitmap);
            }
        }

        return (0);
    }
    else if (VAL_ALL_RBRIDGE_ID_IN_USE == status)
    {
        //Cannot find a slot for Rbridge-Id
        return (-2);
    }
    else
    {
        // Framework error. Message generic error.
        return (-1);
    }
}

}
