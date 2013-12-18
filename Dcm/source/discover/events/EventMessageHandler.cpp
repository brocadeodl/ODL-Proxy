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



#include <string>
#include <iostream>
#include "EventMessageHandler.h"
#include "discover/vsphere/Debug.h"
#include "VirtualDb/DAO/DAOInclude.h"
#include "discover/utils/FileUtils.h"
#include "correlator/SwitchAdapter.h"
#include "correlator/VcenterUtils.h"
#include "VirtualDb/Domain/DomainInclude.h"
#include "ClientInterface/APPM/appm_cdp_def.h"
#include "discover/utils/URLEncode.h"

#define PROFILE_NAME(vcenter_Name,datacenter,port_group) "auto_" + vcenter_Name + "_" + URLEncode::urlencode(datacenter) + "_" + URLEncode::urlencode(port_group);

namespace DcmNs {

    EventMessageHandler::EventMessageHandler() {
        // TODO Auto-generated constructor stub

    }

    EventMessageHandler::~EventMessageHandler() {
        // TODO Auto-generated destructor stub
    }

    void EventMessageHandler::handleDataCenterEvent(LocalDatacenterEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing datacenter %s:%d : %s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "------opening DB----------- %s", vcenterDB.c_str());

        DataCenterDAO dcDao(vcenterDB);
        switch(message->m_opcode) {
            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a datacenter ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dcDao.insertDatacenter(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a datacenter ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dcDao.updateDatacenter(message->m_domain);
                break;
            case VNM_DELETE: 
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a datacenter ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dcDao.deleteDatacenter(message->m_domain);
                break;

            default:
                break;
        }
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                "Update a datacenter DONE!! ----------- ");
        DAO::closeDb();
        return;
    }

    void EventMessageHandler::handleVswitchEvent(LocalVswitchEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing vswitch %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        VsDAO vsDao(vcenterDB);
        switch(message->m_opcode) {
            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "------inserting a vswitch -----------");
                vsDao.insertVs(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "------update a vswitch -----------");
                vsDao.updateVs(message->m_domain);
                break;
            case VNM_DELETE: 
                {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                            "------deleting a vswitch -----------");
                    vsDao.deleteVs(message->m_domain);

                    /* delete Pgs */
                    PgHostVsDAO phvDao;
                    // retrieve pg ids on the vswitch
                    vector<PgHostVsDomain> phv_vector = phvDao.findPgHostVsOnVs(message->m_domain.getHostId(), message->m_domain.getName());
                    // delete the pgs on vswitch
                    phvDao.deletePgHostVsOnVs(message->m_domain.getHostId(), message->m_domain.getName());
                    // delete or update the profile
                    vector<PgHostVsDomain>::iterator it;
                    for (it = phv_vector.begin(); it != phv_vector.end(); it++) {
                        if (phvDao.findCountOnPg(it->getPgId()) == 0) {
                            //find pg name
                            PgDAO pgDao;
                            PgDomain p = pgDao.findPg(it->getPgId());
                            //delete the port-profile
                            if(isPublishValid()) {
                                SwitchAdapter switchAdapter(message->m_vcenter);
                                switchAdapter.deActivatePortProfile(p.getDcId(), p.getPgNN());
                                switchAdapter.deletePortProfile(p.getDcId(), p.getPgNN());
                            }
                            //delete the pg from networks
                            p.setId(it->getPgId());
                            pgDao.deletePg(p);    
                        } else {
                            //TODO: check if vlan of the profile should be updated
                        }
                    }

                    //TODO: delete special pgs and nics. Have to fix retrieveOneHost or host extractdata to set vs for special pgs

                    // update vmnic vswitch
                    VmnicDAO vmnicDao;
                    vmnicDao.removeVmnicVs(message->m_domain.getName(), message->m_domain.getHostId());

                    break;
                }

            default:
                break;
        }
        DAO::closeDb();
        return;

    }


    void EventMessageHandler::handleVmnicEvent(LocalVmnicEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing vmnic %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getMac().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);
        VmnicDAO vmnicDao(vcenterDB);
        switch(message->m_opcode) {
            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "------inserting a vmnic -----------");
                vmnicDao.insertVmnic(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "------update a vmnic -----------");
                vmnicDao.updateVmnicDvs(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "------deleting a vmnic -----------");
                vmnicDao.deleteVmnic(message->m_domain);
                break;

            default:
                break;
        }         
        DAO::closeDb();
        return;

    }

    void EventMessageHandler::handlePgHostVsEvent(LocalPgHostVsEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing pghostvs %s:%d:%s:%s:%s:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getPgNN().c_str(),message->m_domain.getHostId().c_str(),message->m_domain.getVs().c_str(),message->m_domain.getVlan().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        PgHostVsDAO phvDao(vcenterDB);
        SwitchAdapter switchAdapter(message->m_vcenter);
        switch(message->m_opcode) {
            case VNM_CREATE:
                {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                            "------inserting a pghostvs -----------");
                    phvDao.insertPgHostVs(message->m_domain);
                    // check if this is a new portgroup
                    if (phvDao.findCountOnPg(message->m_domain.getPgId()) == 1) {
                        // insert it in networks 
                        PgDomain p(message->m_domain.getPgId(), message->m_domain.getPgNN(), message->m_domain.getVlan(), message->m_domain.getDcId());
                        PgDAO pgDao(vcenterDB);
                        pgDao.insertPg(p);
                        // create the port profile
                        string vlanStr = VcenterUtils::convertToVlanUI32Range(p.getVlan());
                        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s: %s.",
                                "------create VLANS -----------",vlanStr.c_str());
                        if(isPublishValid()) {
                            switchAdapter.createVlans(vlanStr);
                            switchAdapter.createProfileAndSubProfiles(p.getDcId(), p.getPgNN(), vlanStr);
                        }
                    }

                    break;
                }
            case VNM_UPDATE: 
                {
                    VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                            "------update a pghostvs -----------");
                    // update port profile
                    //if (phvDao.findCountOnPg(message->m_domain.getPgId()) == 1) {
                    // update networks
                    //PgDomain p1 = pgDao.findPg(message->m_domain.getPgId());
                    PgHostVsDomain p1=  phvDao.findPg(message->m_domain.getDcId(), message->m_domain.getHostId(), message->m_domain.getVs(), message->m_domain.getPgId());

                    VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "------old VLAN %s new VLAN %s",p1.getVlan().c_str(), message->m_domain.getVlan().c_str());
                    int vlanChanged = (p1.getVlan().compare(message->m_domain.getVlan()));
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "VLAN %d",vlanChanged);
                    VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                            "------update pg host vs ");

                    phvDao.updatePgHostVs(message->m_domain);

                    PgDomain p(message->m_domain.getPgId(), message->m_domain.getPgNN(), message->m_domain.getVlan(), message->m_domain.getDcId());
                    PgDAO pgDao(vcenterDB);

                    pgDao.updatePgVlan(p);
                    if(vlanChanged ==0) {
                        break;
                    }
                    // is vlan changed?
                    // update port profile
                    string vlan_str = VcenterUtils::convertToVlanUI32Range(p.getVlan());
                    if(isPublishValid()) {
                        switchAdapter.createVlans( vlan_str);

                        switchAdapter.updateVlanProfile(p.getDcId(), p.getPgNN(),
                                vlan_str);
                    }
                    //}
                    break;
                }
            case VNM_DELETE:
                {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "------deleting a pghostvs ----------- %s",message->m_domain.getPgId().c_str());
                    phvDao.deletePgHostVsOnId(message->m_domain);
                    if (phvDao.findCountOnPg(message->m_domain.getPgId()) == 0) {
                        //delete the port-profile
                        if(isPublishValid()) {
                            switchAdapter.deActivatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());                    
                            switchAdapter.deletePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                        }
                        //delete the pg from networks
                        PgDomain p;
                        p.setId(message->m_domain.getPgId());
                        PgDAO pgDao;
                        pgDao.deletePg(p);
                    } else {
                        //TODO: check if vlan of the profile should be updated
                    }

                    break;
                }
            default:
                break;
        }
        DAO::closeDb();
        return;
    }

    void EventMessageHandler::handleVMEvent(LocalVMEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing VM %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );

        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        VmDAO dao(vcenterDB);
        switch(message->m_opcode) 
        {

            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a vm ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.insertVm(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a vm ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.updateVmName(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a vm ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.deleteVm(message->m_domain);
                break;
            case VNM_RECONF:
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "------reconf a VM ----------- %s:%s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str(),message->m_domain.getHostNN().c_str());
                //dao.updateVmName(message->m_domain);
                dao.updateVmHost(message->m_domain);
                //          dao.updateDatacenter(message->getDomain());
                break;
            default:
                break;
        }
        DAO::closeDb();

        return;
    }

    void EventMessageHandler::handleDVSEvent(LocalDVSEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing DVS %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        DvsDAO dao(vcenterDB);
        switch(message->m_opcode) 
        {

            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a dvs ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.insertDvs(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a dvs ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.updateDvs(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a dvs ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.deleteDvs(message->m_domain);
                break;
            case VNM_RECONF:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------reconf a dvs ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                //          dao.updateDatacenter(message->getDomain());
                break;
            default:
                break;
        }
        DAO::closeDb();
        return;
    }


    void EventMessageHandler::handleDVPGEvent(LocalDVPGEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing DVPG to DB %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        DvpgDAO dao(vcenterDB);
        SwitchAdapter switchAdapter(message->m_vcenter);

        switch(message->m_opcode) 
        {

            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a DVPG ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.insertDvpg(message->m_domain);
                // create the port profile
                //            string vlanStr = "10";
                //            ;
                if (isUpLink(message->m_domain.getDVPGType())){
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s: %s.",
                            "Skipping VLANS and Profile creation for an uplink PG ",message->m_domain.getName().c_str());
                    break;
                }

                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s: %s.",
                        "------create VLANS -----------",VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()).c_str());

                if(isPublishValid()) {
                    switchAdapter.createVlans( VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()));

                    switchAdapter.createProfileAndSubProfiles(message->m_domain.getDcId(), message->m_domain.getPgNN(), VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()));
                }

                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a DVPG ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.updateDvpgName(message->m_domain);
                dao.updateDvsName(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a DVPG ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.deleteDvpg(message->m_domain);
                {
                    DvpgDomain p2 = dao.findBy(message->m_domain.getId());

                    if (isUpLink(p2.getDVPGType())){
                        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s: %s.",
                                "Skipping Profile deletion for an uplink PG ",message->m_domain.getName().c_str());
                        break;
                    }
                }

                //delete the port-profile
                if(isPublishValid()) {
                    switchAdapter.deActivatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                    switchAdapter.deletePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                }

                break;
            case VNM_RECONF: 
                {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Reconf a DVPG ----------- %s:%s:%s", 
                            message->m_domain.getId().c_str(),
                            message->m_domain.getName().c_str(),
                            VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()).c_str());

                    DvpgDomain p1 = dao.findBy(message->m_domain.getId());

                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "old VLAN %s new VLAN %s",p1.getVlan().c_str(), message->m_domain.getVlan().c_str());
                    int vlanChanged = (p1.getVlan().compare(message->m_domain.getVlan()));
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "old QOS %s new QOS %s",p1.getQos().c_str(), message->m_domain.getQos().c_str());
                    int qosChanged = (p1.getQos().compare(message->m_domain.getQos()));
                    bool isValid = isPublishValid();
                    bool isUplink = isUpLink(message->m_domain.getDVPGType());

                    UI32 status = 0;
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Checking VLAN : valid %d vlanChanged %d.",isValid, vlanChanged);
                    if(!isUplink && isValid && (vlanChanged !=0)) {
                        status = switchAdapter.createVlans( VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()));

                        status = switchAdapter.updateVlanProfile(message->m_domain.getDcId(), 
                                message->m_domain.getPgNN(), 
                                VcenterUtils::convertToVlanUI32Range(message->m_domain.getVlan()));
                        if (status != WAVE_MESSAGE_SUCCESS) {
                            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s.",
                                    "error creating  VLAN -----------");
                            //return ;
                        }
                    }
                    if((status == WAVE_MESSAGE_SUCCESS)||(isUplink))
                    {
                        dao.updateDvpgVlan(message->m_domain);
                        status = 0;
                    }
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Checking Qos : valid %d qosChanged %d.",isValid, qosChanged);

                    if(!isUplink && isValid && (qosChanged !=0))
                    {
                        status = switchAdapter.updateQoSProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getQos());
                        if (status != WAVE_MESSAGE_SUCCESS) {
                            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s.",
                                    "error creating  QoS -----------");
                            //return ;
                        }
                    }
                    if((status == WAVE_MESSAGE_SUCCESS)||(isUplink))
                        dao.updateDvpgQos(message->m_domain);

                }
                break;
            default:
                break;
        }
        DAO::closeDb();
        return;
    }

    void EventMessageHandler::handleHostEvent(LocalHostEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing Host %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getName().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        HostDAO dao(vcenterDB);
        switch(message->m_opcode)
        {

            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a Host ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.insertHost(message->m_domain);
                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a Host ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.updateHost(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a Host ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                dao.deleteHost(message->m_domain);
                break;
            case VNM_RECONF:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------reconf a Host ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                //          dao.updateDatacenter(message->getDomain());
                break;
            default:
                break;
        }
        DAO::closeDb();
        return;
    }


    void EventMessageHandler::handleVnicEvent(LocalVNICEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing VNIC %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getMac().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        VnicDAO dao(vcenterDB);
        SwitchAdapter switchAdapter(message->m_vcenter);
        switch(message->m_opcode) 
        {

            case VNM_CREATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------inserting a Vnic ----------- %s:%s", message->m_domain.getPgNN().c_str(),message->m_domain.getMac().c_str()); 
                // add nic to the pp
                //switchAdapter.deActivatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());


                //Port-group Names should not be empty
                if (message->m_domain.getPgNN().compare("") != 0) {
                    if (isPublishValid()) {
                        switchAdapter.activatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                        switchAdapter.associateMactoPortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getMac());
                    }                   
                    string profile = PROFILE_NAME(message->m_vcenter,message->m_domain.getDcId(),message->m_domain.getPgNN() );
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG, "------inserting a profile ----------- %s", profile.c_str());
                    message->m_domain.setPortProfile(profile);
                }
                dao.insertVnic(message->m_domain);
                dao.updateVnicPortProfile(message->m_domain);



                break;
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------update a Vnic ----------- %s:%s", message->m_domain.getPgNN().c_str(),message->m_domain.getMac().c_str());
                // add nic to the pp
                //switchAdapter.deActivatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                if(isPublishValid()) {
                    switchAdapter.activatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                    switchAdapter.associateMactoPortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getMac());
                }

                //dao.updateVnicMac(message->m_domain);
                break;
            case VNM_DELETE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------delete a Vnic ----------- %s:%s", message->m_domain.getPgNN().c_str(),message->m_domain.getMac().c_str());
                // deact the pp
                // remove nic to the pp
                // act the pp
                //switchAdapter.deActivatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                if (message->m_domain.getPgNN().compare("") != 0) {
                    if(isPublishValid()) {
                        //switchAdapter.activatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());
                        switchAdapter.deAssociateMactoPortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getMac());
                    }
                }
                dao.deleteVnic(message->m_domain);

                break;
            case VNM_RECONF:
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        "------reconf update a Vnic ----------- %s:%s", message->m_domain.getPgNN().c_str(),message->m_domain.getMac().c_str());
                //          dao.updateDatacenter(message->getDomain());
                break;
            default:
                break;
        }
        DAO::closeDb();
        return;
    }

    void EventMessageHandler::handleSpgEvent(LocalSPGEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing SPG %s:%d:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getPgNN().c_str());
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        SpecialPgDAO dao(vcenterDB);
        SwitchAdapter switchAdapter(message->m_vcenter);
        switch(message->m_opcode) {

            case VNM_CREATE: {
                                 VIC_PLUG_LOG(
                                         VIC_TRACE_DEBUG,
                                         "------inserting a SPG ----------- %s:%s",
                                         message->m_domain.getId().c_str(), message->m_domain.getPgNN().c_str());
                                 dao.insertSpecialPg(message->m_domain);

                                 if (dao.findCountOnPgName(message->m_domain.getPgNN()) == 1) {
                                     // create the port profile
                                     string vlanStr = VcenterUtils::convertToVlanUI32Range(
                                             message->m_domain.getVlan());
                                     VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s: %s.",
                                             "------create VLANS -----------", vlanStr.c_str());
                                     if (isPublishValid()) {
                                         switchAdapter.createVlans(vlanStr);
                                         switchAdapter.createProfileAndSubProfiles(
                                                 message->m_domain.getDcId(),
                                                 message->m_domain.getPgNN(), vlanStr);
                                     }
                                 }
                             }
                             break;
            case VNM_UPDATE: {

                                 VIC_PLUG_LOG(
                                         VIC_TRACE_DEBUG,
                                         "------update a SPG ----------- %s:%s",
                                         message->m_domain.getPgNN().c_str(), message->m_domain.getPgNN().c_str());

                                 dao.updateSpecialPgVS(message->m_domain);
                                 // update port profile
                                 if (dao.findCountOnPgName(message->m_domain.getPgNN()) == 1) {
                                     SpecialPgDomain p1 = dao.findPg(message->m_domain.getPgNN(),
                                             message->m_domain.getDcId(),
                                             message->m_domain.getHostId());
                                     VIC_PLUG_LOG(
                                             VIC_TRACE_DEBUG,
                                             "------old VLAN %s new VLAN %s",
                                             p1.getVlan().c_str(), message->m_domain.getVlan().c_str());
                                     int vlanChanged = (p1.getVlan().compare(
                                                 message->m_domain.getVlan()));
                                     VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VLAN %d", vlanChanged);

                                     if (vlanChanged == 0) {
                                         break;
                                     }
                                     dao.updateSpecialPgVLan(message->m_domain);

                                     // is vlan changed?
                                     // update port profile
                                     string vlan_str = VcenterUtils::convertToVlanUI32Range(
                                             message->m_domain.getVlan());
                                     if (isPublishValid()) {
                                         switchAdapter.createVlans(vlan_str);

                                         switchAdapter.updateVlanProfile(
                                                 message->m_domain.getDcId(),
                                                 message->m_domain.getPgNN(), vlan_str);
                                     }
                                 }
                             }
                             break;
            case VNM_DELETE:

                             VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                                     "------deleting a SPG ----------- %s",
                                     message->m_domain.getPgNN().c_str());

                             dao.deleteSpecialPg(message->m_domain);
                             if (dao.findCountOnPgName(message->m_domain.getPgNN()) == 0) {
                                 //delete the port-profile
                                 if (isPublishValid()) {
                                     switchAdapter.deActivatePortProfile(
                                             message->m_domain.getDcId(),
                                             message->m_domain.getPgNN());
                                     switchAdapter.deletePortProfile(
                                             message->m_domain.getDcId(),
                                             message->m_domain.getPgNN());
                                 }

                             }

                             break;
            default:
                             break;
        }
        DAO::closeDb();
        return;        
    }

    void EventMessageHandler::handleSnicEvent(LocalSNICEventMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Processing SNIC %s:%d:%s:%s", message->m_vcenter.c_str(),message->m_opcode,message->m_domain.getPgNN().c_str(),message->m_domain.getDvpgId().c_str() );
        string vcenterDB = FileUtils::getDBLocation(message->m_vcenter);

        SwitchAdapter switchAdapter(message->m_vcenter);
        switch(message->m_opcode) {

            case VNM_CREATE:
                {

                    VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "------inserting a SNIC ----------- %s:%s:%s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str(), message->m_domain.getPgNN().c_str(), message->m_domain.getDvpgId().c_str());

                    if(message->m_domain.getPgNN().empty()) {
                        VIC_PLUG_LOG(VIC_TRACE_INFO,
                                "PG NN is empty, using dvpg_id %s", message->m_domain.getDvpgId().c_str());

                        // get pgname from dvpg_id
                        DvpgDAO dao(vcenterDB);
                        DvpgDomain d1 = dao.findBy(message->m_domain.getDvpgId());
                        message->m_domain.setPgNN(d1.getName());
                        DAO::closeDb();

                    }
                    //SpecialNicDAO s1dao(vcenterDB);
                    VIC_PLUG_LOG(VIC_TRACE_INFO,
                            "Inserting the snic using dvpg_id %s", message->m_domain.getDvpgId().c_str());
                    SpecialNicDAO snicDao(vcenterDB);
                    snicDao.insertSpecialNic(message->m_domain);

                    //add the mac association
                    string profile = PROFILE_NAME(message->m_vcenter,message->m_domain.getDcId(),message->m_domain.getPgNN() );
                    message->m_domain.setPortProfile(profile);

                    if(isPublishValid()) {
                        switchAdapter.activatePortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN());

                        switchAdapter.associateMactoPortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getMac());
                    }
                    snicDao.updateSpecialNicPortProfileOnPgNN(message->m_domain);
                    DAO::closeDb();

                    break;
                }
            case VNM_UPDATE:
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "------TBD update a SNIC ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                break;
            case VNM_DELETE:
                {
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                            "------delete a SNIC ----------- %s:%s", message->m_domain.getId().c_str(),message->m_domain.getName().c_str());
                    SpecialNicDAO snicDao(vcenterDB);
                    snicDao.deleteSpecialNic(message->m_domain);
                    //remove the mac association
                    if(isPublishValid()) {
                        switchAdapter.deAssociateMactoPortProfile(message->m_domain.getDcId(), message->m_domain.getPgNN(), message->m_domain.getMac());
                    }
                    DAO::closeDb();

                    break;
                }
            default:
                break;
        }
        DAO::closeDb();
        return;
    }

    void EventMessageHandler::handleDPEvent(LocalVNMDPMessage *message) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "------ processing cdp and lldp ----------- %s: %s: %s: %s: %d: ", 
                message->m_vcenter.c_str(),
                message->m_hostMor.c_str(),
                message->m_portId.c_str(),
                message->m_ifName.c_str(),
                message->m_cmdCode );
        string ifname="";
        if (message->m_cmdCode == VNM_CDP_RECEIVED) {
            ifname = message->m_ifName;
        }

        VmnicDAO vmnicDao(message->m_vcenter);
        vmnicDao.updateVmnic(message->m_vcenter, ifname, message->m_hostMor, message->m_portId);
        DAO::closeDb();
        return;

    }

    /**
      returns true if this is a primary and we are in MC

     */

    bool EventMessageHandler::isPublishValid() {
        bool isPrimaryLocation = FrameworkToolKit::isPrimaryLocation();

        UI32 locationRole = FrameworkToolKit::getThisLocationRole();
        //IN single Node cluster(MC) locationRole is returned as LOCATION_STAND_ALONE;
        isPrimaryLocation= isPrimaryLocation||(locationRole==LOCATION_STAND_ALONE);
        bool isInMc = FileUtils::isvCenterInManagementCluster();

        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s:%d:%d."," MC Check", isInMc, isPrimaryLocation);
        //Non primary nodes(MGMT cluster) ignore creation of AMPP
        if  (isInMc && (isPrimaryLocation ==false)) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s"," Secondary node in a MC");
            return false;
        }
        else {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s"," Primary node in a MC or Standalone or FC");
            return true;
        }
    }

    bool EventMessageHandler::isUpLink(string uplink){

        if(uplink.compare("uplink")==0){

            return true;
        }
        return false;
    }

} /* namespace DcmNs */
