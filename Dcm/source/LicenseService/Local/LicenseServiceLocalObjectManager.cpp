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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include <iostream>
#include <sstream>

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "LicenseService/Local/LicenseServiceLocalObjectManager.h"
#include "LicenseService/Local/LicensePodLocalManagedObject.h"
#include "LicenseService/Local/LicenseServiceRemoveMessage.h"
#include "LicenseService/Local/LicenseServiceAddMessage.h"
#include "LicenseService/Local/LicenseServiceShow.h"
#include "LicenseService/Local/LicenseServicePodShowMessage.h"
#include "LicenseService/Local/LicenseServiceGetLicenseIdShowMessage.h"
#include "LicenseService/Local/LicenseServicePodMessage.h"
#include "LicenseService/Local/LicenseServiceUpdateLicensePodInfoMessage.h"
#include "LicenseService/Local/LicenseServiceUpdateLicenseFabsysInfoMessage.h"
#include "LicenseService/Local/LicenseServiceTypes.h"
#include "LicenseService/Global/LicenseServiceGlobalObjectManager.h"
#include "LicenseService/Local/LicenseServiceFileAddMessage.h"
#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgGetFCPortInfoMessage.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
    
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"

#include "Utils/Layer3LicenseUtil.h"
#include "Fcoe/Local/FcoeLocalEnodeMessage.h"
#include "Fcoe/Common/FcoePluginUtils.h"
extern "C" {
     #include "libxml/parser.h"
     #include "libxml/xmlmemory.h"
     #include "libxml/xmlschemas.h"
     #include "libxml/xmlschemastypes.h"
}


/*
 * ATTENTION:
 *  All new FOS includes should be added to this
 *  global header file.  This will help to ensure
 *  that libInit() has been called for any libraries
 *  that are required.
 */
//#include "dcm/fos_headers.h"
#include "switch/switch.h"
#include "fos_swbd.h"

namespace DcmNs
{

    LicenseServiceLocalObjectManager::LicenseServiceLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        associateWithVirtualWaveObjectManager (LicenseServiceGlobalObjectManager::getInstance ());
        LicensePodLocalManagedObject    LicensePodLocalManagedObject    (this); 
        LicensePodLocalManagedObject.setupOrm ();
        addManagedClass (LicensePodLocalManagedObject::getClassName ());
        addOperationMap (LICENSESERVICEREMOVE, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceRemoveMessageHandler));
        addOperationMap (LICENSESERVICEADD, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceAddMessageHandler));
        addOperationMap (LICENSESERVICESHOW, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceShowMessageHandler));
        addOperationMap (LICENSESERVICEPODSHOW, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServicePodShowMessageHandler));
        addOperationMap (LICENSESERVICEGETLICENSEIDSHOW, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceGetLicenseIdShowMessageHandler));
        addOperationMap (LICENSESERVICEPOD, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServicePodMessageHandler));
        addOperationMap (LICENSESERVICEUPDATELICENSEPODINFO, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceUpdateLicensePodInfoMessageHandler));
        addOperationMap (LICENSESERVICEFILEADD, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceFileAddMessageHandler));
        addOperationMap (LICENSESERVICEUPDATELICENSEFABSYSINFO, reinterpret_cast<PrismMessageHandler> (&LicenseServiceLocalObjectManager::LicenseServiceUpdateLicenseFabsysInfoMessageHandler));
        PrismFrameworkObjectManager::addToExternalStateSynchronizationRequiredList (getServiceId ());
    }

    LicenseServiceLocalObjectManager::~LicenseServiceLocalObjectManager ()
    {
    }

    LicenseServiceLocalObjectManager  *LicenseServiceLocalObjectManager::getInstance()
    {
        static LicenseServiceLocalObjectManager *pLicenseServiceLocalObjectManager = new LicenseServiceLocalObjectManager ();

        WaveNs::prismAssert (NULL != pLicenseServiceLocalObjectManager, __FILE__, __LINE__);

        return (pLicenseServiceLocalObjectManager);
    }

    PrismServiceId  LicenseServiceLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *LicenseServiceLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case LICENSESERVICEREMOVE :
                pPrismMessage = new LicenseServiceRemoveMessage ();
                break;
            case LICENSESERVICEADD :
                pPrismMessage = new LicenseServiceAddMessage ();
                break;
            case LICENSESERVICESHOW :
                pPrismMessage = new LicenseServiceShowMessage ();
                break;
            case LICENSESERVICEPODSHOW :
                pPrismMessage = new LicenseServicePodShowMessage ();
                break;
            case LICENSESERVICEGETLICENSEIDSHOW :
                pPrismMessage = new LicenseServiceGetLicenseIdShowMessage ();
                break;
            case LICENSESERVICEPOD :
                pPrismMessage = new LicenseServicePodMessage ();
                break;
            case LICENSESERVICEUPDATELICENSEPODINFO :
                pPrismMessage = new LicenseServiceUpdateLicensePodInfoMessage ();
                break;
            case LICENSESERVICEUPDATELICENSEFABSYSINFO :
                pPrismMessage = new LicenseServiceUpdateLicenseFabsysInfoMessage ();
                break;
            case LICENSESERVICEFILEADD :
                pPrismMessage = new LicenseServiceFileAddMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LicenseServiceLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((LicensePodLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new LicensePodLocalManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "LicenseServiceLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);

    }

    ResourceId  LicenseServiceLocalObjectManager::LicenseAddEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceAddMessageContext)
    {
        LicenseServiceAddMessage *pLicenseServiceAddMessage = reinterpret_cast<LicenseServiceAddMessage *> (pLicenseServiceAddMessageContext->getPPrismMessage ());
        string msg = "";
        char key[ENH_LICENSE_LEN_MAX];
        int ret = 0;
        lic_ch_stat_t stat = {0};
        license_feature_t features[LICENSE_FEATURES_MAX] = {0};

        string msgStatus = pLicenseServiceAddMessage->getMsgStatus();
        snprintf(key, ENH_LICENSE_LEN_MAX, "%s", (char *) msgStatus.data());
        ret = licenseAdd((const char *)key, &stat);
        if (ret == 1) {
            msg = "\nLicense Added [";
            msg += key; 
            msg += "]\n";
            ret = licenseGetFeatures(key, features); 
            if (ret && licenseCheck(features[0])) {
               if(features[0] == BASE_FCOE_LICENSE) {
            		ResourceId status = WAVE_MESSAGE_ERROR;
                    FcoeLocalEnodeMessage    *enode_msg_p = new FcoeLocalEnodeMessage();
                    if (enode_msg_p != NULL) {
		    		     enode_msg_p->setFcoeEnodes(FCOE_PLUGIN_GET_FCOE_ENODES());
		    		     enode_msg_p->setFcoeEnodesOperBmp(FCOE_ENODE_OPER_UPD_FCOE_ENODES);
                         status = sendSynchronously (enode_msg_p);
                         if (WAVE_MESSAGE_SUCCESS == status) {
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::Fcoe enode message successful "));

                         } else {
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::Fcoe enode message failed ! "));
                         }
                         delete enode_msg_p;
                     }  
                }
                if (stat.sw_reboot_req) {
                    msg += "\nFor license change to take effect, please reboot the switch now...\n";
                } else if (stat.sw_en_req) {
                    if (switchOffline()) {
                        msg += "\nFor license change to take effect, enable the switch and any disabled ports...\n";
                    } else {
                        msg += "\nFor license change to take effect, it may be necessary to enable ports...\n";
                    }
                }
            } else {
                msg += "\nWARNING: license is not valid; it will not be effective.";
                msg += "\nPlease remove this invalid license and install a valid license.\n";
            }
        } else {
            msg = "\nLicense Add Failed - ";
            if (stat.status == LICENSE_DB_FTR_VER_EXISTS) {
                msg += "License feature already installed\n";
            } else if (stat.status == LICENSE_DB_NO_TRIAL) {
                msg += "Non-trial license feature already installed\n";
            } else if (stat.status == LICENSE_DB_INVALID) {
                msg += "Invalid license key\n";
            } else if (stat.status == LICENSE_DB_INVALID_DATA) {
                msg += "Invalid license data\n";
            } else if (stat.status == LICENSE_DB_EXPIRED) {
                msg += "License has expired\n";
            } else if (stat.status == LICENSE_DB_START_DATE) {
                msg += "System date precedes license start date - check system date\n";
            } else if (stat.status == LICENSE_DB_DOWNGRADE) {
                msg += "Downgrading of existing license capacity not allowed\n";
            } else {
                msg += "Internal - see logs\n";
            }
        }

        pLicenseServiceAddMessage->setMsgStatus(msg);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseAddEntries: exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicenseRemoveEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceRemoveMessageContext)
    {
        LicenseServiceRemoveMessage *pLicenseServiceRemoveMessage = reinterpret_cast<LicenseServiceRemoveMessage *> (pLicenseServiceRemoveMessageContext->getPPrismMessage ());
        string msg = "";
        char key[ENH_LICENSE_LEN_MAX];
        int  ret = 0;
        lic_ch_stat_t stat = {0};
        license_feature_t licFeature[1] = {0};
        int isASFeature = 0; /* is this the AS feature ? */
        int removeAScare = 0; /* do we care about side-effects */
        int isL3Feature = 0; /* is this the L3 feature ? */
        int removeL3care = 0; /* do we care about side-effects */
        int isFCOEFeature = 0; /* is this the FCoE feature ? */
        int feature;

        string msgStatus = pLicenseServiceRemoveMessage->getMsgStatus();
        snprintf(key, ENH_LICENSE_LEN_MAX, "%s", (char *) msgStatus.data());

        if (licenseGetFeatures((const char *)key, licFeature) == 1) {
            int swModel = get_pod_SWBD();
            ResourceId status = WAVE_MESSAGE_ERROR;
            feature = licFeature[0];
            isASFeature = feature == ADVANCED_SERVICES_LICENSE;
            removeAScare = (swModel == SWBD_M4 || swModel == SWBD_M8 || swModel == SWBD_M16) && isASFeature;
            isL3Feature = feature == LAYER_3_LICENSE;
            removeL3care = (swModel == SWBD_M4 || swModel == SWBD_M8 || swModel == SWBD_M16) && isL3Feature;
            isFCOEFeature = feature == BASE_FCOE_LICENSE;

            char * featureName = (char *)licenseNameByFeatureBit(feature);
            if (!licenseInstalled(featureName)) {
                msg = "\nLicense not found - license remove failed.\n";
                ret = 1;
            }

            if (!ret && (feature == VCS_FABRIC_LICENSE || removeAScare) && !switchOffline()) {
                msg = "Unable to remove license. Switch must be disabled prior to removing license.\n";
                ret = 1;
            }

            if (!ret && (isFCOEFeature || removeAScare)) {
                 /* Determine ALL FC Ports are Shut */
                 PortCfgGetFCPortInfoMessage *pcfg_msg_p = NULL;
                 bool isswitchFCportsOffline = 0;
                 UI32 mappedId = DcmToolKit::getLocalMappedId();
                 UI32 slotId = 0;

                 /* Sending message to PortCfg Plugin to determine FC Port Shut Status */
                 pcfg_msg_p = new PortCfgGetFCPortInfoMessage(FC_CMD_CODE_ALL_SHUT, mappedId, slotId, FC_RESP_CODE_NULL);

                 status = sendSynchronously (pcfg_msg_p);
                 if (WAVE_MESSAGE_SUCCESS == status) {
                     switch(pcfg_msg_p->getRespCode()) {
                         case FC_RESP_CODE_ALL_SHUT:
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::isswitchFCportsOffline: - All FC Ports Shut "));
                             isswitchFCportsOffline = 1;
                             break;
                         case FC_RESP_CODE_NOT_ALL_SHUT:
                         case FC_RESP_CODE_NULL:
                         default:
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::isswitchFCportsOffline: - All FC Ports Not Shut "));
                             break;
                     }
                 } else {
                     trace (TRACE_LEVEL_ERROR, string("License Plugin::isswitchFCportsOffline: Failed to Get Info."));
                 }
                 delete pcfg_msg_p;

                 if (!isswitchFCportsOffline) {
                     msg = "Unable to remove license. All FC ports must be disabled first.\n";
                     ret = 1;
                 } else {
                     LocationId locationId = FrameworkToolKit::getThisLocationId ();
                     if (NsmUtils::isFcoeConfiguredForEthPort(locationId)) {
                         msg = "Unable to remove license. FCoE config must be removed from all Ethernet ports and Port-channels.\n";
                         ret = 1;
                     }
                }
            }
            
            if (!ret && isFCOEFeature) {
                    FcoeLocalEnodeMessage    *enode_msg_p = new FcoeLocalEnodeMessage();
					UI32 fcoe_enodes = 0;
                    if (enode_msg_p != NULL) {
		    		     enode_msg_p->setFcoeEnodes(fcoe_enodes);
		    		     enode_msg_p->setFcoeEnodesOperBmp(FCOE_ENODE_OPER_UPD_FCOE_ENODES);
                         status = sendSynchronously (enode_msg_p);
                         if (WAVE_MESSAGE_SUCCESS == status) {
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::Fcoe enode message successful "));
                             status = enode_msg_p->getCompletionStatus();
                            if(status == FCOE_FAB_MAP_ERR_FAILED_MAX_ENODES_NOT_ENOUGH_PORTS) {
                        		msg = "Please remove the fcoe binding configuration before remove licence..\n";
                                ret = 1;
                            }
                         } else {
                             trace (TRACE_LEVEL_DEBUG, string("License Plugin::Fcoe enode message failed ! "));
                         }
                         delete enode_msg_p;
                     }  
            }

            if (!ret && (removeL3care || removeAScare)) {
                bool isL3FeatureConf = false;

                status = Layer3LicenseUtil::isLayer3Configured(isL3FeatureConf);
                if (WAVE_MESSAGE_SUCCESS == status) {
                    if (isL3FeatureConf) {
                        msg = "Unable to remove license. All Layer 3 features must be cleared prior to removing license.\n";
                        ret = 1;
                    }
                } else {
                    msg = "Unable to remove license. Call to obtain Layer 3 config failed. Please retry command.\n";
                    ret = 1;
                }
            }
        }

        if (ret == 0) {
            ret = licenseRemove((const char *)key, &stat);
            if (ret == 1) {
                msg = "\nLicense Removed [";
                msg += key;
                msg += "]\n";

                /* allow silent removal of AS, L3 feature on non-chassis systems */
                if (!(isASFeature || isL3Feature) || removeAScare || removeL3care) {
                    if (stat.sw_reboot_req) {
                        msg += "\nFor license change to take effect, please reboot the switch now...\n";
                    } else if (stat.sw_en_req) {
                        if (switchOffline()) {
                            msg += "\nFor license to take effect, enable the switch and any disabled ports...\n";
                        } else {
                            msg += "\nFor license to take effect, it may be necessary to disable/enable ports or switch...\n";
                        }
                    }
                }
            } else {
                msg = "\nLicense Remove Failed\n";
            }
        }

        pLicenseServiceRemoveMessage->setMsgStatus(msg);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseRemoveEntries: exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicenseShowEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceShowMessageContext)
    {
        LicenseServiceShowMessage *pLicenseServiceShowMessage = reinterpret_cast<LicenseServiceShowMessage *> (pLicenseServiceShowMessageContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "License Plugin: ShowGetEntriesMessage - entering...");
        vector<string> license;
        vector<string> expirydatestr;
        vector<string> featureNamestr;
        vector<string> capacitystr;
        vector<string> consumedstr;
        vector<string> validState;

        int i, max, status;

        enh_license_entry_t *LicenseShow_entry_list = (enh_license_entry_t*) calloc(MAX_SENTINAL_LICENSES, sizeof(enh_license_entry_t));

        status = licenseShow(PRINTF_DISABLED, LicenseShow_entry_list, &max);

        if ((status == LICENSE_DB_OK) && (max > 0) && (LicenseShow_entry_list != NULL)) {
            for (i=0; i<max; i++) {
                license.push_back(ENH_L_P(LicenseShow_entry_list,i)->license);
                expirydatestr.push_back(ENH_L_P(LicenseShow_entry_list,i)->expirydatestr);
                featureNamestr.push_back(ENH_L_P(LicenseShow_entry_list,i)->featureName);
                capacitystr.push_back(ENH_L_P(LicenseShow_entry_list,i)->capacity);
                consumedstr.push_back(ENH_L_P(LicenseShow_entry_list,i)->consumed);
                validState.push_back(ENH_L_P(LicenseShow_entry_list,i)->validState);
            }
        }
        free(LicenseShow_entry_list);

        pLicenseServiceShowMessage->setLicense(license);
        pLicenseServiceShowMessage->setExpirydatestr(expirydatestr);
        pLicenseServiceShowMessage->setFeatureName(featureNamestr);
        pLicenseServiceShowMessage->setCapacity(capacitystr);
        pLicenseServiceShowMessage->setConsumed(consumedstr);
        pLicenseServiceShowMessage->setValidState(validState);

        trace (TRACE_LEVEL_DEBUG, "License Plugin: ShowGetEntriesMessage - exiting...");

        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicensePodShowEntries (PrismSynchronousLinearSequencerContext *pLicenseServicePodShowMessageContext)
    { 
        LicenseServicePodShowMessage *pLicenseServicePodShowMessage = 
            reinterpret_cast<LicenseServicePodShowMessage *> (pLicenseServicePodShowMessageContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicensePodShowEntries - entering...");
        vector<string> podstr;
        vector<UI32> max_DPOD_Ports;
        vector<UI32> num_DPOD_Licenses;
        vector<UI32> max_DPOD_provisioned;
        vector<UI32> reserved_ports;
        vector<UI32> released_ports;
        pod_entry_t podlicense = {{0}};
        UI32         mapped_id;

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            mapped_id = DcmToolKit::getLocalMappedId();
        } else {
            mapped_id = 0;
        }
        if(enh_LicensePodShow(&podlicense, mapped_id) == 0) {
            podstr.push_back(string((char *)podlicense.podstr));
            max_DPOD_Ports.push_back(podlicense.max_DPOD_Ports);
            num_DPOD_Licenses.push_back(podlicense.num_DPOD_Licenses);
            max_DPOD_provisioned.push_back(podlicense.max_DPOD_provisioned);
            reserved_ports.push_back(podlicense.reserved_ports);
            released_ports.push_back(podlicense.released_ports);

        } else {
            trace (TRACE_LEVEL_DEBUG, "License Plugin:LicensePodShowEntries Failed - exiting...");
            return(WAVE_MESSAGE_ERROR);
        }

        pLicenseServicePodShowMessage->setPodstr(podstr);
        pLicenseServicePodShowMessage->setMax_DPOD_Ports(max_DPOD_Ports);
        pLicenseServicePodShowMessage->setNum_DPOD_Licenses(num_DPOD_Licenses);
        pLicenseServicePodShowMessage->setMax_DPOD_provisioned(max_DPOD_provisioned);
        pLicenseServicePodShowMessage->setReserved_ports(reserved_ports);
        pLicenseServicePodShowMessage->setReleased_ports(released_ports);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicensePodShowEntries: Success - exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicensePodEntries (PrismSynchronousLinearSequencerContext *pLicenseServicePodMessageContext)
    {
        LicenseServicePodMessage *pLicenseServicePodMessage =
            reinterpret_cast<LicenseServicePodMessage *> (pLicenseServicePodMessageContext->getPPrismMessage ());
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicensePodEntries: Entering...");

        string msg="";
        char option[8];
        char interface[20];
        char swslotport[11];

        int  ret = 0;

        /* Get parameters parsed and passed to the function */
        msg = pLicenseServicePodMessage->getOption();
        snprintf(option, 8, "%s", (char *) msg.data());
        msg = pLicenseServicePodMessage->getInterface();
        snprintf(interface, 20, "%s", (char *) msg.data());
        msg = pLicenseServicePodMessage->getSwSlotPort();
        snprintf(swslotport, 11, "%s", (char *) msg.data());
        trace (TRACE_LEVEL_DEBUG, string("License Plugin: LicensePodEntries: option:")+option+string(" interface: ")+interface+string("swslotport: ")+swslotport);

        ret = enh_LicensePod((const char *)option, (const char *)interface, (const char *)swslotport);
        
        /* Return Value Message Handling */
        if (ret < 0) {
            msg = pod_err_msg(ret);
            msg += '\n';
        } else if (ret == 0) {
            msg = "Success";
        }    
        pLicenseServicePodMessage->setOption(msg);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicensePodEntries: Exiting...");
        
        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicenseIdShowGetEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceGetLicenseIdShowMessageContext)
    {
        LicenseServiceGetLicenseIdShowMessage *pLicenseServiceGetLicenseIdShowMessage =
            reinterpret_cast<LicenseServiceGetLicenseIdShowMessage *> (pLicenseServiceGetLicenseIdShowMessageContext->getPPrismMessage ());
 
        vector<WorldWideName> licenseid;
        licenseid_entry_t licenseid_entry = {{0}};
 
        if (LicenseIdShow(&licenseid_entry) == 0) {
            licenseid.push_back(string((char *)licenseid_entry.license_id));
        } else {
            trace (TRACE_LEVEL_DEBUG, "License Plugin: GetLicenseIdShowMessage: Failed - exiting...");
            return(WAVE_MESSAGE_ERROR);
        }
        pLicenseServiceGetLicenseIdShowMessage->setLicenseid(licenseid);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: GetLicenseIdShowMessage: Success - exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId  LicenseServiceLocalObjectManager::LicenseFileAddEntries (PrismSynchronousLinearSequencerContext *pLicenseServiceFileAddMessageContext)
    {
        LicenseServiceFileAddMessage *pLicenseServiceFileAddMessage = reinterpret_cast<LicenseServiceFileAddMessage *> (pLicenseServiceFileAddMessageContext->getPPrismMessage ());

        string msg = "";
        char fileDest[256] = "/tmp";
        string fileDestS = "";
        string filename = "";
        string srcfilepath = "";
        string cmd = "";
        int rc = 0; 
        char lickey[256];
        int license_count = 0;
        int ret = 0;
        lic_ch_stat_t licStat = {0};
        xmlDocPtr doc;
        xmlNodePtr cur;
        xmlNodePtr tempCur;     
        size_t found = 0;

        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: Entering...");
        string msgStatus = pLicenseServiceFileAddMessage->getMsgStatus();
        string proto = pLicenseServiceFileAddMessage->getProto();
        string user = pLicenseServiceFileAddMessage->getUser();
        string host = pLicenseServiceFileAddMessage->getHost();
        string password = pLicenseServiceFileAddMessage->getPassword();
        srcfilepath = pLicenseServiceFileAddMessage->getSrcFilePath();

/*********************************************************************************************************************
*                                                validate the file                                                   *
*********************************************************************************************************************/
        found = srcfilepath.find(".xml");
        if (found == string::npos){
            msg = "Invalid file name.\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            return (WAVE_MESSAGE_SUCCESS);
        }           

        found = srcfilepath.find_last_of("/");
        if (found!=string::npos)
            filename = srcfilepath.substr(found);
        else
            filename = srcfilepath;

        strcat(fileDest, filename.c_str());

        msg = fileDest;
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: fileDest :" + msg);
/**********************************************************************************************************************
*                                                file copy to local directory                                         *
**********************************************************************************************************************/
        if(proto.compare("ftp") == 0){
        cmd = "/bin/wget -a /var/log/wget.log -c -T 60 --tries=3 -nH -P /tmp/  ftp://" + user + ":" + password + "@" + host + "/" + srcfilepath + " 1>/dev/null";
        }

        if(proto.compare("scp") == 0){
        cmd = "/usr/bin/scp -q -oPreferredAuthentications=password -oStrictHostKeyChecking=no -oSSHPassword=" + password + " " + user + "@" + host + ":/" + srcfilepath + " /tmp/ >/dev/null 2>&1";
        }

        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: cmd :" + cmd);

        rc = system(cmd.c_str());

        if (WEXITSTATUS(rc) != 0) {
            msg = "File copy operation fail, Please check the file path\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            trace (TRACE_LEVEL_ERROR, "License Plugin: LicenseFileAddEntries: WEXITSTATUS fail.  exiting...");
            return (WAVE_MESSAGE_SUCCESS);          
        }

        struct stat st;
        if(stat(fileDest,&st) != 0){
            msg = "File copy operation fail, Please cheack the file path\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: file is not present in /tmp folder. fail exiting...");
            return (WAVE_MESSAGE_SUCCESS);
        }

/*********************************************************************************************************************
*                                                xml file parsing                                                    *
*********************************************************************************************************************/      
        doc = xmlParseFile(fileDest);
        
        if (doc == NULL ) { 
            msg = "File parsing fail, Please check the file format\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            trace (TRACE_LEVEL_ERROR, "License Plugin: LicenseFileAddEntries: xmlParseFile(docname); fail exiting...");
            unlink(fileDest);
            return (WAVE_MESSAGE_SUCCESS);
        }

        cur = xmlDocGetRootElement(doc);

        if (cur == NULL) {
            msg = "File parsing fail, Please check the file format\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            trace (TRACE_LEVEL_ERROR, "License Plugin: LicenseFileAddEntries: xmlDocGetRootElement(doc) fail exiting...");
            xmlFreeDoc(doc);
            unlink(fileDest);
            return (WAVE_MESSAGE_SUCCESS);          
        }

        if (xmlStrcmp(cur->name, (const xmlChar *) "licenseInfo")) {
            msg = "File parsing fail, Please check the file format\n";
            pLicenseServiceFileAddMessage->setMsgStatus(msg);
            trace (TRACE_LEVEL_ERROR, "License Plugin: LicenseFileAddEntries: XML License File of the wrong type exiting..");
            xmlFreeDoc(doc);
            unlink(fileDest);
            return (WAVE_MESSAGE_SUCCESS);
        }

        cur = cur->xmlChildrenNode;
        msg = "";
        while (cur != NULL) {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"license"))) {
                tempCur = cur;
                tempCur = tempCur->xmlChildrenNode;
                while (tempCur != NULL) {
                    if ((!xmlStrcmp(tempCur->name, (const xmlChar *)"licKey"))) {
                        if (( license_count) < 100) {
                            strcpy(lickey,
                                (char *)xmlNodeListGetString(doc, tempCur->xmlChildrenNode, 1));
                            cmd = lickey;
/****************************************************************************************************************
*                                                   Add license                                                 *
****************************************************************************************************************/
                            ret = licenseAdd((const char *)lickey, &licStat);

                            if (ret == 1) {
                                msg += "\nLicense Added [";
                                msg += lickey; 
                                msg += "] Successfully";
                                if (licStat.sw_reboot_req) {
                                    msg += " - For license change to take effect, please reboot the switch now...\n";
                                } else if (licStat.sw_en_req) {
                                    if (switchOffline()) { 
                                        msg += " - For license change to take effect, enable the switch and any disabled ports...";
                                    } else {
                                        msg += " - For license change to take effect, it may be necessary to enable ports...";
                                    }   
                                }
                                msg += "\n";   
                            } else {    
                                msg += "\nLicense Add [";
                                msg += lickey;
                                msg += "] Failed - ";                           
                                if (licStat.status == LICENSE_DB_FTR_VER_EXISTS) {
                                    msg += "License feature already installed\n";
                                } else if (licStat.status == LICENSE_DB_NO_TRIAL) {
                                    msg += "Non-trial license feature already installed\n";
                                } else if (licStat.status == LICENSE_DB_INVALID) {
                                    msg += "Invalid license key\n";
                                } else if (licStat.status == LICENSE_DB_INVALID_DATA) {
                                    msg += "Invalid license data\n";
                                } else if (licStat.status == LICENSE_DB_EXPIRED) {
                                    msg += "License has expired\n";
                                } else if (licStat.status == LICENSE_DB_START_DATE) {
                                    msg += "System date precedes license start date - check system date\n";
                                } else if (licStat.status == LICENSE_DB_DOWNGRADE) {
                                    msg += "Downgrading of existing license capacity not allowed\n";
                                } else {
                                    msg += "Internal - see logs\n";
                                }
                            }
                            trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: lic key :" + msg);

                            license_count++;
                        } else {
                            xmlFreeDoc(doc);
                            unlink(fileDest);
                            trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: Maximum number of license add:" );
                            msg = "Maximum number of license add\n";
                            pLicenseServiceFileAddMessage->setMsgStatus(msg);
                            return (WAVE_MESSAGE_SUCCESS);              
                        }
                    }
                tempCur = tempCur->next;
                }       
            }
        cur = cur->next;
        }

        xmlFreeDoc(doc);
        unlink(fileDest);   
        pLicenseServiceFileAddMessage->setMsgStatus(msg);
        trace (TRACE_LEVEL_DEBUG, "License Plugin: LicenseFileAddEntries: exiting...");
        return (WAVE_MESSAGE_SUCCESS);
    }


/*      
 *  DPOD WyserEaConfiguration Implementation Functions
 *  1. QueryLicensePodLocalManagedObject()
 *  2. install
 *  3. LicensePodUpdateEntries()
 *  4. LicenseFabsysInfoUpdateEntries()
 *  5. externalStateSynchronization()
 */    
 

    /* Query Local Managed Object */
    LicensePodLocalManagedObject *LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject (string portId)
    {
        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (LicensePodLocalManagedObject::getClassName ());
        trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject - Entering");

        /* Get the object corresponding to this mapped PortId */
        synchronousQueryContext.addAndAttribute (new AttributeString (portId, "portId"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();

            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_ERROR, "LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject - Cannot Find LicensePod Entry");
                pResults->clear ();
                delete pResults;
                return (NULL);
            } else if (1 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject - Found A Entry");
                LicensePodLocalManagedObject *pLicensePodLocalManagedObject =
                    dynamic_cast<LicensePodLocalManagedObject *> ((*pResults)[0]);

                pResults->clear ();
                delete pResults;
                return (pLicensePodLocalManagedObject);
            } else {
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject - There can be only one Local Node.");
                pResults->clear ();
                delete pResults;
                prismAssert (false, __FILE__, __LINE__);
            }
        } else {
            trace (TRACE_LEVEL_FATAL, "LicenseServiceLocalObjectManager::QueryLicensePodLocalManagedObject - System Failure.");
            prismAssert (false, __FILE__, __LINE__);
        }
        return (NULL);
    }

    /* Install Managed Object */
    void LicenseServiceLocalObjectManager::install(WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases)
    {
        ResourceId   status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install");

        unsigned int sys_total_dpod_ports = 0;
        unsigned int i;
        int          swModel = get_pod_SWBD();
        int          vcsEnabled = FALSE;
        UI32         mapped_id = 0;

        /* For Port Formatting */
        string       portStr = "";
        char         portFmt[10];
        string       queryDefaultPort = "";

        /* For platforms that do not support POD/DPOD - just return SUCCESS */
        if (!isDPODplatform(swModel)) {
            trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install: Not Supported on this platform.");
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            vcsEnabled = TRUE;
            mapped_id = DcmToolKit::getLocalMappedId();
            snprintf(portFmt, 10, "%d/0/1", mapped_id);
            queryDefaultPort += portFmt;
        } else {
            queryDefaultPort += POD_QUERY_DEFAULT_PORT;
        }

        trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::install - queryDefaultPort: ") + queryDefaultPort);
        switch (swModel) {
            case SWBD_ELARA2E:
            case SWBD_ELARA2F:
                /* Callisto-24 */ 
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - Platform CALLISTO-24");
                sys_total_dpod_ports = ELARA2_DPOD_PORTS;
                break;
            case SWBD_CALLISTOE:
            case SWBD_CALLISTOF:
                /* Callisto-60 */
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - Platform CALLISTO-60");
                sys_total_dpod_ports = CALLISTO_DPOD_PORTS;
                break;
            case SWBD_CASTOR:
            case SWBD_CASTORT:
                /* Castor platforms */
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - Platform CASTOR");
                sys_total_dpod_ports = CASTOR_10G_DPOD_PORTS + CASTOR_40G_DPOD_PORTS;
                break;
            default:
                /* Invalid platform - should not happen */
                trace (TRACE_LEVEL_ERROR, string("LicenseServiceLocalObjectManager::ESS - Invalid swModel:") + swModel);
                prismAssert (false, __FILE__, __LINE__);
                break;
        }
        LicensePodLocalManagedObject *pLicensePodLocalManagedObject = QueryLicensePodLocalManagedObject(queryDefaultPort);
        vector<LicensePodLocalManagedObject *> plicensepodObjs;
        if (pLicensePodLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - DB Exists");
            status = WAVE_MESSAGE_SUCCESS;
            delete pLicensePodLocalManagedObject;
        } else {
            // if we are here for the very first time, install default db entries
            trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - Create Default DB Entries");
            if (pWaveAsynchronousContextForBootPhases->getBootReason() ==
                                          WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT) {
                trace (TRACE_LEVEL_DEBUG,
                    string("LicenseServiceLocalObjectManager::install - WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT"));
            }
            startTransaction();
            for (i = PORT_START_NUM; i <= sys_total_dpod_ports; i++)
            {
                pLicensePodLocalManagedObject = new LicensePodLocalManagedObject(this);
                plicensepodObjs.push_back(pLicensePodLocalManagedObject);
                snprintf(portFmt, 10, "%s", "");
                portStr ="";
                if (vcsEnabled) {
                    snprintf(portFmt, 10, "%d/0/%d", mapped_id, i);
                } else {
                    snprintf(portFmt, 10, "0/%d", i);
                }
                portStr += portFmt;
                /*      
                 * If install and WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT (happens on secondary
                 * when added to the cluster for the first time.) update from switchdriver to DB
                 */
                if (pWaveAsynchronousContextForBootPhases->getBootReason() ==
                                            WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT) {
                    char port_ass;
                    int rc; 
                    pod_t   pod_info;
                    pod_t   *pinfo_p = &pod_info;
                    /* Get the Snapshot of Fabsys Kernel */
                    if ((rc = get_pod_data(pinfo_p)) != 0) { 
                        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::install get_pod_data fail"));
                    }           
                    port_ass = pinfo_p->pod_assignment.bytes[i];
                    if (port_ass == FABSYS_POD_PORT_IN_BASE) {
                        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::BASE_SET install port: ")+  i + string ("id: ") + portStr);
                        pLicensePodLocalManagedObject->setPortId(portStr);
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_BASE_SET);
                        pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    } else if (port_ass == FABSYS_POD_PORT_IN_SINGLE) {
                        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager:: SINGLE_SET install port:") + i + string ("id: ") + portStr);
                        pLicensePodLocalManagedObject->setPortId(portStr);
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_SINGLE_SET);
                        pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    } else if (port_ass == FABSYS_POD_PORT_IN_DOUBLE) {
                        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::DOUBLE_SET install port: ") + i + string ("id: ") + portStr);
                        pLicensePodLocalManagedObject->setPortId(portStr);
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_DOUBLE_SET);
                        pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    } else {
                        pLicensePodLocalManagedObject->setPortId(portStr);
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_UNASSIGNED);
                        pLicensePodLocalManagedObject->setOperation(DPOD_RELEASE);
                    }
                    trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::install doing portid : ") +
                             pLicensePodLocalManagedObject->getPortId() + string("getAssignedPod: ") +
                        pLicensePodLocalManagedObject->getAssignedPodset());
                 } else {
                    /*
                     * PortId: Contains Slot/Port Format when in stand alone mode.
                     * Contains Rbridge-id/Slot/Port for VCS mode.
                     * AssignedPodset: Contains None as the default value
                     * Operation: Contains Release as the default value
                     */
                    trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::install default entry portStr: ") + portStr);
                    pLicensePodLocalManagedObject->setPortId(portStr);
                    pLicensePodLocalManagedObject->setAssignedPodset(DPOD_UNASSIGNED);
                    pLicensePodLocalManagedObject->setOperation(DPOD_RELEASE);
                }
            } // for
            status = commitTransaction ();
            for (UI32 j = 0; j < plicensepodObjs.size(); j++) {
                delete plicensepodObjs[j];
            }       
            if (FRAMEWORK_SUCCESS == status) {
               trace(TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::install - Completed"));
               status = WAVE_MESSAGE_SUCCESS;
            } else {
               trace(TRACE_LEVEL_ERROR, string ("LicenseServiceLocalObjectManager::install - Failed "));
               prismAssert (false, __FILE__, __LINE__);
            }
        } // else
        trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::install - Completed Creating Entries");
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    /* LINK_UP Event Handling - Update DB Message */
    ResourceId LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries(PrismSynchronousLinearSequencerContext *pLicenseServiceUpdateLicenseFabsysInfoMessageContext) {
        string portStr = "";
        string convertedPortId = "";
        int swModel = get_pod_SWBD();

        LicenseServiceUpdateLicenseFabsysInfoMessage *pLicenseServiceUpdateLicenseFabsysInfoMessage = reinterpret_cast<LicenseServiceUpdateLicenseFabsysInfoMessage *> (pLicenseServiceUpdateLicenseFabsysInfoMessageContext->getPPrismMessage ());

        /* For platforms that do not support POD/DPOD always report SUCCESS */
        if (!isDPODplatform(swModel)) {
            trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries: always allowed."));
            return (WAVE_MESSAGE_SUCCESS); 
        }

        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysUpdateEntries - PortId: ") + pLicenseServiceUpdateLicenseFabsysInfoMessage->getPortId() + string ("  AssignType: ") + pLicenseServiceUpdateLicenseFabsysInfoMessage->getAssignType());
        portStr = pLicenseServiceUpdateLicenseFabsysInfoMessage->getPortId();

        if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
            /* VCS is enabled. */
            if (NsmUtils::isValidTwoTuple(portStr)) {
                NsmUtils::getThreeTupleIfName(portStr, convertedPortId, 0);
                trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysUpdateEntries - VCS mode-0 - convertedPortId: ") + convertedPortId);
            } else {
                convertedPortId = portStr;
                trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysUpdateEntries - VCS mode-1 - convertedPortId: ") + convertedPortId);
            }
        } else {
            /* Stand-alone mode */
            if (NsmUtils::isValidThreeTuple(portStr)) {
                NsmUtils::getTwoTupleIfName(portStr, convertedPortId);
                trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysUpdateEntries - Stand-alone mode-0 - convertedPortId: ") + convertedPortId);
            } else {
                convertedPortId = portStr;
                trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysUpdateEntries - Stand-alone mode-1 - convertedPortId: ") + convertedPortId);
            }
        }
        LicensePodLocalManagedObject *pLicensePodLocalManagedObject = QueryLicensePodLocalManagedObject(convertedPortId);
        portStr = pLicenseServiceUpdateLicenseFabsysInfoMessage->getAssignType();

        if(pLicensePodLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries - Managed Object Exists"));
            pLicensePodLocalManagedObject->setAssignedPodset(portStr);
            pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
            updateWaveManagedObject (pLicensePodLocalManagedObject);
            pLicenseServiceUpdateLicenseFabsysInfoMessageContext->addManagedObjectForGarbageCollection(pLicensePodLocalManagedObject);
            trace(TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries - Update Successfull"));
         } else {
            trace (TRACE_LEVEL_ERROR, string ("LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries - No Entry Found for Port: ") + convertedPortId);
            return WAVE_MESSAGE_ERROR;
        }
        return WAVE_MESSAGE_SUCCESS;
    }

    /* Update LicensePod */
    ResourceId LicenseServiceLocalObjectManager::LicensePodUpdateEntries(PrismSynchronousLinearSequencerContext *pLicenseServiceUpdateLicensePodInfoMessageContext) {

        int    ret;
        int    errRet = WAVE_MESSAGE_SUCCESS;
        char   option[8];
        char   portFmt[5];
        string portStr;
        string twoDuplePortStr = "";
        string errMsg;
        int swModel = get_pod_SWBD();
        
        LicenseServiceUpdateLicensePodInfoMessage *pLicenseServiceUpdateLicensePodInfoMessage = reinterpret_cast<LicenseServiceUpdateLicensePodInfoMessage *> (pLicenseServiceUpdateLicensePodInfoMessageContext->getPPrismMessage ());

        /* For platforms that we don't support POD/DPOD on, return error */
        if (!isDPODplatform(swModel)) {
            trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: ") + pod_err_msg(POD_ERR_NOT_SUPPORTED));
            return (LICENSE_DPOD_NOT_SUPPORTED_ERROR);
        }

        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicensePodUpdateEntries portId ") + pLicenseServiceUpdateLicensePodInfoMessage->getPortId() + string ("  AssignType ") + pLicenseServiceUpdateLicensePodInfoMessage->getAssignType());

        
        /* Get parameters parsed and passed to the function */
        portStr = pLicenseServiceUpdateLicensePodInfoMessage->getAssignType();
        snprintf(option, 8, "%s", (char *) portStr.data());
        portStr = pLicenseServiceUpdateLicensePodInfoMessage->getPortId();
        if (NsmUtils::isValidThreeTuple(portStr)) {
            NsmUtils::getTwoTupleIfName(portStr, twoDuplePortStr);
        } else {
            twoDuplePortStr = portStr;
        }
        snprintf(portFmt, 5, "%s", (char *) twoDuplePortStr.data());
        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicensePodUpdateEntries - portFmt=") + portFmt);

        LicensePodLocalManagedObject *pLicensePodLocalManagedObject = QueryLicensePodLocalManagedObject(pLicenseServiceUpdateLicensePodInfoMessage->getPortId());
        if (pLicensePodLocalManagedObject) {
            trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::LicensePodUpdateEntries - Managed Object Exists - getting PhyMO for " + portStr));
    
            int portUp = 0;
            int portValid = 0;
            PhyIntfLocalManagedObject* phyMO = NULL;

            NsmUtils::getPhyMoByPhyName(portStr, phyMO);
            if (phyMO == NULL) {
                trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: Invalid response from getPhyMoByPhyName for " + portStr));

                /* other interfaces exist if this is a QSFP interface in breakout mode... */
                string colons[4] = {string(":1"), string(":2"), string(":3"), string(":4")};
                int index;
                for (index = 0 ; index < 4 ; index++) {
                    string portStrColon = portStr;
                    portStrColon += colons[index];
                    NsmUtils::getPhyMoByPhyName(portStrColon, phyMO);
                    if (phyMO == NULL) {
                        trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: Invalid colon response from getPhyMoByPhyName for " + portStrColon));
                    } else { 
                        trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: Valid response from getPhyMoByPhyName for " + portStrColon));
                        portValid++;
                        if (!(phyMO->getStatus())) {
                            portUp++;
                            trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: portUp++ for " + portStrColon));
                        }
                        pLicenseServiceUpdateLicensePodInfoMessageContext->addManagedObjectForGarbageCollection(phyMO);
                    }
                }
            } else {
                trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::LicensePodUpdateEntries: Valid response from getPhyMoByPhyName for " + portStr));
                portValid++;
                if (!(phyMO->getStatus())) portUp++;
                pLicenseServiceUpdateLicensePodInfoMessageContext->addManagedObjectForGarbageCollection(phyMO);
            }

            if (!portValid) {
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::LicensePodUpdateEntries: no valid interfaces specified");
                ret = POD_ERR_WRONG_PORT_NUM;
            } else {
                if(strcasecmp("reserve", option) == 0) {
                    /* Not required to check port status for reserve operation */
                    ret = enh_LicenseDPod((const char *)option, (const char *)portFmt);
                    trace (TRACE_LEVEL_DEBUG, string("enh_LicenseDPod - ret Value ") + ret +  string(" option ")+ option );
                } else {
                    if (!portUp) {
                        /* Port is/ports are admin down, OK to proceed with release operation. */
                        ret = enh_LicenseDPod((const char *)option, (const char *)portFmt);
                        trace (TRACE_LEVEL_DEBUG, string("enh_LicenseDPod - ret Value ") + ret +  string(" option ")+ option );
                    } else {
                        /* Port is admin up.  Must be down to change DPOD port assignments. */
                        ret = POD_ERR_PORT_ONLINE;
                    }
                }
            }

            /*
             * Error Handling for return values from enh_LicenseDpod()
             */
            if (ret < 0) {
                errMsg = pod_err_msg(ret);
                trace (TRACE_LEVEL_ERROR, string("enh_LicenseDPod - " + errMsg));
                switch (ret) {
                    case POD_ERR_SYSMOD_INIT:
                        errRet = LICENSE_DPOD_SYSMOD_INIT_ERROR;
                        break;
                    case POD_ERR_WRONG_PORT_NUM:
                        errRet = LICENSE_DPOD_INVALID_CLI_PORT_INPUT_ERROR;
                        break;
                    case POD_ERR_INVALID_ARG:
                        errRet = LICENSE_DPOD_INVALID_CLI_OPERATION_INPUT_ERROR;
                        break;
                    case POD_ERR_GET_POD_DATA:
                        errRet = LICENSE_DPOD_GET_POD_DATA_ERROR;
                        break;
                    case POD_ERR_INACTIVE:
                        errRet = LICENSE_DPOD_POD_INACTIVE_ERROR;
                        break;
                    case POD_ERR_RELEASE_POD_PORT:
                        errRet = LICENSE_DPOD_RELEASE_PORT_ERROR;
                        break;
                    case POD_ERR_ALREADY_LICENSED:
                        trace (TRACE_LEVEL_DEBUG, string("POD port already assigned"));
                        errRet = WAVE_MESSAGE_SUCCESS;
                        break;
                    case POD_ERR_NO_LICENSE:
                        errRet = LICENSE_DPOD_NO_LICENSE_ERROR;
                        break;
                    case POD_ERR_RESERVE_POD_PORT:
                        errRet = LICENSE_DPOD_RESERVE_PORT_ERROR;
                        break;
                    case POD_ERR_PORT_ONLINE:
                        errRet = LICENSE_DPOD_PORT_ONLINE_ERROR;
                        break;
                    case DPOD_ERR_NOT_SUPPORTED:
                    case POD_ERR_NOT_SUPPORTED:
                        errRet = LICENSE_DPOD_NOT_SUPPORTED_ERROR;
                        break;
                    default:
                        errRet = LICENSE_DPOD_MISC_ERROR;
                        break;
                } // switch
                pLicenseServiceUpdateLicensePodInfoMessageContext->addManagedObjectForGarbageCollection(pLicensePodLocalManagedObject);
                return (errRet);
            } // if

           /*
            * AssignedPodset: Contains Base/Single/Double/None
            * Operation: Contains the user input Reserve/Release
            */

            /*
             * Reserve Option Handling
             * For any port set Operation as Reserve when ret >= 0
             * Base on the ret value we decide the AssignedPodset Base/Single/Double
             */
            if (strcasecmp("reserve", option) == 0) {
                switch (ret) {
                    case 1:
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_BASE_SET);
                        trace (TRACE_LEVEL_DEBUG, string("Reserve Request - Assigned BASE Podset"));
                        break;
                    case 2:
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_SINGLE_SET);
                        trace (TRACE_LEVEL_DEBUG, string("Reserve Request - Assigned SINGLE Podset"));
                        break;
                    case 3:
                        pLicensePodLocalManagedObject->setAssignedPodset(DPOD_DOUBLE_SET);
                        trace (TRACE_LEVEL_DEBUG, string("Reserve Request - Assigned DOUBLE Podset"));
                        break;
                    case 4:
                        trace (TRACE_LEVEL_DEBUG, string("Reserve Request - Already assigned"));
                        errRet = WAVE_MESSAGE_SUCCESS;
                        break;
                    default:
                        trace (TRACE_LEVEL_ERROR, string("Reserve Request - Failed"));
                        errRet = LICENSE_DPOD_RESERVE_PORT_ERROR;    
                        break;
                }

                if (errRet == LICENSE_DPOD_RESERVE_PORT_ERROR) {
                    pLicenseServiceUpdateLicensePodInfoMessageContext->addManagedObjectForGarbageCollection(pLicensePodLocalManagedObject);
                    return(errRet);
                } else {
                    pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    updateWaveManagedObject (pLicensePodLocalManagedObject);
                    trace (TRACE_LEVEL_DEBUG, string("Reserve Request - Success"));
                }
            } else {
                /*
                 * Release Operation Handling
                 * For any port set Operation as Release and AssignedPodset as None when ret >= 0
                 */

                //Updating the DB Here
                pLicensePodLocalManagedObject->setOperation(DPOD_RELEASE);
                pLicensePodLocalManagedObject->setAssignedPodset(DPOD_UNASSIGNED);

                updateWaveManagedObject (pLicensePodLocalManagedObject);
                trace (TRACE_LEVEL_DEBUG, string("Release Request - Success"));
            }
            pLicenseServiceUpdateLicensePodInfoMessageContext->addManagedObjectForGarbageCollection(pLicensePodLocalManagedObject);
        } else {
            trace (TRACE_LEVEL_ERROR, string ("LicenseServiceLocalObjectManager::LicensePodUpdateEntries - No Entry Found for Port: ") + pLicenseServiceUpdateLicensePodInfoMessage->getPortId());
            return (LICENSE_DPOD_NO_ENTRY_DB_ERROR);
        }
        return WAVE_MESSAGE_SUCCESS;
}

    /*
    ** This function is the FSS stage handler for License.
    ** Inputs:      Stage number (8. LICENSE_CONFIG_STAGE) and service Type (FCSW)
    ** Functions:   Reads MOs from database and populates the fabsys kernel.
    */

    void  LicenseServiceLocalObjectManager::externalStateSynchronization (
          WaveAsynchronousContextForExternalStateSynchronization
          *pWaveAsynchronousContextForExternalStateSynchronization) 
    {
        ResourceId   status = WAVE_MESSAGE_SUCCESS;
        size_t       single_set_size = 0;
        size_t       double_set_size = 0;
        size_t       sys_total_dpod_ports = 0;
        size_t       sys_base_dpod_ports = 0;
        size_t       sys_total_40G_dpod_ports = 0;
        size_t       sys_base_40G_dpod_ports = 0;
        size_t       first_40G_port_num = 0;
        size_t       last_40G_port_num = 0;
        int          error;
        unsigned int i;
        unsigned int port_num;
        char         portFmt[10];
        string       portStr = "";
        pod_t        pod;
        int          swModel = 0;
        int          podSupported = TRUE;
        int          vcsEnabled = FALSE;
        UI32         mapped_id = 0;
        string       queryDefaultPort = "";
        int          epod10GLicensed = 0;
        int          epod40GLicensed = 0;
        size_t       epod10GCount = 0;
        size_t       epod40GCount = 0;
        int          epodPlatform = FALSE;  

        /*
         * Call to update the Fabsys kernel info from the DB
         */
        trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::externalStateSynchronization: Entering External State Sync"));
        if ((pWaveAsynchronousContextForExternalStateSynchronization->getFssStageNumber() == 8)
             && (pWaveAsynchronousContextForExternalStateSynchronization->getServiceType() == FCSW)) { 

            swModel = get_pod_SWBD();
            trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::ESS - swModel = ") + swModel);
            switch (swModel) {
                case SWBD_ELARA2E:
                case SWBD_ELARA2F:
                    /* Callisto-24 */
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - Platform CALLISTO-24");
                    single_set_size = ELARA2_POD1_LICENSE;
                    double_set_size = ELARA2_POD2_LICENSE;
                    sys_total_dpod_ports = ELARA2_DPOD_PORTS;
                    sys_base_dpod_ports = ELARA2_BASE_DPOD_PORTS;
#if 0
                    /* REMOVE ME: for proto/test purposes, force epodPlatform TRUE on this platform... */
                    /* hack for testing... divide 24 ports into 12 + 8 10G and 4 40G - remove me */
                    sys_total_dpod_ports = 20;
                    sys_base_dpod_ports = 12;
                    sys_base_40G_dpod_ports = 0;
                    sys_total_40G_dpod_ports = 4;
                    first_40G_port_num = sys_total_dpod_ports + 1;
                    last_40G_port_num = sys_total_dpod_ports + sys_total_40G_dpod_ports;
                    epodPlatform = TRUE;
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - forced EPOD Platform");
#endif
                    break;
                case SWBD_CALLISTOE:
                case SWBD_CALLISTOF:
                    /* Callisto-60 */
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - Platform CALLISTO-60");
                    single_set_size = CALLISTO_POD1_LICENSE;
                    double_set_size = CALLISTO_POD2_LICENSE;
                    sys_total_dpod_ports = CALLISTO_DPOD_PORTS;
                    sys_base_dpod_ports = CALLISTO_BASE_DPOD_PORTS;
                    break;
                case SWBD_CASTOR:
                case SWBD_CASTORT:
                    /* Castor platforms */
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - Platform CASTOR");
                    sys_total_dpod_ports = CASTOR_10G_DPOD_PORTS;
                    sys_base_dpod_ports = CASTOR_BASE_10G_DPOD_PORTS;
                    sys_base_40G_dpod_ports = CASTOR_BASE_40G_DPOD_PORTS;
                    sys_total_40G_dpod_ports = CASTOR_40G_DPOD_PORTS;
                    first_40G_port_num = sys_total_dpod_ports + 1;
                    last_40G_port_num = sys_total_dpod_ports + sys_total_40G_dpod_ports;
                    epodPlatform = TRUE;
                    break;
                default:
                    /* all others that don't support DPOD... leave pod set sizes as zero */
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - non-DPOD-supporting Platform");
                    break;
            }

            if (epodPlatform) {
                epod10GLicensed = licenseCheck(PORT_10G_UPGRADE_LICENSE);
                if (epod10GLicensed) {
                    epod10GCount = licenseGetCapacity(PORT_10G_UPGRADE_LICENSE);
                    if (epod10GCount < 0) {
                        epod10GCount = 0;
                    }
                }
                epod40GLicensed = licenseCheck(PORT_40G_UPGRADE_LICENSE);
                if (epod40GLicensed) {
                    epod40GCount = licenseGetCapacity(PORT_40G_UPGRADE_LICENSE);
                    if (epod40GCount < 0) {
                        epod40GCount = 0;
                    }
                }
                if (sys_base_dpod_ports + epod10GCount > sys_total_dpod_ports) {
                    /* don't exceed what's allowed */
                    epod10GCount = sys_total_dpod_ports - sys_base_dpod_ports;
                }
                if (sys_base_40G_dpod_ports + epod40GCount > sys_total_40G_dpod_ports) {
                    /* don't exceed what's allowed */
                    epod40GCount = sys_total_40G_dpod_ports - sys_base_40G_dpod_ports;
                }
                single_set_size = epod10GCount;
                double_set_size = 0; /* force for proto only */

                string       podStr = "";
                char         podFmt[10];
                snprintf(podFmt, 10, "%d", (int) epod10GCount);
                podStr += podFmt;
                
                trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::ESS - additional 10G POD=") + podStr);
                trace (TRACE_LEVEL_DEBUG, string("LicenseServiceLocalObjectManager::ESS - additional QSFP POD=") + epod40GCount);
            }

            /* Filling Pod_t structure to update Fabsys */
            if ((single_set_size == 0) && (double_set_size == 0) && !epodPlatform) {
                podSupported = FALSE;
            }

            pod.is_pod_supported = podSupported? 1: 0;
            pod.is_epod_supported = epodPlatform;
            pod.pod_total_count = sys_total_dpod_ports;
            pod.pod_base_set_size = sys_base_dpod_ports;
            pod.pod_single_set_size = single_set_size; /* overload with upgrade count */
            pod.pod_double_set_size = double_set_size;
            pod.pod_40G_total_count = sys_total_40G_dpod_ports;
            pod.pod_40G_base_set_size = sys_base_40G_dpod_ports;
            pod.pod_40G_single_set_size = epod40GCount;
            pod.pod_40G_first_port_num = first_40G_port_num;
            pod.pod_40G_last_port_num = last_40G_port_num;
            if (epodPlatform) {
               pod.pod_license.port_10G_upgradelicense = epod10GLicensed;
               pod.pod_license.port_40G_upgradelicense = epod40GLicensed;
               /* use a single assignment array for both 10G and 40G ports...*/
               sys_total_dpod_ports = sys_total_dpod_ports + sys_total_40G_dpod_ports;
            } else {
               pod.pod_license.pod1_license = licenseCheck(POD1_LICENSE)? 1: 0;
               pod.pod_license.pod2_license = licenseCheck(POD2_LICENSE)? 1: 0;
            }
            for (i = 0; i <= sys_total_dpod_ports; i++) {
                pod.pod_assignment.bytes[i] = FABSYS_POD_PORT_UNASSIGNED;
            }
            /* Mark all ports up to baseoffset as unusable */
            for (i = 0; i < PORT_START_NUM; ++i) {
                pod.pod_assignment.bytes[i] = FABSYS_POD_PORT_DISCONNECTED;
            }
            for (i = sys_total_dpod_ports + PORT_START_NUM; i <= FABSYS_MAX_POD_PORT; i++) {
                pod.pod_assignment.bytes[i] = FABSYS_POD_PORT_DISCONNECTED;
            }
             
            if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
                /* VCS is enabled. */
                vcsEnabled = TRUE;
                mapped_id = DcmToolKit::getLocalMappedId();
                snprintf(portFmt, 10, "%d/0/1", mapped_id);
                queryDefaultPort += portFmt;
            } else {
                queryDefaultPort += POD_QUERY_DEFAULT_PORT;
            }

            if (podSupported) {
                LicensePodLocalManagedObject *pLicensePodLocalManagedObject =
                                         QueryLicensePodLocalManagedObject(queryDefaultPort);

                if (pLicensePodLocalManagedObject) {
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - MO Exists");
                    delete pLicensePodLocalManagedObject;
                    /* Read the POD data from dcm and populate pod_t structure for fabsys */
                    for (port_num = 1; port_num <= sys_total_dpod_ports; port_num++) {
                        // Form the Rbridge-id/Slot/Port or Slot/Port Format to query all the ports in the DB
                        if (vcsEnabled) {
                            snprintf(portFmt, 10, "%d/0/%d", mapped_id, port_num);
                        } else {
                            snprintf(portFmt, 10, "0/%d", port_num);
                        }
                        portStr += portFmt;
                        trace (TRACE_LEVEL_DEBUG, string("portStr ") + portStr);
                        LicensePodLocalManagedObject *pLicensePodQueryLocalManagedObject =
                                      QueryLicensePodLocalManagedObject(portStr);
                        if (pLicensePodQueryLocalManagedObject) {
                           //Read the PodSet from the DB  
                           string podsetdb = pLicensePodQueryLocalManagedObject->getAssignedPodset();
                           trace (TRACE_LEVEL_DEBUG, port_num + string(".podsetdb : ")+ podsetdb);
                           if (strcmp("Base", (char *)podsetdb.data()) == 0) {
                              pod.pod_assignment.bytes[port_num] = FABSYS_POD_PORT_IN_BASE;
                           } else if (strcmp("Single", (char *)podsetdb.data()) == 0) {
                              pod.pod_assignment.bytes[port_num] = FABSYS_POD_PORT_IN_SINGLE;
                           } else if (strcmp("Double", (char *)podsetdb.data()) == 0) {
                              pod.pod_assignment.bytes[port_num] = FABSYS_POD_PORT_IN_DOUBLE;
                           }
                           delete pLicensePodQueryLocalManagedObject;
                        } else { //If no entry for the port in the DB
                            trace (TRACE_LEVEL_ERROR, string ("LicenseServiceLocalObjectManager::ESS - No Entry Found for Port: ") + portStr);
                            prismAssert (false, __FILE__, __LINE__);
                        }
                        snprintf(portFmt, 10, "%s", "");
                        portStr ="";
                    } //For loop end

                } else {
                    trace (TRACE_LEVEL_ERROR, "LicenseServiceLocalObjectManager::ESS - MO Doesnt Exist");
                    status = WAVE_MESSAGE_ERROR;
                }

               /* push down POD data only if DPOD is supported */
                error = set_pod_data(&pod);
                if (error < 0) {
                    trace (TRACE_LEVEL_ERROR, string("LicenseServiceLocalObjectManager::ESS - Unable to Update Fabsys Error: ") + error);
                    status = WAVE_MESSAGE_ERROR;
                } else {
                    trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - Successful Update to Fabsys");
                }

            } else {
                trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::ESS - No Operation");
            }
        }
        trace (TRACE_LEVEL_DEBUG, "LicenseServiceLocalObjectManager::externalStateSynchronization - Exiting ESS");
        pWaveAsynchronousContextForExternalStateSynchronization->setCompletionStatus (status);
        pWaveAsynchronousContextForExternalStateSynchronization->callback ();
    }

/*      
 *  DPOD WyserEaConfiguration Implementation Functions End Here
 */

    void  LicenseServiceLocalObjectManager::LicenseServiceAddMessageHandler( LicenseServiceAddMessage *pLicenseServiceAddMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseAddEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceAddMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  LicenseServiceLocalObjectManager::LicenseServiceRemoveMessageHandler( LicenseServiceRemoveMessage *pLicenseServiceRemoveMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseRemoveEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceRemoveMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  LicenseServiceLocalObjectManager::LicenseServiceShowMessageHandler( LicenseServiceShowMessage *pLicenseServiceShowMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseShowEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
    void  LicenseServiceLocalObjectManager::LicenseServicePodShowMessageHandler( LicenseServicePodShowMessage *pLicenseServicePodShowMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicensePodShowEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServicePodShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  LicenseServiceLocalObjectManager::LicenseServiceGetLicenseIdShowMessageHandler( LicenseServiceGetLicenseIdShowMessage *pLicenseServiceGetLicenseIdShowMessage)
    {
// Validations and Queries should go here
 
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseIdShowGetEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };
 
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceGetLicenseIdShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
 
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  LicenseServiceLocalObjectManager::LicenseServicePodMessageHandler( LicenseServicePodMessage *pLicenseServicePodMessage)
    {
// Validations and Queries should go here
        
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {   
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicensePodEntries),                
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };
                PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServicePodMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  LicenseServiceLocalObjectManager::LicenseServiceUpdateLicenseFabsysInfoMessageHandler( LicenseServiceUpdateLicenseFabsysInfoMessage *pLicenseServiceUpdateLicenseFabsysInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseFabsysInfoUpdateEntries),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceUpdateLicenseFabsysInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    /*
     * this method overrides the virtual function defined in the WaveObjectManager:: base class.  Is
     * invoked during bringup and is used to maintain parity between the master data managed by the switch
     * driver and that stored in the cluster-wide database.  Upon invokotion, this method interrogates
     * the switch driver for the configuration of each interface and updates the DCMd database accordingly
     */
    void LicenseServiceLocalObjectManager::backendSyncUp(PrismAsynchronousContext *pPrismAsynchronousContext)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =        {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::backendSyncUpLicense),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };
        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
                new PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, this, sequencerSteps,
                        sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId LicenseServiceLocalObjectManager::backendSyncUpLicense(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::backendSyncUpLicense entered "));
        int rc, port, local_id;
        char port_ass;
        /* For Port Formatting */
        char convertToString[40];
        pod_t   pod_info;
        pod_t   *pinfo_p = &pod_info;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        /* Get the Snapshot of Fabsys Kernel */
        if ((rc = get_pod_data(pinfo_p)) != 0) { 
            trace (TRACE_LEVEL_ERROR, string ("LicenseServiceLocalObjectManager::backendSyncUpLicense get_pod_data failed "));
            return (WAVE_MESSAGE_ERROR); 
        }
        for (port = 1; port <= MAX_DPOD_PORTS; port++) {
            if (DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) {
                local_id = DcmToolKit::getLocalMappedId();
                sprintf(convertToString, "%d/%d/%d", local_id, 0, port);
            } else {
                sprintf(convertToString,"0/%d", port);
            }
            string id2(convertToString);
            LicensePodLocalManagedObject *pLicensePodLocalManagedObject = QueryLicensePodLocalManagedObject(id2);

            // fetch the entry from the DCMd database
            if (pLicensePodLocalManagedObject) {
                port_ass = pinfo_p->pod_assignment.bytes[port];
                if (port_ass == FABSYS_POD_PORT_IN_BASE) {
                    trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::BASE_SET backend port: ") +
                                port + string ("id: ") + id2);
                    pLicensePodLocalManagedObject->setAssignedPodset(DPOD_BASE_SET);
                    pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    updateWaveManagedObject (pLicensePodLocalManagedObject);
                } else if (port_ass == FABSYS_POD_PORT_IN_SINGLE) {
                    trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager:: SINGLE_SET backend port: ")
                                + port + string ("id: ") + id2);
                    pLicensePodLocalManagedObject->setAssignedPodset(DPOD_SINGLE_SET);
                    pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    updateWaveManagedObject (pLicensePodLocalManagedObject);
                } else if (port_ass == FABSYS_POD_PORT_IN_DOUBLE) {
                    trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::DOUBLE_SET backend port: ") +                              port + string ("id: ") + id2);
                    pLicensePodLocalManagedObject->setAssignedPodset(DPOD_DOUBLE_SET);
                    pLicensePodLocalManagedObject->setOperation(DPOD_RESERVE);
                    updateWaveManagedObject (pLicensePodLocalManagedObject);
                } else {
                    pLicensePodLocalManagedObject->setAssignedPodset(DPOD_UNASSIGNED);
                    pLicensePodLocalManagedObject->setOperation(DPOD_RELEASE);
                    updateWaveManagedObject (pLicensePodLocalManagedObject);
                }
                pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pLicensePodLocalManagedObject);
            }
        }
        trace (TRACE_LEVEL_DEBUG, string ("LicenseServiceLocalObjectManager::backendsyncup exit: "));
        return(status);
    }

    void  LicenseServiceLocalObjectManager::LicenseServiceUpdateLicensePodInfoMessageHandler( LicenseServiceUpdateLicensePodInfoMessage *pLicenseServiceUpdateLicensePodInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicensePodUpdateEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceUpdateLicensePodInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

void  LicenseServiceLocalObjectManager::LicenseServiceFileAddMessageHandler( LicenseServiceFileAddMessage *pLicenseServiceFileAddMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::LicenseFileAddEntries),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&LicenseServiceLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pLicenseServiceFileAddMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
}

