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

/*
 * SwitchAdapter.h
 *
 *  Created on: Jul 1, 2012
 *      Author: rmadapur
 */

#include "VirtualDb/DAO/DvpgDAO.h"
#include "VirtualDb/DAO/PgDAO.h"
#include "VirtualDb/DAO/VnicDAO.h"
#include "VirtualDb/DAO/SpecialNicDAO.h"

#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "DcmCore/DcmToolKit.h"

#ifndef SWITCHADAPTER_H_
#define SWITCHADAPTER_H_


namespace DcmNs {

    class SwitchAdapter {
        public:
            SwitchAdapter(string vCenter);
			SwitchAdapter();
            virtual ~SwitchAdapter();
            UI32 createPortProfile				(string dc_id,string pgName, bool isNodeRejoin=false);
            UI32 activatePortProfile			(string dc_id,string pgName, bool isNodeRejoin=false);
            UI32 deletePortProfile				(string dc_id,string pgName, bool isNodeRejoin=false);
			UI32 deletePortProfile              (string pName, bool isNodeRejoin=false);
            UI32 deActivatePortProfile			(string dc_id,string pgName, bool isNodeRejoin=false);
			UI32 deActivatePortProfile          (string pName, bool isNodeRejoin=false);

            UI32 createVLANProfile				(string dc_id,string pgName,string vlan, bool isNodeRejoin=false);
			UI32 createVlanProfile				(string vlan,string portProfile,vector<UI32> vlanRangeVector,UI32Range vlanAddrange, bool isNodeRejoin=false);
			UI32 updateVlanProfile				(string dc_id, string pgName,string vlan, bool isNodeRejoin=false);
			UI32 updateVlanProfile				(string portProfile,string vlan, bool isNodeRejoin=false);

            UI32 createProfileAndSubProfiles	(string dc_id,string pgName,string vlan,string qos="",bool isNodeRejoin=false);
			UI32 createProfileAndSubProfiles    (string portProfile, string vlan, bool isReconcile, string qos="", bool isNodeRejoin=false);

            UI32 associateMactoPortProfile		(string dc_id,string pgName,string mac, bool isNodeRejoin=false);
            UI32 deAssociateMactoPortProfile	(string dc_id,string pgName,string mac, bool isNodeRejoin=false);
			UI32 associateMacsToPortProfile		(string dc_id_pgName,vector <string> &macAddressList, bool isNodeRejoin=false);
			UI32 reassociateMacsToPortProfile   (string portProfile, vector	<string> &macAddressList, bool isNodeRejoin=false);
			UI32 deAssociateMacsFromPortProfile (string dc_id_pgName,vector <string> &macAddressList, bool isNodeRejoin=false);

            UI32 updateQoSProfile				(string dc_id, string pgName,string qos, bool isNodeRejoin=false);
			UI32 updateQoSProfile               (string portProfile,string qos, bool isNodeRejoin=false);

			UI32 createVlans					(string vlanScope, bool isNodeRejoin=false);
            UI32 configCdpProtocol				(bool enable);

            UI32 createRejoinCasePortProfile(string portProfile);
            UI32 activateRejoinCasePortProfile(string portProfile);
            UI32 deleteRejoinCasePortProfile(string portProfile);
            UI32 deActivateRejoinCasePortProfile(string portProfile);
            UI32 createRejoinCaseQoSProfileWithDefaultCOSForPortProfile (string portProfile, const UI32 cosValue);
            UI32 createRejoinCaseAccessVlanProfileWithVlanForPortProfile(string portProfile,const UI32 vlanId);
            UI32 createRejoinCaseTrunkVlanNoneProfilePortProfile(string portProfile);
            UI32 createRejoinCaseSwitchportTrunkNativeVlanProfile(string portProfile,const UI32 vlanId);
            UI32 createRejoinCaseTrunkVlanProfileWithVlanForPortProfile(const string portProfile, const UI32Range allowedVlans);
            UI32 createRejoinCaseTrunkVlanAllProfilePortProfile(string portProfile);
            UI32 createRejoinCaseVlanProfileForPortProfile(string portProfile);
            UI32 setRejoinCaseSwitchPortForVlanProfileForPortProfile(string portProfile);
            UI32 deleteRejoinCaseVlanProfileForPortProfile(string portProfile);
            UI32 setRejoinCaseSwitchPortModeTrunkForVlanProfileForPortProfile(string portProfile);
            UI32 deleteRejoinCaseQoSProfileForPortProfile(string portProfile);
            UI32 associateRejoinCaseMacsToPortProfile(const string portProfileName, vector <string> &macAddressList);
            UI32 deAssociateRejoinCaseMacsFromPortProfile(const string portProfileName, vector <string> &macAddressList);
            UI32 createRejoinCaseBulkVlanInterface(const UI32Range vlanRange);
            UI32 createRejoinCaseVlanInterface(const string name, const UI32 ifIndex, const UI32 mtu, const string description);

            UI32 removeRejoinCasePortProfileFromDefaultDomain (const string portProfile);
            UI32 setRejoinCaseDefaultDomainForPortProfile (const string portProfile);

        private:
            ValClientSynchronousConnection *pValClientSynchronousConnection;
            string vCenter;
    };

} /* namespace DcmNs */
#endif /* SWITCHADAPTER_H_ */
