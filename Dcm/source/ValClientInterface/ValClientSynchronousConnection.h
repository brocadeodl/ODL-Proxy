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

#ifndef VALCLIENTSYNCHRONOUSCONNECTION_H
#define VALCLIENTSYNCHRONOUSCONNECTION_H

#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Ssm/Local/SsmTypes.h"

#include "ValClientInterface/ValClientPhysicalInterface.h"
#include "VcsFabric/Local/VcsClientLocalParams.h"
#include "VcsFabric/Local/VcsClientVcsIdParams.h"
#include "VcsFabric/Local/VcsClientVcsModeParams.h"
#include "Framework/Types/BitMap.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

#include "Framework/Types/UI32Range.h"
#include "discover/Framework/EventEngineMessages.h"
#include "discover/Framework/MigrationEventsMessage.h"
#include "APPM/Local/APPMLocalAPPMProfileMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"
#include "correlator/PortProfileObject.h"

#include "Framework/Types/Uri.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"

struct vcslog_t;

#include <string>
#include <vector>

using namespace std;
using namespace WaveNs;
#define MIN_VCENTER_EVENT_MESSAGE_FIELD_CNT 12
namespace DcmNs
{

// WARNING !!!
// The following intrfaces are still in the works and are NOT complete.


class ValClientSynchronousConnection : public WaveClientSynchronousConnection
{
    private :
    protected :
    public :
                                                ValClientSynchronousConnection           (const string &valClientname, const string &waveServerIpAddress, const UI32 &waveServerPort);
                                                ValClientSynchronousConnection           ();
        virtual                                ~ValClientSynchronousConnection           ();

        static  void                            initializeCApi                           (ValClientSynchronousConnection *pValClientSynchronousConnection);
        static  void                            unInitializeCApi                         ();
        static  bool                            isCApiInitialized                        ();
        static  ValClientSynchronousConnection *getValClientSynchronousConnectionForCApi ();

                ResourceId                      valQueryForInterface                     (const string &interfaceName, ValClientPhysicalInterface &valClientPhysicalInterface);

                ResourceId                      createPhysicalInterface                  (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);

                ResourceId                      dcmNsmEventNotify                       (const UI32 &eventid, const UI32 &parameter);
                ResourceId                      vnmAppmCdpNotify                        (const UI32 cmdCode, const string ifName, const string hostMor, const string portId);
                ResourceId                      dcmNsmModifyInterfaceConfig             (void *intfConfig, unsigned int msg_id = 0);
                ResourceId					    dcmNsmNotifyPortroleConfig				(const string &name, unsigned int portrole, unsigned int exception, unsigned int msg_id);
                ResourceId                      dcmNsmNotifyPhyStatusConfig              (const string &name, const unsigned int flag, unsigned int msg_id);
                ResourceId                      isDcmdReadyForConnection                (bool &isReady);
                ResourceId                      notifyWyserEaPhase2Complete               ();
                ResourceId                      isDcmdReadyForHasmLaunch                (bool &isDcmdReadyForHasmLaunch);
                ResourceId                      setHasmLaunched                         (bool &hasmLaunchState);
				ResourceId                      HasmSetMvCFeature 						(bool hasmMvCState);
                ResourceId                      isNodeInCluster                         (const unsigned int rbrigdeId, bool &isNodeInCluster);
                ResourceId                      isLocalNodeInCluster                    (bool &isInCluster);
                ResourceId                      vcsNodeGetIPFromRBridgeId               (const unsigned int rBridgeId, string &ipAddress);
                ResourceId                      isClusterConnectedtoIpAddress               (const string &ipAddress, vector<IpAddressPingResponseParameters*>& connectedNodesList);

                /* Port-Channel Interface */
                ResourceId                      createPoInterface                  (const string &name);
                ResourceId                      deletePoInterface                  (const string &name);
                ResourceId                      setPoDesc                  (const string &name, const string &description);
                ResourceId                      setPoProtocalStatus                  (const string &name, const bool &flag);
                ResourceId                      delPoProtocalStatus                  (const string &name, const bool &flag);

                ResourceId                      delPoDesc                  (const string &name, const string &description);
                ResourceId                      setPoSwitchPort                  (const string &name);
                ResourceId                      setPoMode                  (const string &name, const int &mode);
                ResourceId                      setPoPrimaryLinkDomainId   (const string &name, const unsigned int &poPrimaryLinkDomainId, const unsigned int &poPartnerSystemPriority,
                                                                                    const unsigned int &poPartnerKey, const string &poPartnerSystem,
                                                                                    const unsigned int &commandCode, unsigned int &replyCode);
                ResourceId                      setPoPvlanMode                  (const string &name, const int &mode);
                ResourceId                      setPoPvlanMapping          (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      setPoPvlanTrunkAssociation          (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      setPoPvlanHostAssociation          (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      delPoSwitchPort                  (const string &name);
                ResourceId                      delPoMode                  (const string &name);
                ResourceId                      delPoPvlanMode                  (const string &name);
                ResourceId                      delPoPvlanMapping          (const string &name, const UI32  &name1, const UI32  &name2);
                ResourceId                      delPoPvlanTrunkAssociation          (const string &name, const UI32  &name1, const UI32  &name2);
                ResourceId                      delPoPvlanHostAssociation          (const string &name, const UI32  &name1, const UI32  &name2);


                /* Vlan Interface */
                ResourceId                      createVlanInterface                  (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
                ResourceId                      deleteVlanInterface                  (const string &name);
                ResourceId                      setVlanDesc                  (const string &name, const string &description);
                ResourceId                      setPvlanType                 (const string &name, const UI8 &type);
                ResourceId                      setSecondaryVlanId           (const string &name, const UI32 &vid);
                ResourceId                      setVlanProtocalStatus                  (const string &name, const bool &flag);
                ResourceId                      delVlanDesc                  (const string &name, const string &description);
                ResourceId                      delPvlanType                 (const string &name);
                ResourceId                      delSecondaryVlanId           (const string &name, const UI32 &vid);
                ResourceId                      delVlanProtocalStatus                  (const string &name, const bool &flag);

                /* Physical Interface */
                ResourceId                      createPhyInterface                  (const string &name, const UI64 &ifIndex, const UI32 &mtu, const string &description, UI32 isl_capable = 0);
                ResourceId                      deletePhyInterface                  (const string &name);
                ResourceId                      setPhyDesc                  (const string &name, const string &description);
                ResourceId                      setPhyProtocalStatus                  (const string &name, const bool &flag);
                ResourceId                      delPhyDesc                  (const string &name, const string &description);
                ResourceId                      delPhyProtocalStatus                  (const string &name, const bool &flag);

                ResourceId                      setPhySwitchPort                  (const string &name);
                ResourceId                      setPhyMode                  (const string &name, const int &mode);
                ResourceId                      delPhySwitchPort                  (const string &name);
                ResourceId                      delPhyMode                  (const string &name);

                /* FCOE Logical Interface */
                ResourceId                      createFcoeInterface (const string &name, const UI64 &ifIndex,
                                                                     const string &bind_name, const UI32 &bind_type,
                                                                     const UI32 &fc_uport_num, const UI32 &vn_num,
                                                                     const UI32 &port_num, const string &description);
                ResourceId                      createFcoeInterfaceBulk (const void *msg_p,
                                                                         const unsigned int size);
                ResourceId                      deleteFcoeInterfaceBulk (const void *msg_p,
                                                                         const unsigned int size);
                ResourceId                      createFcoeETInterfaceBulk (const void *msg_p,
                                                                           const unsigned int size);
                ResourceId                      updateFcoeETInterfaceBulk (const void *data_p,
                                                           const unsigned int size,
                                                           const UI32 oper);

                /* FCOE ET interface */
                ResourceId                      createFcoeETInterface (const string &name,
                                                                       const UI64 &ifIndex,
                                                                       const UI32 &fc_uport_num);

                ResourceId                      createPortCfgInterface (const string &name, const UI32 &fc_uport_num);

                ResourceId 			setBridgeXstp(const vector<string> & args);
                ResourceId 			setVlanBridgeXstp(const vector<string> & args);


                ResourceId                      vcsNodeLocalSetFcfMac                    (const char pFcfMac[6]);
                ResourceId                      vcsNodeLocalSetFcfMacServiceIndependent  (const char * pfcfMac );
                ResourceId                      vcsNodeLocalGetMappedId                  (VcsClientLocalParams &vParams);
                ResourceId                      vcsGetVcsId                              (VcsClientVcsIdParams &vcsIdParams);
                ResourceId                      vcsGetVcsMode                            (VcsClientVcsModeParams &vcsModeParams);
                ResourceId                      setNewShowVcs                            (bool newShowVcs);
                ResourceId                      vcsGetRBridgeId                          (UI32 &rbridgeId);
                ResourceId                      vcsGetThisNodeClusterType                (ResourceId &vcsClusterType);
                ResourceId						AssocPolicyToPhyInterface				 (const string &ifName, const string &name, const UI32 &opcode);
                ResourceId						AssocPolicyToLogInterface				 (const string &ifName, const string &name, const UI32 &opcode);
                ResourceId						macAclPolicy(const string &policyName, const UI32 &policyType, const UI32 &opcode);
                ResourceId 					AssocRuleToPolicy(const string &policyName,const UI32 &policyType,const UI32 &ruleid,const UI32 &action,const string &srcMac,const AddrType &srcType, const bool &srcMask,const string &dstMac, const AddrType &dstType, const bool &dstMask,const string &appType, const bool &isCount,const UI32 &opcode);

		ResourceId vcsRASLogging (vcslog_t *vcslog, const string &vcslogString);

                /* Config File Management */
                ResourceId                      isConfigFileAllowed (SI32 &isAllowed);
                ResourceId                      copyToStartupConfig (const string &path);
                ResourceId                      copyRunningToStartup(void);
                ResourceId                      copyDefaultToStartup(void);

				/* legacy FOS CLI interfaces */
				ResourceId                      fosCliPortPersistent (const UI32 slot, const UI32 port, const UI32 persiststate);
				ResourceId                      fosCliPortTrunk (const UI32 slot, const UI32 port, const UI32 trunkcapable);

                ResourceId                      installDemoObject ();
                ResourceId                      messageLeaksStats (vector<string> arguments);
                ResourceId                      dumpObjectLeaks   (vector<string> arguments);
               ResourceId                      configureFileService (const string &sArgument);
               ResourceId                      changeBtTrackerState (vector<string> arguments);

				/* external PoD License interface method */
				ResourceId 						podUpdateLicenseData (const char *setName, const int len, const char *configString);
                ResourceId                      disconnectInstanceClients (vector<string> arguments);
                ResourceId                      queryLinecardType (const UI32 &slotNumber, UI32 &slotType);
                ResourceId                      notifySystemShutdown (const string &databaseDirectory);

        // APPM

                ResourceId                      createPortProfile                                     (const string &portProfileName);
                ResourceId                      deletePortProfile                                     (const string &portProfileName);
                ResourceId                      createVlanProfileForPortProfile                       (const string &portProfileName);
                ResourceId                      setSwitchPortForVlanProfileForPortProfile             (const string &portProfileName);
                ResourceId                      setSwitchPortModeAccessForVlanProfileForPortProfile   (const string &portProfileName);
                ResourceId                      setSwitchPortAccessVlanForVlanProfileForPortProfile   (const string &portProfileName, const UI32 &vlanId);
                ResourceId                      setSwitchPortModeTrunkForVlanProfileForPortProfile    (const string &portProfileName);
                ResourceId                      setSwitchPortAllowedVlansForVlanProfileForPortProfile (const string &portProfileName, const UI32Range &allowedVlans);
                ResourceId                      activatePortProfile                                   (const string &portProfileName);
                ResourceId                      deactivatePortProfile                                 (const string &portProfileName);
                ResourceId                      associateMacToPortProfile                             (const string &portProfileNAme, const string &macAddress);

               ResourceId                      configCdpProtocol (bool &cenable);
                //vCeneter Lyra

                ResourceId                      createAccessVlanProfileWithVlanForPortProfile         (const string &portProfileName, const UI32 &vlanId);
                ResourceId                      updateAccessVlanProfileWithVlanForPortProfile         (const string &portProfileName, const UI32 &vlanId);
                ResourceId                      createTrunkVlanProfileWithVlanForPortProfile          (const string &portProfileName, const UI32Range &allowedVlans);
                ResourceId                      updateTrunkVlanProfileWithVlanForPortProfile          (const string &portProfileName, const UI32Range &allowedVlans);
                ResourceId                      deleteVlanProfileForPortProfile                       (const string &portProfileName);

                ResourceId                      associateMacToVcenterPortProfile                      (const string &portProfileName, const string &macAddress);
                ResourceId                      associateMacsToPortProfile                            (const string &portProfileName, vector <string> &macAddressList);
                ResourceId                      deAssociateMacsFromPortProfile                        (const string &portProfileName, vector <string> &macAddressList);
                ResourceId                      disassociateMacFromVcenterPortProfile                 (const string &portProfileName, const string &macAddress);

                ResourceId                      createQoSProfileWithDefaultCOSForPortProfile          (const string &portProfileName, const UI32 &cosValue);
                ResourceId                      updateQoSProfileWithDefaultCOSForPortProfile          (const string &portProfileName, const UI32 &cosValue);
                ResourceId                      deleteQoSProfileForPortProfile                        (const string &portProfileName);

                ResourceId                      createBulkVlanInterface                               (const UI32Range &vlanRange);

				ResourceId 						createTrunkVlanNoneProfilePortProfile				  (const string &portProfileName);
				ResourceId       				createTrunkVlanAllProfilePortProfile				  (const string &portProfileName);
				ResourceId 						createSwitchportTrunkNativeVlanProfile 				  (const string &portProfileName, const UI32 &vlanId);
				ResourceId                      getAllPortProfilesFromDcmd 							  (string pattern, vector<string> &portprofileList);
				ResourceId                      getProfileInfoFromDcmdDB 							  (PortProfileObject &pPortProfileObject);

                ResourceId                      setDefaultDomainForPortProfile                        (const string &portProfileName);
                ResourceId                      removePortProfileFromDefaultDomain                    (const string &portProfileName);

                // vCenter Lyra End


               /* Client APIs for used for fabric formation */
               ResourceId                      vcsNodeGetWWNFromRBridgeId (WorldWideName &worldWideName, const UI32 rBridgeId);
               ResourceId                      vcsNodeGetRBridgeIdFromWWN (UI32 &rBridgeId, const WorldWideName &worldWideName);

               ResourceId                      getConfigurationNumber (UI16 configurationNumber[16]);
               ResourceId                      getConfigurationTime (char *p_configurationTime, const int configurationTimeStringSize);

               ResourceId                      vcsGetClusterRbridgeIdFromWWN (const WorldWideName &wwn, BitMap &rbridgeIdBitMap);
               ResourceId                      setModeChangeParameters (const UI32 &vcsId, const string &vcsMode, const UI32 &rbridgeId, const bool &rebootSw);

               ResourceId                      isFabricDistributionEnabled (bool &isDistributionEnabled);

               ResourceId                      isGvlanEnabled (bool &gvlanEnabled);
               ResourceId                      isGvlanCapable (bool &gvlanCapable);

               ResourceId                      loadZoneBulkConfiguration (const string &zoneConfigurationFilePath);

       static  void                            initializeSynchronousConnectionForCcm            (ValClientSynchronousConnection *pValClientSynchronousConnection);
       static  void                            unInitializeSynchronousConnectionForCcm          ();
       static  bool                            isSynchronousConnectionForCcmInitialized         ();
        // This method is to be used along with dcm_initialize_client_for_dual_daemons. dcm_initialize_client_for_dual_daemons creates two connections from Pd
        // to Dcmd and Ccmd in case node is in Fabric cluster mode. Hence, it is necessary to use the method getValClientSynchronousConnectionForDualDaemons.
        // This method returns the connection with Ccmd in case of Fabric cluster mode else the conenction with Dcmd.
       static ValClientSynchronousConnection  *getValClientSynchronousConnectionForDualDaemons  ();
              ResourceId                       test_api_for_dual_connection                     ();

              ResourceId                       getCurrentRbridgeIdsAndLocationIds (vector<UI32> &rbridgeIds, vector<LocationId> &locationIds);
              ResourceId                       runDebugScript (const string & fullyQualifiedScriptFileName, const string & nosOrbash, const string & fullyQualifiedOutputFileName = "", const string & rbridgeIdRange = "");

              void                              retrievePowerStatus (vnm_req_t *req,UI32 &message_id,vnm_resp_t **resp);
              ResourceId                        dcmvCenterEventNotify (const vector<string> &arguments);

               /* Client APIs for used for Ha Sync*/
               ResourceId                      sendSyncUpdateToCcmd (const string &peerHaIpAddress, const SI32 &peerHaIpPort, const UI32 &context);
			   
			  ResourceId 				      setIsCliAllowedFromThisLocation (bool isCliAllowedFromThisLocation);
               ResourceId                      getClusterStatistics (WaveClientMessageCallbackHandler handler);
               ResourceId                      setTtyName           (const string &ttyName);
               ResourceId retrievevCenterMachineStatus( vector<vector<string> > &respList);
               ResourceId                      isNosSmdReadyForConnection (bool &isReady);
			   //This method is used to enable/disable a feature in which will block firmware upgrade/downgrades.
				ResourceId						setHasmFeature(char *feature, int state);

        // OpenFlow Related API

               ResourceId                      createOpenFlowSwitch                             (const string &openFlowSwitchName,        const Uri &openFlowSwitchUri);
               ResourceId                      createOpenFlowLogicalSwitch                      (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri);
               ResourceId                      addFlow                                          (const UI32 &ingressIfIndex, const UI32 &egressIfIndex, const UI32 &srcIpV4Address, const UI32 &srcIpV4AddressMask, const UI32 &dstIpV4Address, const UI32 &dstIpV4AddressMask, const string &appType, const UI16 &srcPort, const UI16 &dstPort );
               ResourceId                      addOpenFlowControllerToOpenFlowLogicalSwitch     (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri, const IpV4AddressNetworkMask ipPrefix, UI32 port);
               ResourceId                      connectOpenFlowLogicalSwitchToOpenFlowController (const string &openFlowLogicalSwitchName, const Uri &openFlowLogicalSwitchUri, const IpV4AddressNetworkMask ipPrefix, UI32 port);


    // Now the Data Members

    private :
        static ValClientSynchronousConnection *m_pValClientSynchronousConnectionForCApi;
        static ValClientSynchronousConnection *m_pValClientSynchronousConnectionForCApiWithCcm;

    protected :
		// method used to parse the port numbers passed in during link-up events
		int extractPortNumbers (const char *configString, int portList[]);

    public :
};
//extern UI32 getNextPPid();

}

#endif // VALCLIENTSYNCHRONOUSCONNECTION_H
