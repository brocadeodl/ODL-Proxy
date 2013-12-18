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
 * SwitchAdapter.cpp
 *
 *  Created on: Jul 1, 2012
 *      Author: rmadapur
 */

#include "correlator/SwitchAdapter.h"
#include "discover/utils/URLEncode.h"
#include "APPM/Local/APPMLocalAPPMProfileMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include <APPM/Global/APPMGlobalAPPMProfileMessage.h>
#include <APPM/Global/APPMGlobalAPPMProfileBulkMessage.h>
#include "APPM/Global/APPMGlobalAPPMVlanMessage.h"
#include "APPM/Local/APPMVnmLocalCdpNotifyMessage.h"
#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "APPM/Local/APPMLocalAPPMVlanMessage.h"
#include "APPM/Local/APPMLocalAPPMProfileBulkMessage.h"
#include "APPM/Local/APPMLocalProfileDomainMessage.h"
#include "APPM/Local/APPMLocalAPPMQosMessage.h"
#include "Nsm/Local/NsmCreateVlanIntfSPMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Utils/Integer.h"
#include "discover/utils/FileUtils.h"

#define PROFILE_NAME(vcenter_Name,datacenter,port_group)  "auto_" + vcenter_Name + "_" + URLEncode::urlencode(datacenter) + "_" + URLEncode::urlencode(port_group);

#define PROFILE_NAME_BULK(vcenter_Name,dc_port_group) "auto_" + vcenter_Name + "_" + dc_port_group;

#define MAC_BULK_REQUEST_SIZE 200
namespace DcmNs {

    SwitchAdapter::SwitchAdapter(string _vCenter) :
            vCenter(_vCenter) {
        // TODO Auto-generated constructor stub

        pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();

        prismAssert(NULL != pValClientSynchronousConnection, __FILE__,
                __LINE__);

    }

    SwitchAdapter::SwitchAdapter() {
          pValClientSynchronousConnection = DcmToolKit::getInterDcmCcmClient();
          prismAssert(NULL != pValClientSynchronousConnection, __FILE__, __LINE__);
      }

    SwitchAdapter::~SwitchAdapter() {
        // TODO Auto-generated destructor stub
    }
    UI32 SwitchAdapter::createPortProfile(string dc_id, string pgName,bool isNodeRejoin) {
        string      portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
        if((isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
          return createRejoinCasePortProfile(portProfile);
        }else {
          return pValClientSynchronousConnection->createPortProfile(portProfile);
        }
    }

    UI32 SwitchAdapter::createRejoinCasePortProfile(string portProfile)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCasePortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMProfileMessage *m = new APPMLocalAPPMProfileMessage();
          m->setOpCode(APPM_CREATE);
          m->setCmdCode(PORT_PROFILE_CREATE);
          m->setProfileName(portProfile);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);            
          delete m;
          return status;

    }

    UI32 SwitchAdapter::activatePortProfile(string dc_id, string pgName,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
        if( (isNodeRejoin) &&  (FileUtils::isvCenterInManagementCluster()))
        {
            return activateRejoinCasePortProfile(portProfile);
        }else {
            return pValClientSynchronousConnection->activatePortProfile(portProfile);
       }
    }

    UI32 SwitchAdapter::activateRejoinCasePortProfile(string portProfile)
    {
            trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCasePortProfile: ") + portProfile); 
            UI32 status = WAVE_MESSAGE_SUCCESS;
            APPMLocalAPPMProfileMessage *m = new APPMLocalAPPMProfileMessage();
            m->setOpCode(APPM_SET_ELEM);
            m->setProfileName(portProfile);
            m->setCmdCode(PORT_PROFILE_ACTIVATE);
            status =  WaveObjectManagerToolKit::sendSynchronously(m);
            delete m;
            return status;
    }

    UI32 SwitchAdapter::deletePortProfile(string dc_id, string pgName,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
        UI32 status = WAVE_MESSAGE_SUCCESS;

        trace(TRACE_LEVEL_INFO, string("Removing PP Default domain"));
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            status = removeRejoinCasePortProfileFromDefaultDomain(portProfile);
        }
        else {
            status = pValClientSynchronousConnection->removePortProfileFromDefaultDomain(portProfile);
        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, string("Remove PP from Default domain FAILED!!"));
            //return status;
        }

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return deleteRejoinCasePortProfile(portProfile);
        } else {
            return pValClientSynchronousConnection->deletePortProfile(portProfile);
       }
    }

    UI32 SwitchAdapter::deleteRejoinCasePortProfile(string portProfile)
    {   
            trace (TRACE_LEVEL_INFO, string("NODE-REJOIN deleteRejoinCasePortProfile: ") + portProfile);
            UI32 status = WAVE_MESSAGE_SUCCESS;
            APPMLocalAPPMProfileMessage *m = new APPMLocalAPPMProfileMessage();
            m->setOpCode(APPM_REMOVE);
            m->setCmdCode(PORT_PROFILE_DELETE);
            m->setProfileName(portProfile);
            status =  WaveObjectManagerToolKit::sendSynchronously(m);
            delete m;
            return status;
    }

    UI32 SwitchAdapter::deActivatePortProfile(string dc_id, string pgName,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return deActivateRejoinCasePortProfile(portProfile);    
        }
        else{
            return pValClientSynchronousConnection->deactivatePortProfile(portProfile);
        }
    }

    UI32 SwitchAdapter::deActivateRejoinCasePortProfile(string portProfile)
    {
            trace (TRACE_LEVEL_INFO, string("NODE-REJOIN deActivateRejoinCasePortProfile: ") + portProfile);
            UI32 status = WAVE_MESSAGE_SUCCESS;
            APPMLocalAPPMProfileMessage *m = new APPMLocalAPPMProfileMessage();
            m->setOpCode(APPM_SET_ELEM);
            m->setProfileName(portProfile);
            m->setCmdCode(PORT_PROFILE_DEACTIVATE);
            status =  WaveObjectManagerToolKit::sendSynchronously(m);
            delete m;
            return status;
    }

    UI32 SwitchAdapter::createRejoinCaseBulkVlanInterface(const UI32Range vlanRange)
    {
    vector <UI32> vlanCreateVector;
    vlanRange.getUI32RangeVector(vlanCreateVector);
    vector<UI32>::iterator vlaniter;
    UI32 status = WAVE_MESSAGE_SUCCESS;

    for (vlaniter=vlanCreateVector.begin(); vlaniter != vlanCreateVector.end(); vlaniter++) {

            string vlanStr = (Integer (*vlaniter)).toString();
            if(vlanStr =="0" || vlanStr =="4091" || vlanStr =="4092" || vlanStr =="4093" || vlanStr =="4094" || vlanStr =="4095") {
                trace (TRACE_LEVEL_INFO, "skipping vlan: " + vlanStr);
                continue;
            }
            trace (TRACE_LEVEL_INFO, "creating vlan: " + vlanStr);
            status = createRejoinCaseVlanInterface (vlanStr,0,0,"");
        }
    return status;
    }

    UI32 SwitchAdapter::createRejoinCaseVlanInterface(const string name, const UI32 ifIndex, const UI32 mtu, const string description)
    {   
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseVlanInterface: ") + name);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          UI32 vlanId = atoi(name.c_str());
          NsmCreateVlanIntfSPMessage *m = new NsmCreateVlanIntfSPMessage();
          m->setVlanId (vlanId);
          m->setVlanName (name);
          m->setDesc (description);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::deletePortProfile(string portProfile,bool isNodeRejoin) {

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return deleteRejoinCasePortProfile(portProfile);
        } else {
            return pValClientSynchronousConnection->deletePortProfile(portProfile);
        }
    }
    UI32 SwitchAdapter::deActivatePortProfile(string portProfile,bool isNodeRejoin) {

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return deActivateRejoinCasePortProfile(portProfile);
        }
        else{
            return pValClientSynchronousConnection->deactivatePortProfile( portProfile);
        }
    }

	UI32 SwitchAdapter::createVlans(string vlanScope,bool isNodeRejoin){

      if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
      {
          return createRejoinCaseBulkVlanInterface(vlanScope);
      }else {
    	  return pValClientSynchronousConnection->createBulkVlanInterface(vlanScope);
      }
	}

    UI32 SwitchAdapter::createProfileAndSubProfiles(string dc_id, string pgName, string vlan, string qos,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
		UI32 ret = createProfileAndSubProfiles(portProfile,vlan,false,qos,isNodeRejoin);
		return ret;
    }

    UI32 SwitchAdapter::createProfileAndSubProfiles(string portProfile, string vlan,  bool isReconcile, string qos, bool isNodeRejoin) {
        vector<UI32> vlanRangeVector;
		set<UI32> vlanRangeCreateSet;
		trace(TRACE_LEVEL_INFO, string("createProfileAndSubProfiles: vlan string= ") + vlan);
        UI32Range vlanAddrange = UI32Range("");
        UI32 status = WAVE_MESSAGE_SUCCESS;

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            status = createRejoinCasePortProfile(portProfile);
        } else
        {
            status = pValClientSynchronousConnection->createPortProfile(portProfile);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
            return status;
        }

		if( vlan == "" || vlan == "," || vlan == " ")
		{
		  trace(TRACE_LEVEL_ERROR, string("Empty vlan string received"));
		  return WAVE_MESSAGE_ERROR;
		}

        vlanAddrange.fromString(vlan);
        vlanAddrange.getUI32RangeVector(vlanRangeVector);

		status = createVlanProfile(vlan, portProfile,vlanRangeVector,vlanAddrange,isNodeRejoin);

        if (status != WAVE_MESSAGE_SUCCESS) {
            return status;
        }
        if (qos.size() != 0) {
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
            {
                status = createRejoinCaseQoSProfileWithDefaultCOSForPortProfile(portProfile,atoi(qos.c_str()));
            }else{
                status = pValClientSynchronousConnection->createQoSProfileWithDefaultCOSForPortProfile( portProfile, atoi(qos.c_str()));
            }
            if (status != WAVE_MESSAGE_SUCCESS) {
                return status;
            }
        }

        trace(TRACE_LEVEL_INFO, string("Setting Default domain"));
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            status = setRejoinCaseDefaultDomainForPortProfile(portProfile);
        }
        else {
            status = pValClientSynchronousConnection->setDefaultDomainForPortProfile(portProfile);
        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, string("Setting Default domain FAILED!!"));
            //return status;
        }

        if((isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return activateRejoinCasePortProfile(portProfile);
        }else{
            return pValClientSynchronousConnection->activatePortProfile(portProfile);
        }
    }

    UI32 SwitchAdapter::createRejoinCaseQoSProfileWithDefaultCOSForPortProfile (string portProfile, const UI32 cosValue)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseQoSProfileWithDefaultCOSForPortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMQosMessage *m = new APPMLocalAPPMQosMessage();
          m->setOpCode(APPM_CREATE);
          m->setCmdCode(QOS_PROFILE_CREATE);
          m->setProfileName(portProfile);
          m->setQosTrust(false);
          m->setDefaultCosValue(cosValue);
          m->setFlowControlTxEnable(0);
          m->setFlowControlRxEnable(0);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::createRejoinCaseAccessVlanProfileWithVlanForPortProfile(string portProfile,const UI32 vlanId)
    {
          trace (TRACE_LEVEL_INFO,  string("NODE-REJOIN createRejoinCaseAccessVlanProfileWithVlanForPortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_ACCESS_VLAN_PROFILE_CREATE);
          m->setProfileName(portProfile);
          m->setSwModeFlag(true);
          m->setSwMode(VLAN_SW_MODE_ACCESS);
          m->setAccessVlan(vlanId);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::createRejoinCaseTrunkVlanNoneProfilePortProfile(string portProfile)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseTrunkVlanNoneProfilePortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE);
          m->setProfileName(portProfile);
          m->setSwModeFlag(true);
          m->setSwMode(VLAN_SW_MODE_TRUNK);
          m->setIsAllowedVlanNone(true);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::createRejoinCaseSwitchportTrunkNativeVlanProfile(string portProfile,const UI32 vlanId)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseSwitchportTrunkNativeVlanProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_TRUNK_VLAN_PROFILE_NATIVE_VLAN);
          m->setProfileName(portProfile);
          m->setNativeVlanId(vlanId);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::createRejoinCaseTrunkVlanProfileWithVlanForPortProfile(const string portProfile, const UI32Range allowedVlans)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseTrunkVlanProfileWithVlanForPortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_TRUNK_VLAN_PROFILE_CREATE);
          m->setProfileName(portProfile);
          m->setSwModeFlag(true);
          m->setSwMode(VLAN_SW_MODE_TRUNK);
          m->setVlanRange(allowedVlans);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }
    
    UI32 SwitchAdapter::createRejoinCaseTrunkVlanAllProfilePortProfile(string portProfile)
    {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseTrunkVlanAllProfilePortProfile: ") + portProfile);
          UI32 status = WAVE_MESSAGE_SUCCESS;
          APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_TRUNK_VLAN_PROFILE_CREATE_ALLOWED_NONE);
          m->setProfileName(portProfile);
          m->setSwModeFlag(true);
          m->setSwMode(VLAN_SW_MODE_TRUNK);
          m->setIsAllowedVlanNone(true);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
    }

    UI32 SwitchAdapter::createRejoinCaseVlanProfileForPortProfile(string portProfile)
    {
        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN createRejoinCaseVlanProfileForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
        m->setOpCode(APPM_CREATE);
        m->setCmdCode(VLAN_PROFILE_CREATE);
        m->setProfileName(portProfile);
        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::setRejoinCaseSwitchPortForVlanProfileForPortProfile(string portProfile)
    {
        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN setRejoinCaseSwitchPortForVlanProfileForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
        m->setOpCode(APPM_SET_ELEM);
        m->setCmdCode(VLAN_SW_MODE_FLAG);
        m->setProfileName(portProfile);
        m->setSwModeFlag(true);
        m->setSwMode(VLAN_SW_MODE_ACCESS);
        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::deleteRejoinCaseVlanProfileForPortProfile(string portProfile)
    {
        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN deleteRejoinCaseVlanProfileForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
        m->setOpCode(APPM_VCENTER);
        m->setCmdCode(VCENTER_VLAN_PROFILE_DELETE);
        m->setProfileName(portProfile);
        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::setRejoinCaseSwitchPortModeTrunkForVlanProfileForPortProfile(string portProfile)
    {
        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN setRejoinCaseSwitchPortModeTrunkForVlanProfileForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMVlanMessage *m = new APPMLocalAPPMVlanMessage();
        m->setOpCode(APPM_SET_ELEM);
        m->setCmdCode(VLAN_SW_MODE);
        m->setProfileName(portProfile);
        m->setSwModeFlag(true);
        m->setSwMode(VLAN_SW_MODE_TRUNK);
        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::deleteRejoinCaseQoSProfileForPortProfile(string portProfile)
    {
        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN deleteRejoinCaseQoSProfileForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;
        APPMLocalAPPMQosMessage *m = new APPMLocalAPPMQosMessage();
        m->setOpCode(APPM_REMOVE);
        m->setCmdCode(QOS_PROFILE_DEL);
        m->setProfileName(portProfile);
        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::removeRejoinCasePortProfileFromDefaultDomain (const string portProfile)
    {

        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN removeRejoinCasePortProfileFromDefaultDomain: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;

        APPMLocalProfileDomainMessage *m = new APPMLocalProfileDomainMessage();
        m->setOpCode(APPM_PROFILE_DOMAIN_EDIT);
        m->setCmdCode(PORT_PROFILE_DELETE_FROM_PORT_PROFILE_DOMAIN);
        m->setPortProfileDomainName(DEFAULT_PORT_PROFILE_DOMAIN_NAME);
        m->setPortProfileName(portProfile);

        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;

    }

    UI32 SwitchAdapter::setRejoinCaseDefaultDomainForPortProfile(const string portProfile)
    {

        trace (TRACE_LEVEL_INFO, string("NODE-REJOIN setRejoinCaseDefaultDomainForPortProfile: ") + portProfile);
        UI32 status = WAVE_MESSAGE_SUCCESS;

        APPMLocalProfileDomainMessage *m = new APPMLocalProfileDomainMessage();
        m->setOpCode(APPM_PROFILE_DOMAIN_EDIT);
        m->setCmdCode(PORT_PROFILE_ADD_TO_PORT_PROFILE_DOMAIN);
        m->setPortProfileDomainName(DEFAULT_PORT_PROFILE_DOMAIN_NAME);
        m->setPortProfileName(portProfile);

        status =  WaveObjectManagerToolKit::sendSynchronously(m);
        delete m;
        return status;
    }

    UI32 SwitchAdapter::associateRejoinCaseMacsToPortProfile(const string portProfileName, vector <string> &macAddressList)
    {
    vector <string> macVector;
    int macSize = macAddressList.size();
    int numberOfMessages = (macSize/MAC_BULK_REQUEST_SIZE) + 1;
    UI32 status = WAVE_MESSAGE_SUCCESS;

    for(int j = 0; j<numberOfMessages; j++)
    {
         for(int k =j*MAC_BULK_REQUEST_SIZE;( k < (j*MAC_BULK_REQUEST_SIZE)+MAC_BULK_REQUEST_SIZE) && (k < macSize); k++)
           {
             macVector.push_back(macAddressList[k]);
           }
             trace (TRACE_LEVEL_INFO,string("SwitchAdapter::associateRejoinCaseMacsToPortProfile:: mac in vector= ") + macVector.size());
        if(macVector.size() > 0)
        {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN associateRejoinCaseMacsToPortProfile: ") + portProfileName);
          APPMLocalAPPMProfileBulkMessage *m = new APPMLocalAPPMProfileBulkMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_ASSOC_MAC);
          m->setProfileName(portProfileName);
          m->setMacCount(macVector.size());
          m->setProfileMacs(macVector);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
        }
        macVector.clear();
    }
    return status;
    }

    UI32 SwitchAdapter::deAssociateRejoinCaseMacsFromPortProfile(const string portProfileName, vector <string> &macAddressList)
    {
    vector <string> macVector;
    int macSize = macAddressList.size();
    int numberOfMessages = (macSize/MAC_BULK_REQUEST_SIZE) + 1;
    UI32 status = WAVE_MESSAGE_SUCCESS;

    for(int j = 0; j<numberOfMessages; j++)
    {
         for(int k =j*MAC_BULK_REQUEST_SIZE;( k < (j*MAC_BULK_REQUEST_SIZE)+MAC_BULK_REQUEST_SIZE) && (k < macSize); k++)
           {
             macVector.push_back(macAddressList[k]);
           }
             trace (TRACE_LEVEL_INFO,string("SwitchAdapter::deAssociateRejoinCaseMacsFromPortProfile:: mac in vector= ") + macVector.size());
        if(macVector.size() > 0)
        {
          trace (TRACE_LEVEL_INFO, string("NODE-REJOIN deAssociateRejoinCaseMacsFromPortProfile: ") + portProfileName);
          APPMLocalAPPMProfileBulkMessage *m = new APPMLocalAPPMProfileBulkMessage();
          m->setOpCode(APPM_VCENTER);
          m->setCmdCode(VCENTER_DEASSOC_MAC);
          m->setProfileName(portProfileName);
          m->setMacCount(macVector.size());
          m->setProfileMacs(macVector);
          status =  WaveObjectManagerToolKit::sendSynchronously(m);
          delete m;
          return status;
        }
        macVector.clear();
    }
    return status;
    }

	UI32 SwitchAdapter::createVlanProfile(string vlan,string portProfile,vector<UI32> vlanRangeVector,UI32Range vlanAddrange,bool isNodeRejoin )
	{
    UI32Range vlanAddrange2;
	UI32 status = WAVE_MESSAGE_SUCCESS;
    trace(TRACE_LEVEL_INFO, string("createVlanProfile vlanrange string = ") + vlanAddrange.toString());

        if( ( vlan == "0") || ( (vlanRangeVector.size() < 2) && (vlanRangeVector[0] == 0)) )
        {
          // where only vlan 0 is configured on pp ===> access   1
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
            status = createRejoinCaseAccessVlanProfileWithVlanForPortProfile(portProfile, 1);
          }else {
            status = pValClientSynchronousConnection->createAccessVlanProfileWithVlanForPortProfile(portProfile, 1);
          }
        }
        else if ( ( vlan == "1")  || ( (vlanRangeVector.size() < 2) && (vlanRangeVector[0] == 1)))
        {
          // where only vlan 1 is configured on pp ===> Trunk Vlan Allowed None
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
            status = createRejoinCaseTrunkVlanNoneProfilePortProfile(portProfile);
          }else{
            status = pValClientSynchronousConnection->createTrunkVlanNoneProfilePortProfile(portProfile);
          }
        }
        else if ( (vlanRangeVector.size() == 2) && (vlanRangeVector[0] == 0) && (vlanRangeVector[1] == 1))
        {
          // where only vlan 0 & 1 are configured ===> Trunk Native Vlan & Trunk
          // Vlan Allowed None
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
            status = createRejoinCaseSwitchportTrunkNativeVlanProfile(portProfile,1);
            status = createRejoinCaseTrunkVlanNoneProfilePortProfile(portProfile);
          } else {
            status = pValClientSynchronousConnection->createSwitchportTrunkNativeVlanProfile(portProfile,1);
            status = pValClientSynchronousConnection->createTrunkVlanNoneProfilePortProfile(portProfile);
          }
        }
        else if ( (vlanRangeVector.size() > 1) && (vlanRangeVector[0] == 0) )
        {
            // where vlan 0 and more are configured ===> Trunk Vlan for all
            // vlans except 0 & Native Vlan

          vlanRangeVector.erase(vlanRangeVector.begin());
          vlanAddrange2 = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
            status = createRejoinCaseTrunkVlanProfileWithVlanForPortProfile(portProfile,vlanAddrange2);
            status = createRejoinCaseSwitchportTrunkNativeVlanProfile(portProfile,1); 
          } else {
            status = pValClientSynchronousConnection->createTrunkVlanProfileWithVlanForPortProfile(portProfile,vlanAddrange2);
            status = pValClientSynchronousConnection->createSwitchportTrunkNativeVlanProfile(portProfile,1);
          }
        }
        else if ( (vlan == "4095") || ( (vlanRangeVector.size() < 2) && (vlanRangeVector[0] == 4095)) || ((vlanRangeVector.size() == 2 ) && ( vlanRangeVector[0] == 4095 && vlanRangeVector[1] == 4095)))
        {
          // trunk 4095 ===> "switchport trunk allowed vlan all" plus
          // "switchport trunk native-vlan 1"
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
            status = createRejoinCaseTrunkVlanAllProfilePortProfile(portProfile);
            status = createRejoinCaseSwitchportTrunkNativeVlanProfile(portProfile,1);
          } else {
            status = pValClientSynchronousConnection->createTrunkVlanAllProfilePortProfile(portProfile);
            status = pValClientSynchronousConnection->createSwitchportTrunkNativeVlanProfile(portProfile,1);
          }
        }
		else if((vlan == "4094") || vlanRangeVector[0] == 4094  || (vlan == "4093") || vlanRangeVector[0] == 4093 || (vlan == "4092") || vlanRangeVector[0] == 4092 || vlanRangeVector[0] == 4091 || (vlan == "4091") )
		{
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {
              status = createRejoinCaseVlanProfileForPortProfile(portProfile);
              status = setRejoinCaseSwitchPortForVlanProfileForPortProfile(portProfile); 
              status = setRejoinCaseSwitchPortModeTrunkForVlanProfileForPortProfile(portProfile);
          } else {
    		  status = pValClientSynchronousConnection->createVlanProfileForPortProfile(portProfile);
	    	  status = pValClientSynchronousConnection->setSwitchPortForVlanProfileForPortProfile(portProfile);
		      status = pValClientSynchronousConnection->setSwitchPortModeTrunkForVlanProfileForPortProfile(portProfile);
          }
		}
        else
        {
          // all other cases
          // trunk  x-y ===> add "switchport trunk allowed vlan x-y"
          if (vlanRangeVector[0] == 0)
          {
           // vlanRangeVector.erase(vlanRangeVector.begin());
           // vlanAddrange = UI32Range::getUI32RangeStringFromVector(vlanRangeVector);
              trace(TRACE_LEVEL_ERROR, string("vlanRangeVector[0] = 0 : This should not be hit. Wrong vlan string received"));
              return WAVE_MESSAGE_ERROR;
          }
          if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
          {   
            status = createRejoinCaseTrunkVlanProfileWithVlanForPortProfile(portProfile,vlanAddrange);
          }else {
            status = pValClientSynchronousConnection->createTrunkVlanProfileWithVlanForPortProfile(portProfile,vlanAddrange);
          }
        }
	

	return status;
	}

	UI32 SwitchAdapter::updateVlanProfile(string dc_id, string pgName,string vlan,bool isNodeRejoin)
	{
		string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
		return updateVlanProfile(portProfile, vlan,isNodeRejoin);
	}

 UI32 SwitchAdapter::updateQoSProfile(string dc_id, string pgName,string qos,bool isNodeRejoin)
    {
	 string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
	 return updateQoSProfile(portProfile,qos,isNodeRejoin);
	}

    UI32 SwitchAdapter::updateVlanProfile(string portProfile,string vlan,bool isNodeRejoin)
    {
        vector<UI32> vlanRangeVector;
        UI32 status = WAVE_MESSAGE_SUCCESS;
        UI32Range vlanAddrange = UI32Range("");
		trace(TRACE_LEVEL_INFO , string("SwitchAdapter::updateVlanProfile: VLAN = ") + vlan);

        if( vlan == "" || vlan == "," || vlan == " ")
        {
          trace(TRACE_LEVEL_ERROR, string("Empty vlan string received"));
          return WAVE_MESSAGE_ERROR;
        }
		vlanAddrange.fromString(vlan);
		vlanAddrange.getUI32RangeVector(vlanRangeVector);

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
           status = deActivateRejoinCasePortProfile(portProfile);
        }else {
           status = pValClientSynchronousConnection->deactivatePortProfile(portProfile);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
		  trace(TRACE_LEVEL_ERROR, string("SwitchAdapter::updateVlanProfile: deactivatePortProfile Failed!"));
            return status;
        }

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            status = deleteRejoinCaseVlanProfileForPortProfile(portProfile);
        }else{
            status = pValClientSynchronousConnection->deleteVlanProfileForPortProfile (portProfile);
        }
        if (status != WAVE_MESSAGE_SUCCESS) {
          trace(TRACE_LEVEL_ERROR, string("Delete VLAN Profile returned error, ignoring it. "));
            //return status;
        }

	    status = createVlanProfile(vlan,portProfile,vlanRangeVector,vlanAddrange,isNodeRejoin);

        if (status != WAVE_MESSAGE_SUCCESS) {
		  trace(TRACE_LEVEL_ERROR, string("SwitchAdapter::updateVlanProfile: createVlanProfile Failed!"));
            return status;
        }

        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
          return activateRejoinCasePortProfile(portProfile);
        }else {
          status = pValClientSynchronousConnection->activatePortProfile(portProfile);
        }
		if (status != WAVE_MESSAGE_SUCCESS) {
		  trace(TRACE_LEVEL_ERROR, string("SwitchAdapter::updateVlanProfile: activatePortProfile Failed!"));
		}
        return status;
    }

 UI32 SwitchAdapter::updateQoSProfile(string portProfile,string qos,bool isNodeRejoin)
    {
     UI32 status = WAVE_MESSAGE_SUCCESS;

     if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
    {
        status = deActivateRejoinCasePortProfile(portProfile);
    }else {
        status = pValClientSynchronousConnection->deactivatePortProfile(portProfile);
    }

     if (status != WAVE_MESSAGE_SUCCESS) {
        return status;
     }
     if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
     {
         status = deleteRejoinCaseQoSProfileForPortProfile(portProfile);
     }else {
         status = pValClientSynchronousConnection->deleteQoSProfileForPortProfile (portProfile);
    }

     if (status != WAVE_MESSAGE_SUCCESS) {
          trace(TRACE_LEVEL_ERROR, string("SwitchAdapter::updateQoSProfile:Delete Qos Profile returned error, ignoring it. "));
            //return status;
     }

	 if(qos !="0" && qos != "" && qos != " ") {
       if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            status = createRejoinCaseQoSProfileWithDefaultCOSForPortProfile(portProfile,atoi(qos.c_str()));
        } else {
         	status = pValClientSynchronousConnection->createQoSProfileWithDefaultCOSForPortProfile(portProfile, atoi(qos.c_str()));
        }
	 }
     if (status != WAVE_MESSAGE_SUCCESS) {
	        trace(TRACE_LEVEL_ERROR,string("SwitchAdapter::updateQoSProfile:createQoSProfile Failed!"));
    	    return status;
        }
     if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
    {
        return activateRejoinCasePortProfile(portProfile);
    }else {
        return  pValClientSynchronousConnection->activatePortProfile(portProfile);
    }
    }

    UI32 SwitchAdapter::createVLANProfile(string dc_id, string pgName, string vlan,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return createRejoinCaseAccessVlanProfileWithVlanForPortProfile(portProfile, 1);
        } else {
            return
              pValClientSynchronousConnection->createAccessVlanProfileWithVlanForPortProfile( portProfile, 1);
        }
    }
    UI32 SwitchAdapter::associateMactoPortProfile(string dc_id, string pgName, string _mac,bool isNodeRejoin) {
        string mac = URLEncode::mac_conver_format(_mac);
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
            return pValClientSynchronousConnection->associateMacToVcenterPortProfile( portProfile, mac);
    }

    UI32 SwitchAdapter::deAssociateMactoPortProfile(string dc_id, string pgName, string _mac,bool isNodeRejoin) {
        string mac = URLEncode::mac_conver_format(_mac);
        string portProfile = PROFILE_NAME(vCenter,dc_id,pgName);
            return pValClientSynchronousConnection->disassociateMacFromVcenterPortProfile( portProfile, mac);
    }

    UI32 SwitchAdapter::associateMacsToPortProfile(string dc_id_pgName, vector <string> &macAddressList,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME_BULK(vCenter,dc_id_pgName);
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return associateRejoinCaseMacsToPortProfile(portProfile, macAddressList);
        }else {
            return pValClientSynchronousConnection->associateMacsToPortProfile(portProfile, macAddressList);
        }
    }

    UI32 SwitchAdapter::reassociateMacsToPortProfile(string portProfile, vector <string> &macAddressList,bool isNodeRejoin) {
      if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return associateRejoinCaseMacsToPortProfile(portProfile, macAddressList); 
        } else {
            return pValClientSynchronousConnection->associateMacsToPortProfile(portProfile, macAddressList);
        }
    }


    UI32 SwitchAdapter::deAssociateMacsFromPortProfile(string dc_id_pgName, vector <string> &macAddressList,bool isNodeRejoin) {
        string portProfile = PROFILE_NAME_BULK(vCenter,dc_id_pgName);
        if( (isNodeRejoin) && (FileUtils::isvCenterInManagementCluster()))
        {
            return deAssociateRejoinCaseMacsFromPortProfile(portProfile, macAddressList);
        }else {
            return pValClientSynchronousConnection->deAssociateMacsFromPortProfile(portProfile, macAddressList);
        }
    }

    UI32 SwitchAdapter::configCdpProtocol(bool cenable) {
        trace(TRACE_LEVEL_INFO, string("configCdpProtocol  ") + cenable);
            return pValClientSynchronousConnection->configCdpProtocol(cenable);
    }

} /* namespace DcmNs */
