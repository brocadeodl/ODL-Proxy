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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ValClientInterface/DcmClientInterface.h"
#include "ValClientInterface/DcmClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Core/WaveClient.h"
#include "ValClientInterface/val_interface.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/UnifiedClientUpdateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientCreateMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientDeleteMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientPostbootMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientNotifyMessage.h"
#include "ManagementInterface/ClientInterface/UnifiedClientGetDataFromClientMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerTypes.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManagerMessages.h"
#include "DcmMessaging/DcmClientManagementInterfaceMessage.h"
#include "ClientInterface/VcsFabric/vcs_fabric_c_structures.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmCore/DcmClientToolKit.h"
#include "utils/slot.h"
#include <utils/node_id.h>
#include <hasm/hasm.h>
#include "AG/Local/AgVFPortMessage.h"

using namespace WaveNs;
using namespace DcmNs;

namespace DcmNs
{
static bool terminalMonitorStatus = false;
static DcmClientPostToMainThread s_pDcmClientPostToMainThread = NULL;
static PrismMutex terminalMonitorStatusMutex;


ResourceId dcmCStyleClientPostToMainThread  (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    int status = 0;
    UI32                           operationCode                  = 0;

    prismAssert (NULL != s_pDcmClientPostToMainThread, __FILE__, __LINE__);

    operationCode = pManagementInterfaceMessage->getOperationCode ();
    trace (TRACE_LEVEL_DEBUG, string ("dcmCStyleClientPostToMainThread:DCM C Style Client received a message:") + operationCode);

    status = (*s_pDcmClientPostToMainThread) (pManagementInterfaceMessage);

    if (0 != status)
    {
        trace (TRACE_LEVEL_FATAL, string ("Not Able to Post Message to Main Thread Status :") + status);
        return (DCM_C_STYLE_CLIENT_MESSAGE_HANDLER_FAILED_TO_POST_TO_MAIN_THREAD);
        
        /* prismAssert (false, __FILE__, __LINE__); */
    }
    
    return (WAVE_MGMT_CLIENT_INTF_CLIENT_ASYNC_REPSONSE);
}

void *dcmGetOutputForCStructure (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    DcmClientManagementInterfaceMessage *pMessage = dynamic_cast<DcmClientManagementInterfaceMessage *> (pManagementInterfaceMessage);

    if (NULL == pMessage)
    {
        return (NULL);
    }
    
    return (pMessage->getOutputForCStructure ()); 
}

ResourceId dcmCStyleClientDcmManagementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;
    ResourceId                     status                         = WAVE_MESSAGE_ERROR;
    UI32                           operationCode                  = 0;

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    operationCode = pManagementInterfaceMessage->getOperationCode ();
    trace (TRACE_LEVEL_DEBUG, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : DCM C Style Client received a message:") + operationCode);

    pDcmManagementInterfaceMessage = dynamic_cast<DcmManagementInterfaceMessage *> (pManagementInterfaceMessage);

    if (NULL == pDcmManagementInterfaceMessage)
    {

        bool                                              isFrameworkOpCode   = false;
        const void                                       *pInput              = NULL;
        UnifiedClientUpdateMessage                       *updateMessage       = NULL;
        UnifiedClientCreateMessage                       *createMessage       = NULL;
        UnifiedClientDeleteMessage                       *deleteMessage       = NULL;
        UnifiedClientPostbootMessage                     *postbootMessage     = NULL;
        UnifiedClientNotifyMessage                       *notifyMessage       = NULL;
        UnifiedClientGetDataFromClientMessage            *getDataMessage      = NULL;

        switch (operationCode)
        {

            case UNIFIEDCLIENTUPDATE:
                {
                    isFrameworkOpCode = true;
                    updateMessage = dynamic_cast<UnifiedClientUpdateMessage *> ( pManagementInterfaceMessage);
                    prismAssert (NULL != updateMessage, __FILE__, __LINE__);
                    pInput = updateMessage->getCStructureForInputs ();
                }
                break;
            case UNIFIEDCLIENTCREATE:
                {
                    isFrameworkOpCode = true;
                    createMessage = dynamic_cast<UnifiedClientCreateMessage *> (pManagementInterfaceMessage);
                    prismAssert (NULL != createMessage, __FILE__, __LINE__);
                    pInput = createMessage->getCStructureForInputs ();
                }
                break;
            case UNIFIEDCLIENTDELETE:
                {
                    isFrameworkOpCode = true;
                    deleteMessage = dynamic_cast<UnifiedClientDeleteMessage *> (pManagementInterfaceMessage);
                    prismAssert (NULL != deleteMessage, __FILE__, __LINE__);
                    pInput = deleteMessage->getCStructureForInputs ();
                }
                break;
            case UNIFIEDCLIENTPOSTBOOT:
                {
                    isFrameworkOpCode = true;
                    postbootMessage = dynamic_cast<UnifiedClientPostbootMessage *> (pManagementInterfaceMessage);
                    prismAssert (NULL != postbootMessage, __FILE__, __LINE__);
                    pInput = postbootMessage->getCStructureForInputs ();
                }
                break;
            case UNIFIEDCLIENTNOTIFY:
                {
                    isFrameworkOpCode = true;
                    notifyMessage = dynamic_cast<UnifiedClientNotifyMessage *> (pManagementInterfaceMessage);
                    prismAssert (NULL != notifyMessage, __FILE__, __LINE__);
                    pInput = notifyMessage->getCStructureForInputs ();
                }
                break;
            case UNIFIEDCLIENTGETDATA:
                {
                    isFrameworkOpCode = true;
                    getDataMessage = dynamic_cast<UnifiedClientGetDataFromClientMessage *>(pManagementInterfaceMessage);
                    prismAssert (NULL != getDataMessage, __FILE__, __LINE__);
                    pInput = getDataMessage->getCStructureForInputs ();
                }
                break;
            default :
            isFrameworkOpCode = false;
        }

        if (isFrameworkOpCode)
        {
            ManagementInterfaceMessageHandlerForCStyleClient  handler       = WaveUserInterfaceObjectManager::getMessageHandlerForCStyleApplication (operationCode);
            SI32                                              handlerStatus = 0;
            void                                             *pOutput       = NULL;

            if (NULL != handler)
            {
                trace (TRACE_LEVEL_DEVEL, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : operationCode=") + operationCode);

                handlerStatus = (handler) (pInput, &pOutput);

                trace (TRACE_LEVEL_DEVEL, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : handler execution complete"));

                fflush(stdout);

                switch (operationCode)
                {
                    case UNIFIEDCLIENTUPDATE:
                        updateMessage->deleteCStructureForInputs (pInput);
                        updateMessage->loadOutputsFromCStructure(pOutput);
                        break;
                    case UNIFIEDCLIENTCREATE:
                        createMessage->deleteCStructureForInputs (pInput);
                        createMessage->loadOutputsFromCStructure(pOutput);
                        break;
                    case UNIFIEDCLIENTDELETE:
                        deleteMessage->deleteCStructureForInputs (pInput);
                        deleteMessage->loadOutputsFromCStructure(pOutput);
                        break;
                    case UNIFIEDCLIENTPOSTBOOT:
                        postbootMessage->deleteCStructureForInputs (pInput);
                        break;
                    case UNIFIEDCLIENTNOTIFY:
                        notifyMessage->deleteCStructureForInputs (pInput);
                        break;
                    case UNIFIEDCLIENTGETDATA:
                        getDataMessage->deleteCStructureForInputs (pInput);
                        getDataMessage->loadOutputsFromCStructure(pOutput);
                        break;
                }

                if ((handlerStatus == WAVE_MESSAGE_SUCCESS) || (!handlerStatus))
                {
                    status = WAVE_MESSAGE_SUCCESS;
                }
                else
                {
                     status = handlerStatus;
                }

            }
            else
            {
                trace (TRACE_LEVEL_INFO, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : Handler not registered. Please register the handler for the operationCode ") + operationCode);
                prismAssert (false , __FILE__, __LINE__);
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : DCM C Style Client received a message that was NOT derived from DcmManagementInterfaceMessage."));
            trace (TRACE_LEVEL_ERROR, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : The Operation Code in this context was : ") + operationCode);
            trace (TRACE_LEVEL_ERROR, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : Please derive your message from DcmManagementInterfaceMessage.  Otherwise unsupported message type error will be returned."));

            status = DCM_C_STYLE_CLIENT_MESSAGE_TYPE_NOT_SUPPORTED;
        }
    }
    else
    {
        const void                                             *pInput        = pDcmManagementInterfaceMessage->getCStructureForInputs ();
              ManagementInterfaceMessageHandlerForCStyleClient  handler       = WaveUserInterfaceObjectManager::getMessageHandlerForCStyleApplication (operationCode);
              void                                             *pOutput       = NULL;
        SI32                                                    handlerStatus = 0;

        if (NULL != handler)
        {
	        trace (TRACE_LEVEL_DEBUG, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : operationCode=") + operationCode);

            handlerStatus = (handler) (pInput, &pOutput);

            pDcmManagementInterfaceMessage->loadOutputsFromCStructure (pOutput);

            if (NULL != pInput)
            {
                free ((void *) pInput);
            }

            if (NULL != pOutput)
            {
                free (pOutput);
            }

            pDcmManagementInterfaceMessage->setClientStatus (handlerStatus);

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("dcmCStyleClientDcmManagementInterfaceMessageHandler : Handler for operationCode ") + operationCode + " NOT set.");

            status = DCM_C_STYLE_CLIENT_MESSAGE_HANDLER_NOT_SET;
        }
        
    }
    
    return (status);
}

bool getTerminalMonitorStatus( )
{
    bool status;

    terminalMonitorStatusMutex.lock();

    status = terminalMonitorStatus;

    terminalMonitorStatusMutex.unlock();

    return (status);

}

void setTerminalMonitorStatus( const bool status )
{
    terminalMonitorStatusMutex.lock();

    terminalMonitorStatus = status;

    terminalMonitorStatusMutex.unlock();
}

// This function will be registered with WaveClient for updating client status
SI32 dcm_client_update_from_server ( ManagementInterfaceMessage *pManagementInterfaceMessage )
{   
    WaveUpdateClientStatusMessage* pWaveUpdateClientStatusMessage = NULL;

    pWaveUpdateClientStatusMessage = dynamic_cast<WaveUpdateClientStatusMessage *> (pManagementInterfaceMessage);
    
    if ( pWaveUpdateClientStatusMessage == NULL )
    {   
        trace (TRACE_LEVEL_ERROR, string ("dcm_client_update_from_server :: pWaveUpdateClientStatusMessage is NULL after typecaste "));
        return (-1) ;
    }
    if ( true == pWaveUpdateClientStatusMessage->getTerminalMonitorStatus( ))
    {
        trace (TRACE_LEVEL_DEBUG, string ("dcm_client_update_from_server: terminal monitor is set  ") );
        setTerminalMonitorStatus (true);
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string ("dcm_client_update_from_server: terminal monitor is reset ") );
        setTerminalMonitorStatus (false);
    }
    
    return (0);
}

}

extern "C"
{

int g_thisSlotInstance = 0;

extern int dcmClientMain (const char * const pProgramName, const int port);
static DcmClientSynchronousConnection *s_pDcmClientSynchronousConnection        = NULL;
static DcmClientSynchronousConnection *s_pDcmClientSynchronousConnectionWithCcm = NULL;

void dcm_client_set_post_to_main_thread_function (DcmClientPostToMainThread pDcmClientPostToMainThread)
{
   s_pDcmClientPostToMainThread = pDcmClientPostToMainThread; 
}

void add_client_message_handler (const unsigned int operation_code, DcmClientMessageHandler message_handler)
{
    WaveUserInterfaceObjectManager::addMessageHandlerForCStyleApplication ((UI32 &)  operation_code, message_handler);
}

void dcm_client_main_thread_posted_message_handler (void *pMessage)
{
    ResourceId status;

    ManagementInterfaceMessage *pManagementInterfaceMessage = NULL;
    pManagementInterfaceMessage = reinterpret_cast<ManagementInterfaceMessage *> (pMessage);

    prismAssert (NULL != pManagementInterfaceMessage, __FILE__, __LINE__);

    status = dcmCStyleClientDcmManagementInterfaceMessageHandler (pManagementInterfaceMessage);

    pManagementInterfaceMessage->setCompletionStatus (status);

    WaveUserInterfaceObjectManager::replyToWaveServer (pManagementInterfaceMessage);

}

int dcm_uninitialize_client ()
{
    prismAssert (NULL != s_pDcmClientSynchronousConnection, __FILE__, __LINE__);

    DcmClientSynchronousConnection::unInitializeCApi ();

    s_pDcmClientSynchronousConnection->close ();    

    delete  s_pDcmClientSynchronousConnection;

    s_pDcmClientSynchronousConnection = NULL;

    return (0);
}

int dcm_reconnect_client (void)
{
#if defined (AGENT)
    WaveConnectionStatus connectionstatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;

    prismAssert (NULL != s_pDcmClientSynchronousConnection, __FILE__, __LINE__);
    if (true == (s_pDcmClientSynchronousConnection->isCurrentlyConnected ()))
    {
        s_pDcmClientSynchronousConnection->close();

        if (WAVE_MGMT_CLIENT_INTF_CONNECTION_CLOSED_TO_SERVER !=
	    s_pDcmClientSynchronousConnection->getConnectionStatus ()) {
		trace (TRACE_LEVEL_ERROR, string ("Fail to close WaveServer connection"));
		return (-1);
	}

	trace (TRACE_LEVEL_INFO, string ("Success to close WaveServer connection"));

	string ipaddrServer = "127.1.101.101";
	string ipaddrSelf = "127.1.101.1";
	int rc = 0;
	nin_t nin;
	struct in_addr in;

	if ((rc = ham_get_nin(&nin)) == 0) {
            in.s_addr = nin.bp.cp_anip;
	    ipaddrServer = inet_ntoa(in);

	    in.s_addr = nin.bp.bp_lnip;
	    ipaddrSelf = inet_ntoa(in);

	} else {
	    trace (TRACE_LEVEL_ERROR, string ("Fail to get IP info from HASM Error=") + rc);
	    return (-1);
	}

	trace (TRACE_LEVEL_DEBUG, string ("Current LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());
	(FrameworkToolKit::getPThisLocation ())->resetIpAddress (ipaddrSelf);
	trace (TRACE_LEVEL_DEBUG, string ("New LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());

	trace (TRACE_LEVEL_DEBUG, string ("New ipServer ipSelf=") + ipaddrSelf + string(" ipAddrServer=") + ipaddrServer);

	s_pDcmClientSynchronousConnection->setWaveServerIpAddress (ipaddrServer);

	connectionstatus = s_pDcmClientSynchronousConnection->connect ();

	if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionstatus)
	{
	    trace (TRACE_LEVEL_ERROR, string ("Fail to connect to WaveServer ipaddr=") + ipaddrServer +
	       string (" status=") + connectionstatus);
	    return (-1);
	}
    } else {
	trace (TRACE_LEVEL_ERROR, string ("No WaveServer connection to be close") +
	       string (" status=") + connectionstatus);
	return (-1);
    }
#endif

    return (0);
}

int dcm_initialize_client (const char *p_client_name)
{
    SI32                 status           = 0;
    WaveConnectionStatus connectionstatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
    SI32                 clientPort       = 9910; // This is the base for auto scan.
    char dcm_clientname[80];
    int	instance = g_thisSlotInstance;

    if (NULL == s_pDcmClientPostToMainThread)
    {
        WaveUserInterfaceObjectManager::setManagementInterfaceMessageHandlerForClient (dcmCStyleClientDcmManagementInterfaceMessageHandler);
        WaveUserInterfaceObjectManager::setClientHandlerToLoadOutputForCStructure     (dcmGetOutputForCStructure);
    }
    else
    {
        WaveUserInterfaceObjectManager::setManagementInterfaceMessageHandlerForClient (dcmCStyleClientPostToMainThread);
    }

    // Instruct framework to auto scan or next available client port.

    WaveClientReceiverObjectManager::setAutoScanForAvailablePort (true);

#if defined (AGENT)
    int lcType;
    slot_no2id(get_node_slot(NID_SELF()), &lcType, &instance);
#endif

    sprintf(dcm_clientname, "%s%d", p_client_name, instance);

    // printf ("Registering a Client with DCMd : %s\n", dcm_clientname);

    dcmClientMain (dcm_clientname, clientPort);
    
    // Register the client function used to update status sent from server    
    WaveClient::registerUpdateClientStatusFunction(dcm_client_update_from_server);
    
    string ipaddrServer = "127.1.100.100";
    string ipaddrSelf = "127.1.100.1";

#if defined (AGENT)
    int rc = 0;
    nin_t nin;
    struct in_addr in;

    if ((rc = ham_get_nin(&nin)) == 0) {
        in.s_addr = nin.bp.cp_anip;
	ipaddrServer = inet_ntoa(in);

	in.s_addr = nin.bp.bp_lnip;
	ipaddrSelf = inet_ntoa(in);

    } else {
	trace (TRACE_LEVEL_ERROR, string ("Fail to get IP info from HASM Error=") + rc);
	return (-1);
    }

#else
    ipaddrSelf = FrameworkToolKit::getThisLocationIpAddress ();
    ipaddrServer = "127.0.0.1";
#endif

    trace (TRACE_LEVEL_DEBUG, string ("WDir=") + FrameworkToolKit::getProcessInitialWorkingDirectory () +
	       string (" LID=") + FrameworkToolKit::getThisLocationId () +
	       string (" IP=") + FrameworkToolKit::getThisLocationIpAddress () +
	       string (" Port=") + FrameworkToolKit::getThisLocationPort ());
#if defined(AGENT)
    trace (TRACE_LEVEL_DEBUG, string ("Current LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());
    (FrameworkToolKit::getPThisLocation ())->resetIpAddress (ipaddrSelf);
    trace (TRACE_LEVEL_DEBUG, string ("New LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());
#endif

    trace (TRACE_LEVEL_DEBUG, string ("DCMCLient=") + dcm_clientname +
	   string (" ipaddrSelf=") + ipaddrSelf + string (" ipaddrServer=") + ipaddrServer);

    s_pDcmClientSynchronousConnection = new DcmClientSynchronousConnection (string (dcm_clientname), ipaddrServer,
		     FrameworkToolKit::getManagementInterfaceReceiverPort ());

    prismAssert (NULL != s_pDcmClientSynchronousConnection, __FILE__, __LINE__);

    DcmClientSynchronousConnection::initializeCApi (s_pDcmClientSynchronousConnection);

    connectionstatus = s_pDcmClientSynchronousConnection->connect ("",60);

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
    {
        status = 0;
    }
    else
    {
	trace (TRACE_LEVEL_ERROR, dcm_clientname + string (" Fail to connect to WaveServer ipaddr=") + ipaddrServer +
	       string (" status=") + connectionstatus);
        status = -1;
    }

    return (status);
}

int query_interface (const char *p_name, val_interface *p_val_interface)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ValClientPhysicalInterface      valClientPhysicalInterface;
    string                          interfaceName                   = p_name;
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->valQueryForInterface (interfaceName, valClientPhysicalInterface);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;

        strncpy (p_val_interface->name,       (valClientPhysicalInterface.getName      ()).c_str (), 64);
        strncpy (p_val_interface->ip_address, (valClientPhysicalInterface.getIpAddress ()).c_str (), 64);

        p_val_interface->mtu = valClientPhysicalInterface.getMtu ();
    }
    else
    {
        clientStatus = -2;
    }

    return (clientStatus);
}

int create_physical_interface (const char *p_name, const unsigned int if_index, const unsigned int mtu, const char *p_description)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;
    string                          description                     = p_description;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createPhysicalInterface (name, if_index, mtu, description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_nsm_event_notify (const unsigned int eventid, const unsigned int parameter)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;

	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmEventNotify(eventid, parameter);
	if (WAVE_MESSAGE_SUCCESS != status)
	{
		clientStatus = -1;
	}

	return (clientStatus);
}

int dcm_vnm_ampp_cdp_notify(const unsigned int cmdcode, const char *ifname,
                    const char *host_mor, const char *port_id, const char *vc)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;
    UI32                            cmdCode                         = cmdcode;
    string                          ifName                          = ifname;
    string                          hostMor                         = host_mor;
    string                          portId                          = port_id;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);
    
    status = pDcmClientSynchronousConnection->vnmAppmCdpNotify(cmdCode, ifName, hostMor, portId, vc);
    if (WAVE_MESSAGE_SUCCESS == status) {
        trace (TRACE_LEVEL_DEBUG, "Success sending the cdp notification to APPM Plugin");
    } else {
        trace (TRACE_LEVEL_ERROR, "Failed sending the cdp notification to APPM Plugin");
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_vnm_vc_cdp_notify(const unsigned int cmdcode, const char *ifname,
                    const char *host_mor, const char *port_id, const char *vc)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForDualDaemons ();

	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;
    UI32                            cmdCode                         = cmdcode;
    string                          ifName                          = ifname;
    string                          hostMor                         = host_mor;
    string                          portId                          = port_id;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);
    
    status = pDcmClientSynchronousConnection->vnmVcCdpNotify(cmdCode, ifName, hostMor, portId, vc);
    if (WAVE_MESSAGE_SUCCESS == status) {
        trace (TRACE_LEVEL_DEBUG, "Success sending the cdp notification to APPM Plugin");
    } else {
        trace (TRACE_LEVEL_ERROR, "Failed sending the cdp notification to APPM Plugin");
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_nsm_notify_postatus_change (const char *po_name, const unsigned int flag) {
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;

	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmNotifyPoStatusConfig(po_name, flag);
	if (WAVE_MESSAGE_SUCCESS != status) {
		clientStatus = -1;
        trace (TRACE_LEVEL_ERROR, "Failed sending the postatus notification");
	} else {
        trace (TRACE_LEVEL_DEBUG, "Success sending the postatus notification");
    }

	return (clientStatus);
}

int dcm_nsm_notify_slot_offline (const unsigned int cmdcode, const unsigned int slotid) {
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;
	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmNotifySlotOffline(cmdcode, slotid);
	if (WAVE_MESSAGE_SUCCESS != status)
	{
		clientStatus = -1;
	}

	return (clientStatus);
}

int dcm_modify_interface_config (void *intfConfig)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;
	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmModifyInterfaceConfig(intfConfig);
	if (WAVE_MESSAGE_SUCCESS != status)
	{
		clientStatus = -1;
	}

	return (clientStatus);
}

int dcm_nsm_notify_phystatus_change (const char *p_name, const unsigned int flag, unsigned int msg_id)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;

	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmNotifyPhyStatusConfig(p_name, flag, msg_id);
	if (WAVE_MESSAGE_SUCCESS != status)
	{
		clientStatus = -1;
	}

    if (WAVE_MESSAGE_SUCCESS != status)
        trace (TRACE_LEVEL_ERROR, "Failed sending the phystatus notification");
    else
        trace (TRACE_LEVEL_DEBUG, "Success sending the phystatus notification");

	return (clientStatus);
}

int dcm_nsm_notify_portrole_change (const char *p_name, const unsigned int port_role, unsigned int exception, unsigned int msg_id)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;

	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->dcmNsmNotifyPortroleConfig(p_name, port_role, exception, msg_id);
	if (WAVE_MESSAGE_SUCCESS != status)
	{
		clientStatus = -1;
	}

    if (WAVE_MESSAGE_SUCCESS != status)
        trace (TRACE_LEVEL_ERROR, "Failed sending the portrole notification");
    else
        trace (TRACE_LEVEL_DEBUG, "Success sending the portrole notification");

	return (clientStatus);
}

int dcm_create_portcfg_interface (const char *if_name_p, const unsigned int fc_uport_num)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = if_name_p;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createPortCfgInterface (name, fc_uport_num);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}
 

int dcm_create_fcoe_et_interface (const char *if_name_p, const unsigned int if_index, const unsigned int fc_uport_num)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = if_name_p;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createFcoeETInterface (name, if_index, fc_uport_num);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
} 

int dcm_create_fcoe_interface (const char *if_name_p, const unsigned int if_index, 
                               const char *bind_name_p, const unsigned int bind_type,
                               const unsigned int fc_uport_num, 
                               const unsigned int vn_num, const unsigned int port_num,
                               const char *p_description)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name = if_name_p;
    string                          description = p_description;
    UI64                            ifIndex = if_index;
    string                          bindName = bind_name_p;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createFcoeInterface(name, ifIndex,
                                                                  bindName, bind_type,
                                                                  fc_uport_num,
                                                                  vn_num, port_num, 
                                                                  description);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_delete_fcoe_interface_bulk (const void *msg_p,
                                    const unsigned int size)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->deleteFcoeInterfaceBulk(msg_p, size);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_create_fcoe_interface_bulk (const void *msg_p,
                                    const unsigned int size)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createFcoeInterfaceBulk(msg_p, size);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_create_fcoe_et_interface_bulk (const void *msg_p,
                                       const unsigned int size)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createFcoeETInterfaceBulk(msg_p, size);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_create_sticky_mac_bulk (const void *msg_p, const unsigned int size)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createStickyMacs(msg_p, size);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return clientStatus;
}

int dcm_update_fcoe_et_interface_bulk (const void *msg_p,
                                       const unsigned int size,
                                       const unsigned int oper)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->updateFcoeETInterfaceBulk(msg_p, size, oper);
    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int create_phy_interface (const char *p_name, const unsigned int if_index, const unsigned int mtu, const char *p_description,const unsigned int isl_capable)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;
    string                          description                     = p_description;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createPhyInterface (name, if_index, mtu, description, isl_capable);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}


/* Function called by NSM to request the Global plug-in to set the domain id of the RBridge */
/* that will hold the primary link for the vLAG */
/* If request is for static vLAG, the partner_key, partner_priority, and *p_partner_system will be zero */
int set_primary_link_domain_id (const char *p_port_channel_name, const unsigned int primary_link_domain_id,
                                const unsigned int partner_key, const unsigned int partner_priority,
                                const char *p_partner_system, const unsigned int command_code, unsigned int *reply_code)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    string                          poName                          = p_port_channel_name;
    string                          partnerSystem;
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    int                            clientStatus                    = 0;
    unsigned int                    mctReplyStatus                  = 0;
    int i;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    for (i = 0; i < VLAG_LACP_GRP_ADDR_LEN; i++) {
      partnerSystem[i] = p_partner_system[i];
    }

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->setPoPrimaryLinkDomainId(poName,
                                                                       primary_link_domain_id, partner_key,
                                                                       partner_priority, partnerSystem,
                                                                       command_code, mctReplyStatus);

    *reply_code = mctReplyStatus;
    
    if (status == WAVE_MESSAGE_SUCCESS) {
        clientStatus = 0;
    } else {
      if (status == WAVE_MESSAGE_ERROR) {
      }
      clientStatus = -1;
    }

    return (clientStatus);
}

int create_po_interface (const char *p_name)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createPoInterface (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int set_vlan_protocalstatus (const char *p_name, const int *p_flag)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;
    bool                            flag                            = p_flag;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->setVlanProtocalStatus (name,flag);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int set_vlan_desc (const char *p_name, const char *p_desc)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;
    string                          desc                            = p_desc;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->setVlanDesc (name,desc);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int create_vlan_interface (const char *p_name)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    string                          name                            = p_name;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createVlanInterface (name,0,0,name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int is_configfile_allowed ()
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    int                             isAllowed                       = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->isConfigFileAllowed(isAllowed);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        // FIXME : Sagar : We need to handle this case.  Otherwise, please remove the status variable.
    }

    return (isAllowed);
}

int is_cli_allowed ()
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    int                             isCliAllowed                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    pDcmClientSynchronousConnection->isCliAllowedAtThisLocation(isCliAllowed);

    return (isCliAllowed);
}

cluster_type get_this_node_cluster_type ()
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    UI32                            clusterType                     = 0;
    

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsGetThisNodeClusterType (clusterType);

	if (WAVE_MESSAGE_SUCCESS != status)
	{
		return VCS_CLUSTER_TYPE_INVALID;
	}

    if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_STANDALONE == clusterType)
    {
        return VCS_CLUSTER_TYPE_STANDALONE; 
    }
    else if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_FABRIC == clusterType)
    {
        return VCS_CLUSTER_TYPE_FABRIC;
    }
    else 
    {
        return VCS_CLUSTER_TYPE_MGMT;
    }    

}

int copy_to_startup_config(const char* path)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->copyToStartupConfig(path);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int copy_running_to_startup(void)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->copyRunningToStartup();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int copy_default_to_startup(void)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->copyDefaultToStartup();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

int copy_snapshot_from_ftp ( const char* snapshotId, unsigned int rbridgeId, const char *localFilename, const char *globalFilename )
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;
    
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->copySnapshotFromFtp( snapshotId, rbridgeId, localFilename, globalFilename );

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {   
        clientStatus = -1;    }   
        
    return (clientStatus);
}

int get_vcs_id ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    VcsClientVcsIdParams  vcsIdParams;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsGetVcsId (vcsIdParams);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (vcsIdParams.getVcsId());
    }
    else
    {
        return (-1);
    }
}

int get_vcs_mode ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    VcsClientVcsModeParams vcsModeParams;
    
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsGetVcsMode (vcsModeParams);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (vcsModeParams.getVcsMode());
    }
    else
    {
        return (-1);
    }
}

int get_vcs_rbridge_id ()
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    unsigned int rbridgeId;

    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsGetRBridgeId (rbridgeId);

    if (WAVE_MESSAGE_SUCCESS == status)
    {   
        return (rbridgeId);
    }
    else
    {   
        return (-1);
    }
}

int test_client_for_fc_mode ()
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }   
        
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForDualDaemons ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;

    status = pDcmClientSynchronousConnection->test_api_for_dual_connection ();
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
              return (-1);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "C-api for dual connection tested successfully");
        return (0);
    } 
}
int is_node_in_cluster (const unsigned int rbrigdeid)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    bool                            isNodeInCluster;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    if (0 >= rbrigdeid || rbrigdeid > 239)
    {
        trace (TRACE_LEVEL_ERROR, "Incorrect rbridge-id mentioned. rbridge-id range : 1-239");
    }
    else
    {
        status = pDcmClientSynchronousConnection->isNodeInCluster(rbrigdeid, isNodeInCluster);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (isNodeInCluster);
    }
}

int is_local_node_in_cluster ()
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    bool                            isNodeInCluster;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->isLocalNodeInCluster (isNodeInCluster);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (isNodeInCluster);
    }
}

int is_gvlan_capable ()
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    bool                            isGvlanCapable = false;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->isGvlanCapable (isGvlanCapable);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (isGvlanCapable);
    }
}

int is_gvlan_enabled ()
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    bool                            isGvlanEnabled = false;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->isGvlanEnabled (isGvlanEnabled);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (isGvlanEnabled);
    }
}

int dcm_get_configuration_number (uint16_t configuration_number[16])
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->getConfigurationNumber (configuration_number);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (status);
    }
    else
    {
        return (-1);
    }
}

int dcm_get_configuration_time (char *p_configurationTime, const int configurationTimeStringSize)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->getConfigurationTime (p_configurationTime, configurationTimeStringSize);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (status);
    }
    else
    {
        return (-1);
    }

}

int dcm_acquire_cluster_lock (const char* serviceString)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->acquireLock (string (serviceString));

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

int dcm_release_cluster_lock (const char* serviceString)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->releaseLock (string (serviceString));

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

int dcm_debug_infrastructure_run_script (const char *p_scriptFilename, const char *p_nosOrBash, const char *p_outputFilename, const char *p_rbridgeIdRange)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    string scriptFilename = "";
    string nosOrBash = "";
    string outputFilename = ""; 
    string rbridgeIdRange = "";

    if (NULL != p_scriptFilename)
    {
        scriptFilename.assign (p_scriptFilename);
    }

    if (NULL != p_nosOrBash)
    {
        nosOrBash.assign (p_nosOrBash);
    }

    if (NULL != p_outputFilename)
    {
        outputFilename.assign (p_outputFilename);
    }

    if (NULL != p_rbridgeIdRange)
    {
        rbridgeIdRange.assign (p_rbridgeIdRange);
    }
    
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForDualDaemons ();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);
    
    status = pDcmClientSynchronousConnection->runDebugScript (scriptFilename, nosOrBash, outputFilename, rbridgeIdRange);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

int dcm_debug_vprintf (const char *p_string, va_list variableArguments)
{
    
    if (false ==  getTerminalMonitorStatus( ) )
    {
        //trace (TRACE_LEVEL_INFO, "dcm_debug_vprintf :: terminal monitor is disabled");        
        return (0);
    }

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    char pCString[1024];
    SI32 numberOfChars;
    string tempString;

    numberOfChars = vsnprintf (pCString, 1024, p_string, variableArguments);

    if (0 < numberOfChars)
    {
        tempString = pCString;

        if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
        {
            return (-1);
        }

        DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

        prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

        status = pDcmClientSynchronousConnection->debugPrint (tempString);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            return (status);
        }
        else
        {
            return (-1);
        }
    }

    return (status);
}

int dcm_debug_printf (const char *p_string, ...)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    va_list variableArguments;

    va_start (variableArguments, p_string);

    status = dcm_debug_vprintf (p_string, variableArguments);

    va_end (variableArguments);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        //trace (TRACE_LEVEL_INFO, "Not Able to send to terminal monitor");
    }

    return (status);
}


// this function is the abstraction layer between the 'C'-based legacy FOS CLI and the
// 'C++'-based DCMd plugin.  This 'C' callback function accepts the parameters from 
// the legacy FOS CLI and passes them to the ValSynchronousConnection 'C++' object
// where the message is ultimately generated to the appropriate plugin
int fos_cli_port_persistent(const unsigned int slot, const unsigned int port, const unsigned int persistentstate)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection =
										DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;

	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->fosCliPortPersistent (slot, port, persistentstate);

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		clientStatus = 0;
	}
	else
	{
		clientStatus = -1;
	}

	return (clientStatus);
}

int fos_cli_port_trunk(const unsigned int slot, const unsigned int port, const unsigned int trunkstate)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection =
                                        DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->fosCliPortTrunk (slot, port, trunkstate);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
        clientStatus = -1;
    }

    return (clientStatus);
}

// This function provides an interface for external 'C'-based user-space
// applications to access the messaging capabilities of the DCM infrastructure
int pod_update_license_data(const char *setName, const int len, const char *configString)
{
	DcmClientSynchronousConnection *pDcmClientSynchronousConnection =
										DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi();
	ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
	SI32                            clientStatus                    = 0;
	trace(TRACE_LEVEL_INFO, "DcmClientInterface pod_update_license_data  : entering");
	prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

	status = pDcmClientSynchronousConnection->podUpdateLicenseData (setName, len, configString);

	if (WAVE_MESSAGE_SUCCESS == status)
	{
		trace(TRACE_LEVEL_INFO, "DcmClientInterface pod_update_license_data  : Val client update success\n");
		clientStatus = 0;
	}
	else
	{
		trace(TRACE_LEVEL_INFO, "DcmClientInterface pod_update_license_data  : Val client update failure");
		clientStatus = -1;
	}
	return (clientStatus);
}

void dcm_set_server_port (const int serverPort)
{
    FrameworkToolKit::setManagementInterfaceReceiverPort(serverPort);
}

int is_fabric_distribution_service_enabled ()
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {   
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    bool                            isDistributionServiceEnabled = false;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->isFabricDistributionEnabled(isDistributionServiceEnabled);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (isDistributionServiceEnabled);
    }
}

int dcm_initialize_client_for_dual_daemons (const char *p_client_name)
{
    SI32                 status           = 0;
    WaveConnectionStatus connectionstatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;

    int dcmInitializeStatus = dcm_initialize_client (p_client_name);
    
    if (0 != dcmInitializeStatus)
    {
        return (dcmInitializeStatus);
    }

    // check for cluster mode
    cluster_type clusterTypeForNode = get_this_node_cluster_type ();
    
    trace (TRACE_LEVEL_DEBUG, string("DcmClientInterface dcm_initialize_client_for_dual_daemons : cluster type = ") + clusterTypeForNode);
    
    if (VCS_CLUSTER_TYPE_FABRIC == clusterTypeForNode)
    {
        char dcm_clientname[80];
        int instance = g_thisSlotInstance;
        string ipaddrServer = "127.1.100.100";
        string ipaddrSelf = "127.1.100.1";

#if defined (AGENT)
        int lcType;
        slot_no2id(get_node_slot(NID_SELF()), &lcType, &instance);
#endif

        sprintf(dcm_clientname, "%s%d", p_client_name, instance);

#if defined (AGENT)
        int rc = 0;
        nin_t nin;
        struct in_addr in;

        if ((rc = ham_get_nin(&nin)) == 0) {
        in.s_addr = nin.bp.cp_anip;
        ipaddrServer = inet_ntoa(in);

        in.s_addr = nin.bp.bp_lnip;
        ipaddrSelf = inet_ntoa(in);

        } else {
        trace (TRACE_LEVEL_ERROR, string ("Fail to get IP info from HASM Error=") + rc);
        return (-1);
        }

#else
        ipaddrSelf = FrameworkToolKit::getThisLocationIpAddress ();
        ipaddrServer = "127.0.0.1";
#endif

        trace (TRACE_LEVEL_DEBUG, string ("WDir=") + FrameworkToolKit::getProcessInitialWorkingDirectory () +
           string (" LID=") + FrameworkToolKit::getThisLocationId () +
           string (" IP=") + FrameworkToolKit::getThisLocationIpAddress () +
           string (" Port=") + FrameworkToolKit::getThisLocationPort ());
#if defined(AGENT)
        trace (TRACE_LEVEL_DEBUG, string ("Current LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());
        (FrameworkToolKit::getPThisLocation ())->resetIpAddress (ipaddrSelf);
        trace (TRACE_LEVEL_DEBUG, string ("New LID IP=") + FrameworkToolKit::getThisLocationIpAddress ());
#endif

        trace (TRACE_LEVEL_DEBUG, string ("DCMCLient=") + dcm_clientname +
           string (" ipaddrSelf=") + ipaddrSelf + string (" ipaddrServer=") + ipaddrServer);
        
        // Set a connection with Ccmd
        s_pDcmClientSynchronousConnectionWithCcm = new DcmClientSynchronousConnection (string (dcm_clientname), ipaddrServer, DcmClientToolKit::getManagementInterfaceReceiverPortForCcmd ());

        prismAssert (NULL != s_pDcmClientSynchronousConnectionWithCcm, __FILE__, __LINE__);

        DcmClientSynchronousConnection::initializeSynchronousConnectionForCcm (s_pDcmClientSynchronousConnectionWithCcm);

        connectionstatus = s_pDcmClientSynchronousConnectionWithCcm->connect ();

        if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == connectionstatus)
        {
            status = 0;
        }
        else
        {
            status = -1;
        }
    }
    return (status);
}

int dcm_uninitialize_client_for_dual_daemons ()
{
    dcm_uninitialize_client ();


    if (NULL != s_pDcmClientSynchronousConnectionWithCcm)
    {
        DcmClientSynchronousConnection::unInitializeSynchronousConnectionForCcm ();

        s_pDcmClientSynchronousConnectionWithCcm->close ();

        delete  s_pDcmClientSynchronousConnectionWithCcm;

        s_pDcmClientSynchronousConnectionWithCcm = NULL;
    }

    return (0);
}

void dcm_vcs_fabric_debug_client_callback (uint32_t status, const void *p_output, void *p_input)
{
#if 1
    if (WAVE_MESSAGE_SUCCESS == status)
    {
        vcs_fabric_debug_client_struct_t *vcs_debug_struct = (vcs_fabric_debug_client_struct_t *) p_output;

        trace (TRACE_LEVEL_INFO,  string ("No of domains in Cluster: ") + vcs_debug_struct->number_of_domain_in_cluster);

        free (vcs_debug_struct->domains_in_cluster);

        free (vcs_debug_struct);
    }
#endif
}

int dcm_ag_delete_fcoe_interface_bulk (void *msg_p)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->deleteAGFcoeInterfaceBulk(msg_p);

    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}

int dcm_ag_create_fcoe_interface_bulk (void *msg_p)
{
    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_SUCCESS;
    SI32                            clientStatus                    = 0;

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->createAGFcoeInterfaceBulk(msg_p);

    if (WAVE_MESSAGE_SUCCESS == status) {
        clientStatus = 0;
    } else {
        clientStatus = -1;
    }

    return (clientStatus);
}
int dcm_nsm_trigger_vlan_unprovision() {

    DcmClientSynchronousConnection *dcmClientSynchConn = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();

    prismAssert (NULL != dcmClientSynchConn, __FILE__, __LINE__);

    ResourceId status = dcmClientSynchConn->dcmNsmTriggerVlanUnprovision();
    return status;
}

int load_bulking_config_file (const char *configFileName)
{
    if (true != (DcmClientSynchronousConnection::isCApiInitialized ()))
    {
        return (-1);
    }

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForCApi ();
    ResourceId                      status                          = WAVE_MESSAGE_ERROR;
    
    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    string configFile = configFileName; 

    status = pDcmClientSynchronousConnection->loadZoneBulkConfiguration (configFile);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        return (-1);
    }
    else
    {
        return (0);
    }
    return (0);
}
} // extern C
