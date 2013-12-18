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
 *   Author : Vidyasagara Reddy Guntaka, Kuai Yu                           *
 ***************************************************************************/

#ifndef DCMCLIENTSYNCHRONOUSCONNECTION_H
#define DCMCLIENTSYNCHRONOUSCONNECTION_H

#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "DcmResourceIdEnums.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
//#include "Ssm/Local/SsmTypes.h"

#include "ValClientInterface/ValClientPhysicalInterface.h"
#include "VcsFabric/Local/VcsClientLocalParams.h"
#include "VcsFabric/Local/VcsClientVcsIdParams.h"
#include "VcsFabric/Local/VcsClientVcsModeParams.h"

#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

#include "Framework/Types/UI32Range.h"
//#include "discover/Framework/EventEngineMessages.h"
//#include "discover/Framework/MigrationEventsMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"
#include "Framework/Types/BitMap.h"

struct vcslog_t;

#include <string>
#include <vector>

using namespace std;
using namespace WaveNs;

namespace DcmNs
{

// WARNING !!!
// The following intrfaces are still in the works and are NOT complete.


class DcmClientSynchronousConnection : public WaveClientSynchronousConnection
{
    private :
    protected :
    public :
                                                DcmClientSynchronousConnection           (const string &valClientname, const string &waveServerIpAddress, const UI32 &waveServerPort);
                                                DcmClientSynchronousConnection           ();
        virtual                                ~DcmClientSynchronousConnection           ();

        static  void                            initializeCApi                           (DcmClientSynchronousConnection *pDcmClientSynchronousConnection);
        static  void                            unInitializeCApi                         ();
        static  bool                            isCApiInitialized                        ();
        static  DcmClientSynchronousConnection *getDcmClientSynchronousConnectionForCApi (); 
        static  DcmClientSynchronousConnection *getDcmClientSynchronousConnectionForDualDaemons  ();

                ResourceId                      valQueryForInterface                     (const string &interfaceName, ValClientPhysicalInterface &valClientPhysicalInterface);
                ResourceId                      createPhysicalInterface                  (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
                ResourceId                      dcmNsmEventNotify                        (const UI32 &eventid, const UI32 &parameter);
                ResourceId                      vnmAppmCdpNotify                         (const UI32 cmdCode, const string ifName, const string hostMor, const string portId, const string vc);
                ResourceId                      vnmVcCdpNotify                         (const UI32 cmdCode, const string ifName, const string hostMor, const string portId, const string vc);
                ResourceId                      dcmNsmModifyInterfaceConfig              (void *intfConfig, unsigned int msg_id = 0);
                ResourceId                      dcmNsmNotifySlotOffline                  (const unsigned int cmdcode, const unsigned int slotid);
                ResourceId					    dcmNsmNotifyPortroleConfig				 (const string &name, unsigned int portrole, unsigned int exception, unsigned int msg_id);
                ResourceId                      dcmNsmNotifyPhyStatusConfig              (const string &name, const unsigned int flag, unsigned int msg_id);
                ResourceId                      dcmNsmNotifyPoStatusConfig               (const string &poName, const unsigned int flag);
                ResourceId                      isNodeInCluster                          (const unsigned int rbrigdeId, bool &isNodeInCluster);
                ResourceId                      isLocalNodeInCluster                     (bool &isInCluster);

                /* Port-Channel Interface */
                ResourceId                      createPoInterface                  (const string &name);
                ResourceId                      setPoPrimaryLinkDomainId           (const string &name, const unsigned int &poPrimaryLinkDomainId, const unsigned int &poPartnerSystemPriority,
                                                                                    const unsigned int &poPartnerKey, const string &poPartnerSystem,
                                                                                    const unsigned int &commandCode, unsigned int &replyCode);
                ResourceId                      setPoPvlanMode                     (const string &name, const int &mode);
                ResourceId                      setPoPvlanMapping                  (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      setPoPvlanTrunkAssociation         (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      setPoPvlanHostAssociation          (const string &name, const UI32 &name1, const UI32 &name2);
                ResourceId                      delPoPvlanMode                     (const string &name);
                ResourceId                      delPoPvlanMapping                  (const string &name, const UI32  &name1, const UI32  &name2);
                ResourceId                      delPoPvlanTrunkAssociation         (const string &name, const UI32  &name1, const UI32  &name2);
                ResourceId                      delPoPvlanHostAssociation          (const string &name, const UI32  &name1, const UI32  &name2);


                /* Vlan Interface */
                ResourceId                      createVlanInterface          (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
                ResourceId                      setVlanDesc                  (const string &name, const string &description);
                ResourceId                      setPvlanType                 (const string &name, const UI8 &type);
                ResourceId                      setSecondaryVlanId           (const string &name, const UI32 &vid);
                ResourceId                      setVlanProtocalStatus        (const string &name, const bool &flag);
                ResourceId                      delSecondaryVlanId           (const string &name, const UI32 &vid);

                /* Physical Interface */
                ResourceId                      createPhyInterface           (const string &name, const UI64 &ifIndex, const UI32 &mtu, const string &description, UI32 isl_capable = 0);


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

                ResourceId                      createAGFcoeInterfaceBulk (void *msg_p);
                ResourceId                      deleteAGFcoeInterfaceBulk (void *msg_p);

                ResourceId                      vcsGetVcsId                              (VcsClientVcsIdParams &vcsIdParams);
                ResourceId                      vcsGetVcsMode                            (VcsClientVcsModeParams &vcsModeParams);
                ResourceId                      vcsGetRBridgeId                          (UI32 &rbridgeId);
                ResourceId                      vcsGetThisNodeClusterType                (ResourceId &vcsClusterType);


                /* Config File Management */
                ResourceId                      isConfigFileAllowed (SI32 &isAllowed);
                ResourceId                      copyToStartupConfig (const string &path);
                ResourceId                      copyRunningToStartup(void);
                ResourceId                      copyDefaultToStartup(void);
                ResourceId                      isCliAllowedAtThisLocation (SI32 &isCliAllowed);

                /* Copy Snapshot */
                ResourceId                      copySnapshotFromFtp ( const string &snapshotId, const UI32 &rbridgeId, const string &localFilename, const string &globalFilename);

				/* legacy FOS CLI interfaces */
				ResourceId                      fosCliPortPersistent (const UI32 slot, const UI32 port, const UI32 persiststate);
				ResourceId                      fosCliPortTrunk (const UI32 slot, const UI32 port, const UI32 trunkcapable);


				/* external PoD License interface method */
				ResourceId 						podUpdateLicenseData (const char *setName, const int len, const char *configString);


               /* Client APIs for used for fabric formation */

               ResourceId                      getConfigurationNumber (UI16 configurationNumber[16]);
               ResourceId                      getConfigurationTime (char *p_configurationTime, const int configurationTimeStringSize);
               ResourceId                      isFabricDistributionEnabled (bool &isDistributionEnabled);

               ResourceId                      isGvlanCapable               (bool &gvlanCapable);
               ResourceId                      isGvlanEnabled               (bool &gvlanEnabled);

       static  void                            initializeSynchronousConnectionForCcm            (DcmClientSynchronousConnection *pDcmClientSynchronousConnection);
       static  void                            unInitializeSynchronousConnectionForCcm          ();
       static  bool                            isSynchronousConnectionForCcmInitialized         ();
              ResourceId                       test_api_for_dual_connection                     ();

              ResourceId                       getCurrentRbridgeIdsAndLocationIds (vector<UI32> &rbridgeIds, vector<LocationId> &locationIds);

              ResourceId                       runDebugScript (const string & fullyQualifiedScriptFileName, const string & nosOrbash, const string & fullyQualifiedOutputFileName = "", const string & rbridgeIdRange = "");

              ResourceId                      vcsNodeLocalSetFcfMac                    (const char pFcfMac[6]);

              ResourceId                      vcsNodeLocalGetMappedId                  (VcsClientLocalParams &vParams);

              ResourceId                      vcsNodeGetRBridgeIdFromWWN               (UI32 &rBridgeId, const WorldWideName &worldWideName);

              ResourceId                      vcsNodeGetWWNFromRBridgeId               (WorldWideName &worldWideName, const UI32 rBridgeId);

              ResourceId                      vcsNodeGetIPFromRBridgeId                (const unsigned int rBridgeId, string &ipAddress);

              ResourceId                      vcsGetClusterRbridgeIdFromWWN            (const WorldWideName &wwn, BitMap &rbridgeIdBitMap);

              ResourceId                      vcsRASLogging                            (vcslog_t *vcslog, const string &vcslogString);

			  ResourceId                      createStickyMacs 						   (const void *data, const unsigned int size);

              void                            retrievePowerStatus                      (vnm_req_t *req,UI32 &message_id,vnm_resp_t **resp);
 
              ResourceId dcmNsmTriggerVlanUnprovision();
              ResourceId                      loadZoneBulkConfiguration                (const string &zoneConfigurationFilePath);
    // Now the Data Members

    private :
        static DcmClientSynchronousConnection *m_pDcmClientSynchronousConnectionForCApi;
        static DcmClientSynchronousConnection *m_pDcmClientSynchronousConnectionForCApiWithCcm;

    protected :
		// method used to parse the port numbers passed in during link-up events
		int extractPortNumbers (const char *configString, int portList[]);

    public :
};

}

#endif // DCMCLIENTSYNCHRONOUSCONNECTION_H
