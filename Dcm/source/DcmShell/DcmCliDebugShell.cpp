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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmShell/DcmCliDebugShell.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmShell/VcsBaseServicesCliDebugShell.h"

#include "ValClientInterface/ValClientPhysicalInterface.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ValClientInterface/val_interface.h"
#include "Ssm/Local/SsmTypes.h"
#include "VcsFabric/Local/VcsNodeLocalClientInterface.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include <stdlib.h>
#include <string.h>

extern "C"
{
#include "sys/fabos/bitmap.h"
}

namespace DcmNs
{

DcmCliDebugShell::DcmCliDebugShell (ValClientSynchronousConnection &connection)
    : WaveCliShell                     ("dcmdebug", connection),
      m_valClientSynchronousConnection (connection)
{
    addCommandfunction ("setNewShowVcs", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setNewShowVcs), "set to new ShowVcs Output", NULL);
    addCommandfunction ("printConfigurationNumber", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::printConfigurationNumber), "print Configuration Number", NULL);
    addCommandfunction ("printConfigurationTime", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::printConfigurationTime), "print Configuration Time", NULL);
    addCommandfunction ("debugObjectLeak", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugObjectLeak), "dump Managed Object Leaks", NULL);
    addCommandfunction ("debugObjectLeaks", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugObjectLeak), "dump Managed Object Leaks", NULL);
    addCommandfunction ("debugMessageLeaks", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugMessageLeak), "dump Message Leaks", NULL);
    addCommandfunction ("debugMessageLeak", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugMessageLeak), "dump Message Leaks", NULL);
    addCommandfunction ("changeBtTrackerState", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::changeBtTrackerState), "change Bt Tracking State", NULL);
    addCommandfunction ("install512DemoObjects", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::installDemoObjects), "Install 512 Demo Objects", NULL);
    addCommandfunction ("testdcmdebugprintf", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugPrintInterface), "Test function for dcm_debug_printf funtion.", NULL); 
    addCommandfunction ("queryinterface",     reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryForInterface),      "Queries DCMd for the given Interface.",       reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::queryForInterfaceHelp));
    addCommandfunction ("queryinterfacecway", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryForInterfaceCWay),  "Queries DCMd for the given Interface.",       reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::queryForInterfaceHelp));
    addCommandfunction ("createinterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createPhysicalInterface),"Craetes an Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createPhysicalInterfaceHelp));
    addCommandfunction ("associnterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::AssocPolicyToPhysicalInterface),"Craetes an Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::AssocPolicyToPhysicalInterfaceHelp));
    addCommandfunction ("assocLoginterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::AssocPolicyToLogicalInterface),"Craetes an Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::AssocPolicyToLogicalInterfaceHelp));
    addCommandfunction ("l2aclpolicy",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::L2ACLPolicy),"Craetes an Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::L2ACLPolicyHelp));
    addCommandfunction ("l2ruletopolicy",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::L2AssocRuleToPolicy),"Craetes an Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::L2AssocRuleToPolicyHelp));
    addCommandfunction ("l2aclstresstest",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::l2AclStressTest),"Associates 2K rules to 1 standard MAC ACL", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::L2AssocRuleToPolicyHelp));

    addCommandfunction ("createvlaninterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createVlanInterface),"Craetes an Vlan Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createVlanInterfaceHelp));
    addCommandfunction ("stresstestforcreatevlaninterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createVlanInterfaceStressTest),"Craetes an Vlan Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createVlanInterfaceHelp));
    addCommandfunction ("deletevlaninterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::deleteVlanInterface),"Craetes an Vlan Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createVlanInterfaceHelp));
    addCommandfunction ("setvlandesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setVlanDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delvlandesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delVlanDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
#ifdef HAVE_PVLAN
    addCommandfunction ("setpvlantype", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPvlanType),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpvlantype", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPvlanType),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setsecondaryvlanid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setSecondaryVlanId),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delsecondaryvlanid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delSecondaryVlanId),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
#endif
    addCommandfunction ("setvlanprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setVlanProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));
    addCommandfunction ("delvlanprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delVlanProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));


    addCommandfunction ("createphyinterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createPhyInterface),"Craetes an Phy Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createPoInterfaceHelp));
    addCommandfunction ("deletephyinterface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::deletePhyInterface),"Craetes an Vlan Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createVlanInterfaceHelp));
     addCommandfunction ("setphysw",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPhySw),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delphysw",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPhySw),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));

    addCommandfunction ("setphyswmode",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPhySwMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delphyswmode",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPhySwMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));

    addCommandfunction ("setphydesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPhyDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delphydesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPhyDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setphyprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPhyProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));
    addCommandfunction ("delphyprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPhyProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));


    addCommandfunction ("createpointerface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createPoInterface),"Craetes an Po Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createPoInterfaceHelp));
    addCommandfunction ("deletepointerface",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::deletePoInterface),"Craetes an Vlan Interface in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::createVlanInterfaceHelp));
    addCommandfunction ("setpodesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpodesc",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoDesc),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setposw",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoSw),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delposw",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoSw),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setposwmode",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoSwMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delposwmode",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoSwMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
#ifdef HAVE_PVLAN
    addCommandfunction ("setpopvlanmode",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoPvlanMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpopvlanmode",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoPvlanMode),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setpopvlanmapping",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoPvlanMapping),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpopvlanmapping",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoPvlanMapping),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setpopvlanhostassociation",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoPvlanHostAssociation),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpopvlanhostassociation",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoPvlanHostAssociation),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setpopvlantrunkassociation",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoPvlanTrunkAssociation),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("delpopvlantrunkassociation",reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoPvlanTrunkAssociation),"Update vlan desc in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanDescHelp));
    addCommandfunction ("setpoprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setPoProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));
    addCommandfunction ("delpoprotocalstatus",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::delPoProtocalStatus),"Update vlan protocal status in DCMd based on Input", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanProtocalStatusHelp));
#endif
    addCommandfunction ("mstp",    reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setBridgeXstp),"Configure Bridge Xstp", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setBridgeXstpHelp));
    addCommandfunction ("vbridge", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setVlanBridgeXstp),"Configure VlanBridge Xstp", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setVlanBridgeXstpHelp));
    addCommandfunction ("isdcmdready", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsDcmdReady),  "Queries if Dcmd is ready to accept connections", NULL);
    addCommandfunction ("isdcmdreadyforhasmlaunch", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsDcmdReadyForHasmLaunch),  "Queries if Dcmd is ready to launch HASM", NULL);
    addCommandfunction ("sethasmlaunched", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setHasmLaunched),  "Sends notification to DCMD that HASM has already been launched", NULL);
    addCommandfunction ("getclustertype", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::getThisVcsNodeClusterType),  "Gets the cluster mode on the current node", NULL);
    addCommandfunction ("fileservice", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::configureFileService),  "Enable/Disable File Service on this node", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::fileServiceHelp));
    addCommandfunction ("vcsnodegetrbridgeidfromwwn", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::vcsNodeGetRBridgeIdFromWWN), "Queries for corresponding switch WWN for given rbridge-id", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::vcsNodeGetRBridgeIdFromWWNHelp));
    addCommandfunction ("vcsnodegetwwnfromrbridgeid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::vcsNodeGetWWNFromRBridgeId), "Queries for corresponding rbridge-id for given switch WWN", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::vcsNodeGetWWNFromRBridgeIdHelp));
    addCommandfunction ("isnodeincluster", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsNodeInCluster),  "Check if the node with given rbridge-id is part of a cluster", NULL);
    addCommandfunction ("islocalnodeincluster", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsLocalNodeInCluster),  "Check if the local node is in a cluster", NULL);
    addCommandfunction ("getvcsid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::getVcsIdForNode),  "Gives the VCSID for a local node.", NULL);
    addCommandfunction ("getrbridgeid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::getVcsRBridgeIdForNode),  "Gives rbridge-id for a local node.", NULL);
    addCommandfunction ("vcsnodegetipfromrbridgeid", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::vcsNodeGetIPFromRBridgeId), "Get IP address for a given rbridge-id", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::vcsNodeGetIPFromRBridgeIdHelp));
    addCommandfunction ("isclusterconnectedtoip", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::isClusterConnectedtoIpAddress), "Check if the IP address is reacheable from cluster", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::isClusterConnectedtoIpAddressHelp));
    addCommandfunction ("vcsgetclusterrbridgeidfromwwn", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::vcsGetClusterRbridgeIdFromWWN), "Gets the rbridge-id available in the cluster", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::vcsGetClusterRbridgeIdFromWWNHelp));

    addCommandfunction ("vcsisfabricdistributionenabled", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::vcsIsFabricDistributionEnabled), "IsFabricServiceEnabled", NULL);
    addCommandfunction ("disconnectclient", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::disconnectInstanceClients),  "Disconnect clients associate with input IPAdress", NULL);

    // For now, LC query is happening through IPC (VcsNodeFabosInterfaceObjectManager). So, commenting this out
    //addCommandfunction ("querylinecardtype", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryLinecardType),  "query linecard type", NULL);

    addCommandfunction ("setFcfMacAddress", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setFcfMacAddress),  "set FCF MAC Address", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setFcfMacAddressHelp));
    
    addCommandfunction ("notifysystemshutdown", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::notifySystemShutdown),  "notify system shutdown", NULL);
    addCommandfunction ("cliallowedfromthislocation", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::cliAllowedFromThisLocation), "Enables or disables issuing cli from secondary nodes", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::cliAllowedFromThisLocationHelp));

    addCommandfunction ("debugClusterStatistics", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::debugClusterStatistics), "display cluster statistics", NULL);
    addCommandfunction ("setWyserTty", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setTtyName),  "Set the Wyser Tty", NULL);
    addCommandfunction ("getVcenterStateMachineStatus", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::getVcenterStateMachineStatus),  "Get Status of State Machines", NULL);
    addCommandfunction ("setModeChangeParameters", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::setModeChangeParameters),  "Set vcsid, vcs mode and rbridgeId on a node", reinterpret_cast<WaveShellCommandHelpFunction> (&DcmCliDebugShell::setModeChangeParametersHelp));
    addCommandfunction ("isNosSmdReady", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::getIsNosSmdReady),  "Queries if Nos Smd is ready to accept connections", NULL);
    addCommandfunction ("isgvlanenabled", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsGvlanEnabled),  "Queries if GVLAN is enabled on the switch", NULL);
    addCommandfunction ("isgvlancapable", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::queryIsGvlanCapable),  "Queries if switch is capable of configuring GVLAN", NULL);

    addCommandfunction ("loadZoneBulkConfiguration", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::loadZoneBulkConfiguration),  "Load Zone Bulk Configuration", NULL);

    // OpenFlow

    addCommandfunction ("createOpenFlowSwitch",                             reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createOpenFlowSwitch),                             "Creates Open Flow Switch with a given name.",         NULL);
    addCommandfunction ("createOpenFlowLogicalSwitch",                      reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::createOpenFlowLogicalSwitch),                      "Creates Open Flow Logical Switch with a given name.", NULL);
    addCommandfunction ("addFlow",                                          reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::addFlow),                                          "Adds the given flow in FTM.", NULL);
    addCommandfunction ("addOpenFlowControllerToOpenFlowLogicalSwitch",     reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::addOpenFlowControllerToOpenFlowLogicalSwitch),     "Creates Open Flow Logical Switch with a given name.", NULL);
    addCommandfunction ("connectOpenFlowLogicalSwitchToOpenFlowController", reinterpret_cast<WaveShellCommandFunction> (&DcmCliDebugShell::connectOpenFlowLogicalSwitchToOpenFlowController), "Connects Open Flow Logical Switch to a given (already known) Open Flow Controller.", NULL);
    
    // Subshell for VcsBaseServices debug shell under DcmCliDebugShell
    VcsBaseServicesCliDebugShell *pVcsBaseServicesCliDebugShell = new VcsBaseServicesCliDebugShell (connection);
    prismAssert (NULL != pVcsBaseServicesCliDebugShell, __FILE__, __LINE__);
    addSubShell (pVcsBaseServicesCliDebugShell);
}

DcmCliDebugShell::~DcmCliDebugShell ()
{
}

ValClientSynchronousConnection &DcmCliDebugShell::getValClientSynchronousConnection ()
{
    return (m_valClientSynchronousConnection);
}

void DcmCliDebugShell::briefHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug shell provides a way to execute debug actions.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Developer can execute pre-registered actions from this shell.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Debug actions that are not particularly part of any other shells");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        typically land here.");
}

ResourceId DcmCliDebugShell::queryIsGvlanEnabled (const vector<string> &argument)
{
    ResourceId  status;
    bool gvlanEnabled = false;

    if (0 != argument.size ())
    {
        trace (TRACE_LEVEL_ERROR, "No Arguments Required , USAGE : isglvanenabled");
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    
    status = connection.isGvlanEnabled (gvlanEnabled);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == gvlanEnabled)
        {
            trace (TRACE_LEVEL_INFO, "Gvlan is Enabled");
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Gvlan is not Enabled");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Get Gvlan State : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::queryIsGvlanCapable (const vector<string> &argument)
{
    ResourceId  status;
    bool gvlanCapable = false;

    if (0 != argument.size ())
    {
        trace (TRACE_LEVEL_ERROR, "No Arguments Required , USAGE : isglvancapable");
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    
    status = connection.isGvlanCapable (gvlanCapable);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == gvlanCapable)
        {
            trace (TRACE_LEVEL_INFO, "Switch/Fabric is Gvlan capable");
        }
        else
        {
            trace (TRACE_LEVEL_INFO, "Switch/Fabric is not Gvlan capable");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Get Gvlan Capable State : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::printConfigurationNumber (const vector<string> &argument)
{
    ResourceId  status;
    UI16        configurationNumber[16];

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    
    status = connection.getConfigurationNumber (configurationNumber);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "Configuration Number is ");

        for (UI32 i = 0; i < 4; i++)
        {
            tracePrintf (TRACE_LEVEL_INFO, "%x%x%x%x", configurationNumber[(i * 4)], configurationNumber[(i * 4) + 1], configurationNumber[(i * 4) + 2], configurationNumber[(i * 4) + 3]);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Get Configuration Number : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::setNewShowVcs (const vector<string> &argument)
{

    ResourceId  status;
    bool newShowVcs = false;

    if (1 != argument.size ())
    {   
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setNewShowVcs <true/false>");
        return (WAVE_MESSAGE_ERROR);
    }
    else
    {
        if ( !( argument[0].compare("true")) )
        {
            newShowVcs = true;
        }
        else if ( !(argument[0].compare("false")) )
        {
            newShowVcs = false;
        }    
        else
        {
            tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setNewShowVcs <true/false>");
            return (WAVE_MESSAGE_ERROR);
        }    
    }
        

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.setNewShowVcs (newShowVcs);

    if (WAVE_MESSAGE_SUCCESS == status)
    {   
        trace (TRACE_LEVEL_INFO, "DcmCliDebugShell::setNewShowVcs : setting NewShowVcs succeeded");
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setNewShowVcs : Failed to Set NewShowVcs  " + FrameworkToolKit::localize (status));
    }

    return (status);    

}


ResourceId DcmCliDebugShell::printConfigurationTime (const vector<string> &argument)
{
    ResourceId  status;
    char        configurationTime[256] = {'\0'};

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.getConfigurationTime (configurationTime, 256);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "Configuration Time is ");

        tracePrintf (TRACE_LEVEL_INFO, "%s", configurationTime);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Failed to Get Configuration Number : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::debugObjectLeak (const vector<string> &argument)
{
    ResourceId status;

    if (0 == argument.size ())
    {
        trace (TRACE_LEVEL_ERROR, "Please Specify any Prism Service Id or all option");
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.dumpObjectLeaks (argument);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to dump object leaks");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to dump object leaks : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::debugMessageLeak (const vector<string> &argument)
{
    ResourceId  status;

    if (0 == argument.size ())
    {
        trace (TRACE_LEVEL_ERROR, "Please specify a Prism Service Id or all option");
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.messageLeaksStats (argument);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to dump message leaks");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to dump message leaks : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::installDemoObjects (const vector<string> &argument)
{
    ResourceId  status;

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.installDemoObject ();

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Install 512 Demo Objects");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Install Demo Objects Status : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::debugPrintInterface (const vector<string> &argument)
{
    ResourceId  status = 0;

    UI32 noOfArgument = argument.size ();
   
    UI32 i,j;

    UI32 noOfMessages = 100;

    string tempString;
    string space=" ";

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    ValClientSynchronousConnection::initializeCApi (&connection); 

    for (i = 0; i < noOfArgument; i++)
    {
        tempString.append (argument[i]);

        if (i < noOfArgument - 2)
        {
            tempString.append (space);
        }
    }

    for (j = 0; j < noOfMessages; j++)
    {
        status = dcm_debug_printf("%u:%s\n", j, tempString.c_str ());
    }
    
    return (status);
}


ResourceId DcmCliDebugShell::queryForInterface (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    string                         interfaceName;
    ValClientPhysicalInterface     valClientPhysicalInterface;

    if (1 != arguments.size ())
    {
        queryForInterfaceHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    interfaceName = arguments[0];

    status = connection.valQueryForInterface (interfaceName, valClientPhysicalInterface);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Query for Interface : " + interfaceName);

        trace (TRACE_LEVEL_INFO,    string ("IP Address : ") + valClientPhysicalInterface.getIpAddress ());
        trace (TRACE_LEVEL_INFO,    string ("MTU        : ") + valClientPhysicalInterface.getMtu ());
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::queryForInterfaceCWay (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    string                         interfaceName;
    val_interface                  vinterface;

    if (1 != arguments.size ())
    {
        queryForInterfaceHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    interfaceName = arguments[0];

    ValClientSynchronousConnection::initializeCApi (&connection);

    status = query_interface (interfaceName.c_str (), &vinterface);

    if (0 == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string ("SUCCEEDED to Query for Interface : ") + vinterface.name);

        trace (TRACE_LEVEL_INFO,    string ("IP Address : ")                       + vinterface.ip_address);
        trace (TRACE_LEVEL_INFO,    string ("MTU        : ")                       + vinterface.mtu);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Interface. Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::queryForInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId DcmCliDebugShell::createPhysicalInterface (const vector<string> &arguments)
{
    if (4 != arguments.size ())
    {
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           ifIndex     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           mtu         = strtoul (arguments[2].c_str (), NULL, 0);
    const string                         description = arguments[3];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.createPhysicalInterface (name, ifIndex, mtu, description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Physical Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Physical Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::createPhysicalInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}
ResourceId DcmCliDebugShell::AssocPolicyToLogicalInterface (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[1];
    const string                           ifName     = arguments[0];
    const UI32                           opcode = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.AssocPolicyToLogInterface(ifName, name, opcode);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Assoc Physical Interface : " +  name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Assoc policy to Physical Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::AssocPolicyToLogicalInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : AssocPolicyToLogicalInterface");
}
ResourceId DcmCliDebugShell::L2ACLPolicy (const vector<string> &arguments)
{
    const string policyName        = arguments[0];
    const UI32 policyType = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32   opcode = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.macAclPolicy(policyName, policyType, opcode);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED mac acl policy creation/deletion: " +  policyName);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to create/delete mac acl policy : " + policyName + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::L2ACLPolicyHelp ()
{
}

ResourceId DcmCliDebugShell::L2AssocRuleToPolicy (const vector<string> &arguments)
{
    const string policyName        = arguments[0];
    const UI32 policyType = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32 ruleid = strtoul (arguments[2].c_str (), NULL, 0);
    const UI32 action = strtoul (arguments[3].c_str (), NULL, 0);
    const string srcMac        = arguments[4];
    const bool srcMask = strtoul (arguments[5].c_str (), NULL, 0);
    const string dstMac        = arguments[6];
    const bool dstMask = strtoul (arguments[7].c_str (), NULL, 0);
    const string appType = arguments[8];
    const bool isCount = strtoul (arguments[9].c_str (), NULL, 0);
    const UI32   opcode = strtoul (arguments[10].c_str (), NULL, 0);
    AddrType srcType = ADDR_TYPE_NONE;
    AddrType dstType = ADDR_TYPE_NONE;

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

          if (srcMac.compare("any") == 0) {
          		srcType = ADDR_TYPE_ANY;
		  } else if (srcMask) {
		  		srcType = ADDR_TYPE_HOST;
		  }

          if (dstMac.compare("any") == 0) {
          		dstType = ADDR_TYPE_ANY;
		  } else if (dstMask) {
		  		dstType = ADDR_TYPE_HOST;
		  }


    status = connection.AssocRuleToPolicy(policyName, policyType, ruleid, action, srcMac, srcType, srcMask, dstMac, dstType, dstMask, appType, isCount, opcode);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Asso Rule to policy : " +  policyName);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Assoc rule to policy : " + policyName + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::L2AssocRuleToPolicyHelp ()
{
}

ResourceId DcmCliDebugShell::l2AclStressTest (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    // acl1 2 1 1 1111.1111.$tempy$tempx 1111.1111.$tempy$tempx 1111.1111.$tempy$tempx 1111.1111.$tempy$tempx 1 1 8

    const string policyName        = arguments[0];
    const UI32 policyType = 2;
    UI32 ruleid = 0;
    const UI32 action = 1;
    string srcMac = "1111.1111.";
    const bool srcMask = true;
    string dstMac;
    const bool dstMask = true;
    const string appType = "1";
    const bool isCount = true;
    const UI32   opcode = 8;
    AddrType srcType = ADDR_TYPE_NONE;
    AddrType dstType = ADDR_TYPE_NONE;
    char buffer[16];
    UI32 i, j;

    if (srcMac.compare("any") == 0) {
          srcType = ADDR_TYPE_ANY;
    } else if (srcMask) {
          srcType = ADDR_TYPE_HOST;
    }

    if (dstMac.compare("any") == 0) {
          dstType = ADDR_TYPE_ANY;
    } else if (dstMask) {
          dstType = ADDR_TYPE_HOST;
    }

    for (i = 1; i <= 8; i++)
    {
        for (j = 1; j <=255; j++ )
        {
            ruleid = (i - 1) * 255 + j;

            sprintf (buffer, "%02x%02x", i, j);

            srcMac = string ("1111.1111.") + buffer;

            dstMac = srcMac;

            status = connection.AssocRuleToPolicy (policyName, policyType, ruleid, action, srcMac, srcType, srcMask, dstMac, dstType, dstMask, appType, isCount, opcode);

            if (WAVE_MESSAGE_SUCCESS == status)
            {
                tracePrintf (TRACE_LEVEL_DEBUG, true, true, "SUCCEEDED to Associate Rule %u to policy : %s", ruleid, policyName.c_str ());
            }
            else
            {
                tracePrintf (TRACE_LEVEL_ERROR, true, true, "FAILED to Associate rule %u to policy : %s,  Status : %s", ruleid, policyName.c_str (), (FrameworkToolKit::localize (status)).c_str ());
            }
        }

        printf ("Completed Processing %u rules.\n", ruleid);
    }

    return (status);
}

ResourceId DcmCliDebugShell::AssocPolicyToPhysicalInterface (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[1];
    const string ifName     = arguments[0];
    const UI32                           opcode = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.AssocPolicyToPhyInterface(ifName, name, opcode);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Assoc Physical Interface : " +  name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Assoc policy to Physical Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::AssocPolicyToPhysicalInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

ResourceId DcmCliDebugShell::createPoInterface (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];


    ResourceId                     status;
    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.createPoInterface (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Po Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPoProtocalStatus (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPoProtocalStatus (name,false);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Po Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Po Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}


ResourceId DcmCliDebugShell::delVlanProtocalStatus (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delVlanProtocalStatus (name,false);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Vlan Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Vlan Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPoProtocalStatus (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.setPoProtocalStatus (name,true);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Po Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Po Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setVlanProtocalStatus (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.setVlanProtocalStatus (name,true);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Vlan Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Vlan Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPoDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description;

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPoDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Po Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Po Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delVlanDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description;

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delVlanDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Vlan Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Vlan Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

#ifdef HAVE_PVLAN
ResourceId DcmCliDebugShell::delPvlanType (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to del pvlan type : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPvlanType (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update pvlan type : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update pvlan type : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delSecondaryVlanId (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to del secondary vlan : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32 vid = strtoul (arguments[1].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delSecondaryVlanId (name,vid);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to del secondary vlan : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to del secondary vlan : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
#endif
ResourceId DcmCliDebugShell::setPoDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description = arguments[1];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Po Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Po Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setVlanDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description = arguments[1];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setVlanDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Vlan Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Vlan Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

#ifdef HAVE_PVLAN
ResourceId DcmCliDebugShell::setPvlanType (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to update pvlan type : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32 type = strtoul (arguments[1].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPvlanType (name,type);
    trace (TRACE_LEVEL_INFO, "Type: " + type);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update pvlan type Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Pvlan type : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setSecondaryVlanId (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to add secondary Vlan : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32 vid = strtoul (arguments[1].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setSecondaryVlanId (name,vid);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to add seconadry pvlan: " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to add secondary pvlan : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
#endif
ResourceId DcmCliDebugShell::deletePoInterface (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.deletePoInterface (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Po Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Po Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::deleteVlanInterface (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.deleteVlanInterface (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Vlan Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::createVlanInterface (const vector<string> &arguments)
{
    if (4 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           ifIndex     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           mtu         = strtoul (arguments[2].c_str (), NULL, 0);
    const string                         description = arguments[3];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.createVlanInterface (name,ifIndex,mtu,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Vlan Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::createVlanInterfaceStressTest (const vector<string> &arguments)
{
    UI32                           numberOfVlansToBeCreated = 2500;
    UI32                           i                        = 0;
    ResourceId                     status;
    ValClientSynchronousConnection connection               = getValClientSynchronousConnection ();

    for (i = 0; i < numberOfVlansToBeCreated; i++)
    {
        const string                         name        = string ("") + (i + 2);
        const UI32                           ifIndex     = i + 1;
        const UI32                           mtu         = 2500;
        const string                         description = string ("Stress Test Vlan");

        status = connection.createVlanInterface (name,ifIndex,mtu,description);

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Vlan Interface : " + name);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "FAILED to Create Vlan Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
        }
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId DcmCliDebugShell::createPhyInterface (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];


    ResourceId                     status;
    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.createPhyInterface (name,1,1,name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Phy Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPhyProtocalStatus (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPhyProtocalStatus (name,false);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}


ResourceId DcmCliDebugShell::setPhyProtocalStatus (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.setPhyProtocalStatus (name,true);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Status Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Protocal Status Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPhySwMode (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPhyMode (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPhySwMode (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPhyMode (name,1);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPhySw (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPhySwitchPort (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPhySw (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPhySwitchPort (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
ResourceId DcmCliDebugShell::delPoSwMode (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPoMode (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPoSwMode (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoMode (name,1);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

#ifdef HAVE_PVLAN
ResourceId DcmCliDebugShell::delPoPvlanMode (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPoPvlanMode (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPoPvlanMode (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           mode     = strtoul (arguments[1].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoPvlanMode (name,mode);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPoPvlanMapping (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoPvlanMapping (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}


ResourceId DcmCliDebugShell::setPoPvlanHostAssociation (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoPvlanHostAssociation (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}


ResourceId DcmCliDebugShell::setPoPvlanTrunkAssociation (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoPvlanTrunkAssociation (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
#endif

ResourceId DcmCliDebugShell::delPoSw (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPoSwitchPort (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

#ifdef HAVE_PVLAN
ResourceId DcmCliDebugShell::delPoPvlanMapping (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.delPoPvlanMapping (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPoPvlanHostAssociation (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.delPoPvlanHostAssociation (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::delPoPvlanTrunkAssociation (const vector<string> &arguments)
{
    if (3 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to PO Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const UI32                           name1     = strtoul (arguments[1].c_str (), NULL, 0);
    const UI32                           name2     = strtoul (arguments[2].c_str (), NULL, 0);

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.delPoPvlanTrunkAssociation (name,name1,name2);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update PO Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update PO Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
#endif
ResourceId DcmCliDebugShell::setPoSw (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPoSwitchPort (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name );
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}
ResourceId DcmCliDebugShell::delPhyDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description;

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

    status = connection.delPhyDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setPhyDesc (const vector<string> &arguments)
{
    if (2 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];
    const string                         description = arguments[1];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.setPhyDesc (name,description);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Update Phy Desc Interface : " + name + description);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Update Phy Desc Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::deletePhyInterface (const vector<string> &arguments)
{
    if (1 != arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : ");
        return (WAVE_MESSAGE_ERROR);
    }

    const string                         name        = arguments[0];

          ResourceId                     status;
          ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();


    status = connection.deletePhyInterface (name);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Create Phy Interface : " + name);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Create Phy Interface : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setBridgeXstp (const vector<string> &arguments)
{
    const string                         name        = arguments[0];

	  ResourceId                     status;
	  ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

	if (arguments.size() == 0) {
		setBridgeXstpHelp();
		return WAVE_MESSAGE_SUCCESS;
	}

	unsigned int i = 0;
	trace(TRACE_LEVEL_INFO, "DcmCliDebugShell::setBridgeXstp: Enter\n");
	for (i=0; i<arguments.size();i++) {
		trace(TRACE_LEVEL_INFO, arguments[i]+" ");
	}
	trace(TRACE_LEVEL_INFO, "\n");

    status = connection.setBridgeXstp (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Configure Bridge Xstp");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Configure Bridge Xstp : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setVlanBridgeXstp (const vector<string> &arguments)
{
    const string                         name        = arguments[0];

	  ResourceId                     status;
	  ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();

	if (arguments.size() == 0) {
		setVlanBridgeXstpHelp();
		return WAVE_MESSAGE_SUCCESS;
	}

	unsigned int i = 0;
	trace(TRACE_LEVEL_INFO, "DcmCliDebugShell::setVlanBridgeXstp: Enter\n");
	for (i=0; i<arguments.size();i++) {
		trace(TRACE_LEVEL_INFO, arguments[i]+" ");
	}
	trace(TRACE_LEVEL_INFO, "\n");

    status = connection.setVlanBridgeXstp (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to Configure Vlan Bridge Xstp");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Configure Vlan Bridge Xstp : " + name + "Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::queryIsDcmdReady (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    bool                           isReady;

    status = connection.isDcmdReadyForConnection (isReady);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == isReady)
        {
            return (WAVE_MESSAGE_SUCCESS);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Dcmd connection. Status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

void DcmCliDebugShell::createPoInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

void DcmCliDebugShell::createVlanInterfaceHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}



void DcmCliDebugShell::setVlanProtocalStatusHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}
void DcmCliDebugShell::setVlanDescHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : queryinterface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Creates the BIST services present in the Wave based application.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Createing is done in the order that the BIST services are registered with WAVe Framework.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "INPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    name - Name of the Interface to be queried.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "OUTPUT");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    Displays the parameters of the Interface, if an Interface with the given name could be found.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "    The following parameters will be displayed :");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        Name       - The Name of the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        IP Address - IP Address currently assigned to the Interface");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "        MTU        - MTU setting for the Interface.");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "");
}

void DcmCliDebugShell::setVlanBridgeXstpHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : Vlan Bridge Xstp Config");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  pvst/rpvst vlan [id]              ");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "		hellotime		<2-10>");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "		forward-delay	<2-30>");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "		max-age			<4-30>");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "		aging-time		<0-1000>");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "		priority		<0-65536>");
}


void DcmCliDebugShell::setBridgeXstpHelp ()
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : Bridge Xstp Config");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  protocol               Bridge priority commands");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      stp                STP");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      rstp               RSTP");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      mstp               MSTP");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      pvstp              PVSTP");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      rpvstp             RPVSTP");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  bridge-priority        Bridge priority commands");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  cisco-interoperability Configure CISCO Interoperability");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  description            Spanning tree description");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  error-disable-timeout  Set Error-disable-timeout for the spanning tree");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      enable             Enable Error-disable-timeout");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      val                Set the value");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  forward-delay          Set the forward delay for the spanning tree");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  max-age                Set the max age for the spanning tree");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  port-channel           Controls behaviour of port-channel for spanning-tree");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      standard           Follow standard mode for port-channel pathcost");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "      custom             Follow customerize mode for port-channel pathcost");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  shutdown               Reset");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  transmit-holdcount     Set Transmit hold count of the bridge");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "  bpdu-mac               Set PVST BPDU MAC");
	tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");
}

ResourceId DcmCliDebugShell::queryIsDcmdReadyForHasmLaunch (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    bool                           isDcmdReadyForHasmLaunch;

    status = connection.isDcmdReadyForHasmLaunch (isDcmdReadyForHasmLaunch);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == isDcmdReadyForHasmLaunch)
        {
            return (WAVE_MESSAGE_SUCCESS);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Dcmd connection. Status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

ResourceId DcmCliDebugShell::setHasmLaunched (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    bool                           isHasmLaunched              = true;

    status = connection.setHasmLaunched (isHasmLaunched);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Dcmd connection. Status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

UI32 DcmCliDebugShell::queryLinecardType (const vector<string> &arguments)
{
    if (1 > arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "usage : querylinecardtype <slotNumber>");
        return WAVE_MESSAGE_ERROR;
    }

    ResourceId                     status;
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    const UI32                     slotNumber                  = strtoul (arguments[0].c_str (), NULL, 0);
    UI32                           slotType                    = WAVE_MESSAGE_ERROR;

    status = connection.queryLinecardType (slotNumber, slotType);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, string("DcmCliDebugShell::queryLinecardType, slot:") + slotNumber + string(" type:") + slotType);
        exit (slotType);
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "Linecard type mismatch or verification failed : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

ResourceId DcmCliDebugShell::notifySystemShutdown (const vector<string> &arguments)
{
    if (!arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::notifySystemShutdown : No arguments sent to command. Please add arguments");
    }

    ResourceId                      status;
    ValClientSynchronousConnection  connection                  = getValClientSynchronousConnection ();
    const string                    databaseDirectory           = arguments[0];

    status = connection.notifySystemShutdown (databaseDirectory);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        return (WAVE_MESSAGE_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::notifySystemShutdown : System Shutdown Notified Failed with error status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}



ResourceId DcmCliDebugShell::getThisVcsNodeClusterType (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    cluster_type                   clusterType;

    ValClientSynchronousConnection::initializeCApi (&connection);
    
    clusterType = get_this_node_cluster_type ();

    if (VCS_CLUSTER_TYPE_STANDALONE == clusterType)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Standalone mode");
    } 
    else if (VCS_CLUSTER_TYPE_FABRIC == clusterType)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Fabric Cluster mode");
    }
    else if (VCS_CLUSTER_TYPE_MGMT == clusterType)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Management Cluster mode");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Dcmd connection.");
    }
    
    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId DcmCliDebugShell::configureFileService(const vector<string> &sArgs)
{

    ResourceId  status;

    if (0 == sArgs.size ())
    {
        fileServiceHelp();
        return (WAVE_MESSAGE_ERROR);
    }
    
    if ((sArgs[0].compare("enable") != 0) && (sArgs[0].compare("disable") != 0))
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to change File Service Status : Invalid Arguments.\n");
        fileServiceHelp();        
        return (WAVE_MESSAGE_ERROR);    
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.configureFileService (sArgs[0]);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string("File Service ") + sArgs[0]);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Change File Service Status : " + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::fileServiceHelp(void)
{
     tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : fileService <enable|disable>");
     tracePrintf (TRACE_LEVEL_INFO, true, true, "  enable  - File Service on this node.");
     tracePrintf (TRACE_LEVEL_INFO, true, true, "  disable - File Service on this node.");     
	tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");     
}        

ResourceId DcmCliDebugShell::changeBtTrackerState (const vector<string> &argument)
{
    ResourceId status;

    if (2 != argument.size ())
    {
        trace (TRACE_LEVEL_ERROR, "<cmdname> <object/message> <on/offAndClear>");
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.changeBtTrackerState (argument);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to change bt tracker state.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to change bt tracker state : " + FrameworkToolKit::localize (status));
    }

    return (status);

}

ResourceId DcmCliDebugShell::vcsNodeGetRBridgeIdFromWWN (const vector<string> &arguments)
{
    wwn_t           wwn;
    unsigned int    rBridgeId;
    ResourceId      status          = WAVE_MESSAGE_ERROR;

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    if (1 != arguments.size ())
    {
        vcsNodeGetRBridgeIdFromWWNHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    if (false == WorldWideName::isValidWorldWideNameString (arguments[0]))
    {
        vcsNodeGetRBridgeIdFromWWNHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    WorldWideName switchWWN (arguments[0]);

    for (UI32 i = 0; i < 8; i++)
    {
        wwn.chars[i] = switchWWN[i];
    }

    int result = vcs_get_rbridge_id_interface (&rBridgeId, &wwn);

    if (0 == result)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "RBridge-Id : %d", rBridgeId);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (-1 == result)
    {
        // Framework error
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : Framework error!");
        status = WAVE_MESSAGE_ERROR;
    }
    else if (-2 == result)
    {
        // RBridge-Id not found
        tracePrintf (TRACE_LEVEL_WARN, true, true, "Return Status : RBridge-Id could not be found for switch WWN : %s", arguments[0].c_str ());
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {   
        // Unknown return code
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : vcs_get_rbridge_id_interface API returned unknown error code.");
        status = WAVE_MESSAGE_ERROR;
    }    

    return (status);
}

void DcmCliDebugShell::vcsNodeGetRBridgeIdFromWWNHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : vcsNodeGetRBridgeIdFromWWNHelp <switch WWN: XX:XX:XX:XX:XX:XX:XX:XX>");
}
                        
ResourceId DcmCliDebugShell::vcsNodeGetWWNFromRBridgeId (const vector<string> &arguments)
{
    wwn_t           wwn;
    unsigned int    rBridgeId;
    ResourceId      status          = WAVE_MESSAGE_ERROR;


    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    if (1 != arguments.size ())
    {
        vcsNodeGetWWNFromRBridgeIdHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    rBridgeId = static_cast<unsigned int> (atoi (arguments[0].c_str ()));
    
    if (1 > rBridgeId || 256 < rBridgeId)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Invalid RBridge-Id specified.");

        vcsNodeGetWWNFromRBridgeIdHelp ();
        return (WAVE_MESSAGE_ERROR);
    }
    
    int result = vcs_get_wwn_interface (&wwn, &rBridgeId);

    if (0 == result)
    {
        WorldWideName switchWWN ((UI8 *) wwn.chars);

        tracePrintf (TRACE_LEVEL_INFO, true, true, "World Wide Name : %s", switchWWN.toString().c_str ());
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (-1 == result)
    {
        // Framework error
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : Framework error!");
        status = WAVE_MESSAGE_ERROR;
    }
    else if (-2 == result)
    {
        // RBridge-Id not found
        tracePrintf (TRACE_LEVEL_WARN, true, true, "Return Status : Switch WWN could not be found for given RBridge-Id : %d" , rBridgeId);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        // Unknown return code
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : vcs_get_wwn_interface API returned unknown error code.");
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

void DcmCliDebugShell::vcsNodeGetWWNFromRBridgeIdHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : vcsNodeGetWWNFromRBridgeIdHelp <RBridge-Id: 1-239>");
}

ResourceId DcmCliDebugShell::queryIsNodeInCluster (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    UI32                           rbridgeId;

    if (1 != arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : isnodeincluster <rbridge-id>");
        return (WAVE_MESSAGE_ERROR);
    }

    rbridgeId = strtoul (arguments[0].c_str (), NULL, 10);

     ValClientSynchronousConnection::initializeCApi (&connection);

    int inClusterStatus = is_node_in_cluster (rbridgeId); 

    if (-1 == inClusterStatus)
    {
        trace (TRACE_LEVEL_ERROR, "Failed to get the status.");
    }
    else if (1 == inClusterStatus)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Node with rbridge-id %d is a part of cluster", rbridgeId);    
    }
    else if (0 == inClusterStatus)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Node with rbridge-id %d is not in a cluster", rbridgeId);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId DcmCliDebugShell::queryIsLocalNodeInCluster (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();


    ValClientSynchronousConnection::initializeCApi (&connection);

    int inClusterStatus = is_local_node_in_cluster ();

    if (-1 == inClusterStatus)
    {
        trace (TRACE_LEVEL_ERROR, "Failed to get the status.");
    }
    else if (1 == inClusterStatus)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "This node is a part of cluster");
    }
    else if (0 == inClusterStatus)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "This node is not in a cluster");
    }

    return (WAVE_MESSAGE_SUCCESS);
}

ResourceId DcmCliDebugShell::vcsNodeGetIPFromRBridgeId (const vector<string> &arguments)
{
    char            *ip = NULL;
    unsigned int    rBridgeId;
    ResourceId      status          = WAVE_MESSAGE_ERROR;

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    if (1 != arguments.size ())
    {
        vcsNodeGetIPFromRBridgeIdHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    rBridgeId = static_cast<unsigned int> (atoi (arguments[0].c_str ()));
    
    if (1 > rBridgeId || 256 < rBridgeId)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Invalid rbridge-id %d specified.", rBridgeId);

        vcsNodeGetIPFromRBridgeIdHelp ();
        return (WAVE_MESSAGE_ERROR);
    }
    
    int result = vcs_get_ip_interface (&ip, &rBridgeId);

    if (0 == result)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "IP Address : %s\n", ip);
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (-1 == result)
    {
        // Framework error
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : Framework error!");
        status = WAVE_MESSAGE_ERROR;
    }
    else if (-2 == result)
    {
        tracePrintf (TRACE_LEVEL_WARN, true, true, "Return Status : Switch IP could not be found for given rbridge-id : %d" , rBridgeId);
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        // Unknown return code
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : vcs_get_ip_interface API returned unknown error code.");
        status = WAVE_MESSAGE_ERROR;
    }

    if (NULL != ip)
    {
        free (ip);
    }    

    return (status);
}

void DcmCliDebugShell::vcsNodeGetIPFromRBridgeIdHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : vcsNodeGetIPFromRBridgeIdHelp <RBridge-Id: 1-239>");
}


ResourceId DcmCliDebugShell::getVcsIdForNode (const vector<string> &arguments)
{
    ResourceId      status          = WAVE_MESSAGE_ERROR;


    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    int result = get_vcs_id ();

    if (-1 == result)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Failed to get the vcs-id for the node.");
    }
    else 
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "%d", result);
        status = WAVE_MESSAGE_SUCCESS;
    }
    return (status);
}

ResourceId DcmCliDebugShell::getVcsRBridgeIdForNode (const vector<string> &arguments)
{
    ResourceId      status          = WAVE_MESSAGE_ERROR;


    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    int result  = get_vcs_rbridge_id ();

    if (-1 == result)
    {
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Failed to get rbridge-id for the node.");
    }
    else
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "%d", result);
        status = WAVE_MESSAGE_SUCCESS;
    }
    return (status);
}

ResourceId DcmCliDebugShell::isClusterConnectedtoIpAddress (const vector<string> &arguments)
{

    ResourceId status = WAVE_MESSAGE_ERROR;

    if (1 != arguments.size ())
    {
        isClusterConnectedtoIpAddressHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    vector<IpAddressPingResponseParameters*> connectedNodesList;
    string ipaddress = arguments[0];

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    status = connection.isClusterConnectedtoIpAddress (ipaddress, connectedNodesList);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, string("No of connected rbridge-ids =" ) + connectedNodesList.size());
        for (UI32 i=0; i<connectedNodesList.size();i++)
        {
            cout<<"\n Rbridge-id   = "<< connectedNodesList[i]->rbridgeId;
         }
         cout<<"\n ";
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to get connection status : " + FrameworkToolKit::localize (status));
    }

    for (UI32 i=0; i<connectedNodesList.size();i++)
    {
        delete connectedNodesList[i];
    }
    connectedNodesList.clear();

    return (status);

}

void DcmCliDebugShell::isClusterConnectedtoIpAddressHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : isclusterconnectedtoip <ip-address>");
}

ResourceId DcmCliDebugShell::vcsGetClusterRbridgeIdFromWWN (const vector<string> &arguments)
{
    ResourceId          status                    = WAVE_MESSAGE_ERROR;
    domainBitMap_t      rbridgeIdBitmap;
    //wwn_t               wwn;
    UI32                maxRbridgeId              = 239;
    BitMap              rbridgeIdVector           (maxRbridgeId);
    int                 result                    = 0;

    BM_Clear (rbridgeIdBitmap, maxRbridgeId);

    if (1 != arguments.size ())
    {
        vcsGetClusterRbridgeIdFromWWNHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    if (false == WorldWideName::isValidWorldWideNameString (arguments[0]))
    {
        vcsGetClusterRbridgeIdFromWWNHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    WorldWideName switchWWN (arguments[0]);

#if 0
    for (UI32 i = 0; i < 8; i++)
    {
        wwn.chars[i] = switchWWN[i];
    }
#endif

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    status = connection.vcsGetClusterRbridgeIdFromWWN (switchWWN, rbridgeIdVector);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        result = 0;

        for (UI32 i = 0; i < maxRbridgeId; i++)
        {
            if (rbridgeIdVector [i])
            {
                BM_SetBitN (i+1, rbridgeIdBitmap);
            }
        }        
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

    if (0 == result)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Available RBridge-Ids : ");
        string output ("");

        for (UI32 i = 1; i <= maxRbridgeId; i++)
        {
            if (BM_isBitNSet (i, rbridgeIdBitmap))
            {
                output = output + string (" ") + i;
            }
        }

        tracePrintf (TRACE_LEVEL_INFO, true, true, "%s", output.c_str ());
        status = WAVE_MESSAGE_SUCCESS;
    }
    else if (-1 == result)
    {
        // Framework error
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : Framework error!");
        status = WAVE_MESSAGE_ERROR;
    }
    else if (-2 == result)
    {
        // RBridge-Id not found
        tracePrintf (TRACE_LEVEL_WARN, true, true, "Return Status : RBridge-Id could not be found for switch WWN. All the available slots in use. : %s", arguments[1].c_str ());
        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        // Unknown return code
        tracePrintf (TRACE_LEVEL_ERROR, true, true, "Return Status : vcs_get_cluster_rbridge_id_from_wwn API returned unknown error code.");
        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

void DcmCliDebugShell::vcsGetClusterRbridgeIdFromWWNHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : vcsgetclusterrbridgeidfromwwn <switchwwn>");
}

ResourceId DcmCliDebugShell::vcsIsFabricDistributionEnabled (const vector<string> &arguments)
{


    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();


    ValClientSynchronousConnection::initializeCApi (&connection);

    int isFabricDistributionEnabled = is_fabric_distribution_service_enabled ();

    if (-1 == isFabricDistributionEnabled)
    {
        trace (TRACE_LEVEL_ERROR, "Failed to get the status.");
    }
    else if (1 == isFabricDistributionEnabled)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Fabric Distribution is enabled");
    }
    else if (0 == isFabricDistributionEnabled)
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "Fabric Distribution is not enabled");
    }

    return (WAVE_MESSAGE_SUCCESS);


}

ResourceId DcmCliDebugShell::disconnectInstanceClients (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                  = getValClientSynchronousConnection ();
    ResourceId                     status;

    if (0 == arguments.size ())
    {
        trace (TRACE_LEVEL_ERROR, "Please Specify any instnace client IPAdress");
        return (WAVE_MESSAGE_ERROR);
    }

    status = connection.disconnectInstanceClients (arguments);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "SUCCEEDED to disconnect clients");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to disconnect clients: " + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::setFcfMacAddress (const vector<string> &arguments)
{
    ResourceId                     status;

    if ( 1 != arguments.size ())
    {
        setFcfMacAddressHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    // this should be a service independent message else if cluster formation is going on the message might get dropped 
    status = connection.vcsNodeLocalSetFcfMacServiceIndependent (arguments[0].c_str());

    if (WAVE_MESSAGE_SUCCESS == status)
    {   
        trace (TRACE_LEVEL_SUCCESS, " Setting FCF MAC Address Successful");
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, " Setting FCF MAC Address Failed  ");
    }

    return (status);

}

void DcmCliDebugShell::setFcfMacAddressHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setFcfMacAddress [MacAddress string]");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");
}

ResourceId DcmCliDebugShell::cliAllowedFromThisLocation (const vector<string> &arguments)
{

    ResourceId status = WAVE_MESSAGE_ERROR;

    if (1 != arguments.size ())
    {
        cliAllowedFromThisLocationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }
    
    bool isCliAllowedFromThisLocation = false;
    
    if ( !( arguments[0].compare("true")) )
    {
        isCliAllowedFromThisLocation = true;
    }
    else if ( !(arguments[0].compare("false")) )
    {
        isCliAllowedFromThisLocation = false;
    }    
    else
    {
        cliAllowedFromThisLocationHelp ();
        return (WAVE_MESSAGE_ERROR);
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);
    
    status = connection.setIsCliAllowedFromThisLocation (isCliAllowedFromThisLocation);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_INFO, string("DcmCliDebugShell::cliAllowedFromThisLocation ValClientSynchronousConnection failure status = " ) + FrameworkToolKit::localize (status));
    }

    return (status);

}

void DcmCliDebugShell::cliAllowedFromThisLocationHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : cliallowedfromthislocation true | false");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");
}

ResourceId DcmCliDebugShell::setModeChangeParameters (const vector<string> &arguments)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    if ( 0 == arguments.size ()) // arguments vcsid,rbridgeid,vcs mode,noreboot
    {
        trace (TRACE_LEVEL_ERROR, string ("DcmCliDebugShell::setModeChangeParameters: ") + arguments.size ());
        setModeChangeParametersHelp ();
        return (WAVE_MESSAGE_ERROR);
    }
    
    UI32   rbridgeId = 0;
    string vcsMode = "";
    UI32   vcsId = 0;
    bool   rebootSw = true;

    vector<string>::const_iterator iter = arguments.begin ();
    string temp = "";
 
    while (iter != arguments.end ())
    {
        temp = *iter;

        if ( !(*iter).compare ("vcsid"))
        {
            temp = *++iter;
            vcsId = atoi (temp.c_str());
            if (0 == vcsId)
            {
                trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setModeChangeParameters: Missing vcsId.");
                return (WAVE_MESSAGE_ERROR);
            }
        }
        else if ( !(*iter).compare("rbridgeid"))
        {
            temp = *++iter;
            rbridgeId = atoi (temp.c_str());
            if (0 == rbridgeId)
            {
                trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setModeChangeParameters: Missing rbridgeId.");
                return (WAVE_MESSAGE_ERROR);
            }
        }
        else if ( !(*iter).compare("vcsmode"))
        {
            if (++iter != arguments.end ())
            {
                vcsMode = *iter;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setModeChangeParameters: Missing vcsMode.");
                return (WAVE_MESSAGE_ERROR);
            }
        }
        else if ( !(*iter).compare("no-reboot"))
        {
            rebootSw = false;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DcmCliDebugShell::setModeChangeParameters: No matching argument ") + temp);
            setModeChangeParametersHelp ();
            return (WAVE_MESSAGE_ERROR);
        }
        ++iter;
    }
    
    if (vcsMode == "SA")
    {
        if ( (0 != rbridgeId) || (0 != vcsId))
        {
            trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setModeChangeParameters: rbridgeId and vcsId cannot be given if the desired mode is SA");
            return (WAVE_MESSAGE_ERROR);
        }
    }
    else if ( ("MC" == vcsMode) || ("FC" == vcsMode))
    {
        if ( 0 == vcsId)
        {
            trace (TRACE_LEVEL_INFO, "DcmCliDebugShell::setModeChangeParameters: Defaulting vcsId to 1");
            vcsId = 1;
        }
        if ( 0 == rbridgeId)
        {
            trace (TRACE_LEVEL_INFO, "DcmCliDebugShell::setModeChangeParameters: Defaulting rbridgeId to 1");
            rbridgeId = 1;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DcmCliDebugShell::setModeChangeParameters: Desired mode can only be one of (SA|FC|MC) and mandatory.");
        return WAVE_MESSAGE_ERROR;
    }
    
    if (true == rebootSw)
    {
        trace (TRACE_LEVEL_INFO, "DcmCliDebugShell::setModeChangeParameters: Initiate node transition to desired mode and Reboot.");
    }
    else
    {
        trace (TRACE_LEVEL_INFO, "DcmCliDebugShell::setModeChangeParameters: Set vcs parameters and mode on the switch.");
    }

    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);
    
    status = connection.setModeChangeParameters (vcsId, vcsMode, rbridgeId, rebootSw);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string("DcmCliDebugShell::setModeChangeParameters ValClientSynchronousConnection failure status = " ) + FrameworkToolKit::localize (status));
    }

    return (status);
}

void DcmCliDebugShell::setModeChangeParametersHelp (void)
{
    tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setModeChangeParameters vcsid <val> vcsmode <SA | FC | MC> rbridgeid <val> [no-reboot]");
    tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");
}

ResourceId DcmCliDebugShell::debugClusterStatistics (const vector<string> &arguments)
{
    ResourceId status;

    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    // this should be a service independent message else if cluster formation is going on the message might get dropped 
    status = connection.getClusterStatistics (dcm_vcs_fabric_debug_client_callback);

    if (WAVE_MESSAGE_SUCCESS == status)
    {    
        trace (TRACE_LEVEL_SUCCESS, "Send the command to get the cluster statistics");
    }    
    else 
    {    
        trace (TRACE_LEVEL_ERROR, "Not able to get the cluster statistics");
    }    

    return (status);
}

ResourceId DcmCliDebugShell::setTtyName (const vector<string> &arguments)
{
    ResourceId status;

    string ttyName = "";

    if (0 == arguments.size ())
    {
        ttyName = FrameworkToolKit::getCurrentTtyName ();

        trace (TRACE_LEVEL_INFO, string (" TTYNAME : ") + ttyName);
    }
    else if (1 == arguments.size ())
    {
        ttyName = arguments[0];
    }
    else if (1 < arguments.size ())
    {
        tracePrintf (TRACE_LEVEL_INFO, true, true, "USAGE : setTtyName <ttyName (optional)>");
        tracePrintf (TRACE_LEVEL_INFO, true, true, "\n");
    }

    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    // this should be a service independent message else if cluster formation is going on the message might get dropped 
    status = connection.setTtyName (ttyName);

    if (WAVE_MESSAGE_SUCCESS == status)
    {    
        trace (TRACE_LEVEL_SUCCESS, "Sent the message to set the TtyName");
    }    
    else 
    {    
        trace (TRACE_LEVEL_ERROR, "Not able to set the TtyName ");
    }    

    return (status);
    
}

ResourceId DcmCliDebugShell::getVcenterStateMachineStatus (const vector<string> &arguments)
{
    ResourceId status;




    ValClientSynchronousConnection connection  = getValClientSynchronousConnection ();
    ValClientSynchronousConnection::initializeCApi (&connection);

    // this should be a service independent message else if cluster formation is going on the message might get dropped
    vector<vector<string> > respList;
    status = connection.retrievevCenterMachineStatus (respList);

    if (WAVE_MESSAGE_SUCCESS == status)
    {

        printf("\n");
        for(size_t i =0;i<respList.size();i++){
            vector<string> inner = respList.at(i);

            for(size_t j=0;j<inner.size();j++){
                string resp = inner.at(j);
                printf("%-20s",resp.c_str());
            }
                printf("\n")
;        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "Not able get vCenter Details ");
    }



    return (status);

}

ResourceId DcmCliDebugShell::getIsNosSmdReady ()
{
    ResourceId                     status;
    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();
    bool                           isReady;

    status = connection.isNosSmdReadyForConnection (isReady);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        if (true == isReady)
        {
            return (WAVE_MESSAGE_SUCCESS);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "FAILED to Query for Dcmd connection. Status : " + FrameworkToolKit::localize (status));
    }

    return (WAVE_MESSAGE_ERROR);
}

ResourceId DcmCliDebugShell::loadZoneBulkConfiguration (const vector<string> &arguments)
{
    ResourceId                     status;
    ValClientSynchronousConnection connection = getValClientSynchronousConnection ();

    status = connection.loadZoneBulkConfiguration (arguments[0]);

    return (status);
}

ResourceId DcmCliDebugShell::createOpenFlowSwitch (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection          = getValClientSynchronousConnection ();
    ResourceId                     status              = WAVE_MESSAGE_SUCCESS;
    UI32                           numberOfArguments   = arguments.size ();
    string                         openFlowSwitchName;
    Uri                            openFlowSwitchUri;

    if (1 <= numberOfArguments)
    {
        openFlowSwitchName = arguments[0];
    }

    if (2 <= numberOfArguments)
    {
        openFlowSwitchUri.fromString (arguments[1]);
    }

    status = connection.createOpenFlowSwitch (openFlowSwitchName, openFlowSwitchUri);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully Created Open Flow Switch.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failed to Create Open Flow Switch.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::createOpenFlowLogicalSwitch (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection          = getValClientSynchronousConnection ();
    ResourceId                     status              = WAVE_MESSAGE_SUCCESS;
    UI32                           numberOfArguments   = arguments.size ();
    string                         openFlowLogicalSwitchName;
    Uri                            openFlowLogicalSwitchUri;

    if (1 <= numberOfArguments)
    {
        openFlowLogicalSwitchName = arguments[0];
    }

    if (2 <= numberOfArguments)
    {
        openFlowLogicalSwitchUri.fromString (arguments[1]);
    }

    status = connection.createOpenFlowLogicalSwitch (openFlowLogicalSwitchName, openFlowLogicalSwitchUri);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully Created Open Flow Logical Switch.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failed to Create Open Flow Logical Switch.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::addFlow (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection          = getValClientSynchronousConnection ();
    ResourceId                     status              = WAVE_MESSAGE_SUCCESS;
    UI32                           numberOfArguments   = arguments.size ();
    UI32                           ingressIfIndex;
    UI32                           srcIpV4Address;
    UI32                           srcIpV4AddressMask;
    UI32                           dstIpV4Address;
    UI32                           dstIpV4AddressMask;
    string                         appType;
    UI16                           srcPort;
    UI16                           dstPort;
    UI32                           egressIfIndex;


    if (1 <= numberOfArguments)
    {
        ingressIfIndex = atoi (arguments[0].c_str ());
    }

    if (2 <= numberOfArguments)
    {
        inet_pton(AF_INET, arguments[1].c_str (), &srcIpV4Address);
    }

    if (3 <= numberOfArguments)
    {
        inet_pton(AF_INET, arguments[2].c_str (), &srcIpV4AddressMask);
    }


    if (4 <= numberOfArguments)
    {
        inet_pton(AF_INET, arguments[3].c_str (), &dstIpV4Address);
    }

    if (5 <= numberOfArguments)
    {
        inet_pton(AF_INET, arguments[4].c_str (), &dstIpV4AddressMask);
    }

    if (6 <= numberOfArguments)
    {
        appType = arguments[5];
    }

    if (7 <= numberOfArguments)
    {
        srcPort = (UI16) (atoi (arguments[6].c_str ()));
    }

    if (8 <= numberOfArguments)
    {
        dstPort = (UI16) (atoi (arguments[7].c_str ()));
    }

    if (9 <= numberOfArguments)
    {
        egressIfIndex = atoi (arguments[8].c_str ());
    }


    status = connection.addFlow (ingressIfIndex, egressIfIndex, srcIpV4Address, srcIpV4AddressMask, dstIpV4Address, dstIpV4AddressMask, appType, srcPort, dstPort);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully Sent Add Flow to FTM.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failed to send Add Flow to FTM.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::addOpenFlowControllerToOpenFlowLogicalSwitch (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                   = getValClientSynchronousConnection ();
    ResourceId                     status                       = WAVE_MESSAGE_SUCCESS;
    UI32                           numberOfArguments            = arguments.size ();
    string                         openFlowLogicalSwitchName;
    Uri                            openFlowLogicalSwitchUri;
    IpV4AddressNetworkMask         openFlowControllerIpAddress;
    UI32                           openFlowControllerPort       = 6633;

    if (1 <= numberOfArguments)
    {
        openFlowLogicalSwitchName = arguments[0];
    }

    if (2 <= numberOfArguments)
    {
        openFlowLogicalSwitchUri.fromString (arguments[1]);
    }


    if (3 <= numberOfArguments)
    {
        openFlowControllerIpAddress.fromString (arguments[2]);
    }


    if (4 <= numberOfArguments)
    {
        openFlowControllerPort = strtoul (arguments[3].c_str (), NULL, 10);
    }

    status = connection.addOpenFlowControllerToOpenFlowLogicalSwitch (openFlowLogicalSwitchName, openFlowLogicalSwitchUri, openFlowControllerIpAddress, openFlowControllerPort);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully Added Open Flow Controller to Open Flow Logical Switch.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failed to Add Open Flow Controller to Open Flow Logical Switch.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

ResourceId DcmCliDebugShell::connectOpenFlowLogicalSwitchToOpenFlowController (const vector<string> &arguments)
{
    ValClientSynchronousConnection connection                   = getValClientSynchronousConnection ();
    ResourceId                     status                       = WAVE_MESSAGE_SUCCESS;
    UI32                           numberOfArguments            = arguments.size ();
    string                         openFlowLogicalSwitchName;
    Uri                            openFlowLogicalSwitchUri;
    IpV4AddressNetworkMask         openFlowControllerIpAddress;
    UI32                           openFlowControllerPort       = 6633;

    if (1 <= numberOfArguments)
    {
        openFlowLogicalSwitchName = arguments[0];
    }

    if (2 <= numberOfArguments)
    {
        openFlowLogicalSwitchUri.fromString (arguments[1]);
    }


    if (3 <= numberOfArguments)
    {
        openFlowControllerIpAddress.fromString (arguments[2]);
    }


    if (4 <= numberOfArguments)
    {
        openFlowControllerPort = strtoul (arguments[3].c_str (), NULL, 10);
    }

    status = connection.connectOpenFlowLogicalSwitchToOpenFlowController (openFlowLogicalSwitchName, openFlowLogicalSwitchUri, openFlowControllerIpAddress, openFlowControllerPort);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        trace (TRACE_LEVEL_SUCCESS, "Successfully Connected Open Flow Logical Switch to Open Flow Controller.");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("Failed to Connect Open Flow Logical Switch to Open Flow Controller.  Status : ") + FrameworkToolKit::localize (status));
    }

    return (status);
}

}
