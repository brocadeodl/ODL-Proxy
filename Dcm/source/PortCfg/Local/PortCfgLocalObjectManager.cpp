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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : dchung                                                     *
 **************************************************************************/

#include "DcmResourceIdEnums.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Global/PortCfgGlobalObjectManager.h"
#include "PortCfg/Local/PortCfgPortCfgPortStateUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgTrunkUpdateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "PortCfg/Local/PortCfgPortCfgPortTranslateMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "PortCfg/Local/PortCfgPortCfgCreateMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "PortCfg/Local/PortFCCfgLocalManagedObject.h"
#include "PortCfg/Local/PortCfgUpdateFCInfoMessage.h"
#include "PortCfg/Local/PortCfgFCPortTranslateMessage.h"
#include "PortCfg/Local/PortCfgFCPortInterfaceInfoMessage.h"
#include "PortCfg/Local/PortCfgFCPortDetailedInfoMessage.h"
#include "PortCfg/Local/PortCfgFibreChannelClearMessage.h"
#include "PortCfg/Local/PortCfgFibreChannelSfpShowMessage.h"
#include "PortCfg/Local/PortCfgFCPortLoginInfoMessage.h"
#include "PortCfg/Local/PortCfgGetFCPortInfoMessage.h"
#include "DcmResourceIdEnums.h"
#include "fabos/license.h"
#include "vcs.h"
#include <cmath>

extern "C" {
#include <utils/claFltr.h>
#include <switch/port.h>
#include <fabric/getid.h>
#include "sys/fabos/fabcfg.h"
#include <switch/switch.h>
#include <fabric/fabric.h>
#include <fkio/fkiolib.h>
#include "thresh/fwdPub.h"
#include "iswitch/isw_ipc.h"
#include "agd/public.h"
}

#include <stdio.h>

#define NOT_SPECIFIED -2
#define QUERY_DEFAULT_PORT "0/1"
#define MAXNAMELEN 256
#define MAXROWCHAR 100
#define MAX_PORTSHOW_ARR 32
#define PC_FC_MAX_PORT 17
namespace DcmNs {
    static void copy_cfg_from_FC_object(pcfg_bm_t &portcfg, PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject);

    /* Related to Port Status Message */
    static void addExtendedPortText(int portType, int uport, char *buf, int buf_size);
    static int isUpstream(int uport, mmap_port_info_t *pt_info);
    static int isEPortDisabled(int uport, mmap_port_info_t *pt_info);
    static int isEPortDisabled2(int uport, mmap_port_info_t *pt_info);
    static int isDnstream(int uport, mmap_port_info_t *pt_info);
    static int isEXPortDisabled(int uport, mmap_port_info_t *pt_info);
    static int isSegmented(int uport, mmap_port_info_t *pt_info);
    static int isTmasterport(int uport, mmap_port_info_t *pt_info);
    static int isTrunkPort(int uport, mmap_port_info_t *pt_info);
    static int isEXPortActive(int uport, mmap_port_info_t *pt_info);
    static void getEportDisabledReason(int uport, char *buf, int size);
    static void getEportSegmentedReason(int uport, char *buf, int size);
    static void getEXPortFabricID(int uport, char *buf, int size);
    static int disabledReasonPrt(reason_t orig_reason, char *buf, int size);
    static int segReasonPrt(u_int orig_reason, char *buf, int size);
    static int segDetReasonPrt(u_int det_reason, char *buf, int size, u_int short_reason);

    /* Fill PortCfgFCPortInterfaceInfoMessage */
    static int port_type_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void trunk_master_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void port_state_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void wwn_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void port_status_fill_info(int switchPort, int portType,
                                      PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void port_health_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage);
    static void port_state_info(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg);
    static int retrieve_portinfo_proc(int swno, int uport, PortCfgFCPortDetailedInfoMessage *pFC_DetailMsg);
    static int retrieve_port_proc(int swno, int uport, PortCfgFCPortDetailedInfoMessage *pFC_DetailMsg);
    static int port_getalloc_buffers(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg);
    static int check_sufficient_buf (int switchport, int longdistmode, int estd_dist);

    /* Update FC DB with Last Clearing Counter  */
    static void update_FC_DB_last_clearing_value(PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject);
    static void port_performance_detail(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg);
    static void portPrint(u_int  n, u_int base, char *buf);

    /* FC SFP Show */
    static int sfp_show_fill_info(PortCfgFibreChannelSfpShowMessage *pFC_SfpShowMsg);
    static void getport_protocolstatus(int switchPort, char *buf);
    static void getportWWN_Connecteddevice_Speed(int swno, PortCfgFCPortLoginInfoMessage *pFC_logininfoMsg);

    PortCfgLocalObjectManager::PortCfgLocalObjectManager ()
    : WaveLocalObjectManager  (getClassName ())
    {
        associateWithVirtualWaveObjectManager (PortCfgGlobalObjectManager::getInstance ());
        PortFCCfgLocalManagedObject    PortFCCfgLocalManagedObject    (this);
        PortFCCfgLocalManagedObject.setupOrm ();
        addManagedClass (PortFCCfgLocalManagedObject::getClassName ());
        addOperationMap (PORTCFGPORTCFGPORTTRANSLATE, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgPortCfgPortTranslateMessageHandler));
        addOperationMap (PORTCFGUPDATEFCINFO, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgUpdateFCInfoMessageHandler));
        addOperationMap (PORTCFGFCPORTTRANSLATE, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFCPortTranslateMessageHandler));
        addOperationMap (PORTCFGFCPORTINTERFACEINFO, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFCPortInterfaceInfoMessageHandler));
        addOperationMap (PORTCFGFCPORTDETAILEDINFO, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFCPortDetailedInfoMessageHandler));
        addOperationMap (PORTCFGFIBRECHANNELCLEAR, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFibreChannelClearMessageHandler));
        addOperationMap (PORTCFGFIBRECHANNELSFPSHOW, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFibreChannelSfpShowMessageHandler));
        addOperationMap (PORTCFGFCPORTLOGININFO, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgFCPortLoginInfoMessageHandler));
        addOperationMap (PORTCFGGETFCPORTINFO, reinterpret_cast<PrismMessageHandler> (&PortCfgLocalObjectManager::PortCfgGetFCPortInfoMessageHandler));
        m_ConfigPushed = false;
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    PortCfgLocalObjectManager::~PortCfgLocalObjectManager ()
    {
    }

    PortCfgLocalObjectManager  *PortCfgLocalObjectManager::getInstance()
    {

        static PortCfgLocalObjectManager *pPortCfgLocalObjectManager = new PortCfgLocalObjectManager ();


        WaveNs::prismAssert (NULL != pPortCfgLocalObjectManager, __FILE__, __LINE__);


        return(pPortCfgLocalObjectManager);
    }

    PrismServiceId  PortCfgLocalObjectManager::getPrismServiceId()
    {
        return((getInstance ())->getServiceId ());
    }

    void  PortCfgLocalObjectManager::externalStateSynchronization  (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization)
    {

        char convertToString[40];
        char trace_string[128];
        int local_id;
        pt_cfg_prop_t *portcfg;
        sys_pcfg_bm_t portsys;
        int i;

        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber()) == 1 &&
           (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW)) {

           int vcsEnabled = DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED;
           sprintf(trace_string, "PortCfgLocalObjectManager::externalStateSynchronization"
              " - getFssStageNumber %d type %d, FCSW %d",
              pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() ,
              pWaveAsynchronousContextForExternalStateSynchronization->getServiceType(),  FCSW);
              trace(TRACE_LEVEL_INFO, trace_string);

            fabosInit(MYSWITCH);

            if (vcsEnabled) {
                //FC DB Creation only if VCS is enabled
                ResourceId fc_status = WAVE_MESSAGE_SUCCESS;
                fc_status = addFcPortcfgEntries(0);

                if (fc_status != WAVE_MESSAGE_SUCCESS) {
                    trace (TRACE_LEVEL_ERROR,
                           string("PortCfgLocalObjectManager::ESS - Error Creating FC DB - Status") + fc_status);
                    // TBD: Error Handling
                } else {
                    trace (TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager::ESS - FC DB Creation Success"));
                }
            } else {
                trace (TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager::ESS - FC DB Not Created in Non VCS Mode"));
            }

            if ((local_id = DcmToolKit::getLocalMappedId()) == -1) {
                trace(TRACE_LEVEL_FATAL, "externalStateSynchronization local id = -1");
                local_id = 1;
            }

            sprintf(trace_string, "externalStateSynchronization local_id = %d\n", local_id);

            trace(TRACE_LEVEL_DEBUG, trace_string);

            portcfg = (pt_cfg_prop_t *)calloc(SMI_MAX_PORTS, sizeof (pt_cfg_prop_t));

            /* I think we still need to do this for all ports as FCoE may not be doing it yet */
            for (i = 0; i < MAX_PORT; i++) {
                /* first init all to default settings */
                portcfg[i].port = i;

                /* this should only happen if user has never saved active config,
                 * push down temp entries until the real ones are created
                 */

                trace(TRACE_LEVEL_DEBUG, trace_string);

                portcfg[i].pcfg_bm._pvt_port_persistently_disabled = 1;
                portcfg[i].pcfg_bm.tport = 0;
                portcfg[i].pcfg_bm.vcxlt_linit = 0;
                portcfg[i].pcfg_bm.disable_eport = 0;
                portcfg[i].pcfg_bm.npiv_enabled = 0;
                portcfg[i].pcfg_bm.auto_disable = 0;
                portcfg[i].pcfg_bm.rlimit = 0;
                portcfg[i].pcfg_bm.mirror_port = 0;
                portcfg[i].pcfg_bm.cr_recov = 0;
                portcfg[i].pcfg_bm.fport_buffers = 0;

                portcfg[i].lgcl_pcfg_bm  = portcfg[i].pcfg_bm;

                if (!vcsEnabled && portNOSIsFC(i)) {
                   portDisable_direct(i);
                }

            }

            if (vcsEnabled && (licenseCheck(BASE_FCOE_LICENSE) == 1)) {

                // Pushing FC DB Entries To Switch Driver only if VCS is Enabled & FCOE License is found
                for (i = 0; i < PC_FC_MAX_PORT; i++) {
                    /* fill in saved config */
                    sprintf(convertToString, "%d/%d/%d", local_id, 0, i);
                    string id2(convertToString);

                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                                                   QueryFCLocalManagedObjectById(id2);

                    if (pPortFCCfgLocalManagedObject) {

                        sprintf(trace_string, "PortFCCfgLocalObjectManager::ESS - %s"
                              "exists shutdown = %d switch_port = %d\n", id2.c_str(),
                              pPortFCCfgLocalManagedObject->getPortStateShutdown(),
                              pPortFCCfgLocalManagedObject->getSwitchPort());

                        trace(TRACE_LEVEL_DEBUG, trace_string);

                        if (switchSetPortTuple(pPortFCCfgLocalManagedObject->getSwitchPort(),
                                 pPortFCCfgLocalManagedObject->getSlot(),
                                 pPortFCCfgLocalManagedObject->getCliPort(),
                                 pPortFCCfgLocalManagedObject->getSlot(),
                                 pPortFCCfgLocalManagedObject->getCliPort(), eINT_FC, eTYPE_FC, -1)) {
                            sprintf(trace_string, "externalStateSynchronization switchsetPortTuple (FC)"
                                 "failed blade %d port %d\n", pPortFCCfgLocalManagedObject->getSlot(),
                                  pPortFCCfgLocalManagedObject->getCliPort());
                            trace(TRACE_LEVEL_INFO, trace_string);
                        }

                        /* the remote tuple information will be filled in for ISLs by FSPF
                         * but we will init it to default here
                         */
                        switchSetPortTupleRemote(pPortFCCfgLocalManagedObject->getSwitchPort(),
                                  -1, -1, -1, eINT_INVALID, eTYPE_INVALID, -1);

                        /* Filling PortCfg Structure with FC DB values */

                        copy_cfg_from_FC_object(portcfg[pPortFCCfgLocalManagedObject->getSwitchPort()].pcfg_bm, pPortFCCfgLocalManagedObject);

                        portcfg[pPortFCCfgLocalManagedObject->getSwitchPort()].estd_dist =
                        pPortFCCfgLocalManagedObject->getDistance();

                        portcfg[pPortFCCfgLocalManagedObject->getSwitchPort()].lgcl_pcfg_bm  = portcfg[pPortFCCfgLocalManagedObject->getSwitchPort()].pcfg_bm;

                        delete (pPortFCCfgLocalManagedObject);
                    }
                } // end for all ports
            } //end of vcs enabled check for FC Entries

            portsys.sw_num = MYSWITCH;
            portsys.num_ports = i; //In non-vcs mode here i=MAX_PORT 
            portsys.entries = portcfg;

            if (switchSetPortCfg(&portsys)) {
                trace(TRACE_LEVEL_FATAL, "!!!!externalStateSynchronization returned error\n");
            }

            m_ConfigPushed = true;

            free(portcfg);
        }

        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForExternalStateSynchronization->callback();
    }

    PrismMessage  *PortCfgLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode) {
        case PORTCFGPORTCFGPORTTRANSLATE :
            pPrismMessage = new PortCfgPortCfgPortTranslateMessage ();
            break;
        case PORTCFGUPDATEFCINFO :
            pPrismMessage = new PortCfgUpdateFCInfoMessage ();
            break;
        case PORTCFGFCPORTTRANSLATE :
            pPrismMessage = new PortCfgFCPortTranslateMessage ();
            break;
        case PORTCFGFCPORTINTERFACEINFO :
            pPrismMessage = new PortCfgFCPortInterfaceInfoMessage ();
            break;
        case PORTCFGFCPORTDETAILEDINFO :
            pPrismMessage = new PortCfgFCPortDetailedInfoMessage ();
            break;
        case PORTCFGFIBRECHANNELCLEAR :
            pPrismMessage = new PortCfgFibreChannelClearMessage ();
            break;
        case PORTCFGFIBRECHANNELSFPSHOW :
            pPrismMessage = new PortCfgFibreChannelSfpShowMessage ();
            break;
        case PORTCFGFCPORTLOGININFO :
            pPrismMessage = new PortCfgFCPortLoginInfoMessage ();
            break;
        case PORTCFGGETFCPORTINFO :
            pPrismMessage = new PortCfgGetFCPortInfoMessage ();
            break;
        default :
            pPrismMessage = NULL;
        }

        return(pPrismMessage);
    }

    WaveManagedObject  *PortCfgLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PortFCCfgLocalManagedObject::getClassName ()) == managedClassName) {
            pWaveManagedObject = new PortFCCfgLocalManagedObject(this);
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return(pWaveManagedObject);
    }


    void  PortCfgLocalObjectManager::PortCfgPortCfgPortTranslateMessageHandler( PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            // Don't need a transaction for queries.....
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::PortTranslateToSwitchPort),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgPortCfgPortTranslateMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgUpdateFCInfoMessageHandler( PortCfgUpdateFCInfoMessage *pPortCfgUpdateFCInfoMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCInfoUpdateEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgUpdateFCInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgFCPortTranslateMessageHandler( PortCfgFCPortTranslateMessage *pPortCfgFCPortTranslateMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortTranslateToSwitchPort),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFCPortTranslateMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgFCPortInterfaceInfoMessageHandler( PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortInterfaceInfo),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFCPortInterfaceInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgFCPortDetailedInfoMessageHandler( PortCfgFCPortDetailedInfoMessage *pPortCfgFCPortDetailedInfoMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortDetailedInfo),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFCPortDetailedInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgFibreChannelClearMessageHandler( PortCfgFibreChannelClearMessage *pPortCfgFibreChannelClearMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortClearCounters),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFibreChannelClearMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  PortCfgLocalObjectManager::PortCfgFibreChannelSfpShowMessageHandler( PortCfgFibreChannelSfpShowMessage *pPortCfgFibreChannelSfpShowMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortSFPShow),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFibreChannelSfpShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    void  PortCfgLocalObjectManager::PortCfgFCPortLoginInfoMessageHandler( PortCfgFCPortLoginInfoMessage *pPortCfgFCPortLoginInfoMessage) {
        // Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::FCPortLoginShow),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgFCPortLoginInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }
    void  PortCfgLocalObjectManager::PortCfgGetFCPortInfoMessageHandler( PortCfgGetFCPortInfoMessage *pPortCfgGetFCPortInfoMessage)
    {
        // Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            // Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            // Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::GetFCPortInfo),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPortCfgGetFCPortInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    /* FC Translation Handler */

    ResourceId PortCfgLocalObjectManager::FCPortTranslateToSwitchPort(PrismSynchronousLinearSequencerContext *pPortCfgFCPortTranslateMessageContext)
    {
        PortCfgFCPortTranslateMessage *pPortCfgFCPortTranslateMessage =
        reinterpret_cast<PortCfgFCPortTranslateMessage *> (pPortCfgFCPortTranslateMessageContext->getPPrismMessage ());

        if (pPortCfgFCPortTranslateMessage->getSwitchPort() == -1) {
            /*
             * Retrieves the SwitchPort, Only if valid Id is Provided.
            */
            PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
            QueryFCLocalManagedObjectById(pPortCfgFCPortTranslateMessage->getId());

            if (pPortFCCfgLocalManagedObject) {
                trace (TRACE_LEVEL_DEBUG, string ("found object for id ") +
                       pPortCfgFCPortTranslateMessage->getId() + string ("value ") +
                       pPortFCCfgLocalManagedObject->getSwitchPort());

                pPortCfgFCPortTranslateMessage->setSwitchPort(pPortFCCfgLocalManagedObject->getSwitchPort());
                trace(TRACE_LEVEL_DEBUG,
                string("PortFCCfgLocalObjectManager::FCPortTranslateToSwitchPort - Update Successfull"));
                // GC call to clear the MO
                pPortCfgFCPortTranslateMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
            }
            return(WAVE_MESSAGE_SUCCESS);
        } else {
            /*
             * Retrieves the Id from the DB only if Valid SwitchPort & RbridgeId is sent.
            */
            PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
            QueryFCLocalManagedObjectFromSwitchPortRbridgeId(
                                                            pPortCfgFCPortTranslateMessage->getSwitchPort(),
                                                            pPortCfgFCPortTranslateMessage->getRbridgeId(), 1);

            if (pPortFCCfgLocalManagedObject) {
                trace (TRACE_LEVEL_DEBUG, string ("PortFCCfgLocalObjectManager::"
                    "QueryFCLocalManagedObjectById - Found object for switchport: ") +
                       pPortCfgFCPortTranslateMessage->getSwitchPort()
                       + string("RbridgeId: ") + pPortCfgFCPortTranslateMessage->getRbridgeId()
                       + string("id: ") + pPortCfgFCPortTranslateMessage->getId());

                pPortCfgFCPortTranslateMessage->setId(pPortFCCfgLocalManagedObject->getId());
                // GC call to clear the MO
                pPortCfgFCPortTranslateMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
            }
            return(WAVE_MESSAGE_SUCCESS);
        }
        return(WAVE_MESSAGE_SUCCESS);
    }

    ResourceId PortCfgLocalObjectManager::FCPortInterfaceInfo (PrismSynchronousLinearSequencerContext *pPortCfgFCPortInterfaceInfoMessageContext)
    {

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage =
            reinterpret_cast<PortCfgFCPortInterfaceInfoMessage *>
            (pPortCfgFCPortInterfaceInfoMessageContext->getPPrismMessage ());

            string PortId = pPortCfgFCPortInterfaceInfoMessage->getPortId();
            SI32 switchPort = pPortCfgFCPortInterfaceInfoMessage->getSwitchPort();
            trace (TRACE_LEVEL_DEBUG, string ("Entering PortCfgLocalObjectManager::FCPortInterfaceInfo - PortId: ") + PortId);

            /* Fill the InterfaceInfo Message when SwitchPort has valid value */
            if (switchPort >= 0 ) {
                trace (TRACE_LEVEL_DEBUG, string("Port: ") + switchPort);

                /* Trunk Master & Trunk */
                trunk_master_fill_info(switchPort, pPortCfgFCPortInterfaceInfoMessage);

                /* Port State */
                port_state_fill_info(switchPort, pPortCfgFCPortInterfaceInfoMessage);

                /* Port Type */
                int portType = port_type_fill_info(switchPort, pPortCfgFCPortInterfaceInfoMessage);

                /*PID */
                char portaddr[10];
                u_int pid = portId(switchPort);
                snprintf(portaddr, 10,"%06x", pid);
                string portaddress = portaddr;
                pPortCfgFCPortInterfaceInfoMessage->setPortaddr(portaddress);

                /* WWN */
                wwn_fill_info(switchPort, pPortCfgFCPortInterfaceInfoMessage);

                /* Port Status */
                port_status_fill_info(switchPort, portType, pPortCfgFCPortInterfaceInfoMessage);

                /* Port Health */
                port_health_fill_info(switchPort, pPortCfgFCPortInterfaceInfoMessage);

                /* Actual Distance */
                int actual_distance = 0, estimated_distance;
                if (portLxDistance(switchPort, &actual_distance, &estimated_distance)) {
                    pPortCfgFCPortInterfaceInfoMessage->setActualDistance(0);
                    trace (TRACE_LEVEL_DEBUG,
                             string("Cannot get Actual Distance for port ") + switchPort);
                } else {
                    pPortCfgFCPortInterfaceInfoMessage->setActualDistance(actual_distance);
                }

                /* BufferAllocated  & DesiredCredit*/
                buf_usage_t buf_info;
                if (portBufferAlloc(switchPort, &buf_info)) {
                    pPortCfgFCPortInterfaceInfoMessage->setBufferAllocated(0);
                    pPortCfgFCPortInterfaceInfoMessage->setDesiredCredit(0);
                    trace (TRACE_LEVEL_DEBUG,
                          string("Cannot get portBufferAlloc for port ") + switchPort);
                } else {
                    trace (TRACE_LEVEL_DEBUG, string("portBufferAlloc for port ") + switchPort
                           + string(" - Allocated Buffer: ") + buf_info.alloc_bufs );
                    pPortCfgFCPortInterfaceInfoMessage->setBufferAllocated(buf_info.alloc_bufs);
                    trace (TRACE_LEVEL_DEBUG, string("portBufferAlloc for port ") + switchPort
                           + string(" - DesiredCredit: ") + buf_info.alloc_bufs );
                    pPortCfgFCPortInterfaceInfoMessage->setDesiredCredit(buf_info.desired_bufs);
                }
                // check for FCOE_BASE license
                if (licenseCheck(BASE_FCOE_LICENSE) == 1) {
                    pPortCfgFCPortInterfaceInfoMessage->setLicensed(1);
                } else {
                    pPortCfgFCPortInterfaceInfoMessage->setLicensed(0);
                }
            }
            trace (TRACE_LEVEL_DEBUG, string ("Exiting PortCfgLocalObjectManager::FCPortInterfaceInfo"));
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId PortCfgLocalObjectManager::FCPortDetailedInfo (PrismSynchronousLinearSequencerContext *pPortCfgFCPortDetailedInfoMessageContext)
    {
        const char *pchTemp;
        string phyportStatus;
        string speedsupport;
        string lastclearing_value = "";
        fabosInit(MYSWITCH);
        int ret = 0;
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            PortCfgFCPortDetailedInfoMessage *pPortCfgFCPortDetailedInfoMessage =
            reinterpret_cast<PortCfgFCPortDetailedInfoMessage *>
            (pPortCfgFCPortDetailedInfoMessageContext->getPPrismMessage ());
            SI32 switchPort = pPortCfgFCPortDetailedInfoMessage->getSwitchPort();

            /* Fill the InterfaceInfo Message when SwitchPort has valid value */
            if (switchPort >= 0 ) {
                trace (TRACE_LEVEL_DEBUG, string("Port: ") + switchPort);
                /* Port State */
                port_state_info(switchPort, pPortCfgFCPortDetailedInfoMessage);

                /* Media present */
                pchTemp = portPhysName(switchPort);
                if (strcasecmp("No_Module", pchTemp) == 0) {
                    phyportStatus = "Pluggable media not present\n";
                } else {
                    phyportStatus = "Pluggable media present\n";
                }
                pPortCfgFCPortDetailedInfoMessage->setPortMediaPresence(phyportStatus);

                /*Actual Line Speed of the port is displayed only when link is up*/
                if (portActive(switchPort)) {
                    speedsupport += portGetActualLinkSpeed(switchPort);
                } else {
                    speedsupport += "";
                }
                trace (TRACE_LEVEL_DEBUG, string("Actualspeed: ") + speedsupport);
                pPortCfgFCPortDetailedInfoMessage->setPortLineSpeedActual(speedsupport);

                string PortId = pPortCfgFCPortDetailedInfoMessage->getPortId();
                PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject;
                if ((pPortFCCfgLocalManagedObject = QueryFCLocalManagedObjectById(PortId))) {
                    trace (TRACE_LEVEL_DEBUG,
                           string("PortCfgLocalObjectManager::FCPortDetailedInfo - ClearCounter TimeStamp")
                           + pPortFCCfgLocalManagedObject->getClear_counter_ts());
                    lastclearing_value += "Last clearing of show interface counters: ";
                    lastclearing_value += pPortFCCfgLocalManagedObject->getClear_counter_ts();
                    pPortCfgFCPortDetailedInfoMessage->setPortCounters(lastclearing_value);
                    // updateWaveManagedObject(pPortFCCfgLocalManagedObject); Not required
                    pPortCfgFCPortDetailedInfoMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                } else {
                    trace (TRACE_LEVEL_ERROR,
                           string("PortCfgLocalObjectManager::FCPortDetailedInfo - Error: ClearCounter TimeStamp"));
                    lastclearing_value += "Last clearing of show interface counters: 00:00:00";
                }

                ret = retrieve_port_proc(MYSWITCH, switchPort, pPortCfgFCPortDetailedInfoMessage);
                if (ret < 0) {
                    trace (TRACE_LEVEL_ERROR, string ("Error retrieving Stats Proc Entry"));
                    return WAVE_MESSAGE_SUCCESS; // TBD: Fix Error Msg
                }

                ret = retrieve_portinfo_proc(MYSWITCH, switchPort, pPortCfgFCPortDetailedInfoMessage);
                if (ret < 0) {
                    trace (TRACE_LEVEL_ERROR, string ("Error retrieving Info Proc Entry"));
                    return WAVE_MESSAGE_SUCCESS; // TBD: Fix Error Msg
                }
                /* Get Portbuffer Info */
                ret = port_getalloc_buffers(switchPort, pPortCfgFCPortDetailedInfoMessage);
                if (ret < 0) {
                    trace (TRACE_LEVEL_ERROR, string ("Error retrieving portbuffer Info "));
                    return WAVE_MESSAGE_SUCCESS;
                }

                /*Rate info */
                port_performance_detail(switchPort, pPortCfgFCPortDetailedInfoMessage);
            }
        }
        return WAVE_MESSAGE_SUCCESS;
    }


    /* Clear FC Port Counters */
    ResourceId PortCfgLocalObjectManager::FCPortClearCounters (PrismSynchronousLinearSequencerContext *pPortCfgFibreChannelClearMessageContext)
    {
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            PortCfgFibreChannelClearMessage *pPortCfgFibreChannelClearMessage =
            reinterpret_cast<PortCfgFibreChannelClearMessage *>
            (pPortCfgFibreChannelClearMessageContext->getPPrismMessage ());

            SI32 switchPort=pPortCfgFibreChannelClearMessage->getSwitchPort();

            /* Fill the InterfaceInfo Message when SwitchPort has valid value */
            if ((switchPort >= 0) &&
                (pPortCfgFibreChannelClearMessage->getCmdCode() == FC_CMD_CODE_CLEAR_UPORT)) {
                trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::FCPortClearCounters - Port: ")
                       + switchPort + string(" Command Code: ") + pPortCfgFibreChannelClearMessage->getCmdCode());

                /* Clear Counters Per Port */
                if (portClrStats(switchPort) >= 0) {
                    trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::FCPortClearCounters -"
                                                     " Cleared Counters for Port: ") + switchPort);
                    pPortCfgFibreChannelClearMessage->setCmdCode(FC_CMD_CODE_CLEAR_SUCCESS); //Success
                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                    QueryFCLocalManagedObjectById(pPortCfgFibreChannelClearMessage->getPortId());

                    if (pPortFCCfgLocalManagedObject) {
                        update_FC_DB_last_clearing_value(pPortFCCfgLocalManagedObject);
                        updateWaveManagedObject(pPortFCCfgLocalManagedObject);
                    }
                    pPortCfgFibreChannelClearMessageContext->
                    addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                } else {
                    trace (TRACE_LEVEL_ERROR, string("PortCfgLocalObjectManager::FCPortClearCounters -"
                                                     " Error Clearing Counters for Port: ") + switchPort);
                    pPortCfgFibreChannelClearMessage->setCmdCode(FC_CMD_CODE_ERR_CLEAR_UPORT); //Error
                }
            } else if ( pPortCfgFibreChannelClearMessage->getCmdCode() == FC_CMD_CODE_CLEAR_ALL) {
                /* Clear All Port Counters
                 * Query the FC DB to determine the existence of port and
                 * clear the port counter and update the timestamp.
                 */
                SI32 switch_port;
                int rbridgeId = pPortCfgFibreChannelClearMessage->getRbridgeId();
                for (switchPort = 0; switchPort < PC_FC_MAX_PORT; switchPort++) {

                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                    QueryFCLocalManagedObjectFromSwitchPortRbridgeId(switchPort, rbridgeId);

                    if (pPortFCCfgLocalManagedObject == NULL)
                        continue;
                    if (pPortFCCfgLocalManagedObject) {
                        switch_port = pPortFCCfgLocalManagedObject->getSwitchPort();
                        if (portClrStats(switch_port) >= 0) {
                            pPortCfgFibreChannelClearMessage->setCmdCode(FC_CMD_CODE_CLEAR_SUCCESS);

                            update_FC_DB_last_clearing_value(pPortFCCfgLocalManagedObject);

                            updateWaveManagedObject(pPortFCCfgLocalManagedObject);
                        } else {
                            trace (TRACE_LEVEL_ERROR, string("PortCfgLocalObjectManager::FCPortClearCounters - ALL "
                                                             "Error Clearing Counters for Port: ") + switchPort);
                        }

                        pPortCfgFibreChannelClearMessageContext->
                        addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                    }
                }
            } else if ( pPortCfgFibreChannelClearMessage->getCmdCode() == FC_CMD_CODE_CLEAR_SLOTID) {
                /* Clear Slot Port Counters */
                int rbridgeId = pPortCfgFibreChannelClearMessage->getRbridgeId();
                UI32 slotId = pPortCfgFibreChannelClearMessage->getSlotId();

                SI32 switch_port;
                for (switchPort = 0; switchPort < PC_FC_MAX_PORT; switchPort++) {
                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                    QueryFCLocalManagedObjectFromSwitchPortRbridgeId(switchPort, rbridgeId);

                    if (pPortFCCfgLocalManagedObject == NULL)
                        continue;

                    if (pPortFCCfgLocalManagedObject) {
                        unsigned int mapId, bladeNo, bladePort;
                        if (pPortFCCfgLocalManagedObject->getId().find("/") !=
                            pPortFCCfgLocalManagedObject->getId().rfind("/")) {
                            sscanf(pPortFCCfgLocalManagedObject->getId().c_str(),
                                   "%u/%u/%u", &mapId, &bladeNo, &bladePort);
                        }
                        if (slotId == bladeNo) {
                            switch_port = pPortFCCfgLocalManagedObject->getSwitchPort();
                            if (portClrStats(switch_port) >= 0) {
                                trace (TRACE_LEVEL_DEBUG, string("Slot Match, port: ") + switch_port);
                                pPortCfgFibreChannelClearMessage->setCmdCode(FC_CMD_CODE_CLEAR_SUCCESS);
                                update_FC_DB_last_clearing_value(pPortFCCfgLocalManagedObject);
                                updateWaveManagedObject(pPortFCCfgLocalManagedObject);
                            } else {
                                trace (TRACE_LEVEL_ERROR, string("PortCfgLocalObjectManager::FCPortClearCounters"
                                                                 "- Slot Id: ") + slotId +
                                       string(" - Error Clearing Counters for Port: ") + switchPort);
                            }
                        } else {
                            trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::FCPortClearCounters"
                                                             "- Slot Id: ") + slotId + string(" - Slot No Match for Port: ") + switchPort);
                        }
                        pPortCfgFibreChannelClearMessageContext->
                        addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                    }
                }
            } else {
                trace (TRACE_LEVEL_ERROR, string("PortCfgLocalObjectManager::FCPortClearCounters"
                                                 " - Invalid Port/Command Code"));
                pPortCfgFibreChannelClearMessage->setCmdCode(FC_CMD_CODE_ERR_INVALID_UPORT);
            }
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    /*
     * Get FC Port Info()
     * Used to retrieve FC Port Shut Status.
    */
    ResourceId PortCfgLocalObjectManager::GetFCPortInfo (PrismSynchronousLinearSequencerContext *pPortCfgGetFCPortInfoMessageContext)
    {
        SI32 rbridgeId;
        SI32 switchPort;
        bool shut = 1;
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            PortCfgGetFCPortInfoMessage *pPortCfgGetFCPortInfoMessage =
            reinterpret_cast<PortCfgGetFCPortInfoMessage *>
            (pPortCfgGetFCPortInfoMessageContext->getPPrismMessage ());

            switch (pPortCfgGetFCPortInfoMessage->getCmdCode()) {
            case FC_CMD_CODE_ALL_SHUT:
                rbridgeId = pPortCfgGetFCPortInfoMessage->getRbridgeId();
                /*
                 * FC Ports for CallistoF-24 0-7
                 */
                for (switchPort = 0; switchPort < PC_FC_MAX_PORT; switchPort++) {
                    if (portNOSIsFC(switchPort) == -1)
                        continue;

                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                    QueryFCLocalManagedObjectFromSwitchPortRbridgeId(switchPort, rbridgeId);

                    if (pPortFCCfgLocalManagedObject == NULL)
                        continue;

                    if (pPortFCCfgLocalManagedObject) {
                        /* Get Port Shut Status */
                        trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::GetFCPortInfo - Port: ") + switchPort
                               + string(" - Status ") + pPortFCCfgLocalManagedObject->getPortStateShutdown());
                        shut &= pPortFCCfgLocalManagedObject->getPortStateShutdown();
                    }
                    pPortCfgGetFCPortInfoMessageContext->
                    addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                    /* Breaking the loop, when one of the FC port is not shut */
                    if (shut == !TRUE) {
                        trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::GetFCPortInfo"
                                                         " - FC_RESP_CODE_NOT_ALL_SHUT"));
                        pPortCfgGetFCPortInfoMessage->setRespCode(FC_RESP_CODE_NOT_ALL_SHUT);
                        break;
                    }
                }

                if (shut == TRUE) {
                    trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::GetFCPortInfo - FC_RESP_CODE_ALL_SHUT"));
                    pPortCfgGetFCPortInfoMessage->setRespCode(FC_RESP_CODE_ALL_SHUT);
                }
                break;
            default:
                break;
            }
        } else {
            /* make like as if all FC ports are always SHUT in non-VCS mode */
            PortCfgGetFCPortInfoMessage *pPortCfgGetFCPortInfoMessage =
            reinterpret_cast<PortCfgGetFCPortInfoMessage *>
            (pPortCfgGetFCPortInfoMessageContext->getPPrismMessage ());

            switch (pPortCfgGetFCPortInfoMessage->getCmdCode()) {
            case FC_CMD_CODE_ALL_SHUT:
                trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::GetFCPortInfo - non-VCS - FC_RESP_CODE_ALL_SHUT"));
                pPortCfgGetFCPortInfoMessage->setRespCode(FC_RESP_CODE_ALL_SHUT);
                break;
            default:
                break;
            }
        }



        return WAVE_MESSAGE_SUCCESS;
    }

    /* SFP Show for FC Ports */
    ResourceId PortCfgLocalObjectManager::FCPortSFPShow (PrismSynchronousLinearSequencerContext *pPortCfgFibreChannelSfpShowMessageContext) {
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            PortCfgFibreChannelSfpShowMessage *pPortCfgFibreChannelSfpShowMessage =
            reinterpret_cast<PortCfgFibreChannelSfpShowMessage *>
            (pPortCfgFibreChannelSfpShowMessageContext->getPPrismMessage ());


            /* Fill the InterfaceInfo Message when SwitchPort has valid value */
            if (pPortCfgFibreChannelSfpShowMessage->getCmdCode() == FC_CMD_CODE_ONE_PORT) {
                trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::SFP - FC_CMD_CODE_ONE_PORT"));
                sfp_show_fill_info(pPortCfgFibreChannelSfpShowMessage);
            } else if (pPortCfgFibreChannelSfpShowMessage->getCmdCode() == FC_CMD_CODE_ALL_PORTS) {
                trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::SFP - FC_CMD_CODE_ALL_PORTS"));
                SI32 rbridgeId = DcmToolKit::getLocalMappedId();
                SI32 switchPort;

                vector <string> portId;
                vector <SI32> switch_Port;
                vector <UI32> mapped_Id;

                for (switchPort = 0; switchPort < PC_FC_MAX_PORT; switchPort++) {
                    PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                    QueryFCLocalManagedObjectFromSwitchPortRbridgeId(switchPort, rbridgeId, 1);

                    if (pPortFCCfgLocalManagedObject == NULL)
                        continue;

                    if (pPortFCCfgLocalManagedObject) {
                        portId.push_back(pPortFCCfgLocalManagedObject->getId().c_str());
                        mapped_Id.push_back(pPortFCCfgLocalManagedObject->getRbridgeId());
                        switch_Port.push_back(pPortFCCfgLocalManagedObject->getSwitchPort());
                    }
                    pPortCfgFibreChannelSfpShowMessageContext->
                    addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                }

                /* Fill the Message with valid port details */
                pPortCfgFibreChannelSfpShowMessage->setPortId(portId);
                pPortCfgFibreChannelSfpShowMessage->setRbridgeId(mapped_Id);
                pPortCfgFibreChannelSfpShowMessage->setSwitchPort(switch_Port);

                /* Retrieve SFP Info and fill the message */
                sfp_show_fill_info(pPortCfgFibreChannelSfpShowMessage);
            }
        }
        return WAVE_MESSAGE_SUCCESS;
    }
    /* Show FC Login information for FC Ports */
    ResourceId PortCfgLocalObjectManager::FCPortLoginShow
    (PrismSynchronousLinearSequencerContext *pPortCfgFCPortLoginInfoMessageContext) {
        vector <UI32> switch_Port;
        vector <UI32> mapped_Id;
        vector <string> interface_Id;
        vector <string> portstatus;
        vector <string> speed;

        UI32 rbridgeId;
        string interfaceId;
        UI32 cmdcode;
        char buf[20];
        string portStatusMsg = "";
        string actualspeed = "";
        trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::FCPortLoginShow - Entering"));
        PortCfgFCPortLoginInfoMessage *pPortCfgFCPortLoginInfoMessage =
        reinterpret_cast<PortCfgFCPortLoginInfoMessage *> (pPortCfgFCPortLoginInfoMessageContext->getPPrismMessage ());

        switch (pPortCfgFCPortLoginInfoMessage->getCmdCode()) {
		case FC_CMD_CODE_LOCAL:
        case FC_CMD_CODE_RBRIDGE_ID:
            rbridgeId = pPortCfgFCPortLoginInfoMessage->getRbridgeId()[0];

            for (SI32 switchPort = 0; switchPort < PC_FC_MAX_PORT; switchPort++) {
                PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                QueryFCLocalManagedObjectFromSwitchPortRbridgeId(switchPort, rbridgeId);

                if (pPortFCCfgLocalManagedObject == NULL) {
                    continue;
                } else {
                    interface_Id.push_back(pPortFCCfgLocalManagedObject->getId().c_str());
                    mapped_Id.push_back(pPortFCCfgLocalManagedObject->getRbridgeId());
                    switch_Port.push_back(pPortFCCfgLocalManagedObject->getSwitchPort());

                    /* get port protocol status and speed */
                    getport_protocolstatus(pPortFCCfgLocalManagedObject->getSwitchPort(), buf);
                    portStatusMsg = buf;
                    portstatus.push_back(portStatusMsg);
                    actualspeed = portGetActualLinkSpeed(pPortFCCfgLocalManagedObject->getSwitchPort());
                    speed.push_back(actualspeed);
                }
                pPortCfgFCPortLoginInfoMessageContext->
                addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
            }
            break;
        case FC_CMD_CODE_INTERFACE_ID:
            interfaceId = pPortCfgFCPortLoginInfoMessage->getPortId()[0];

            if (NsmUtils::isValidThreeTuple(interfaceId)) {
                /* Query FC DB By Id */
                PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                QueryFCLocalManagedObjectById(interfaceId);

                if (pPortFCCfgLocalManagedObject == NULL) {
                    cmdcode = FC_CMD_CODE_ERR_INVALID_INTERFACE_ID;
                    pPortCfgFCPortLoginInfoMessage->setCmdCode(cmdcode);
                    return WAVE_MESSAGE_SUCCESS;
                } else {
                    interface_Id.push_back(pPortFCCfgLocalManagedObject->getId().c_str());
                    mapped_Id.push_back(pPortFCCfgLocalManagedObject->getRbridgeId());
                    switch_Port.push_back(pPortFCCfgLocalManagedObject->getSwitchPort());
                    /* get port protocol status and speed */
                    getport_protocolstatus(pPortFCCfgLocalManagedObject->getSwitchPort(), buf);
                    portStatusMsg = buf;
                    portstatus.push_back(portStatusMsg);
                    actualspeed = portGetActualLinkSpeed(pPortFCCfgLocalManagedObject->getSwitchPort());
                    speed.push_back(actualspeed);
                }
                trace(TRACE_LEVEL_DEBUG, string("PID: ") +  portId(pPortFCCfgLocalManagedObject->getSwitchPort()) +
                      string(" InterfaceId: ") + pPortFCCfgLocalManagedObject->getId() +
                      string(" mapped_Id: ") + pPortFCCfgLocalManagedObject->getRbridgeId() +
                      string(" switch_Port: ") + pPortFCCfgLocalManagedObject->getSwitchPort());

            } else {
                cmdcode = FC_CMD_CODE_ERR_INVALID_INTERFACE_ID;
                pPortCfgFCPortLoginInfoMessage->setCmdCode(cmdcode);
                return WAVE_MESSAGE_SUCCESS;
            }
            break;
        default:
            return(FC_IF_ERR_NO_ARGS);
        }

        /* Fill the Messages */
        pPortCfgFCPortLoginInfoMessage->setPortId(interface_Id);
        pPortCfgFCPortLoginInfoMessage->setRbridgeId(mapped_Id);
        pPortCfgFCPortLoginInfoMessage->setIndex(switch_Port);
        pPortCfgFCPortLoginInfoMessage->setPortstatus(portstatus);
        pPortCfgFCPortLoginInfoMessage->setPortspeed(speed);

        // Get connected devices WWN one shot for all ports after PortId size set
        getportWWN_Connecteddevice_Speed(MYSWITCH, pPortCfgFCPortLoginInfoMessage);

        return WAVE_MESSAGE_SUCCESS;
    }

    /* Update FC DB */
    ResourceId PortCfgLocalObjectManager::FCInfoUpdateEntries(PrismSynchronousLinearSequencerContext *pPortCfgUpdateFCInfoMessageContext) {
        string portStr = "";
        string convertedPortId = "";
        PortCfgUpdateFCInfoMessage *pPortCfgUpdateFCInfoMessage =
        reinterpret_cast<PortCfgUpdateFCInfoMessage *> (pPortCfgUpdateFCInfoMessageContext->getPPrismMessage ());

        UI32 DbEntryType = pPortCfgUpdateFCInfoMessage->getDbEntryType();
        bool send_to_switch = false;

        trace (TRACE_LEVEL_DEBUG,
               string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - portId: ") +
               pPortCfgUpdateFCInfoMessage->getId()
               + string (" FillWord: ") + pPortCfgUpdateFCInfoMessage->getFillWord()
               + string (" LongDistance: ") + pPortCfgUpdateFCInfoMessage->getLongDistance()
               + string (" VcLinkInit: ") + pPortCfgUpdateFCInfoMessage->getVcLinkInit()
               + string (" IslRRdyMode: ") + pPortCfgUpdateFCInfoMessage->getIslRRdyMode()
               + string (" Status: ") + pPortCfgUpdateFCInfoMessage->getStatus()
               + string (" Desired distance: ") + pPortCfgUpdateFCInfoMessage->getDistance());

        portStr = pPortCfgUpdateFCInfoMessage->getId();
        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            if (NsmUtils::isValidTwoTuple(portStr)) {
                NsmUtils::getThreeTupleIfName(portStr, convertedPortId, 0);
                trace (TRACE_LEVEL_INFO,
               string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - VCS mode-0 - convertedPortId: ")
                       + convertedPortId);
            } else {
                convertedPortId = portStr;
                trace (TRACE_LEVEL_INFO,
               string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - VCS mode-1 - convertedPortId: ")
                       + convertedPortId);
            }
        } else {
            /* Stand-alone mode */
            if (NsmUtils::isValidThreeTuple(portStr)) {
                NsmUtils::getTwoTupleIfName(portStr, convertedPortId);
                trace (TRACE_LEVEL_INFO,
                  string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - "
                  "Stand-alone mode-0 - convertedPortId: ") + convertedPortId);
            } else {
                convertedPortId = portStr;
                trace (TRACE_LEVEL_INFO,
                       string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Stand-alone "
                       "mode-1 - convertedPortId: ") + convertedPortId);
            }
        }

        if (licenseCheck(BASE_FCOE_LICENSE) == 0) {
          trace (TRACE_LEVEL_ERROR, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - FCOE License Missing"));
          return(FC_BASE_FCOE_LICENSE_MISSING);
        }

        /* Query FC DB By Id*/
        PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
        QueryFCLocalManagedObjectById(pPortCfgUpdateFCInfoMessage->getId());

        if (pPortFCCfgLocalManagedObject) {
        pPortCfgUpdateFCInfoMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
            switch (pPortCfgUpdateFCInfoMessage->getDbEntryType()) {
            case fc_db_type_speed:

                if (pPortFCCfgLocalManagedObject->getSpeed() != pPortCfgUpdateFCInfoMessage->getSpeed()) {

                    pPortFCCfgLocalManagedObject->setSpeed(pPortCfgUpdateFCInfoMessage->getSpeed());
                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Speed: ")
                           + pPortCfgUpdateFCInfoMessage->getSpeed());
                    send_to_switch = true;
                }
                break;
            case fc_db_type_fillWord:

                if (pPortFCCfgLocalManagedObject->getFillWord() != pPortCfgUpdateFCInfoMessage->getFillWord()) {

                    pPortFCCfgLocalManagedObject->setFillWord(pPortCfgUpdateFCInfoMessage->getFillWord());
                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Fillword: ")
                           + pPortCfgUpdateFCInfoMessage->getFillWord());
                    send_to_switch = true;

                }
                break;
            case fc_db_type_longDistance:

                if (pPortFCCfgLocalManagedObject->getLongDistance() != pPortCfgUpdateFCInfoMessage->getLongDistance()) {
                    //Distance field is for Desired distance,setting to default becoz it depends on Longdistance modes
                    pPortFCCfgLocalManagedObject->setLongDistance(pPortCfgUpdateFCInfoMessage->getLongDistance());
                    pPortFCCfgLocalManagedObject->setDistance(0);

                    /*VClink depends on long distance modes so set it to default in normal mode  */
                    if (pPortCfgUpdateFCInfoMessage->getLongDistance() == brocade_interface_l0) {
                        pPortFCCfgLocalManagedObject->setVcLinkInit(0);
                    }
                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - LongDistance: ")
                           + pPortCfgUpdateFCInfoMessage->getLongDistance() + string ("Desiredistance: ") +
                           + pPortFCCfgLocalManagedObject->getDistance());
                    send_to_switch = true;
                }
                break;
            case fc_db_type_vcLinkInit:

                //VClink is not allowed when longdistance in normal mode
                if ((pPortFCCfgLocalManagedObject->getLongDistance() != brocade_interface_l0) &&
                    (pPortFCCfgLocalManagedObject->getVcLinkInit() != pPortCfgUpdateFCInfoMessage->getVcLinkInit())) {

                    pPortFCCfgLocalManagedObject->setVcLinkInit (pPortCfgUpdateFCInfoMessage->getVcLinkInit());
                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - VcLinkInit: ")
                           + pPortCfgUpdateFCInfoMessage->getVcLinkInit());
                    send_to_switch = true;
                } else {
                    return(FC_vc_NotSupported);
                }
                break;
            case fc_db_type_desireddistance:

                trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries -Desireddistance: ")
                       + pPortCfgUpdateFCInfoMessage->getDistance() + string ("LongDistance: ") +
                       pPortFCCfgLocalManagedObject->getLongDistance());

                //Based on Long distance option set the desired distance
                if ((pPortFCCfgLocalManagedObject->getLongDistance() == brocade_interface_ld) &&
                    (pPortFCCfgLocalManagedObject->getDistance() != pPortCfgUpdateFCInfoMessage->getDistance())) {

                    //Based on the speed set the desired distance level for LD
                    if (pPortFCCfgLocalManagedObject->getSpeed() == brocade_interface_1gbps) {
                        if (pPortCfgUpdateFCInfoMessage->getDistance() <= 3000) {
                            if (check_sufficient_buf (pPortFCCfgLocalManagedObject->getSwitchPort(),
                                                      pPortFCCfgLocalManagedObject->getLongDistance(), pPortCfgUpdateFCInfoMessage->getDistance()) < 0) {
                                return(FC_Noavail_buf);
                            } else {
                                pPortFCCfgLocalManagedObject->setDistance(pPortCfgUpdateFCInfoMessage->getDistance());
                                send_to_switch = true;
                            }
                        } else {
                            return(FC_Distance_LD_1Gb);
                        }
                    } else if (pPortFCCfgLocalManagedObject->getSpeed() == brocade_interface_2gbps) {
                        if (pPortCfgUpdateFCInfoMessage->getDistance() <= 1500) {
                            if (check_sufficient_buf (pPortFCCfgLocalManagedObject->getSwitchPort(),
                                                      pPortFCCfgLocalManagedObject->getLongDistance(), pPortCfgUpdateFCInfoMessage->getDistance()) < 0) {
                                return(FC_Noavail_buf);
                            } else {
                                pPortFCCfgLocalManagedObject->setDistance(pPortCfgUpdateFCInfoMessage->getDistance());
                                send_to_switch = true;
                            }
                        } else {
                            return(FC_Distance_LD_2Gb);
                        }
                    } else if ((pPortFCCfgLocalManagedObject->getSpeed() == brocade_interface_4gbps) ||
                               (pPortFCCfgLocalManagedObject->getSpeed() == brocade_interface_8gbps)) {
                        if (pPortCfgUpdateFCInfoMessage->getDistance() <= 750) {
                            if (check_sufficient_buf (pPortFCCfgLocalManagedObject->getSwitchPort(),
                                                      pPortFCCfgLocalManagedObject->getLongDistance(), pPortCfgUpdateFCInfoMessage->getDistance()) < 0) {
                                return(FC_Noavail_buf);
                            } else {
                                pPortFCCfgLocalManagedObject->setDistance(pPortCfgUpdateFCInfoMessage->getDistance());
                                send_to_switch = true;
                            }
                        } else {
                            return(FC_Distance_LD_4and8Gb);
                        }
                    } else if (pPortFCCfgLocalManagedObject->getSpeed() == brocade_interface_auto) {
                        /*
                         * DEFECT 370957 - Desired distance is not configured, when the port speed is Auto and
                         * the Long distance mode is LD. Returning Appropriate Message.
                         */
                        return(FC_NO_CFG_DESIRED_DIST);
                    }
                } else if ((pPortFCCfgLocalManagedObject->getLongDistance() == brocade_interface_ls) &&
                           (pPortFCCfgLocalManagedObject->getDistance() != pPortCfgUpdateFCInfoMessage->getDistance())) {
                    //on successful case update switchdriver
                    if (pPortCfgUpdateFCInfoMessage->getDistance() >= 10) {

                        if (check_sufficient_buf (pPortFCCfgLocalManagedObject->getSwitchPort(),
                                                  pPortFCCfgLocalManagedObject->getLongDistance(), pPortCfgUpdateFCInfoMessage->getDistance()) < 0) {
                            return(FC_Noavail_buf);
                        } else {
                            pPortFCCfgLocalManagedObject->setDistance(pPortCfgUpdateFCInfoMessage->getDistance());
                            send_to_switch = true;
                        }
                    } else {
                        return(FC_Distance_LS);
                    }

                } else {
                    return(FC_Distance_NotSupported);
                }
                break;
            case fc_db_type_islRRdyMode:
                if (pPortFCCfgLocalManagedObject->getIslRRdyMode() != pPortCfgUpdateFCInfoMessage->getIslRRdyMode()) {
                    /* Need to disable trunk before enabling isl r_rdy mode */
                    if ((pPortCfgUpdateFCInfoMessage->getIslRRdyMode() == 1) &&
                        (pPortFCCfgLocalManagedObject->getTrunkEnable()== 1)) {
                        return(FC_ERR_DISABLE_TRUNK);
                    } else {
                        pPortFCCfgLocalManagedObject->setIslRRdyMode(pPortCfgUpdateFCInfoMessage->getIslRRdyMode());
                        trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - ISLRRdyMode:")
                               + pPortCfgUpdateFCInfoMessage->getIslRRdyMode());
                        send_to_switch = true;
                    }
                }
                break;
            case fc_db_type_shutdown:

                if (pPortCfgUpdateFCInfoMessage->getStatus() == 0) {
                    // no shut case
                    pPortFCCfgLocalManagedObject->setPortStateShutdown(0);
                } else {
                    // shut case
                    pPortFCCfgLocalManagedObject->setPortStateShutdown(1);
                }

                trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Shutdown: ")
                       + pPortCfgUpdateFCInfoMessage->getStatus());
                send_to_switch = true;
                break;
            case fc_db_type_trunk:
                if (pPortFCCfgLocalManagedObject->getTrunkEnable() != pPortCfgUpdateFCInfoMessage->getStatus()) {
                    /* Need to disable isl r_rdy mode before enabling trunk */
                    if ((pPortCfgUpdateFCInfoMessage->getStatus() == 1) &&
                        (pPortFCCfgLocalManagedObject->getIslRRdyMode() == 1)) {
                        return(FC_ERR_DISABLE_ISL);
                    } else {
                        pPortFCCfgLocalManagedObject->setTrunkEnable(pPortCfgUpdateFCInfoMessage->getStatus());
                        trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Trunk")
                               + pPortCfgUpdateFCInfoMessage->getStatus());
                        send_to_switch = true;
                    }
                }
                break;
            case fc_db_type_modelock:
                trace (TRACE_LEVEL_DEBUG, string ("Plom::FCInfoUpdateEntries - portlockMode:") +
                       pPortFCCfgLocalManagedObject->getLock_port());
                if (pPortFCCfgLocalManagedObject->getLock_port() != pPortCfgUpdateFCInfoMessage->getLock_port()) {

                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - lock_port: ")
                           + pPortCfgUpdateFCInfoMessage->getLock_port());
                    pPortFCCfgLocalManagedObject->setLock_port(pPortCfgUpdateFCInfoMessage->getLock_port());

                    switch (pPortCfgUpdateFCInfoMessage->getLock_port()) {
                    case FC_CFG_MODE_E_PORT:
                        pPortFCCfgLocalManagedObject->setDisableEport(0);
                        break;
                    case FC_CFG_MODE_N_PORT:
                        if (agd_is_accessgateway()) {
                            pPortFCCfgLocalManagedObject->setDisableEport(1);
                        } else {
                            trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - AG Mode:")
                                   + agd_is_accessgateway());
                            return(FC_ERR_AG_MODE_NOT_SUPPORTED);
                        }
                        break;
                    case FC_CFG_MODE_F_PORT:
                        pPortFCCfgLocalManagedObject->setDisableEport(1);
                        break;
                    case FC_CFG_MODE_AUTO:
                        pPortFCCfgLocalManagedObject->setDisableEport(0);
                        break;
                    default:
                        trace (TRACE_LEVEL_INFO, "No Such Configuration");
                        break;
                    }

                    trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - lockmode: ")
                           + pPortCfgUpdateFCInfoMessage->getLock_port());

                    send_to_switch = true;
                }
                break;
            default:
                trace (TRACE_LEVEL_ERROR, string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Invalid DB Type"));
                /* TBD: Return Appropriate Error Message */
                break;
            } // Switch Case  end

            /* User input from CLI committing to DB */
            updateWaveManagedObject(pPortFCCfgLocalManagedObject);

            if (send_to_switch) {
                /* Update to Switch Driver */
                sendPortFCCfgSettingsToSwitchDriver(pPortFCCfgLocalManagedObject, DbEntryType, R_INVALID_REASON);
            }
        } else {
            trace (TRACE_LEVEL_ERROR,
                   string ("PortCfgLocalObjectManager::FCInfoUpdateEntries - Managed Object Doesnt Exists"));
            /* TBD: Return Appropriate Error Message */
        }

        return WAVE_MESSAGE_SUCCESS;
    } /* End of FCInfoUpdateEntries */

    int PortCfgLocalObjectManager::addFcPortcfgEntries (int startNewTransaction) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject;
        bool    vcsEnabled = !TRUE;
        char    portId[10];
        UI32    mapped_id = 0;
        string  portStr="";
        UI32    cli_port_number = 1;
        UI32    slot = 0;
        SI32    switch_port = 0;
        string  queryDefaultPort = "";
        vector<PortFCCfgLocalManagedObject *> pfccfgObjs;
        fabosInit(MYSWITCH);

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            vcsEnabled = TRUE;
            mapped_id = DcmToolKit::getLocalMappedId();
            snprintf(portId, 10, "%d/0/1", mapped_id);
            queryDefaultPort = portId;
        } else {
            queryDefaultPort = QUERY_DEFAULT_PORT;
        }

        trace (TRACE_LEVEL_DEBUG, string("PortCfgLocalObjectManager::addFcPortcfgEntry - queryDefaultPort: ")
               + queryDefaultPort);
        /* start transaction here.  Have to commit before deleting objects */
        startTransaction();
        
        /* Install FC Portcfg DB */
        trace (TRACE_LEVEL_INFO, "PortCfgLocalObjectManager::addFcPortcfgEntry - FC DB Created For First Time");

        // if we are here for the very first time, install default db entries
        for (switch_port = 0; switch_port < PC_FC_MAX_PORT; switch_port++) {
            SI32 is_FC_port = portNOSIsFC(switch_port);

            if (is_FC_port == -1) {
                continue;
            }

            trace (TRACE_LEVEL_DEBUG,
                   string("PortCfgLocalObjectManager::addFcPortcfgEntry - Port(") + switch_port
                   + string(") - Ret Val: ") + portNOSIsFC(switch_port)
                   + string("- getInterfaceType: ") + (int)getInterfaceType(switch_port));

            /* If FC Port Create an Entry in the FC DB */
            if (is_FC_port) {
                if (vcsEnabled) {
                    snprintf(portId, 10, "%d/%d/%d", mapped_id, slot, cli_port_number);
                } else {
                    snprintf(portId, 10, "%d/%d", slot, cli_port_number);
                }

                portStr = "";
                portStr = portId;
                /* Check for existence (MO added during boot) and continue */
                if ((pPortFCCfgLocalManagedObject = QueryFCLocalManagedObjectById(portStr))) {
                    trace(TRACE_LEVEL_INFO,
                          string("PortCfgLocalObjectManager::addFcPortcfgEntry - FC DB Entry"
                          " exists - CLI PORT: ") + cli_port_number + string(" - portStr: ") +
                          portStr + string (" - switch_port: ") + switch_port);
                    pfccfgObjs.push_back(pPortFCCfgLocalManagedObject);
                    cli_port_number++;
                    continue;
                }

                trace(TRACE_LEVEL_INFO,
                      string("PortCfgLocalObjectManager::addFcPortcfgEntry - FC DB addFcPortcfgEntry" 
                      "called - CLI PORT: ") + cli_port_number + string(" - portStr: ") + portStr +
                      string (" - switch_port: ") + switch_port);

                pPortFCCfgLocalManagedObject = new PortFCCfgLocalManagedObject(this);
                pfccfgObjs.push_back(pPortFCCfgLocalManagedObject);

                pPortFCCfgLocalManagedObject->setId(portStr);
                /* Defect TR000393341:
                 * As FC Port are licensed, the FCOE_BASE License is mandatory for port to
                 * be in "no shut" state,
                 * in absence of license the ports will be in "shut" state, when the DB is being created.
                 */
                if (licenseCheck(BASE_FCOE_LICENSE) == 1) {
                    pPortFCCfgLocalManagedObject->setPortStateShutdown(0); /* Port Enabled */
                } else {
                    pPortFCCfgLocalManagedObject->setPortStateShutdown(1); /* Port Disabled */
                }
                pPortFCCfgLocalManagedObject->setFillWord(0);
                pPortFCCfgLocalManagedObject->setLongDistance(0);
                pPortFCCfgLocalManagedObject->setVcLinkInit(0);
                pPortFCCfgLocalManagedObject->setIslRRdyMode(0);
                pPortFCCfgLocalManagedObject->setStatus(0);
                pPortFCCfgLocalManagedObject->setSwitchPort(switch_port);
                pPortFCCfgLocalManagedObject->setCliPort(cli_port_number);
                pPortFCCfgLocalManagedObject->setType("Fi");
                pPortFCCfgLocalManagedObject->setSlotPort(0);
                pPortFCCfgLocalManagedObject->setSlot(slot);
                pPortFCCfgLocalManagedObject->setOldDistance(0);
                pPortFCCfgLocalManagedObject->setTrunkEnable(1);
                pPortFCCfgLocalManagedObject->setSpeed(0);
                pPortFCCfgLocalManagedObject->setFcAccConfig(0);
                pPortFCCfgLocalManagedObject->setDisableEport(0);
                pPortFCCfgLocalManagedObject->setLportLockdown(0);
                pPortFCCfgLocalManagedObject->setPrivLportLockdown(0);
                pPortFCCfgLocalManagedObject->setVcxltLinit(0);
                pPortFCCfgLocalManagedObject->setDelayFlogi(0);
                pPortFCCfgLocalManagedObject->setIslInterop(0);
                pPortFCCfgLocalManagedObject->setPvtPortPersistentlyDisabled(0);
                pPortFCCfgLocalManagedObject->setDistance(0);
                pPortFCCfgLocalManagedObject->setBufStarvFlag(0);
                pPortFCCfgLocalManagedObject->setLport_halfduplex(0);
                pPortFCCfgLocalManagedObject->setLport_fairness(0);
                pPortFCCfgLocalManagedObject->setCredit_sharing(0);
                pPortFCCfgLocalManagedObject->setPvt_port_permanently_disabled(0);
                pPortFCCfgLocalManagedObject->setNpiv_enabled(0);
                pPortFCCfgLocalManagedObject->setNpiv_configured(0);
                pPortFCCfgLocalManagedObject->setRscn_suppress(0);
                pPortFCCfgLocalManagedObject->setEx_port(0);
                pPortFCCfgLocalManagedObject->setMirror_port(0);
                pPortFCCfgLocalManagedObject->setSoft_neg(0);
                pPortFCCfgLocalManagedObject->setQos_enabled(0);
                pPortFCCfgLocalManagedObject->setQos_configured(0);
                pPortFCCfgLocalManagedObject->setRlimit(0);
                pPortFCCfgLocalManagedObject->setAlpa13(0);
                pPortFCCfgLocalManagedObject->setExport_mode(0);
                pPortFCCfgLocalManagedObject->setLosig(0);
                pPortFCCfgLocalManagedObject->setAsn_frc_hwretry(0);
                pPortFCCfgLocalManagedObject->setCr_recov(0);
                pPortFCCfgLocalManagedObject->setAuto_disable(0);
                pPortFCCfgLocalManagedObject->setFport_buffers(0);
                pPortFCCfgLocalManagedObject->setRbridgeId(mapped_id);

                /* Last Clearing Value of Port Counter */
                pPortFCCfgLocalManagedObject->setClear_counter_ts("00:00:00");
                if (agd_is_accessgateway() != 1) {
                    /* Default Value: G-Port */
                    pPortFCCfgLocalManagedObject->setLock_port(FC_CFG_MODE_AUTO);
                } else {
                    /* Default Value: N-Port; when in AG Mode */
                    pPortFCCfgLocalManagedObject->setLock_port(FC_CFG_MODE_N_PORT);
                }
                cli_port_number++;
            }// if (is_FC_port) end
        } // for
        /* Commit before deleting object */
        status = commitTransaction ();
        for (UI32 j = 0; j < pfccfgObjs.size(); j++) {
           delete pfccfgObjs[j];
        }
        if (FRAMEWORK_SUCCESS == status) {
           trace(TRACE_LEVEL_INFO,string("PortCfgLocalObjectManager::addFcPortcfgEntry - FC DB Added"));
           status = WAVE_MESSAGE_SUCCESS;
        } else {
           trace (TRACE_LEVEL_FATAL,"PortCfgLocalObjectManager::addFcPortcfgEntry: "
                    "Committransaction failed.Status:" + FrameworkToolKit::localize (status));
           // prismAssert (false, __FILE__, __LINE__);
        }
        trace(TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager::addFcPortcfgEntry - Exit"));
        return(status);
    }
    /*
     * Impossible to fill in the FC portnumber during initial install,
     * so need to do it later during FSS (external state sync_
     *
     * If install and WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT (happens on secondary when
     * added to the cluster for the first time.)
     * then add proper default entries, all MO are wiped out prior to this.
     */
    void PortCfgLocalObjectManager::install(
                                           WaveAsynchronousContextForBootPhases *
                                           pWaveAsynchronousContextForBootPhases) {

        ResourceId fc_status = WAVE_MESSAGE_SUCCESS;
        /* Install FC Portcfg DB */
        UI32 Swbd_model = DcmToolKit::getSwBDModel();

        trace (TRACE_LEVEL_INFO,
               string("PortCfgLocalObjectManager::install - Swbd_model=") + Swbd_model);

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            if (pWaveAsynchronousContextForBootPhases->getBootReason() ==
                            WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT) {
                trace (TRACE_LEVEL_INFO,
                 string("PortCfgLocalObjectManager::install - WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT"));
                fc_status = addFcPortcfgEntries(0);
            }
        } else {
            trace (TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager::install - VCS DISABLED"));
        }

        if (fc_status != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_ERROR,
               string("PortCfgLocalObjectManager::install - Error Creating FC DB - Status") + fc_status);
            // TBD: Error Handling
        } else {
            trace (TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager::install - FC DB Creation Success"));
        }

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (fc_status);
        pWaveAsynchronousContextForBootPhases->callback ();
        return;
    }

    /* Query By Id for FC DB */
    PortFCCfgLocalManagedObject *PortCfgLocalObjectManager::QueryFCLocalManagedObjectById(string id) {
        WaveManagedObjectSynchronousQueryContext
        synchronousQueryContext (PortFCCfgLocalManagedObject::getClassName ());

        /* Get the object corresponding to this mapped ID */
        synchronousQueryContext.addAndAttribute (new AttributeString (id, "id"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "PortCfgLocalObjectManager::QueryFCLocalManagedObjectById:"
                       " Cannot find Port FC Cfg entry");
                pResults->clear ();
                delete pResults;
                return(NULL);
            } else if (1 == numberOfResults) {
                PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                dynamic_cast<PortFCCfgLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return(pPortFCCfgLocalManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::QueryFCLocalManagedObjectById"
                       ": There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
                return(NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::QueryFCLocalManagedObjectById"
                   ": System failure");
            prismAssert (false, __FILE__, __LINE__);
            return(NULL);
        }

        return(NULL);
    }


    FcoeETIntfLocalManagedObject *PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromId(string id) {

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (FcoeETIntfLocalManagedObject::getClassName ());

        /* Get the object corresponding to this mapped ID */
        synchronousQueryContext.addAndAttribute (new AttributeString (id, "et_if_name"));

        vector<WaveManagedObject *> *pResults =
        querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromId Cannot find Fcoe ET entry");
                pResults->clear ();
                delete pResults;
                return(NULL);
            } else if (1 == numberOfResults) {
                FcoeETIntfLocalManagedObject *pFcoeETIntfLocalManagedObject =
                dynamic_cast<FcoeETIntfLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return(pFcoeETIntfLocalManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromId There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
                return(NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromId System failure");
            prismAssert (false, __FILE__, __LINE__);
            return(NULL);
        }

        return(NULL);
    }


	FcoeIntfLocalManagedObject *PortCfgLocalObjectManager::queryFcoeIntfLocalManagedObjectFromId(string id) {

		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (FcoeIntfLocalManagedObject::getClassName ());

		/* Get the object corresponding to this mapped ID */
		synchronousQueryContext.addAndAttribute (new AttributeString (id, "fcoe_if_name"));

		vector<WaveManagedObject *> *pResults =
			querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, "PortCfgLocalObjectManager::queryFcoeIntfLocalManagedObjectFromId Cannot find Fcoe ET entry");
				pResults->clear ();
				delete pResults;
				return (NULL);
			} else if (1 == numberOfResults) {
				FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject =
					dynamic_cast<FcoeIntfLocalManagedObject *> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return (pFcoeIntfLocalManagedObject);
			} else {
				pResults->clear ();
				delete pResults;
				trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeIntfLocalManagedObjectFromId There can be only one local node.");
				prismAssert (false, __FILE__, __LINE__);
				return (NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeIntfLocalManagedObjectFromId System failure");
			prismAssert (false, __FILE__, __LINE__);
			return (NULL);
		}

		return (NULL);
	}


    FcoeETIntfLocalManagedObject *PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId) {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (FcoeETIntfLocalManagedObject::getClassName ());

        UI32 uport, rb_id, i;
        uport = (SI32)switchPort;
        rb_id = (SI32)rbridgeId;

        /* Get the object corresponding to this mapped switchPort */
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (uport,"fc_uport_num"));
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (rb_id,"rb_id"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromSwitchPort"
                       "RbridgeId:Error Finding Fcoe MO entry");
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return(NULL);
            } else if (1 == numberOfResults) {
                FcoeETIntfLocalManagedObject *pFcoeETIntfLocalManagedObject =
                dynamic_cast<FcoeETIntfLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return(pFcoeETIntfLocalManagedObject);
            } else {
                trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromSwitchPort"
                       "RbridgeId: There can be only one local node. Results found:");
                for (i = 0; i < numberOfResults; i++) {
                    FcoeETIntfLocalManagedObject *pFcoeETIntfLocalManagedObject =
                    dynamic_cast<FcoeETIntfLocalManagedObject *> ((*pResults)[i]);
                    trace (TRACE_LEVEL_DEBUG, string("ifindex ") + i + string(":") +
                           pFcoeETIntfLocalManagedObject->getIfindex());
                }
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return(NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromSwitchPort"
                   "RbridgeId: System failure");
            prismAssert (false, __FILE__, __LINE__);
            return(NULL);
        }

        return(NULL);
    }



    FcoeIntfLocalManagedObject *PortCfgLocalObjectManager::queryFcoeLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId) {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (FcoeIntfLocalManagedObject::getClassName ());

        UI32 uport, rb_id, i;
        uport = (SI32)switchPort;
        rb_id = (SI32)rbridgeId;

        /* Get the object corresponding to this mapped switchPort */
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (uport,"fc_uport_num"));
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (rb_id,"mapped_id"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "PortCfgLocalObjectManager::"
                    "queryFcoeLocalManagedObjectFromSwitchPort RbridgeId:Error Finding Fcoe MO entry");
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return(NULL);
            } else if (1 == numberOfResults) {
                FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject =
                dynamic_cast<FcoeIntfLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return(pFcoeIntfLocalManagedObject);
            } else {
                trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::"
                   "queryFcoeLocalManagedObjectFromSwitchPort RbridgeId:" 
                    "There can be only one local node. Results found:");
                for (i = 0; i < numberOfResults; i++) {
                    FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject =
                    dynamic_cast<FcoeIntfLocalManagedObject *> ((*pResults)[i]);
                    trace (TRACE_LEVEL_DEBUG, i + string("ifindex: ") +
                           pFcoeIntfLocalManagedObject->getIfindex() + string(" , mapped-id:") +
                           pFcoeIntfLocalManagedObject->getMappedId());
                }
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return(NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::"
                   "queryFcoeLocalManagedObjectFromSwitchPort RbridgeId: System failure");
            prismAssert (false, __FILE__, __LINE__);
            return(NULL);
        }

        return(NULL);
    }


    PortFCCfgLocalManagedObject *PortCfgLocalObjectManager::QueryFCLocalManagedObjectFromSwitchPortRbridgeId(SI32 switchPort, SI32 rbridgeId,UI32 limitDisplay) {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (PortFCCfgLocalManagedObject::getClassName ());

        /* Get the object corresponding to this mapped switchPort */
        synchronousQueryContext.addAndAttribute (new AttributeSI32 (switchPort,"switchPort"));
        synchronousQueryContext.addAndAttribute (new AttributeSI32 (rbridgeId,"rbridgeId"));
        if(limitDisplay == 1) {
            synchronousQueryContext.addSelectField("id");
            synchronousQueryContext.addSelectField("rbridgeId");
            synchronousQueryContext.addSelectField("switchPort");
        }
        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG,
                   string("PortCfgLocalObjectManager::QueryFCLocalManagedObjectFromSwitchPortRbridgeId")
                       + string(":Error Finding FC Port Cfg entry - SwitchPort") + switchPort);
                pResults->clear ();
                delete pResults;
                return(NULL);
            } else if (1 == numberOfResults) {
                PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                dynamic_cast<PortFCCfgLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return(pPortFCCfgLocalManagedObject);
            } else {
                pResults->clear ();
                delete pResults;
                trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::"
                       "queryFCLocalManagedObjectFromSwitchPort"
                       "RbridgeId: There can be only one local node.");
                prismAssert (false, __FILE__, __LINE__);
                return(NULL);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "PortCfgLocalObjectManager::queryFCLocalManagedObjectFromSwitchPort"
                   "RbridgeId: System failure");
            prismAssert (false, __FILE__, __LINE__);
            return(NULL);
        }
        return(NULL);
    }


    ResourceId PortCfgLocalObjectManager::PortTranslateToSwitchPort(PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext)
    {
        PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage = reinterpret_cast<PortCfgPortCfgPortTranslateMessage *> (pPortCfgPortCfgPortTranslateMessageContext->getPPrismMessage ());

        if (pPortCfgPortCfgPortTranslateMessage->getSwitchPort() == -1) {
            populatePortTranslationFromIdType(pPortCfgPortCfgPortTranslateMessage, pPortCfgPortCfgPortTranslateMessageContext);

            trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::PortTranslateToSwitchPort"
                  " - ID/Type query result content switchport: ") +
                   pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                   + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                   + string(" ID: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                   + string(" Type: ") + pPortCfgPortCfgPortTranslateMessage->getType()
                   + string(" Phy ID: ") + pPortCfgPortCfgPortTranslateMessage->getPhyPortId()
                   + string(" Phy Type: ") + pPortCfgPortCfgPortTranslateMessage->getPhyPortType());

            return(WAVE_MESSAGE_SUCCESS);

        } else if (pPortCfgPortCfgPortTranslateMessage->isDISet()) {

            /*
             * Retrieves the Id from the DB only if Valid SwitchPort & RbridgeId is sent.
             */
            if (pPortCfgPortCfgPortTranslateMessage->getSwitchPort() >= 0 &&
                pPortCfgPortCfgPortTranslateMessage->getRbridge() > 0) {

                populatePortTranslationFromRbridgeSwitchport(pPortCfgPortCfgPortTranslateMessage, pPortCfgPortCfgPortTranslateMessageContext);

                trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::PortTranslateToSwitchPort"
                   " - D,I query result content switchport: ") +
                       pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                       + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                       + string(" ID: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                       + string(" Type: ") + pPortCfgPortCfgPortTranslateMessage->getType()
                       + string(" Phy ID: ") + pPortCfgPortCfgPortTranslateMessage->getPhyPortId()
                       + string(" Phy Type: ") + pPortCfgPortCfgPortTranslateMessage->getPhyPortType());
            }
            return(WAVE_MESSAGE_SUCCESS);
        } else {
            /* Incomplete data for proper translate */
            pPortCfgPortCfgPortTranslateMessage->reset();
        }
        return(WAVE_MESSAGE_SUCCESS);
    }

    bool PortCfgLocalObjectManager::populatePortTranslationFromRbridgeSwitchport(PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage, PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext)
    {
        char phyTuple[20];
        PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject = NULL;

        if (pPortCfgPortCfgPortTranslateMessage == NULL || pPortCfgPortCfgPortTranslateMessageContext == NULL) {
            return(false);
        }

        /*
         * To translate a switch port to an interface, we query the Fcoe ET MO,
         * Fcoe Intf MO and the PortCfg FC MO in order. The first query that
         * succeeds, is the required interface id. The switch port numbers are
         * unique across all these MOs. By the way, the order of querying the 3 MOs
         * is just arbitrary.
         */

        /***********************
         * Querying Fcoe ET MO *
         ***********************/
        FcoeETIntfLocalManagedObject *pFcoeETIntfLocalManagedObject =
        queryFcoeETLocalManagedObjectFromSwitchPortRbridgeId(
                         pPortCfgPortCfgPortTranslateMessage->getSwitchPort(),
                         pPortCfgPortCfgPortTranslateMessage->getRbridge());

        if (pFcoeETIntfLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG, string ("PortCfgLocalObjectManager::"
                   "populatePortTranslationFromRbridgeSwitchport - Found object for switchport: ") +
                   pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                   + string("RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                   + string("id: ") + pPortCfgPortCfgPortTranslateMessage->getId());

            // Tuple and type should be same for the "user" and physical port for an ET port
            pPortCfgPortCfgPortTranslateMessage->setId(pFcoeETIntfLocalManagedObject->getName());
            pPortCfgPortCfgPortTranslateMessage->setType(string(InterfaceTypeToString((interface_display_type_t)pFcoeETIntfLocalManagedObject->getBindType())));
            pPortCfgPortCfgPortTranslateMessage->setPhyPortId(pFcoeETIntfLocalManagedObject->getName());
            pPortCfgPortCfgPortTranslateMessage->setPhyPortType(string(InterfaceTypeToString((interface_display_type_t)pFcoeETIntfLocalManagedObject->getBindType())));

            // GC call to clear the MO
            pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pFcoeETIntfLocalManagedObject);

            return(true);
        }

        /********************************************************
         * No match found in Fcoe ET MO, now query Fcoe Intf MO *
         ********************************************************/
        FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject =
        queryFcoeLocalManagedObjectFromSwitchPortRbridgeId(
                                                          pPortCfgPortCfgPortTranslateMessage->getSwitchPort(),
                                                          pPortCfgPortCfgPortTranslateMessage->getRbridge());

        if (pFcoeIntfLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG,
               string ("PortCfgLocalObjectManager::populatePortTranslationFromRbridgeSwitchport"
               " - Found object for switchport: ") + pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                 + string("RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                 + string("id: ") + pPortCfgPortCfgPortTranslateMessage->getId());

            pPortCfgPortCfgPortTranslateMessage->setId(pFcoeIntfLocalManagedObject->getName());
            pPortCfgPortCfgPortTranslateMessage->setType(string("FCOE"));

            pPortCfgPortCfgPortTranslateMessage->setPhyPortId(pFcoeIntfLocalManagedObject->getBinding());
            pPortCfgPortCfgPortTranslateMessage->setPhyPortType(string(InterfaceTypeToString((interface_display_type_t)pFcoeIntfLocalManagedObject->getBindingType())));

            // GC call to clear the MO
            pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pFcoeIntfLocalManagedObject);
            return(true);
        }

        trace (TRACE_LEVEL_DEBUG,
              string ("PortCfgLocalObjectManager::populatePortTranslationFromRbridgeSwitchport"
                                      " - Missed FCoE Port Type Checks"));

        /********************************************************
         * No match found so far, last try, query PortCfg FC MO *
         ********************************************************/
        pPortFCCfgLocalManagedObject = QueryFCLocalManagedObjectFromSwitchPortRbridgeId(
                                      pPortCfgPortCfgPortTranslateMessage->getSwitchPort(),
                                      pPortCfgPortCfgPortTranslateMessage->getRbridge());

        if (pPortFCCfgLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG,
              string ("PortCfgLocalObjectManager::populatePortTranslationFromRbridgeSwitchport"
              " - Found object for switchport: ") + pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                   + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                   + string(" id: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                   + string(" type: ") + pPortCfgPortCfgPortTranslateMessage->getType());

            pPortCfgPortCfgPortTranslateMessage->setId(pPortFCCfgLocalManagedObject->getId());
            pPortCfgPortCfgPortTranslateMessage->setType(pPortFCCfgLocalManagedObject->getType());

            /* Set Physical Interface Details */
            memset(&phyTuple[0], 0, sizeof (phyTuple));
            snprintf(&phyTuple[0], sizeof (phyTuple), "%d/%d/%d",
                     pPortFCCfgLocalManagedObject->getRbridgeId(),
                     pPortFCCfgLocalManagedObject->getSlot(),
                     pPortFCCfgLocalManagedObject->getCliPort());
            pPortCfgPortCfgPortTranslateMessage->setPhyPortId(phyTuple);
            pPortCfgPortCfgPortTranslateMessage->setPhyPortType(pPortFCCfgLocalManagedObject->getType());

            /* GC call to clear the MO */
            pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
            return(true);
        }

        return(false);
    }

    bool PortCfgLocalObjectManager::populatePortTranslationFromIdType(PortCfgPortCfgPortTranslateMessage *pPortCfgPortCfgPortTranslateMessage, PrismSynchronousLinearSequencerContext *pPortCfgPortCfgPortTranslateMessageContext)
    {
        char phyTuple[20];
        PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject = NULL;
        FcoeETIntfLocalManagedObject *pFcoeETIntfLocalManagedObject = NULL;
        FcoeIntfLocalManagedObject *pFcoeIntfLocalManagedObject = NULL;

        if (pPortCfgPortCfgPortTranslateMessage == NULL || pPortCfgPortCfgPortTranslateMessageContext == NULL) {
            return(false);
        }

        if (strcasecmp(pPortCfgPortCfgPortTranslateMessage->getType().c_str(), "Fi") == 0) {
            /*
             * Query PortCfg FC DB
             */

            pPortFCCfgLocalManagedObject = QueryFCLocalManagedObjectById(pPortCfgPortCfgPortTranslateMessage->getId());

            if (pPortFCCfgLocalManagedObject) {
               trace (TRACE_LEVEL_DEBUG,
                 string ("PortFCCfgLocalObjectManager::QueryFCLocalManagedObjectById"
               " - Found object for switchport: ") + pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                       + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                       + string(" id: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                       + string(" type: ") + pPortCfgPortCfgPortTranslateMessage->getType());

                pPortCfgPortCfgPortTranslateMessage->setRbridge(pPortFCCfgLocalManagedObject->getRbridgeId());
                pPortCfgPortCfgPortTranslateMessage->setSwitchPort(pPortFCCfgLocalManagedObject->getSwitchPort());

                /* Set Physical Interface Details */
                memset(&phyTuple[0], 0, sizeof (phyTuple));
                snprintf(&phyTuple[0], sizeof (phyTuple), "%d/%d/%d",
                         pPortFCCfgLocalManagedObject->getRbridgeId(),
                         pPortFCCfgLocalManagedObject->getSlot(),
                         pPortFCCfgLocalManagedObject->getCliPort());
                pPortCfgPortCfgPortTranslateMessage->setPhyPortId(phyTuple);
                pPortCfgPortCfgPortTranslateMessage->setPhyPortType(pPortFCCfgLocalManagedObject->getType());

                /* GC call to clear the MO */
                pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pPortFCCfgLocalManagedObject);
                return(true);
            }
        } else if ((strcasecmp(pPortCfgPortCfgPortTranslateMessage->getType().c_str(), "Te") == 0) ||
                   (strcasecmp(pPortCfgPortCfgPortTranslateMessage->getType().c_str(), "Fo") == 0)) {
            /*
             * Query Fcoe ET DB (Te: Tengig Ethernet, Fo: Fortygig Ethernet)
             */
            pFcoeETIntfLocalManagedObject  = queryFcoeETLocalManagedObjectFromId(pPortCfgPortCfgPortTranslateMessage->getId());
            if (pFcoeETIntfLocalManagedObject) {
               trace (TRACE_LEVEL_DEBUG,
                string ("PortCfgLocalObjectManager::queryFcoeETLocalManagedObjectFromId"
               " - Found object for switchport: ") + pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                       + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                       + string(" id: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                       + string(" type: ") + pPortCfgPortCfgPortTranslateMessage->getType());

               pPortCfgPortCfgPortTranslateMessage->setRbridge(pFcoeETIntfLocalManagedObject->getRbId());
               pPortCfgPortCfgPortTranslateMessage->setSwitchPort(pFcoeETIntfLocalManagedObject->getFcUportNum());

               pPortCfgPortCfgPortTranslateMessage->setPhyPortId(pFcoeETIntfLocalManagedObject->getName());
                pPortCfgPortCfgPortTranslateMessage->setPhyPortType(string(InterfaceTypeToString((interface_display_type_t)pFcoeETIntfLocalManagedObject->getBindType())));

                /* GC call to clear the MO */
                pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pFcoeETIntfLocalManagedObject);
                return(true);
            }
        } else if (strcasecmp(pPortCfgPortCfgPortTranslateMessage->getType().c_str(), "Fcoe") == 0) {
            /*
             * Query Fcoe Intf DB
             */
            pFcoeIntfLocalManagedObject  = queryFcoeIntfLocalManagedObjectFromId(pPortCfgPortCfgPortTranslateMessage->getId());
            if (pFcoeIntfLocalManagedObject) {
               trace (TRACE_LEVEL_DEBUG,
               string ("PortCfgLocalObjectManager::queryFcoeIntfLocalManagedObjectFromId"
               " - Found object for switchport: ") + pPortCfgPortCfgPortTranslateMessage->getSwitchPort()
                       + string(" RbridgeId: ") + pPortCfgPortCfgPortTranslateMessage->getRbridge()
                       + string(" id: ") + pPortCfgPortCfgPortTranslateMessage->getId()
                       + string(" type: ") + pPortCfgPortCfgPortTranslateMessage->getType());

                pPortCfgPortCfgPortTranslateMessage->setRbridge(pFcoeIntfLocalManagedObject->getMappedId());
                pPortCfgPortCfgPortTranslateMessage->setSwitchPort(pFcoeIntfLocalManagedObject->getFcUportNum());

                pPortCfgPortCfgPortTranslateMessage->setPhyPortId(pFcoeIntfLocalManagedObject->getBinding());
                pPortCfgPortCfgPortTranslateMessage->setPhyPortType(string(InterfaceTypeToString((interface_display_type_t)pFcoeIntfLocalManagedObject->getBindingType())));

                /* GC call to clear the MO */
                pPortCfgPortCfgPortTranslateMessageContext->addManagedObjectForGarbageCollection(pFcoeIntfLocalManagedObject);
                return(true);
            }
        }
        return(false);
    }

    // The following  methods builds the port configuration bitmap using the data fetched from the
    // database while processing the update message.  This method is called after one
    // or more of the parameters have been changed.  Once filled in, the bitmap is
    // delivered to the switch driver via IOCTL to be cached there.
    // 1. sendPortFCCfgSettingsToSwitchDriver()

    void PortCfgLocalObjectManager::sendPortFCCfgSettingsToSwitchDriver(PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject, const UI32 operationCode, reason_t disReason)
    {
        pcfg_bm_t pcfg_bm;
        unsigned int bladeNo, bladePort;
        char    trace_string[512];
        int uport;

        memset((void *)&pcfg_bm, 0, sizeof(pcfg_bm_t));
        if (pPortFCCfgLocalManagedObject->getId().find("/") !=
                  pPortFCCfgLocalManagedObject->getId().rfind("/")) {
            unsigned int mapId;
            sscanf(pPortFCCfgLocalManagedObject->getId().c_str(), "%u/%u/%u",
                                     &mapId, &bladeNo, &bladePort);
        } else {
            sscanf(pPortFCCfgLocalManagedObject->getId().c_str(), "%u/%u", &bladeNo, &bladePort);
        }

        if ((uport = pPortFCCfgLocalManagedObject->getSwitchPort()) < 0) {
            trace (TRACE_LEVEL_ERROR,
                   string ("Error sending FC port configuration data to switch driver no uport\n"));
            return;
        }

        sprintf(trace_string, "sendPortFCCfgSettingsToSwitchDriver %s uports %d\n",
                pPortFCCfgLocalManagedObject->getId().c_str(), uport);
        trace(TRACE_LEVEL_INFO, trace_string);

        // Update the portcfg structure with FC DB

        copy_cfg_from_FC_object(pcfg_bm, pPortFCCfgLocalManagedObject);


        //Set the desire distance with FC DB for desired distance
        if (portSetDistance(uport, pPortFCCfgLocalManagedObject->getDistance()) < 0) {
            trace (TRACE_LEVEL_ERROR,
                string ("Error sending FC port desired distance configuration data to switch driver."));
        }

        // construct and send the IOCTL to the switch driver to update the port configuration bitmap
        if (portSetCfgBm(uport, &pcfg_bm) < 0) {
            trace (TRACE_LEVEL_ERROR,
                    string ("Error sending FC port configuration data to switch driver."));
        }

        // also update the logical port configuration bitmap...
        if (portSetLgclCfgBm(uport, &pcfg_bm) < 0) {
            trace (TRACE_LEVEL_ERROR, string ("Error updating logical port configuration bitmap."));
        }

        // the following code handles follow-up tasks based on the specific needs of each of the
        // possible port configuration actions availble to the user
        if (operationCode == fc_db_type_shutdown) {

            // in addition to updating the persistent state of the port, update the dynamic state
            if (!pPortFCCfgLocalManagedObject->getPortStateShutdown()) {
                portEnable_direct(uport);
                trace (TRACE_LEVEL_DEBUG, string ("sendPortFCCfgSettingsToSwitchDriver: port: ") + uport
                       + string(" - enabled"));
            } else {
                if (disReason) {
                    trace (TRACE_LEVEL_DEBUG,
                           string ("sendPortFCCfgSettingsToSwitchDriver: port ") + uport +
                           string (" disabled with reason ") + disReason);
                    /* persistently disable the port with reason */
                    portDisableWithReason(uport, disReason);
                } else {
                    portDisable_direct(uport);
                    trace (TRACE_LEVEL_DEBUG,
                           string ("sendPortFCCfgSettingsToSwitchDriver: port: ") + uport
                           + string(" - direct disabled"));
                }
            }
        } else if (operationCode == fc_db_type_trunk) {
            trace (TRACE_LEVEL_DEBUG, string ("Trunk activates: port:") + uport);
            // the trunk setting of the port was changed -- blink the port IF the port is enabled
            if (!pcfg_bm._pvt_port_persistently_disabled) {
                portDisable_direct(uport);
                portEnable_direct(uport);
            }
        } else if ((operationCode == fc_db_type_speed) ||
                   (operationCode == fc_db_type_fillWord) ||
                   (operationCode == fc_db_type_longDistance) ||
                   (operationCode == fc_db_type_islRRdyMode) ||
                   (operationCode == fc_db_type_vcLinkInit) ||
                   (operationCode == fc_db_type_desireddistance)
                   || (operationCode == fc_db_type_modelock)
                  ) {
            trace (TRACE_LEVEL_DEBUG,
                   string ("Speed/FillWord/LongDistance/Vclinkinit/isrrdymode/"
                           "desireddistance/configmode-lock Configured - port:") + uport);
            /* blink the port IF the port is enabled */
            if (!pcfg_bm._pvt_port_persistently_disabled) {
                portDisable_direct(uport);
                portEnable_direct(uport);
            }
        }
    }


    // this method registers for the SCN events the port configuration plugin needs to know about
    // as part of the registration, the event type is associated to a callback method
    void PortCfgLocalObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases)
    {
        listenForEvent (VcsFabricLocalObjectManager::getPrismServiceId (),
                        VCS_NODE_DOMAIN_VALID,
                        reinterpret_cast<PrismEventHandler> (&PortCfgLocalObjectManager::SwitchPortDisableEventHandler));

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    // handle the SCN event when a port is disabled in the kernel
    void PortCfgLocalObjectManager::SwitchPortDisableEventHandler (const VcsNodeSwitchPortDisableEvent *&pEvent)
    {
        static UI32 numberOfEventsRceivedSoFar = 0;
        numberOfEventsRceivedSoFar++;

        UI32 portNumber = pEvent->getPortNumber ();
        trace (TRACE_LEVEL_INFO,
               string ("PortCfgLocalObjectManager::SwitchPortDisableEventHandler: Event count = ")+numberOfEventsRceivedSoFar+ " port = "+ portNumber);

        // update port status to disabled in the DCMd database
        // TODO

        // return...
        reply (reinterpret_cast<const PrismEvent *&> (pEvent));
    }


    /*
     * this method overrides the virtual function defined in the WaveObjectManager:: base class.  Is is
     * invoked during bringup and is used to maintain parity between the master data managed by the switch
     * driver and that stored in the cluster-wide database.  Upon invokotion, this method interrogates
     * the switch driver for the configuration of each interface and updates the DCMd database accordingly
     */
    void PortCfgLocalObjectManager::backendSyncUp(PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::backendSyncUpPortCfg),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
//            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::allocatedObjectCleanup),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortCfgLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
        new PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, this, sequencerSteps,
                                                    sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId PortCfgLocalObjectManager::backendSyncUpPortCfg(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_INFO,
        string ("PortCfgLocalObjectManager::backendSyncUpPortCfg -- Not doing anything at the moment "));
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        return(status);
    }

	void PortCfgLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		int local_id;
		int switch_port = 0;
		pcfg_bm_t pcfg_bm;
		char convertToString[40];
		int uport = 0;
		int status = 0;

		string  name = pWaveAsynchronousContextForPostBootPhase->getPassName();
		UI32 slot = pWaveAsynchronousContextForPostBootPhase->getSlotNum();
		trace (TRACE_LEVEL_INFO,
			string("PortCfgLocalObjectManager::postboot Entered with pass:") +
					name + " slot# = " + slot);

		memset((void *)&pcfg_bm, 0, sizeof(pcfg_bm_t));

		// Pushing FC DB Entries To Switch Driver only if VCS is Enabled & FCOE License is found
		if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED)
							&& (licenseCheck(BASE_FCOE_LICENSE) == 1)) {
			if ((local_id = DcmToolKit::getLocalMappedId()) == -1) {
				local_id = 1;
			}       
			for (switch_port = 0; switch_port < PC_FC_MAX_PORT; switch_port++) {
				sprintf(convertToString, "%d/%d/%d", local_id, 0, switch_port);
				string id2(convertToString);
				PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject =
                                            QueryFCLocalManagedObjectById(id2);

				if (pPortFCCfgLocalManagedObject) {
					uport = pPortFCCfgLocalManagedObject->getSwitchPort();
					trace (TRACE_LEVEL_DEBUG,
						string ("PortCfgLocalObjectManager::postboot uport:") + uport +
						string ("distance:") + pPortFCCfgLocalManagedObject->getDistance());
					// Update the portcfg structure with FC DB
					copy_cfg_from_FC_object(pcfg_bm, pPortFCCfgLocalManagedObject);
					//push to switch driver
					if (portSetDistance(uport, pPortFCCfgLocalManagedObject->getDistance()) < 0) {
						trace (TRACE_LEVEL_ERROR,
							string ("PortCfgLocalObjectManager::Error sending FC port"
								" desired distance configuration data to switch driver."));
					}

					/*
					 * construct and send the IOCTL to the switch driver
					 * to update the port configuration bitmap
					 */

					status = portSetCfgBm(uport, &pcfg_bm);
					if (status < 0) {
						trace (TRACE_LEVEL_ERROR,
							string ("Error sending FC port configuration data to switch driver."));
					} else {
						// also update the logical port configuration bitmap...
						status = portSetLgclCfgBm(uport, &pcfg_bm);
						if (status < 0) {
							trace (TRACE_LEVEL_ERROR,
								string ("Error updating logical port configuration bitmap."));
						} else {
							// Toggle the port only if both setcfgbm is success
							if (pPortFCCfgLocalManagedObject->getPortStateShutdown() == 0) {
								trace(TRACE_LEVEL_DEBUG,
									string ("PortCfgLocalObjectManager getPortStateEnable port:") +
									pPortFCCfgLocalManagedObject->getSwitchPort());
								portDisable_direct(uport);
								portEnable_direct(uport);
							}
						}
					}
					delete (pPortFCCfgLocalManagedObject);
				}
			} // end for all ports
		} //end of vcs enabled check for FC Entries
		trace (TRACE_LEVEL_INFO,
			string("PortCfgLocalObjectManager::postboot done:") + name + " slot# = " + slot);
		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();

	}

    static void update_FC_DB_last_clearing_value(PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject)
    {
        /* Retrieve the Local Time and set fill the PortFCCfgLocalManagedObject.
         * The time is in 24Hr Format HH:MM:SS
         */

        struct tm *clear_ts;
        time_t time_val = time(NULL);
        char ts_str[10];
        clear_ts = localtime(&time_val);
        string ts = "";

        if (clear_ts == NULL) {
            ts = "00:00:00";
        } else {
            if (strftime(ts_str, sizeof(ts_str), "%T", clear_ts) == 0) {
                ts = "00:00:00";
            } else {
                /* Valid TS is filled here */
                ts = ts_str;
            }
        }
        pPortFCCfgLocalManagedObject->setClear_counter_ts(ts);
    }

    static void copy_cfg_from_FC_object(pcfg_bm_t &pcfg_bm, PortFCCfgLocalManagedObject *pPortFCCfgLocalManagedObject)
    {
        pcfg_bm._pvt_port_persistently_disabled  =
        (pPortFCCfgLocalManagedObject->getPortStateShutdown() ? 1 : 0);

        pcfg_bm.tport  =
        (pPortFCCfgLocalManagedObject->getTrunkEnable() ? 1 : 0);

        pcfg_bm.speed =
        (pPortFCCfgLocalManagedObject->getSpeed());

        pcfg_bm.fill_word =
        (pPortFCCfgLocalManagedObject->getFillWord());
        pcfg_bm.old_distance =
        (pPortFCCfgLocalManagedObject->getOldDistance());

        pcfg_bm.fcacc =
        (pPortFCCfgLocalManagedObject->getFcAccConfig() ? 1 : 0);

        pcfg_bm.disable_eport =
        (pPortFCCfgLocalManagedObject->getDisableEport() ? 1 : 0);
        pcfg_bm.lport_lockdown =
        (pPortFCCfgLocalManagedObject->getLportLockdown() ? 1 : 0);

        pcfg_bm.priv_lport_lockdown =
        (pPortFCCfgLocalManagedObject->getPrivLportLockdown() ? 1 : 0);
        pcfg_bm.vcxlt_linit =
        (pPortFCCfgLocalManagedObject->getVcLinkInit() ? 1 : 0);

        pcfg_bm.delay_flogi =
        (pPortFCCfgLocalManagedObject->getDelayFlogi());
        pcfg_bm.isl_interop =
        (pPortFCCfgLocalManagedObject->getIslRRdyMode() ? 1 : 0 );

        pcfg_bm.distance =
        (pPortFCCfgLocalManagedObject->getLongDistance());
        pcfg_bm.BufStarvFlag =
        (pPortFCCfgLocalManagedObject->getBufStarvFlag() ? 1 : 0 );

        pcfg_bm.lport_halfduplex =
        (pPortFCCfgLocalManagedObject->getLport_halfduplex() ? 1 : 0 );
        pcfg_bm.lport_fairness =
        (pPortFCCfgLocalManagedObject->getLport_fairness() ? 1 : 0 );

        pcfg_bm.credit_sharing =
        (pPortFCCfgLocalManagedObject->getCredit_sharing() ? 1 : 0 );
        pcfg_bm._pvt_port_permanently_disabled =
        (pPortFCCfgLocalManagedObject->getPvt_port_permanently_disabled() ? 1 : 0 );

        pcfg_bm.npiv_enabled =
        (pPortFCCfgLocalManagedObject->getNpiv_enabled() ? 1 : 0 );
        pcfg_bm.npiv_configured =
        (pPortFCCfgLocalManagedObject->getNpiv_configured() ? 1 : 0 );

        pcfg_bm.rscn_suppress =
        (pPortFCCfgLocalManagedObject->getRscn_suppress() ? 1 : 0 );
        pcfg_bm.ex_port =
        (pPortFCCfgLocalManagedObject->getEx_port() ? 1 : 0 );

        pcfg_bm.mirror_port =
        (pPortFCCfgLocalManagedObject->getMirror_port() ? 1 : 0 );
        pcfg_bm.soft_neg =
        (pPortFCCfgLocalManagedObject->getSoft_neg() ? 1 : 0 );

        pcfg_bm.qos_enabled =
        (pPortFCCfgLocalManagedObject->getQos_enabled() ? 1 : 0 );
        pcfg_bm.qos_configured =
        (pPortFCCfgLocalManagedObject->getQos_configured() ? 1 : 0 );

        pcfg_bm.rlimit =
        (pPortFCCfgLocalManagedObject->getRlimit() ? 1 : 0 );
        pcfg_bm.alpa13 =
        (pPortFCCfgLocalManagedObject->getAlpa13() ? 1 : 0 );

        pcfg_bm.export_mode =
        (pPortFCCfgLocalManagedObject->getExport_mode() ? 1 : 0 );
        pcfg_bm.losig =
        (pPortFCCfgLocalManagedObject->getLosig() ? 1 : 0 );

        pcfg_bm.asn_frc_hwretry =
        (pPortFCCfgLocalManagedObject->getAsn_frc_hwretry() ? 1 : 0 );
        pcfg_bm.cr_recov =
        (pPortFCCfgLocalManagedObject->getCr_recov() ? 1 : 0 );

        pcfg_bm.auto_disable =
        (pPortFCCfgLocalManagedObject->getAuto_disable() ? 1 : 0 );
        pcfg_bm.fport_buffers =
        (pPortFCCfgLocalManagedObject->getFport_buffers() ? 1 : 0 );

        trace (TRACE_LEVEL_DEBUG, string ("copy_cfg_from_FC_object - lock_port: ") +
             pPortFCCfgLocalManagedObject->getLock_port());
        if (pPortFCCfgLocalManagedObject->getLock_port() == FC_CFG_MODE_E_PORT) {
            trace (TRACE_LEVEL_DEBUG, string ("copy_cfg_from_FC_object - CFG_MODE_E_PORT"));
            /* When eportlock enabled, also enable eport capability */
            pcfg_bm.gport_lockdown = 0;
            pcfg_bm.eport_lockdown = 1;
            pcfg_bm.disable_eport = 0;
        } else if ((pPortFCCfgLocalManagedObject->getLock_port() == FC_CFG_MODE_N_PORT)) {
            trace (TRACE_LEVEL_DEBUG, string ("copy_cfg_from_FC_object - CFG_MODE_N_PORT"));
            /* N Port Lock - G-port=1, E-port disabled =1 */
            if (agd_is_accessgateway()) {
                pcfg_bm.gport_lockdown = 1;
                pcfg_bm.eport_lockdown = 0;
                pcfg_bm.disable_eport = 1;
            }
        } else if ((pPortFCCfgLocalManagedObject->getLock_port() == FC_CFG_MODE_F_PORT)) {
            trace (TRACE_LEVEL_DEBUG, string ("copy_cfg_from_FC_object - CFG_MODE_F_PORT"));
            pcfg_bm.gport_lockdown = 0;
            pcfg_bm.eport_lockdown = 0;
            pcfg_bm.disable_eport =  1;
        } else {
            /* G Port lock */
            trace (TRACE_LEVEL_DEBUG, string ("copy_cfg_from_FC_object - CFG_MODE_G_PORT"));
            pcfg_bm.gport_lockdown = 1;
            pcfg_bm.eport_lockdown = 0;
            pcfg_bm.disable_eport = 0;
        }

    }

    /* Related to FCPortInterfaceInfo() */
    static void addExtendedPortText(int portType, int uport, char *buf, int buf_size)
    {
        char tmp[128];
        int i, size;
        typedef struct extendedPortText_ {
            int portType;
            int (*fn)(int port, mmap_port_info_t *pt_info);
            const char *text;
            void (*textfn)(int port, char *buf, int size);
        } extendedPortText;

        extendedPortText portTexts[] = {
            {EPort, isUpstream, " (upstream)", NULL},
            {EPort, isEPortDisabled, " (disabled E-Port)", NULL},
            {EPort, isEPortDisabled2, NULL, getEportDisabledReason},
            {EPort, isDnstream, " (downstream)", NULL},
            {EPort, isSegmented, " (Segmented)", getEportSegmentedReason},
            {EPort, isTmasterport, " (Trunk master)", NULL},
            {EPort, isTrunkPort, " (Trunk port)", NULL},
            {EXPort, isEXPortActive, "", getEXPortFabricID},
            {EXPort, isEXPortDisabled, " (disabled EX-Port)", NULL},
            {EXPort, isSegmented, " (Segmented)", getEportSegmentedReason},
            {EXPort, isTmasterport, " (Trunk master)", NULL},
            {EXPort, isTrunkPort, " (Trunk port)", NULL}
        };
        extendedPortText *ptr = NULL;
        mmap_port_info_t *pt_info = portGetMemMap(uport);

        memset(tmp, 0, sizeof (tmp));
        size = sizeof (portTexts) / sizeof (extendedPortText);

        for (i = 0; i < size; i++) {
            ptr = &portTexts[i];
            if ((portType == ptr->portType) &&
                ptr->fn(uport, pt_info)) {
                if (ptr->text) {
                    strncat(tmp, ptr->text, sizeof (tmp));
                }
                if (ptr->textfn) {
                    char txt[64];

                    memset(txt, 0, sizeof (txt));
                    ptr->textfn(uport, txt, sizeof (txt));
                    strncat(tmp, txt, sizeof (tmp));
                }
            }
        }
        strncat(buf, tmp, buf_size);
    }

    static int isUpstream(int uport, mmap_port_info_t *pt_info)
    {
        return(pt_info != NULL && (pt_info->p_flags & P_UPSTREAM));
    }

    static int isEPortDisabled(int uport, mmap_port_info_t *pt_info)
    {
        /* If the port is not segmented, then return true. Else it */
        /* will be covered by isSegemented call */
        return(portEportDisabled(uport) && !portSegmented(uport));
    }

    static int isEPortDisabled2(int uport, mmap_port_info_t *pt_info)
    {
        return(portEportDisabled(uport));
    }

    static int isDnstream(int uport, mmap_port_info_t *pt_info)
    {
        return(pt_info != NULL && (pt_info->p_flags & P_DNSTREAM));
    }

    static int isEXPortDisabled(int uport, mmap_port_info_t *pt_info)
    {
        return(pt_info != NULL && (pt_info->p_flags & P_DISABLED));
    }

    static int isSegmented(int uport, mmap_port_info_t *pt_info)
    {
        return(portSegmented(uport) > 0);
    }

    static int isTmasterport(int uport, mmap_port_info_t *pt_info)
    {
        return(portTmasterport(uport));
    }

    static int isTrunkPort(int uport, mmap_port_info_t *pt_info)
    {
        return(portTport(uport));
    }

    static int isEXPortActive(int uport, mmap_port_info_t *pt_info)
    {
        return(pt_info != NULL && portActive(uport));
    }

    static void getEportDisabledReason(int uport, char *buf, int size)
    {
        disabledReasonPrt(portGetDisableReason(uport), buf, size);
    }

    static void getEportSegmentedReason(int uport, char *buf, int size)
    {
        segReasonPrt(portGetSegmentReason(uport), buf, size);
    }

    static void getEXPortFabricID(int uport, char *buf, int size)
    {
        int rc = -1;
        isw_ex_cfg exCfg;

        memset(&exCfg, 0, sizeof (exCfg));

        trace (TRACE_LEVEL_DEBUG, string("get_ex_cfg-Start"));
        rc = isw_get_ex_cfg(uport, &exCfg);
        trace (TRACE_LEVEL_DEBUG, string("get_ex_cfg-End"));

        if (rc == 0) {
            snprintf(buf, size, " (fabric id = %d )", exCfg.fid);
        }
    }

    static int disabledReasonPrt(reason_t orig_reason, char *buf, int size)
    {
        switch (orig_reason) {
        case R_AUTH_REJECTED:
            snprintf(buf, size, "(authentication is rejected)");
            break;
        case R_AUTH_TIMEOUT:
            snprintf(buf, size, "(authentication timeout)");
            break;
        case R_AUTH_FAIL_RETRY:
            snprintf(buf, size, "(fail to retry authentication)");
            break;
        default:
            buf[0] = 0;
            break;
        }
        return(0);
    }

    static int segReasonPrt(u_int orig_reason, char *buf, int size)
    {
        u_int reason;

        reason = SEG_GET_SHORT_REASON(orig_reason);

        trace (TRACE_LEVEL_DEBUG, string("The orig reason = ") + orig_reason
               + string(", short reason = ") + reason);

        if (!SEG_REASON_VALID(reason))
            return(-1);

        switch (reason) {
        case SEG_ECP_LONGDIST:
            snprintf(buf, size, "(LD incompat)"); break;
        case SEG_LOOPBACK:
            snprintf(buf, size, "(loopback)"); break;
        case SEG_INCOMPAT:
            if (SEG_GET_DET_REASON(orig_reason) == 0)
                snprintf(buf, size, "(incompatible)");
            else {
                segDetReasonPrt(SEG_GET_DET_REASON(orig_reason),
                                buf, size, reason);
            }
            break;
        case SEG_OVERLAP:
            snprintf(buf, size, "(domain overlap)");
            break;
        case SEG_ZONE:
            snprintf(buf, size, "(zone conflict)");
            break;
        case SEG_AD:
            snprintf(buf, size, "(AD conflict)");
            break;
        case SEG_LICENSE:
            if (SEG_GET_DET_REASON(orig_reason) == 0) {
                snprintf(buf, size, "(no license)");
            } else {
                segDetReasonPrt(SEG_GET_DET_REASON(orig_reason),
                                buf, size, reason);
            }
            break;
        case SEG_EPORT_DISABLED:
            snprintf(buf, size, "(disabled E_Port)");
            break;
        case SEG_PLATFORM_DB:
            snprintf(buf, size, "(Platform Database conflict)");
            break;
        case SEG_SEC_INCOMPAT:
            if (SEG_GET_DET_REASON(orig_reason) == 0) {
                snprintf(buf, size, "(Security Incompatibility)");
            } else {
                segDetReasonPrt(SEG_GET_DET_REASON(orig_reason),
                                buf, size, reason);
            }
            break;
        case SEG_SEC_VIOLATION:
            snprintf(buf, size, "(Security Violation)");
            break;
        case SEG_DUP_WWN:
            snprintf(buf, size, "(Duplicate WWN)");
            break;
        case SEG_MSFR:
            snprintf(buf, size, "(MSFR/RD Conflict)");
            break;
        case SEG_ENH_TIZONE:
            snprintf(buf, size, "(Enhanced TI Zone Conflict)");
            break;
        case SEG_ESC_CONFLICT:
            segDetReasonPrt(SEG_GET_DET_REASON(orig_reason),
                            buf, size, reason);
            break;
        case SEG_FDS_CONFLICT:
            snprintf(buf, size, "(FDS Zone Conflict)");
            break;
        default:
            break;
        }
        return(1);
    }

    //should be synch with fab_seg.c.h
    static const char *segDetReason_msg[] = {
        "(Unknown incompat)",
        "(FCSW version incompat)",
        "(Flow ctl length incompat)",
        "(Incorrect struct size)",
        "(BB credit incompat)",
        "(Recv data field size incompat)",
        "(RA TOV incompat)",
        "(ED TOV incompat)",
        "(Op Mode incompat)",
        "(Link Control incompat)",
        "(VC Class2 incompat)",
        "(VC Class3 incompat)",
        "(VC multicast incompat)",
        "(VC config incompat)",
        "(VC PID map incompat)",
        "(Class1 data size incompat)",
        "(Class1 options incompat)",
        "(Class2 data size incompat)",
        "(Class2 options incompat)",
        "(Class3 data size incompat)",
        "(Class3 options incompat)",
        "(ClassF options incompat)",
        "(ClassF init ctl incompat)",
        "(ClassF recepient ctl incompat)",
        "(ClassF data size incompat)",
        "(ClassF concurrent seqs incompat)",
        "(ClassF EE credit incompat)",
        "(ClassF open sequences incompat)",
        "(ClassF reserved incompat)",
        "(Flow ctrl mode incompat)",
        "(VC Link Init incompat)",
        "(Non-zero reserved field)",
        "(Receiver mode incompat)",
        "(VC count incompat)",
        "(VC priority incompat)",
        "(Tachyon compatability mode incompat)",
        "(E-Port isolated incompat)",
        "(SCSI FCP probe incompat)",
        "(No broadcast or multicast incompat)",
        "(VC encoding in DID incompat)",
        "(CS_CTL in FC_header incompat)",
        "(No class F frames or long distance mode incompat)",
        "(Long distance mode incompat)",
        "(No class F frames, Long distance fabric or LD mode incompat)",
        "(Long distance fabric or mode incompat)",
        "(No pahntom incompat)",
        "(Core PID format incompat)",
        "(Displaced PID incompat)",
        "(FCIP Tunnel Configuration incompat)",
        "(NOS incompatible)",
        "(Unknown reason)"
    };


    static const char *segEscDetReason_msg[] = {
        "(ESC mismatch, Unknown)",
        "(ESC mismatch, Enhanced TI Zone)",
        "(ESC mismatch, Domain id Offset)",
        "(ESC mismatch, Fabric ID)",
        "(ESC mismatch, Config Mode)",
        "(ESC mismatch, Distributed Config DB)",
        "(ESC mismatch, VCS Fabric License)",
        "(ESC mismatch, Fabric Distribution Service)"
    };

    static const char *segLicDetReason_msg[] = {
        "(no license)",
        "(No Fabric License)",
        "(No Zoning License)",
        "(No ICL License)"
    };

    static const char *segSecDetReason_msg[] = {
        "(Security Incompatibility - Unknown incompat)",
        "(Security Incompatibility - Security parameters incompat)",
        "(Security Incompatibility - Exchange FCS failed)",
        "(Security Incompatibility - Data incompat)",
        "(Security Incompatibility - MS Platform config incompat)"
        "(Security Incompatibility - FDD policy conflict)"
    };

#define SEG_DET_MAX (sizeof (segDetReason_msg) / sizeof (char *))
#define SEG_ESC_DET_MAX (sizeof (segEscDetReason_msg) / sizeof (char *))
#define SEG_LIC_MAX (sizeof (segLicDetReason_msg) /sizeof (char *))
#define SEG_SEC_MAX (sizeof (segSecDetReason_msg) / sizeof (char *))

    static int segDetReasonPrt(u_int det_reason, char *buf, int size, u_int short_reason)
    {
        trace(TRACE_LEVEL_DEBUG, string("The det reason = ") + det_reason);

        if (short_reason == SEG_INCOMPAT) {
            if (det_reason < SEG_DET_MAX) {
                snprintf(buf, size, segDetReason_msg[det_reason]);
            } else { /* print unknown incompat reason which is index 0 */
                snprintf(buf, size, segDetReason_msg[0]);
            }
        } else if (short_reason == SEG_SEC_INCOMPAT) {
            if (det_reason < SEG_SEC_MAX) {
                snprintf(buf, size, segSecDetReason_msg[det_reason]);
            } else { /* print unknown incompat reason which is index 0 */
                snprintf(buf, size, segSecDetReason_msg[0]);
            }
        } else if (short_reason == SEG_LICENSE) {
            if (det_reason < SEG_LIC_MAX) {
                snprintf(buf, size, segLicDetReason_msg[det_reason]);
            } else { /* print generic no license string */
                snprintf(buf, size, segLicDetReason_msg[0]);
            }
        } else if (short_reason == SEG_ESC_CONFLICT) {
            if (det_reason < SEG_ESC_DET_MAX) {
                snprintf(buf, size, segEscDetReason_msg[det_reason]);
            } else { /* print unknown reason which is index 0 */
                snprintf(buf, size, segEscDetReason_msg[0]);
            }
        }
        return(1);
    }

    static void trunk_master_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {
        /* Trunk Master Port */
        if (portTport(switchPort) || portTFport(switchPort)) {
            if (portTmasterport(switchPort) || portTFmasterport(switchPort)) {
                pPortCfgFCPortInterfaceInfoMessage->setTrunkMaster(1);
            } else {
                pPortCfgFCPortInterfaceInfoMessage->setTrunkMaster(0);
            }
        } else {
            pPortCfgFCPortInterfaceInfoMessage->setTrunkMaster(0);
        }

        /* Trunk Port */
        if (portTport(switchPort) || portTFport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setTrunked(1);
        } else {
            pPortCfgFCPortInterfaceInfoMessage->setTrunked(0);
        }
    }

    static void port_state_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {
        if (portOnline(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setState("Online");
        } else if (portOffline(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setState("Offline");
        } else if (portTesting(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setState("Testing");
        } else if (portFaulty(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setState("Faulty");
        } else {
            pPortCfgFCPortInterfaceInfoMessage->setState("Unknown-State");
        }
    }

    static int port_type_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {
        int portType = UnknownPort;
        if (portPhyEport(switchPort) || portEport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("E-Port");
            portType = EPort;
        } else if (portUport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("U-Port");
            portType = UPort;
        } else if (portNportEnabled(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("N-Port");
            portType = AG_NPort;
        } else if (portPhysFport(switchPort) || portFport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("F-Port");
            portType = FPort;
        } else if (portGport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("G-Port");
            portType = GPort;
        } else if (portLport(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("L-Port");
            portType = LPort;
        } else if (portSegmented(switchPort)) {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("Port Segmented");
        } else {
            pPortCfgFCPortInterfaceInfoMessage->setPortType("Unknown-Port");
        }
        trace(TRACE_LEVEL_DEBUG, string("PortType: ") + pPortCfgFCPortInterfaceInfoMessage->getPortType());
        return(portType);
    }

    static void wwn_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {

        wwn_t wwn;
        char pwwn[40];
        string portwwn;

        getPortName(switchPort, &wwn);
        if (wwnfmt(&wwn) == NULL) {
            pPortCfgFCPortInterfaceInfoMessage->setPortWWN("00:00:00:00:00:00:00:00");
        } else {
            strcpy(pwwn, wwnfmt(&wwn));
            portwwn = pwwn;
            pPortCfgFCPortInterfaceInfoMessage->setPortWWN(portwwn);
        }
        trace(TRACE_LEVEL_DEBUG, string("PortWWN: ") + portwwn);

        /* Remote WWN */
        wwn_t nbrPortWwn, nbrNodeWwn;
        getNpPortName(switchPort, &nbrPortWwn);
        getNpNodeName(switchPort, &nbrNodeWwn);

        if (wwnfmt(&nbrNodeWwn) == NULL || wwnzero(&nbrNodeWwn)) {
            pPortCfgFCPortInterfaceInfoMessage->setRemotePortWWN("");
        } else {
            strcpy(pwwn, wwnfmt(&nbrNodeWwn));
            portwwn = pwwn;
            pPortCfgFCPortInterfaceInfoMessage->setRemoteNodeWWN(portwwn);
        }
        trace(TRACE_LEVEL_DEBUG, string("RemoteNodeWWN: ") + portwwn);

        if (wwnfmt(&nbrPortWwn) == NULL || wwnzero(&nbrPortWwn)) {
            pPortCfgFCPortInterfaceInfoMessage->setRemotePortWWN("");
        } else {
            strcpy(pwwn, wwnfmt(&nbrPortWwn));
            portwwn = pwwn;
            pPortCfgFCPortInterfaceInfoMessage->setRemotePortWWN(portwwn);
        }
        trace(TRACE_LEVEL_DEBUG, string("RemotePortWWN: ") + portwwn);
    }

    static void port_status_fill_info(int switchPort, int portType, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {

        string portStatusMsg;
        string port_status;

        char buffer[256];
        bool mod_Inv = 0;
        const char *pchTemp;

        memset(buffer, 0, sizeof (buffer));

        /* Port Status */
        port_status = portDisabled(switchPort) ? "Disabled" :
                      (portActive(switchPort) ? portStateName(switchPort) : portPhysName(switchPort));
        trace(TRACE_LEVEL_DEBUG, string("PortStatus: ") + port_status);
        pPortCfgFCPortInterfaceInfoMessage->setStatus(port_status);

        /* StatusMessage */
        pchTemp = (portDisabled(switchPort) ? (char *)"Disabled" :
                   (portActive(switchPort) ? portStateName(switchPort) :  portPhysName(switchPort)));

        if (strcasecmp("Mod_Inv", pchTemp) == 0) {
            mod_Inv = 1;
        } else {
            mod_Inv = 0;
        }

        if (portDisabled(switchPort)) {
            portGetDisableReasonStr(switchPort, buffer);
            if (strcmp(buffer, "None") == 0) {
                memset(buffer, 0, sizeof (buffer));
            }
        }

        if (mod_Inv) {
            strcpy(buffer, "Speed Mismatch / Incompatible Vendor");
        } else {
            addExtendedPortText(portType, switchPort, buffer, sizeof (buffer));
        }

        portStatusMsg = buffer;
        pPortCfgFCPortInterfaceInfoMessage->setStatusMessage(portStatusMsg);

    }

    static void port_health_fill_info(int switchPort, PortCfgFCPortInterfaceInfoMessage *pPortCfgFCPortInterfaceInfoMessage)
    {

        /* Port Health */
        string portHealth;

        portStateArray_t arr;   /* struct used to obtain information */
        int rc, i;              /* return code and incrementers */
        fwPortHealth_e health;  /* health of port */

        rc = fwPortStatesGet(&arr);

        /* Fabric Watch must be licensed in order for health to be displayed */
        if (rc == thUnlicensed) {
            portHealth = "No Fabric Watch License";
        } else if (rc < 0) {
            /*
             * if fwPOrtStatesGet returns any other type of error,
             * return that back up
             */
            portHealth = "";
        } else {
            for (i = 0; i < PC_FC_MAX_PORT; i++) {
                if (portTFport(i)) {
                    /*
                     * skip logical f_port trunk
                     */
                    continue;
                }
                if (arr.healthArray[i].area == switchPort) {
                    health = arr.healthArray[i].healthState;
                    switch (health) {
                    case FW_PORT_HEALTHY:
                        portHealth = "HEALTHY";
                        break;
                    case FW_PORT_MARGINAL:
                        portHealth = "MARGINAL";
                        break;
                    case FW_PORT_FAULTY:
                        portHealth = "FAULTY";
                        break;
                    case FW_PORT_OFFLINE:
                        portHealth = "OFFLINE";
                        break;
                    case FW_PORT_ERROR:
                        portHealth = "ERROR";
                        break;
                    default:
                        portHealth = "UNKNOWN";
                        break;
                    }
                } else {
                    portHealth = "";
                }
            } // For
        }
        pPortCfgFCPortInterfaceInfoMessage->setHealth(portHealth);
    }

    static const char *stats_entry_fields[] = {
        "stat_wtx",
        "stat_wrx",
        "stat_ftx",
        "stat_frx",
        "stat_c2_frx",
        "stat_c3_frx",
        "stat_lc_rx",
        "stat_mc_rx",
        "stat_mc_to",
        "stat_mc_tx",
        "tim_rdy_pri",
        "tim_txcrd_z",
        "tim_txcrd_z_vc  0- 3:",
        "tim_txcrd_z_vc  4- 7:",
        "tim_txcrd_z_vc  8-11:",
        "tim_txcrd_z_vc 12-15:",
        "er_enc_in",
        "er_crc",
        "er_trunc",
        "er_toolong",
        "er_bad_eof",
        "er_enc_out",
        "er_bad_os",
        "er_rx_c3_timeout",
        "er_tx_c3_timeout",
        "er_c3_dest_unreach",
        "er_other_discard",
        "er_type1_miss",
        "er_type2_miss",
        "er_type6_miss",
        "er_zone_miss",
        "er_lun_zone_miss",
        "er_crc_good_eof",
        "er_inv_arb",
        "open",
        "transfer",
        "opened",
        "starve_stop",
        "fl_tenancy",
        "nl_tenancy",
        "zero_tenancy"
    };

    /*
     * retrieve_port_proc():
     *            Retrieve port proc information based on the info (stats/error) specified
     * Returns:
     *      0 - On Success
     *      -1 - Invalid Uport
     *      -2 - No Valid Permissions to read File
     *      -3 - File Opening Failed
     *      -4 - Error Reading File
     */
    static int retrieve_port_proc(int swno, int uport, PortCfgFCPortDetailedInfoMessage *pFC_DetailMsg)
    {
        char    stats_fname[MAXNAMELEN];
        char    full_line[MAXROWCHAR];
        FILE    *portflags_fp;

        /* Port Stat */
        bool tx_statflag = true;
        bool rx_statflag = true;
        bool er_statflag = true;
        vector<string> statsTx;
        vector<string> statsRx;
        vector<string> errInfo;

        /* Common */
        char              field[20];
        unsigned long int value;
        char              comment[40];
        int               i = 0;

        if (uport < 0) {
            return -1;
        }

        /* Proc Stats */
        sprintf(stats_fname, "/proc/fabos/switch/%d/%d/%s", swno, uport, "stats");
        trace(TRACE_LEVEL_DEBUG, string("stats_fname: ") + stats_fname);

        /* Retrieving Stats Proc Entry */
        if (!access(stats_fname, R_OK)) {
            portflags_fp = fopen(stats_fname, "r");
            if (!portflags_fp) {
                /* File Opening Failed */
                trace(TRACE_LEVEL_ERROR, string("stat proc entry: ") + stats_fname + string (" - Open FAILED"));
                return -3;
            }
            while (!feof(portflags_fp)) {

                if (fgets(full_line, MAXROWCHAR, portflags_fp) == NULL) {
                    continue;
                }
                /* Retrieving Stats Proc Entry */
                sscanf(full_line, "%s %lu %s", field, &value, comment);
                if (strstr(full_line, stats_entry_fields[i]) != NULL) {
                    if ((strncmp(stats_entry_fields[i], field, 5) == 0)) {
                        if ((strncmp(field, "stat_", 5) == 0 ) || (strncmp(field, "tim_", 4) == 0)) {
                            if (strstr(field, "tx")) {
                                /* Valid Tx Fields */
                                if (tx_statflag) {
                                    statsTx.push_back("Tx Statistics:\n");
                                    tx_statflag = false;
                                }
                                statsTx.push_back("\t");
                                statsTx.push_back(full_line);
                                trace(TRACE_LEVEL_DEBUG, string("Tx: ") + full_line);
                            } else if (strstr(field, "rx")) {
                                /* Valid Rx Fields */
                                if (rx_statflag) {
                                    statsRx.push_back("Rx Statistics:\n");
                                    rx_statflag = false;
                                }
                                statsRx.push_back("\t");
                                statsRx.push_back(full_line);
                                trace(TRACE_LEVEL_DEBUG, string("Rx: ") + full_line);
                            }
                        } else if (strncmp(field, "er_", 3) == 0) {
                            if (er_statflag) {
                                errInfo.push_back("Error Statistics:\n");
                                er_statflag = false;
                            }
                            errInfo.push_back("\t");
                            errInfo.push_back(full_line);
                            trace(TRACE_LEVEL_DEBUG, string("Err: ") + full_line);
                        }
                    }
                }
                i++;
            } //While

            /* Filling the Message with vectors */
            pFC_DetailMsg->setStatsRx(statsRx);
            pFC_DetailMsg->setStatsTx(statsTx);
            pFC_DetailMsg->setStatsErr(errInfo);

            fclose(portflags_fp);
        } else {
            trace(TRACE_LEVEL_ERROR, string("Error Accessing Stat Proc File"));
            return(-2);
        }
        return 0;
    }

    static void port_state_info(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg)
    {
        string protocol = "fibrechannel ";
        protocol += pFCPortDetail_Msg->getPortId().c_str();
        string port_status = portPhysName(switchPort);

        /*
         * Code to Trim trailing Spaces only
         * Find the first character position from reverse
         */
        size_t endpos = port_status.find_last_not_of(" ");
        if ( string::npos != endpos ) {
            port_status = port_status.substr( 0, endpos+1 );
        }

        if (portOnline(switchPort) && portActive(switchPort)) {
            protocol += " is up (";
            protocol += port_status;
            protocol += ").  Protocol state is up (connected).\n";
        } else if (!portOnline(switchPort)) {
            protocol += " is up (";
            protocol += port_status;
            protocol += ").  Protocol state is down.\n";
        }
        pFCPortDetail_Msg->setPortState(protocol);
    }

    static int port_getalloc_buffers(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg)
    {
        const char *lx_mode;
        int max_buffers, buffer_usage;
        int needed_buffers, remaining_buffers;
        char mb_string[8];
        int show_lx_info = FALSE;
        string bufferinfo = " ";
        int actual_distance = 0, estimated_distance;
        string linkdist;
        buf_usage_t buf_info;
        pcfg_bm_t *pcfg ;

        if (portPhyEport(switchPort)) {
            show_lx_info = TRUE;
        }
        if (portBufferAlloc(switchPort, &buf_info)) {
            trace (TRACE_LEVEL_INFO, string("Cannot get portBufferAlloc for port ") + switchPort);
            return(-1);
        } else {
            buffer_usage = buf_info.alloc_bufs;
            remaining_buffers = buf_info.unused_bufs;
            needed_buffers = buf_info.desired_bufs;
            max_buffers = buf_info.user_est_bufs;

            if (portLxDistance(switchPort, &actual_distance, &estimated_distance)) {
                trace (TRACE_LEVEL_INFO, string("Cannot get portLxDistance for port ") + switchPort);
                return(-2);
            }
            if (max_buffers <= 0) {
                pFCPortDetail_Msg->setResvbuf(0);

            } else {
                pFCPortDetail_Msg->setResvbuf(max_buffers);
            }

            pcfg = (pcfg_bm_t*)calloc(1, sizeof (pcfg_bm_t));
            if (!pcfg || portGetCfgBm(switchPort, pcfg) < 0) {
                trace (TRACE_LEVEL_INFO,
                   string("Error: Unable to get port configuration for port:") + switchPort);
                if (pcfg)
                    free(pcfg);
                return(-3);
            }

            if (portCfgIsNormalDistance(*pcfg)) {
                bufferinfo += "-";
                pFCPortDetail_Msg->setLxmode(bufferinfo);
            } else {
                lx_mode = portDistance2chCode(PORTCFG_DISTANCE(*pcfg));
                bufferinfo += lx_mode;
                pFCPortDetail_Msg->setLxmode(bufferinfo);
            }
            free(pcfg);
            pFCPortDetail_Msg->setBufusage(buffer_usage);
            if (show_lx_info) {
                pFCPortDetail_Msg->setNeededbuf(needed_buffers);
                if (actual_distance < 0) {
                    linkdist = "N/A";
                    pFCPortDetail_Msg->setLinkdist(linkdist);
                } else if (actual_distance < 2) {
                    linkdist = "<2km";
                    pFCPortDetail_Msg->setLinkdist(linkdist);
                } else {
                    sprintf(mb_string, "%3dkm", actual_distance);
                    linkdist = mb_string;
                    pFCPortDetail_Msg->setLinkdist(linkdist);
                }
            } else {
                pFCPortDetail_Msg->setNeededbuf(0);
                linkdist = "-";
                pFCPortDetail_Msg->setLinkdist(linkdist);
            }
            pFCPortDetail_Msg->setRembuf(remaining_buffers);

            trace (TRACE_LEVEL_DEBUG, string("Port: ") + switchPort + string("bufUsg: ") + buffer_usage + string("remBuf: ")
                   + remaining_buffers + string("NeedBuf: ") + needed_buffers + string("MaxBuf: ") + max_buffers +
                   string("Linebufferinfo: ") + bufferinfo + string("linkdist: ") + linkdist);

        }

        return(0);
    }

    static void port_performance_detail(int switchPort, PortCfgFCPortDetailedInfoMessage *pFCPortDetail_Msg) {
        u_int words_txtemp = 0;
        u_int words_rxtemp = 0;
        int bandwidth = 0;
        string txperf = "";
        string rxperf = "";
        char buf[15];

        //Bandwidth
        bandwidth = (portGetLinkSpeed(switchPort))/1000;
        pFCPortDetail_Msg->setBandwidth(bandwidth);
        //Tx performance
        portTxPerf(switchPort,&words_txtemp);
        portPrint(words_txtemp,0, buf);
        txperf += buf;
        pFCPortDetail_Msg->setTxperf(txperf);
        //Rx performance
        portRxPerf(switchPort,&words_rxtemp);
        portPrint(words_rxtemp,0, buf);
        rxperf += buf;
        pFCPortDetail_Msg->setRxperf(rxperf);
    }

    static void portPrint(u_int  n, u_int base, char *buf) {
        int i;
        static char c[] = { ' ', 'K', 'M', 'G'};
        for (i = base; i < 4; i++, n /= 1000) {
            if (n < 1000) {
                sprintf(buf, "%3u%c",  n, c[i]);
                break;
            }
            if ((n < 1000000) && (i < 3)) {
                sprintf(buf, "%3d.%1d%c", n/1000, n/100%10, c[i+1]);
                break;
            }
            if ((n < 1000000000) && (i < 2)) {
                sprintf(buf, "%3d.%1d%c", n/1000000, n/100000%10, c[i+2]);
                break;
            }
        }
    }

    /* Check for Sufficient Buffers for Long Distance Ports */
    static int check_sufficient_buf (int switchport, int longdistmode, int estd_dist) {
        port_cfg_arg_t port_arg;
        portCfgRet_t cfgRet;

        port_arg.cmd = PORTCFG_CMD_LONGDISTANCE;
        port_arg.level = longdistmode;
        port_arg.distance = estd_dist;
        memset(port_arg.port_bm, 0, MAX_PORT_BM * sizeof (int));
        if (INVALID_CONFIG == portValidateConfig(switchport, &port_arg, &cfgRet.type)) {

            return(-1);
        }
        return(0);
    }
    /* These fields order needs to match the proc entry of pinfo */
    static const char *portshow_entry_fields[MAX_PORTSHOW_ARR] = {
        "portDisableReason:",
        "portCFlags:",
        "portFlags:",
        "portType:",
        "LocalSwcFlags:",
        "POD Port:",
        "portState:",
        "Protocol:",
        "portPhys:",
        "port generation number:",
        "state transition count:",
        "portId:",
        "portIfId:",
        "portWwn:",
        "portWwn of device(s) connected:",
        "Distance:",
        "portSpeed:",
        "LE domain:",
        "FC Fastwrite:",
        "Interrupts:",
        "Unknown:",
        "Lli:",
        "Proc_rqrd:",
        "Timed_out:",
        "Rx_flushed:",
        "Tx_unavail:",
        "Free_buffer:",
        "Overrun:",
        "Suspended:",
        "Parity_err:",
        "2_parity_err:",
        "CMI_bus_err:"
    };

    static int retrieve_portinfo_proc(int swno, int uport, PortCfgFCPortDetailedInfoMessage *pFC_DetailMsg)
    {
        FILE *fp ;
        char full_line[MAXNAMELEN];
        char pinfo_fname[MAXNAMELEN];
        int i = 0;
        int index = 0;
        char field[5][20];

        vector<string> configparam;
        vector<string> detailinfo;

        string speed;
        string lossofsync;
        string lossofsig;
        string frjt;
        string fbsy;
        string Disreason;
        string Distline;

        if (uport < 0) {
            return -1;
        }

        /* Proc Pinfo */
        sprintf(pinfo_fname, "/proc/fabos/switch/%d/%d/%s", swno, uport, "info");
        trace(TRACE_LEVEL_DEBUG, string("pinfo_fname: ") + pinfo_fname);

        if (!access(pinfo_fname, R_OK)) {
            fp = fopen (pinfo_fname, "r");
            if (!fp) {
                trace(TRACE_LEVEL_DEBUG, string("Failed Reading info\n"));
                return(-1);
            }

            while (!feof(fp) ) {
                if (fgets(full_line, MAXNAMELEN, fp) == NULL) {
                    continue;
                }
                for (i = 0; i < MAX_PORTSHOW_ARR; i++) {
                    if (strstr(full_line,  portshow_entry_fields[i]) != NULL) {
                        index = i;
                        break;
                    }
                }
                if (i == MAX_PORTSHOW_ARR) {
                    trace(TRACE_LEVEL_DEBUG, string("Error: Reached EOF Array - No Such Element"));
                    continue;
                }
                /* Index fields match the corresponding static array of portshow_entry_fields */
                switch (index) {
                case 0: /* portDisableReason */
                    Disreason = full_line;
                    Disreason.replace (18,1,"              ");

                    /* Push the field to the portcfg message */
                    configparam.push_back(Disreason);
                    trace(TRACE_LEVEL_DEBUG, string("configparam: ") + Disreason);
                    break;
                case 11: /* portId */
                    sscanf(full_line, "%s %s", field[0], field[1]);
                    sscanf(full_line, "%s %s", field[0], field[1]);
                    sprintf(full_line, "PortId:                         %s\n", field[1]);

                    /* Push the field to the portcfg message */
                    configparam.push_back(full_line);
                    trace(TRACE_LEVEL_DEBUG, string("configparam: ") + full_line);
                    break;
                case 12: /* portIfId */
                    sscanf(full_line, "%s %s", field[0], field[1]);
                    sprintf(full_line, "PortIfId:                       %s\n", field[1]);

                    /* Push the field to the portcfg message */
                    configparam.push_back(full_line);
                    trace(TRACE_LEVEL_DEBUG, string("configparam: ") + full_line);
                    break;
                case 13: /* PortWwn */
                    sscanf(full_line, "%s %s", field[0], field[1]);
                    sprintf(full_line, "PortWwn:                        %s\n", field[1]);

                    /* Push the field to the portcfg message */
                    configparam.push_back(full_line);
                    trace(TRACE_LEVEL_DEBUG, string("configparam: ") + full_line);
                    break;
                case 15: /* Distance */
                    Distline = full_line;
                    Distline.replace (9,2,"                       ");

                    /* Push the field to the portcfg message */
                    configparam.push_back(Distline);
                    trace(TRACE_LEVEL_DEBUG, string("configparam: ") + Distline);
                    break;
                case 16: /* portSpeed */
                    sscanf(full_line, "%s %s", field[0], field[1]);
                    speed = "PortSpeed:                      ";
                    speed += field[1];
                    speed +="\n";
                    trace(TRACE_LEVEL_DEBUG, string("speed: ") + speed);
                    break;
                case 19: /* Interrupts */
                    detailinfo.push_back(full_line);
                    /* Extracting Error Fileds - To be pushed into the respective fields */
                    sscanf(full_line, "%s %s %s %s %s %s",
                           field[0], field[1], field[2], field[3], field[4], field[5]);

                    frjt = "frjt:                   ";
                    frjt += field[5];
                    trace(TRACE_LEVEL_DEBUG, string("frjt: ") + frjt);
                    break;
                case 20: /* Unknown */
                    detailinfo.push_back(full_line);
                    sscanf(full_line, "%s %s %s %s %s %s",
                           field[0], field[1], field[2], field[3], field[4], field[5]);

                    lossofsync = "loss_of_sync:           ";
                    lossofsync += field[3];
                    fbsy = "fbsy:                   ";
                    fbsy += field[5];

                    trace(TRACE_LEVEL_DEBUG, string("fbsy: ") + fbsy);
                    trace(TRACE_LEVEL_DEBUG, string("lossofsync: ") + lossofsync);
                    break;
                case 21: /* Lli */
                    detailinfo.push_back(full_line);
                    sscanf(full_line, "%s %s %s %s %s %s",
                           field[0], field[1], field[2], field[3], field[4], field[5]);

                    lossofsig = "lossofsig:              ";
                    lossofsig += field[3];

                    trace(TRACE_LEVEL_DEBUG, string("lossofsig: ") + lossofsig);
                    break;
                case 22: /* Proc_rqrd */
                case 23: /* Timed_out */
                case 24: /* Rx_flushed */
                case 25: /* Tx_unavail */
                case 26: /* Free_buffer */
                case 27: /* Overrun */
                case 28: /* Suspended */
                case 29: /* Parity_err */
                case 30: /* 2_parity_err */
                case 31: /* CMI_bus_err */
                    /* Push these fields to the portinfo vector */
                    detailinfo.push_back(full_line);
                    break;
                default:
                    /*** Ignore the other fields ***/
                    break;
                } // Switch Case
            } //While

            /* Fill the Message with Extracted Fields */
            pFC_DetailMsg->setLossOfSig(lossofsig);
            pFC_DetailMsg->setLossOfSync(lossofsync);
            pFC_DetailMsg->setFrjt(frjt);
            pFC_DetailMsg->setFbsy(fbsy);
            pFC_DetailMsg->setPortInfo(detailinfo);
            pFC_DetailMsg->setPortCfg(configparam);
            pFC_DetailMsg->setPortLineSpeedCfg(speed);

            fclose(fp);
        } else {
            trace(TRACE_LEVEL_INFO, string("Error Accessing Proc File"));
            return(-2);
        }
        return 0;
    }

    /*
     * sfp_show_fill_info()
     * Used to Retrieve SFP Values.
     */

    static int sfp_show_fill_info(PortCfgFibreChannelSfpShowMessage *pFC_SfpShowMsg)
    {
        gbic_t  gbic;
        SI32 switchPort;
        vector <string> identifier;
        vector <string> connector;
        vector <string> transceiver;
        vector <string> encoding;
        vector <string> baud_rate;
        vector <string> len9m_km;
        vector <string> len9um;
        vector <string> len50um;
        vector <string> len625um;
        vector <string> lencu;
        vector <string> vendor;
        vector <string> oui;
        vector <string> partno;
        vector <string> rev;
        vector <string> wavelength;
        vector <string> options;
        vector <string> br_max;
        vector <string> br_min;
        vector <string> serialno;
        vector <string> date_code;
        vector <string> ddType;
        vector <string> enhOption;
        vector <string> status_ctrl;
        vector <string> alarm_flags;
        vector <string> warn_flags;
        vector <string> temperature;
        vector <string> current;
        vector <string> voltage;
        vector <string> rx_power;
        vector <string> tx_power;
        vector <string> err_info;
        char temp[100];

        for (UI32 i = 0; i < pFC_SfpShowMsg->getPortId().size(); i++) {
            switchPort = (pFC_SfpShowMsg->getSwitchPort().at(i));

            if (switchPort >= 0) {
                if ((gbicSerialId(switchPort, &gbic) == NULL) || portDisabled(switchPort)) {
                    sprintf(temp, "Error: Interface fibrechannel %s - Pluggable media not present\n",pFC_SfpShowMsg->getPortId()[i].c_str());

                    /* Assuming port disable has high priority if both conditions hit */
                    if (portDisabled(switchPort)) {
                        sprintf(temp, "Media data is not available due to Interface %s offline.\n", pFC_SfpShowMsg->getPortId()[i].c_str()     );
                    }
                    err_info.push_back(temp);

                    /* Pushing Blank Fields */
                    strcpy(temp, "");
                    identifier.push_back(temp);
                    connector.push_back(temp);
                    transceiver.push_back(temp);
                    encoding.push_back(temp);
                    baud_rate.push_back(temp);
                    len9m_km.push_back(temp);
                    len9um.push_back(temp);
                    len50um.push_back(temp);
                    len625um.push_back(temp);
                    lencu.push_back(temp);
                    vendor.push_back(temp);
                    oui.push_back(temp);
                    partno.push_back(temp);
                    rev.push_back(temp);
                    wavelength.push_back(temp);
                    options.push_back(temp);
                    br_max.push_back(temp);
                    br_min.push_back(temp);
                    serialno.push_back(temp);
                    date_code.push_back(temp);
                    ddType.push_back(temp);
                    enhOption.push_back(temp);
                    status_ctrl.push_back(temp);
                    alarm_flags.push_back(temp);
                    warn_flags.push_back(temp);
                    temperature.push_back(temp);
                    current.push_back(temp);
                    voltage.push_back(temp);
                    rx_power.push_back(temp);
                    tx_power.push_back(temp);
                } else {
                    /* Identifier */
                    sprintf(temp, "%-4d %s\n", gbic.identifier, gbicIdType(&gbic));
                    identifier.push_back(temp);

                    /* Connector */
                    sprintf(temp, "%-4d %s\n", gbic.connector,  gbicIdConnector(&gbic));
                    connector.push_back(temp);

                    string tempStr = "";
                    /* Transceiver */
                    sprintf(temp, "%02x%02x%02x%02x%02x%02x%02x%02x %s %s %s %s\n",
                            gbic.transceiver[7], gbic.transceiver[6], gbic.transceiver[5],
                            gbic.transceiver[4], gbic.transceiver[4], gbic.transceiver[2],
                            gbic.transceiver[1], gbic.transceiver[0], gbicIdSpeed(&gbic),
                            gbicIdMedia(&gbic), gbicIdName(&gbic), gbicIdDistance(&gbic));
                    transceiver.push_back(temp);

                    /* Encoding */
                    sprintf(temp, "%-4d %s\n", gbic.encoding, gbicIdEncoding(&gbic));
                    encoding.push_back(temp);

                    /* Baud Rate */
                    sprintf(temp, "%-4d (units 100 megabaud)\n", gbic.baud_rate);
                    baud_rate.push_back(temp);

                    /* Length 9u Km */
                    sprintf(temp, "%-4u (units km)\n", gbic.len9m_km);
                    len9m_km.push_back(temp);

                    /* Length 9u m */
                    sprintf(temp, "%-4u (units 100 meters)\n", gbic.len9um);
                    len9um.push_back(temp);

                    /* Length 50u */
                    sprintf(temp, "%-4u (units 10 meters)\n", gbic.len50um);
                    len50um.push_back(temp);

                    /* Length 62.5u */
                    sprintf(temp, "%-4u (units 10 meters)\n", gbic.len625um);
                    len625um.push_back(temp);

                    /* Length Cu */
                    sprintf(temp, "%-4u (units 1 meter)\n", gbic.lencu);
                    lencu.push_back(temp);

                    /* Vendor Name */
                    sprintf(temp, "%.16s\n", gbic.vendor);
                    vendor.push_back(temp);

                    /* Vendor OUI */
                    sprintf(temp, "%02x:%02x:%02x\n", gbic.oui[0], gbic.oui[1], gbic.oui[2]);
                    oui.push_back(temp);

                    /* Vendor PN */
                    sprintf(temp, "%.16s\n", gbic.partno);
                    partno.push_back(temp);

                    /* Vendor Rev */
                    sprintf(temp, "%.4s\n", gbic.rev);
                    rev.push_back(temp);

                    /* Wavelength */
                    sprintf(temp, "%-4u (units nm)\n", gbic.wavelength[0] << 8 | gbic.wavelength[1]);
                    wavelength.push_back(temp);

                    /* Options */
                    sprintf(temp,"%02x%02x %s\n", gbic.options[0], gbic.options[1], gbicIdOptions(&gbic));
                    options.push_back(temp);

                    /* BR Max */
                    sprintf(temp,"%-4d\n", gbic.br_max);
                    br_max.push_back(temp);

                    /* BR Min */
                    sprintf(temp,"%-4d\n", gbic.br_min);
                    br_min.push_back(temp);

                    /* Serial No */
                    sprintf(temp,"%.16s\n", gbic.serialno);
                    serialno.push_back(temp);

                    /* Date Code */
                    sprintf(temp,"%.8s\n", gbic.date_code);
                    date_code.push_back(temp);

                    /* DD Type */
                    sprintf(temp,"0x%x\n", gbic.res6[0]);
                    ddType.push_back(temp);

                    /* Enh Options */
                    sprintf(temp,"0x%x\n", gbic.res6[1]);
                    enhOption.push_back(temp);

                    /* Error Info */
                    strcpy(temp, "");
                    err_info.push_back(temp);

                    if (gbic.smart) {
                        /* Status/Ctrl */
                        sprintf(temp,"0x%x\n", gbic.status_ctrl);
                        status_ctrl.push_back(temp);

                        /* Alarm flags */
                        sprintf(temp,"0x%x, 0x%x\n", gbic.alarm_flags[0], gbic.alarm_flags[1]);
                        alarm_flags.push_back(temp);

                        /* Warn Flags */
                        sprintf(temp, "0x%x, 0x%x\n", gbic.warn_flags[0], gbic.warn_flags[1]);
                        warn_flags.push_back(temp);

                        /* Temperature */
                        tempStr = "";
                        sprintf(temp, "                                               Alarm                  Warn\n");
                        tempStr += temp;
                        sprintf(temp, "                                           low        high       low "
                                "        high\n");
                        tempStr += temp;
                        sprintf(temp, "  Temperature      %-7d Centigrade     %-7d     %-7d    %-7d     %-d\n",
                                gbic.temperature, gbic.temp_low_alrm, gbic.temp_high_alrm,
                                gbic.temp_low_warn, gbic.temp_high_warn);
                        tempStr += temp;
                        temperature.push_back(tempStr);

                        /* Current */
                        sprintf(temp, "  Current          %-7.3f mAmps          %-7.3f     %-7.3f    %-7.3f     "
                                "%-7.3f\n", (gbic.opt_current * 0.002), (gbic.bias_low_alrm * 0.002),
                                (gbic.bias_high_alrm * 0.002), (gbic.bias_low_warn * 0.002),
                                (gbic.bias_high_warn * 0.002));
                        current.push_back(temp);

                        /* Voltage */
                        sprintf(temp, "  Voltage          %-7.1f mVolts         %-7.1f     %-7.1f    %-7.1f     "
                                "%-7.1f\n", (gbic.voltage * 0.1), (gbic.volt_low_alrm * 0.1),
                                (gbic.volt_high_alrm * 0.1), (gbic.volt_low_warn * 0.1),
                                (gbic.volt_high_warn * 0.1));
                        voltage.push_back(temp);

                        /* RX Power */
                        sprintf(temp, "  RX Power         %-7.1f dBm (%-5.1f uW) %-7.1fuW   %-7.1fuW  %-7.1fuW   "
                                "%-7.1fuW\n", (10 * (log10((gbic.rx_power * 0.1) / 1000))),
                                (gbic.rx_power * 0.1), (gbic.rxpwr_low_alrm * 0.1),
                                (gbic.rxpwr_high_alrm * 0.1), (gbic.rxpwr_low_warn * 0.1),
                                (gbic.rxpwr_high_warn * 0.1));
                        rx_power.push_back(temp);

                        /* TX Power */
                        sprintf(temp, "  TX Power         %-7.1f dBm (%-5.1f uW) %-7.1fuW   %-7.1fuW  %-7.1fuW   "
                                "%-7.1fuW\n", (10 * (log10((gbic.tx_power * 0.1) / 1000))),
                                (gbic.tx_power * 0.1), (gbic.txpwr_low_alrm * 0.1),
                                (gbic.txpwr_high_alrm * 0.1), (gbic.txpwr_low_warn * 0.1),
                                (gbic.txpwr_high_warn * 0.1));
                        tx_power.push_back(temp);
                    } else {
                        strcpy(temp, "");
                        status_ctrl.push_back(temp);
                        alarm_flags.push_back(temp);
                        warn_flags.push_back(temp);
                        temperature.push_back(temp);
                        current.push_back(temp);
                        voltage.push_back(temp);
                        rx_power.push_back(temp);
                        tx_power.push_back(temp);
                    }
                } //else
            }
        }

        /* Fill SFP values in Message */
        pFC_SfpShowMsg->setIdentifier(identifier);
        pFC_SfpShowMsg->setConnector(connector);
        pFC_SfpShowMsg->setTransceiver(transceiver);
        pFC_SfpShowMsg->setEncoding(encoding);
        pFC_SfpShowMsg->setBaudRate(baud_rate);
        pFC_SfpShowMsg->setLength_9u_Km(len9m_km);
        pFC_SfpShowMsg->setLength_9u_m(len9um);
        pFC_SfpShowMsg->setLength_50u(len50um);
        pFC_SfpShowMsg->setLength_625u(len625um);
        pFC_SfpShowMsg->setLength_Cu(lencu);
        pFC_SfpShowMsg->setVendor_Name(vendor);
        pFC_SfpShowMsg->setVendor_OUI(oui);
        pFC_SfpShowMsg->setVendor_PN(partno);
        pFC_SfpShowMsg->setVendor_Rev(rev);
        pFC_SfpShowMsg->setWavelength(wavelength);
        pFC_SfpShowMsg->setOptions(options);
        pFC_SfpShowMsg->setBrMax(br_max);
        pFC_SfpShowMsg->setBrMin(br_min);
        pFC_SfpShowMsg->setSerialNo(serialno);
        pFC_SfpShowMsg->setDateCode(date_code);
        pFC_SfpShowMsg->setDdType(ddType);
        pFC_SfpShowMsg->setEnhOptions(enhOption);
        pFC_SfpShowMsg->setStatusCtrl(status_ctrl);
        pFC_SfpShowMsg->setAlarmFlags(alarm_flags);
        pFC_SfpShowMsg->setWarnFlags(warn_flags);
        pFC_SfpShowMsg->setTemperature(temperature);
        pFC_SfpShowMsg->setVoltage(voltage);
        pFC_SfpShowMsg->setCurrent(current);
        pFC_SfpShowMsg->setTxPower(tx_power);
        pFC_SfpShowMsg->setRxPower(rx_power);
        pFC_SfpShowMsg->setErrInfo(err_info);

        return 0;
    }

    /* Get port protocol status */
    static void getport_protocolstatus(int switchPort, char *buf)
    {
        string protocol;
        string port_status = portPhysName(switchPort);

        if (portOnline(switchPort) && portActive(switchPort)) {
            protocol += " up(";
            protocol += port_status;
            protocol += ")  up     ";
            strcpy (buf, protocol.c_str());
        } else if (!portOnline(switchPort)) {
            protocol += " up(";
            protocol += port_status;
            protocol += ") down    ";
            strcpy (buf, protocol.c_str());
        }
        trace (TRACE_LEVEL_DEBUG, string("getport_protocolstatus port") +
               switchPort + string("portstatusspeed") + buf);
    }
    
    /* Get portWwn of device(s) connected and speed */
    static void getportWWN_Connecteddevice_Speed(int swno, PortCfgFCPortLoginInfoMessage *pFC_logininfoMsg)
    {
       FILE *fp ;
       char full_line[MAXNAMELEN];
       char pinfo_fname[MAXNAMELEN];
       int count = 0;
       char field[6][24];
       vector<string> wwnconnecteddevice;
       vector<UI32> device_count;
       vector <string> pid;
       int switchPort;
       string portid = "";
       string wwn = ""; 

       for (UI32 k = 0; k < pFC_logininfoMsg->getPortId().size(); k++) {
          switchPort = (pFC_logininfoMsg->getIndex().at(k));
          if (switchPort >= 0) {
			count = 0;
            portid = "";
            wwn = "";
            /* Proc Pinfo */
            sprintf(pinfo_fname, "/proc/fabos/switch/%d/%d/%s", swno, switchPort, "login");
            trace(TRACE_LEVEL_DEBUG, string("getportWWN_Connecteddevice_Speed :pinfo_fname: ") + pinfo_fname);

            if (!access(pinfo_fname, R_OK)) {
                fp = fopen (pinfo_fname, "r");
                if (!fp) { 
                   trace(TRACE_LEVEL_INFO, string("PortCfgLocalObjectManager Failed Reading info switchPort: ") + switchPort);
                   //failure case fill with default values
                   device_count.push_back(count);
                   wwnconnecteddevice.push_back(wwn);
                   pid.push_back(portid);
                }
                while (!feof(fp) ) {
                   while (fgets(full_line, MAXNAMELEN, fp) != NULL) {
				
                       sscanf(full_line, "%s %s %s %s %s %s %s ",
                            field[0], field[1], field[2], field[3], field[4], field[5],field[6]);

                       if (strncmp(field[0], "fe", 2) == 0 ) {
                          portid = field[1];
                          wwn = field[2];
                          count++;
                          pid.push_back(portid);
                          wwnconnecteddevice.push_back(wwn);
                          trace(TRACE_LEVEL_DEBUG,string("field1: ") + field[1] + string("field2: ") + field[2]);
                       }
                   } 
                   device_count.push_back(count);
                   trace(TRACE_LEVEL_DEBUG,string(" port= ") + switchPort + string(" count: ") + count);
                } //While
                fclose(fp);
           } else {
              trace(TRACE_LEVEL_INFO, string("Error Accessing Proc File"));
              //failure case fill with default values
              pid.push_back(portid);
              device_count.push_back(count);
              wwnconnecteddevice.push_back(wwn);
          }
        } // switchport loop end
      } // size loop end
      pFC_logininfoMsg->setPortWWN(wwnconnecteddevice);
      pFC_logininfoMsg->setDevice_count(device_count);
      pFC_logininfoMsg->setPid(pid);
   }

}  // end of DcmNs namespace
