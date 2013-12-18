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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DCMCLIDEBUGSHELL_H
#define DCMCLIDEBUGSHELL_H

#include "Shell/WaveCliShell.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"

using namespace WaveNs;

namespace DcmNs
{

class DcmCliDebugShell : public WaveCliShell
{
    private :
        void       briefHelp             ();
        
        ResourceId setNewShowVcs               (const vector<string> &argument); 
        ResourceId printConfigurationTime      (const vector<string> &argument);
        ResourceId printConfigurationNumber    (const vector<string> &argument);
        ResourceId debugObjectLeak             (const vector<string> &argument);
        ResourceId debugMessageLeak            (const vector<string> &argument);
        ResourceId changeBtTrackerState        (const vector<string> &argument);
        ResourceId installDemoObjects          (const vector<string> &argument);
        ResourceId debugPrintInterface         (const vector<string> &argument); 
        ResourceId queryForInterface           (const vector<string> &arguments);
        ResourceId queryForInterfaceCWay       (const vector<string> &arguments);
        void       queryForInterfaceHelp       (void);

        ResourceId createPhysicalInterface     (const vector<string> &arguments);
        ResourceId createPhysicalInterfaceCWay (const vector<string> &arguments);
        ResourceId AssocPolicyToPhysicalInterface(const vector<string> &arguments);
        ResourceId AssocPolicyToLogicalInterface(const vector<string> &arguments);
        void AssocPolicyToPhysicalInterfaceHelp(void);
        void       createPhysicalInterfaceHelp (void);
        void AssocPolicyToLogicalInterfaceHelp(void);
        ResourceId L2AssocRuleToPolicy(const vector<string> &arguments);
        void L2AssocRuleToPolicyHelp(void);
        ResourceId L2ACLPolicy(const vector<string> &arguments);
        void L2ACLPolicyHelp(void);
        ResourceId l2AclStressTest (const vector<string> &arguments);
        ResourceId createVlanInterface     (const vector<string> &arguments);
        ResourceId createVlanInterfaceStressTest     (const vector<string> &arguments);
        ResourceId createVlanInterfaceCWay (const vector<string> &arguments);
        void       createVlanInterfaceHelp (void);

        ResourceId deleteVlanInterface     (const vector<string> &arguments);
        ResourceId deleteVlanInterfaceCWay (const vector<string> &arguments);
        void       deleteVlanInterfaceHelp (void);
       
        ResourceId setVlanDesc     (const vector<string> &arguments);
        ResourceId setVlanDescCWay (const vector<string> &arguments);
        void       setVlanDescHelp (void);
        ResourceId getVcsIdForNode          (const vector<string> &arguments);
        ResourceId getVcsRBridgeIdForNode   (const vector<string> &arguments);
        ResourceId queryIsNodeInCluster     (const vector<string> &arguments);
        ResourceId queryIsLocalNodeInCluster(const vector<string> &arguments);

        ResourceId setVlanProtocalStatus    (const vector<string> &arguments);
        ResourceId setVlanProtocalStatusCWay (const vector<string> &arguments);
        void       setVlanProtocalStatusHelp (void);

        ResourceId delVlanDesc     (const vector<string> &arguments);
        ResourceId delVlanDescCWay (const vector<string> &arguments);
        void       delVlanDescHelp (void);

        ResourceId delVlanProtocalStatus    (const vector<string> &arguments);
        ResourceId delVlanProtocalStatusCWay (const vector<string> &arguments);
        void       delVlanProtocalStatusHelp (void);
 
        ResourceId createPoInterface     (const vector<string> &arguments);
        ResourceId createPoInterfaceCWay (const vector<string> &arguments);
        void       createPoInterfaceHelp (void);

        ResourceId deletePoInterface     (const vector<string> &arguments);
        ResourceId deletePoInterfaceCWay (const vector<string> &arguments);
        void       deletePoInterfaceHelp (void);

        ResourceId setPoDesc     (const vector<string> &arguments);
        ResourceId setPoDescCWay (const vector<string> &arguments);
        void       setPoDescHelp (void);

        ResourceId setPoProtocalStatus    (const vector<string> &arguments);
        ResourceId setPoProtocalStatusCWay (const vector<string> &arguments);
        void       setPoProtocalStatusHelp (void);

        ResourceId delPoDesc     (const vector<string> &arguments);
        ResourceId delPoDescCWay (const vector<string> &arguments);
        void       delPoDescHelp (void);

        ResourceId setPoSw     (const vector<string> &arguments);
        ResourceId setPoSwCWay (const vector<string> &arguments);
        void       setPoSwHelp (void);

        ResourceId setPoSwMode     (const vector<string> &arguments);
        ResourceId setPoSwModeCWay (const vector<string> &arguments);
        void       setPoSwModeHelp (void);
        ResourceId delPoSw     (const vector<string> &arguments);
        ResourceId delPoSwCWay (const vector<string> &arguments);
        void       delPoSwHelp (void);

        ResourceId delPoSwMode     (const vector<string> &arguments);
        ResourceId delPoSwModeCWay (const vector<string> &arguments);
        void       delPoSwModeHelp (void);

        ResourceId delPoProtocalStatus    (const vector<string> &arguments);
        ResourceId delPoProtocalStatusCWay (const vector<string> &arguments);
        void       delPoProtocalStatusHelp (void);

        ResourceId createPhyInterface     (const vector<string> &arguments);
        ResourceId createPhyInterfaceCWay (const vector<string> &arguments);
        void       createPhyInterfaceHelp (void);

        ResourceId deletePhyInterface     (const vector<string> &arguments);
        ResourceId deletePhyInterfaceCWay (const vector<string> &arguments);
        void       deletePhyInterfaceHelp (void);

        ResourceId setPhyDesc     (const vector<string> &arguments);
        ResourceId setPhyDescCWay (const vector<string> &arguments);
        void       setPhyDescHelp (void);

        ResourceId setPhyProtocalStatus    (const vector<string> &arguments);
        ResourceId setPhyProtocalStatusCWay (const vector<string> &arguments);
        void       setPhyProtocalStatusHelp (void);

        ResourceId delPhyDesc     (const vector<string> &arguments);
        ResourceId delPhyDescCWay (const vector<string> &arguments);
        void       delPhyDescHelp (void);

        ResourceId setPhySw     (const vector<string> &arguments);
        ResourceId setPhySwCWay (const vector<string> &arguments);
        void       setPhySwHelp (void);

        ResourceId setPhySwMode     (const vector<string> &arguments);
        ResourceId setPhySwModeCWay (const vector<string> &arguments);
        void       setPhySwModeHelp (void);

        ResourceId delPhySw     (const vector<string> &arguments);
        ResourceId delPhySwCWay (const vector<string> &arguments);
        void       delPhySwHelp (void);

        ResourceId delPhySwMode     (const vector<string> &arguments);
        ResourceId delPhySwModeCWay (const vector<string> &arguments);
        void       delPhySwModeHelp (void);

        ResourceId delPhyProtocalStatus    (const vector<string> &arguments);
        ResourceId delPhyProtocalStatusCWay (const vector<string> &arguments);
        void       delPhyProtocalStatusHelp (void); 

        ResourceId setBridgeXstp    (const vector<string> &arguments);
        ResourceId setBridgeXstpCWay (const vector<string> &arguments);
        void       setBridgeXstpHelp (void); 

        ResourceId setVlanBridgeXstp    (const vector<string> &arguments);
        ResourceId setVlanBridgeXstpCWay (const vector<string> &arguments);
        void       setVlanBridgeXstpHelp (void); 

        ResourceId queryIsDcmdReady (const vector<string> &arguments);

        ResourceId queryIsDcmdReadyForHasmLaunch (const vector<string> &arguments);
        ResourceId setHasmLaunched (const vector<string> &arguments);

        ResourceId getThisVcsNodeClusterType (const vector<string> &arguments);
        ResourceId disconnectInstanceClients (const vector<string> &arguments);
        ResourceId configureFileService(const vector<string> &sArgs);
        void       fileServiceHelp(void);

        ResourceId vcsNodeGetRBridgeIdFromWWN (const vector<string> &arguments);
        void       vcsNodeGetRBridgeIdFromWWNHelp (void);

        ResourceId vcsNodeGetWWNFromRBridgeId (const vector<string> &arguments);
        void       vcsNodeGetWWNFromRBridgeIdHelp (void);

        ResourceId vcsNodeGetIPFromRBridgeId (const vector<string> &arguments);
        void       vcsNodeGetIPFromRBridgeIdHelp (void);

        ResourceId isClusterConnectedtoIpAddress (const vector<string> &arguments);
        void       isClusterConnectedtoIpAddressHelp (void);

        ResourceId vcsGetClusterRbridgeIdFromWWN (const vector<string> &arguments);
        void       vcsGetClusterRbridgeIdFromWWNHelp (void);

        ResourceId vcsIsFabricDistributionEnabled (const vector<string> &arguments);
 
        UI32 queryLinecardType(const vector<string> &arguments);
        
        ResourceId notifySystemShutdown(const vector<string> &arguments);

        ResourceId setFcfMacAddress (const vector<string> &arguments);
        void       setFcfMacAddressHelp (void);
        ResourceId sendvCenterEvents (const vector<string> &arguments);
        void       sendvCenterEventsHelp (void);
        
        ResourceId cliAllowedFromThisLocation (const vector<string> &arguments);
        void       cliAllowedFromThisLocationHelp (void);

        ResourceId loadZoneBulkConfiguration (const vector<string> &arguments);
        void       loadZoneBulkConfigurationHelp (void);

        ResourceId debugClusterStatistics (const vector<string> &arguments);

        ResourceId setTtyName (const vector<string> &arguments);
        ResourceId getVcenterStateMachineStatus (const vector<string> &arguments);

        ResourceId setModeChangeParameters (const vector<string> &arguments);
        void       setModeChangeParametersHelp (void);
        
        ResourceId getIsNosSmdReady ();

        ResourceId queryIsGvlanEnabled (const vector<string> &arguments);
        ResourceId queryIsGvlanCapable (const vector<string> &arguments);

        ResourceId createOpenFlowSwitch                             (const vector<string> &arguments);
        ResourceId createOpenFlowLogicalSwitch                      (const vector<string> &arguments);
        ResourceId addFlow                                          (const vector<string> &arguments);
        ResourceId addOpenFlowControllerToOpenFlowLogicalSwitch     (const vector<string> &arguments);
        ResourceId connectOpenFlowLogicalSwitchToOpenFlowController (const vector<string> &arguments);

    protected :
    public :
                                                DcmCliDebugShell                  (ValClientSynchronousConnection &connection);
        virtual                                ~DcmCliDebugShell                  ();

                ValClientSynchronousConnection &getValClientSynchronousConnection ();

    // Now the data members

    private :
        ValClientSynchronousConnection &m_valClientSynchronousConnection;

    protected :
    public :
};

}

#endif // DCMCLIDEBUGSHELL_H
