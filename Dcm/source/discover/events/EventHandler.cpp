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



#include "EventHandler.h"

#include "discover/connector/NetworkBuilder.h"
#include "discover/connector/Host.h"
#include "discover/connector/Network.h"

namespace DcmNs {
    EventHandler::EventHandler(string vcenter, Event* event): m_vcenter(vcenter), m_event(event) {

    }

    EventHandler::~EventHandler() {
    }
    void EventHandler::handle() {

    }

    void EventHandler::handle(ServiceInstance *serviceInstance) {
        printEvent();
        if (m_event->type.compare(DATACENTER_CREATE) == 0)  {

            DatacenterEventMessage *pMessage = new DatacenterEventMessage();
            DataCenterDomain dc(m_event->datacenter.datacenter.val, URLEncode::dbencode(m_event->datacenter.name));
            pMessage->m_opcode=VNM_CREATE;
            pMessage->m_vcenter=m_vcenter;
            pMessage->m_domain=dc;
            DistributionSender *ds = DistributionSender::getInstance();
            ds->publishEvent(pMessage);
            delete pMessage;
        }else if (m_event->type.compare(DATACENTER_RENAMED) == 0) {
            // dc rename
            DatacenterEventMessage *pMessage = new DatacenterEventMessage();
            DataCenterDomain dc(m_event->datacenter.datacenter.val, URLEncode::dbencode(m_event->datacenter.name));
            pMessage->m_opcode=VNM_UPDATE;
            pMessage->m_vcenter=m_vcenter;
            pMessage->m_domain=dc;
            DistributionSender *ds = DistributionSender::getInstance();
            ds->publishEvent(pMessage);
            delete pMessage;
        }else if (m_event->type.compare(VM_CREATE) == 0) {
            waitForUpdates(15);
            Host* host = retrieveOneHost(serviceInstance, m_event->host.host.val);
            if (host == NULL) {
                VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this Host, Skipping Host Create",m_event->host.host.val.c_str());
                return;
            }
            string host_name;
            host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);

            vmCreate(serviceInstance, m_event->vm.vm.val,  m_event->host.host.val,  host_name, m_vcenter, m_event->datacenter.datacenter.val);
            delete host;

        }else if (m_event->type.compare(VM_RENAMED) == 0) {
            waitForUpdates(15);
            vmRename(serviceInstance, m_event->host.host.val, m_event->vm.vm.val, m_event->vm.name, m_event->datacenter.datacenter.val );

        }else if (m_event->type.compare(VM_DELETE) == 0) {
            vmDelete();

        }else if (m_event->type.compare(VM_RECONF) == 0) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "------VM VNM_RECONF EVENT ----------- %s ", m_event->type.c_str());

            waitForUpdates(30);
            class VM* asset1 = retrieveOneVM(serviceInstance, m_event->vm.vm.val);

            if (asset1 == NULL) {
                VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this VM, Skipping VM ",m_event->vm.vm.val.c_str());
                return;
            }

            vector<VNics *> vnicsList = asset1->vnicsList;

            for (size_t s = 0; s < vnicsList.size(); s++) {
                VNics * vnic1 = vnicsList.at(s);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "---mac(%s):dvpg(%s) ", vnic1->mac.c_str(), vnic1->dvpg_nn.c_str());                    
            }

            Host* host = new Host();
            host->id=m_event->host.host.val;
            host->hostName=m_event->host.name;

            string vcenterDB = FileUtils::getDBLocation(m_vcenter);
            EventCorrelator ec(vcenterDB, m_event->datacenter.datacenter.val, m_vcenter);
            ec.correlateVMReconf(host,asset1);

            delete asset1;
            delete host;


        }else if (m_event->type.compare(DVS_CREATE) == 0) {
            waitForUpdates(30);
            DVS *dvs = retrieveOneDVS(serviceInstance, m_event->dvs.dvs.val);
            if (dvs == NULL) {
                VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this DVS, Skipping DVS Create",m_event->dvs.dvs.val.c_str());
                return;
            }
            DVSEventMessage *pMessage = new DVSEventMessage();

            DvsDomain dc(m_event->dvs.dvs.val, URLEncode::dbencode(m_event->dvs.name), m_event->net.network.val, URLEncode::dbencode(m_event->net.name), m_event->datacenter.datacenter.val);

            pMessage->m_opcode=VNM_CREATE;
            pMessage->m_vcenter=m_vcenter;
            pMessage->m_domain=dc;
            DistributionSender *ds = DistributionSender::getInstance();
            ds->publishEvent(pMessage);
            delete pMessage;
            multimap<string, string>::iterator ithost_pnics;
            for (ithost_pnics = dvs->host_pnicList.begin();
                    ithost_pnics != dvs->host_pnicList.end(); ithost_pnics++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "DV HOST ID %s = DVS PNIC ID %s\n",
                        (*ithost_pnics).first.c_str(),
                        (*ithost_pnics).second.c_str());
                VmnicDomain vmnic;
                vmnic.setName((*ithost_pnics).second);
                vmnic.setHostId((*ithost_pnics).first);
                vmnic.setDvs(dvs->id);
                vmnic.setDvsNN(URLEncode::dbencode(dvs->name));
                vmnic.setDcId(m_event->dvs.dvs.val);
                VmnicEventMessage *vnMessage = new VmnicEventMessage();

                vnMessage->m_opcode=VNM_UPDATE;
                vnMessage->m_vcenter=m_vcenter;
                vnMessage->m_domain=vmnic;
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "VMNIC Created message \n");

                ds->publishEvent(vnMessage);
                delete vnMessage;

            }                
            delete dvs;

        }else if (m_event->type.compare(DVS_RENAMED) == 0) {

            dvsRename();
        }else if (m_event->type.compare(DVS_DELETE) == 0) {
            dvsRemove();
        }else if (m_event->type.compare(DVS_RECONF) == 0) {

            waitForUpdates(30);
            DVSEventMessage *pMessage = new DVSEventMessage();
            DvsDomain dc(m_event->dvs.dvs.val, URLEncode::dbencode(m_event->dvs.name), m_event->net.network.val,
                    URLEncode::dbencode(m_event->net.name), m_event->datacenter.datacenter.val);
            pMessage->m_opcode=VNM_RECONF;
            pMessage->m_vcenter=m_vcenter;
            pMessage->m_domain=dc;
            DistributionSender *ds = DistributionSender::getInstance();
            ds->publishEvent(pMessage);
            delete pMessage;

        }else if (m_event->type.compare(DVPG_CREATE) == 0) {
            dvpgCreate(serviceInstance);
        }else if (m_event->type.compare(DVPG_RENAMED) == 0) {
            //get all vms for this dvpg
            string vcenterDB = FileUtils::getDBLocation(m_vcenter);

            VnicDAO vnicDao(vcenterDB);
            vector<Domain*> vdaoVec = vnicDao.findAllBy(m_event->net.network.val , m_event->datacenter.datacenter.val);
            VIC_PLUG_LOG(VIC_TRACE_INFO, "VNIC GET message %s %d\n", m_event->net.network.val.c_str(), vdaoVec.size());
            for (unsigned int j = 0 ; j < vdaoVec.size(); j++ ) {
                Domain *vdomain = vdaoVec[j];
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNIC RENAME message %s-%s\n", m_event->net.network.val.c_str(),
                        vdomain->getMac().c_str());
            }

            sendDbConfigUpdate(vdaoVec, VNM_DELETE);
            // dvpg delete
            dvpgDelete(((DatacenterRenamedEvent *) m_event->obj)->oldName);
            // dvpg create
            dvpgCreate(serviceInstance);
            // add vms back.
            sendDbConfigUpdate(vdaoVec, VNM_CREATE);

        }else if (m_event->type.compare(DVPG_DELETE) == 0) {
            dvpgDelete(m_event->net.name);
        }else if (m_event->type.compare(DVPG_RECONF) == 0) {
            waitForUpdates(15);
            // vlan changed.
            dvpgReconf(serviceInstance);
        }else if (m_event->type.compare(HOST_ADD) == 0) {
            if(isHostPresent(m_event->host.host.val, m_vcenter, m_event->datacenter.datacenter.val)) {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Host already created skipping host create %s ",m_event->host.host.val.c_str());
                return;
            }

            waitForUpdates(30);
            hostCreate(serviceInstance, m_event->host.host.val,m_vcenter, m_event->datacenter.datacenter.val );
        }else if (m_event->type.compare(HOST_REMOVE) == 0) {
            if(isHostPresent(m_event->host.host.val, m_vcenter, m_event->datacenter.datacenter.val)) {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Removing host %s ",m_event->host.host.val.c_str());
                waitForUpdates(10);
                hostRemove(m_event->host.host.val,m_vcenter, m_event->datacenter.datacenter.val );
                return;
            }else
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Host Already removed %s ",m_event->host.host.val.c_str());

        }

    }


    void EventHandler::dvpgReconf(ServiceInstance *serviceInstance) {
        DVPG* dvpg = retrieveOneDVPG(serviceInstance, m_event->net.network.val);
        if (dvpg == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this DVPG, Skipping DVPG ",m_event->net.network.val.c_str());
            return;
        }

        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s",
                " invoking retrieveQosMap----------- ");
        map<string, string> nrpToQosMap;
        retrieveQosMap(serviceInstance,nrpToQosMap);

        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "returned  retrieveQosMap----------- %s ",dvpg->networkResourcePoolKey.c_str());
        map<string, string>::iterator itnrpQosMap;
        string qos="";
        for (itnrpQosMap = nrpToQosMap.find(dvpg->networkResourcePoolKey);
                itnrpQosMap != nrpToQosMap.end();
                itnrpQosMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " resourceKey %s =QosVal %s",
                    (*itnrpQosMap).first.c_str(),
                    (*itnrpQosMap).second.c_str());

            if ((*itnrpQosMap).first == dvpg->networkResourcePoolKey) {
                qos = (*itnrpQosMap).second;
                break;
            }
        }

        VIC_PLUG_LOG(VIC_TRACE_INFO, 
                "DVPG, QOS (%s, %s) ",dvpg->networkResourcePoolKey.c_str(), qos.c_str());

        DvpgDomain dvpgDb(dvpg->id, URLEncode::dbencode(dvpg->name), dvpg->vlan, qos,
                dvpg->dvs_id, "",
                m_event->datacenter.datacenter.val,
                dvpg->networkResourcePoolKey,dvpg->uplinktype); // dc and dvs shouldbe present
        DVPGEventMessage *pMessage = new DVPGEventMessage();

        pMessage->m_opcode=VNM_RECONF;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dvpgDb;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;

        delete dvpg; 
    }

    void EventHandler::dvpgCreate(ServiceInstance *serviceInstance) {
        VIC_PLUG_LOG(VIC_TRACE_INFO, 
                "DVPG CREATE  ----------- %s ",m_event->net.network.val.c_str());
        DVPG* dvpg = retrieveOneDVPG(serviceInstance, m_event->net.network.val);
        if (dvpg == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this DVPG, Skipping DVPG",m_event->net.network.val.c_str());
            return;
        }
        DVPGEventMessage *pMessage = new DVPGEventMessage();
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "%s",
                " invoking retrieveQosMap----------- ");
        map<string, string> nrpToQosMap;
        retrieveQosMap(serviceInstance,nrpToQosMap);

        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "returned  retrieveQosMap----------- %s ",dvpg->networkResourcePoolKey.c_str());
        map<string, string>::iterator itnrpQosMap;
        string qos="";
        for (itnrpQosMap = nrpToQosMap.find(dvpg->networkResourcePoolKey);
                itnrpQosMap != nrpToQosMap.end();
                itnrpQosMap++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " resourceKey %s =QosVal %s",
                    (*itnrpQosMap).first.c_str(),
                    (*itnrpQosMap).second.c_str());

            if ((*itnrpQosMap).first == dvpg->networkResourcePoolKey) {
                qos = (*itnrpQosMap).second;
                break;
            }
        }

        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "DVPG, QOS (%s, %s) ",dvpg->networkResourcePoolKey.c_str(), qos.c_str());

        DvpgDomain dvpgDb(dvpg->id, URLEncode::dbencode(dvpg->name), dvpg->vlan, qos,
                dvpg->dvs_id, m_event->dvs.name,
                m_event->datacenter.datacenter.val,
                dvpg->networkResourcePoolKey,dvpg->uplinktype); // dc and dvs shouldbe present

        pMessage->m_opcode=VNM_CREATE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dvpgDb;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete dvpg;
        delete pMessage;
    }

    void EventHandler::dvpgDelete(string dvpg) {
        VIC_PLUG_LOG(VIC_TRACE_INFO, 
                "DVPG DElete  ----------- %s ",m_event->net.network.val.c_str());
        DVPGEventMessage *pMessage = new DVPGEventMessage();
        DvpgDomain dc(m_event->net.network.val,
                URLEncode::dbencode(dvpg),
                "", "",
                m_event->dvs.dvs.val, "",
                m_event->datacenter.datacenter.val,
                "",""); // dc and dvs shouldbe present
        pMessage->m_opcode=VNM_DELETE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);

        delete pMessage;
    }


    void EventHandler::vmCreate(ServiceInstance *serviceInstance, string vm_id, string host_id, string host_name, string vc_id, string dc_id) {

        class VM* asset1 = retrieveOneVM(serviceInstance, vm_id);
        if (asset1 == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this VM, Skipping VM Create",vm_id.c_str());
            return;
        }
        VMEventMessage *pMessage = new VMEventMessage();

        VmDomain dc(vm_id, URLEncode::dbencode(asset1->name), host_id, URLEncode::dbencode(host_name), dc_id);
        //          VmDomain::VmDomain(m_event->vm.vm.val, m_event->vm.name, m_event->host.host.val, event->host.name, event->datacenter.datacenter.val);

        pMessage->m_opcode=VNM_CREATE;
        pMessage->m_vcenter=vc_id;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);

        delete pMessage;
        publishVNICEvent(asset1->vnicsList);

        delete asset1;

    }

    void EventHandler::publishVNICEvent(vector<VNics *> vnicsList) {
        DistributionSender *ds = DistributionSender::getInstance();
        for (size_t s = 0; s < vnicsList.size(); s++) {
            VNics * vnic1 = vnicsList.at(s);
            VNICEventMessage *message = new VNICEventMessage();

            VnicDomain* vnic = static_cast<VnicDomain *>( vnic1->convertToDomain(m_event->host.host.val, "", m_event->datacenter.datacenter.val));
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, 
                    "VNIC create----------- %s ",vnic->getMac().c_str());

            message->m_opcode=VNM_CREATE;
            message->m_vcenter=m_vcenter;
            message->m_domain=*vnic;
            ds->publishEvent(message);
            delete message;
        }

    }

    bool EventHandler::isHostPresent(string host_id, string vc_id, string dc_id) {
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        HostDAO dao(vcenterDB);
        vector<Domain*>daoVec  =  dao.findAllBy(host_id,dc_id);

        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "find all returned %d",daoVec.size());

        if (0 != daoVec.size() ) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "Host already present host create %s %d",host_id.c_str(), daoVec.size());
            return true;
        }
        return false;

    }

    void EventHandler::hostCreate(ServiceInstance *serviceInstance, string host_id, string vc_id, string dc_id) {
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        Host* host = retrieveOneHost(serviceInstance, host_id );
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this HOST, Skipping HOST",host_id.c_str());
            return;
        }
        if (host != NULL) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************parse one host ********");
            parseOneHost(serviceInstance, host, dc_id);
            correlateOperation_t op = EVENT_INSERT;

            //added portgroup, vmkernel, console vnic, or vswitch
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************Creating VSWITCH********");

            EventCorrelator ec(vcenterDB, host, dc_id, vc_id);
            ec.findSendTaskUpdates(op, VSWITCH);
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************CREATING PORTGROUP********");
            ec.findSendTaskUpdates(op, PORTGROUP);
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************CREATING VMK********");
            ec.findSendTaskUpdates(op, VMKERNEL);
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************CREATING CONSOLE VNIC********");
            ec.findSendTaskUpdates(op, CONSOLEVNIC);
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************CREATING VMNIC ********");
            ec.findSendTaskUpdates(op, VMNIC );


            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s",
                    "******************CREATING vms********");
            string host_name;
            host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);

            //Iterate through VMList
            vector<VirtualMachine *>::iterator itvmList;
            for (itvmList = host->vmList.begin(); itvmList < host->vmList.end();
                    itvmList++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "******************VM Created******************************");
                VirtualMachine *vm = static_cast<VirtualMachine *>(*itvmList);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM id %s\n",
                        vm->id.c_str());

                vmCreate(serviceInstance, vm->id,  host_id,  host_name, vc_id, dc_id);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                        "************************************************\n");
            }

            //Iterate through VMList
            //publishVNICEvent(host->vnicsList);

            delete host;
        }

    }

    void EventHandler::vmReconf(ServiceInstance *serviceInstance, string host_id, string vm_id, string vm_name, string dc_id) {
        Host* host = retrieveOneHost(serviceInstance, host_id);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this Host, Skipping Host ",host_id.c_str());
            return;
        }

        string host_name;
        host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);


        VMEventMessage *pMessage = new VMEventMessage();
        VmDomain dc(vm_id, URLEncode::dbencode(vm_name), host_id, host_name, dc_id);
        pMessage->m_opcode=VNM_RECONF;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;
    }

    void EventHandler::vmRename(ServiceInstance *serviceInstance, string host_id, string vm_id, string vm_name, string dc_id) {
        Host* host = retrieveOneHost(serviceInstance, host_id);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this Host, Skipping Host ",host_id.c_str());
            return;
        }

        string host_name;
        host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);


        VMEventMessage *pMessage = new VMEventMessage();
        VmDomain dc(vm_id, URLEncode::dbencode(vm_name), host_id, host_name, dc_id);
        pMessage->m_opcode=VNM_UPDATE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;
    }

    void EventHandler::dvsRename() {
        // update the dvs nn in dvvpgs. and rename dvs
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        string dc_id = m_event->datacenter.datacenter.val;
        string dvs_id = m_event->dvs.dvs.val;

        DvpgDAO dao(vcenterDB);
        vector<Domain*> daoVec = dao.findAllBy(dvs_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG RENAME message %s  %s", dc_id.c_str(),
                    domain->getName().c_str());
            domain->setDvsNN(URLEncode::dbencode(m_event->dvs.name));
        }
        sendDbConfigUpdate(daoVec, VNM_UPDATE);


        DVSEventMessage *pMessage = new DVSEventMessage();
        DvsDomain dc(m_event->dvs.dvs.val, URLEncode::dbencode(m_event->dvs.name), m_event->net.network.val, URLEncode::dbencode(m_event->net.name), m_event->datacenter.datacenter.val);
        pMessage->m_opcode=VNM_UPDATE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;
    }

    void EventHandler::vmDelete() {
        // delete the vnics

        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        string dc_id = m_event->datacenter.datacenter.val;
        string vm_id = m_event->vm.vm.val;
        VnicDAO dao(vcenterDB);
        vector<Domain*> daoVec = dao.findAllBy(vm_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNIC DELETE message %s -%s .\n", dc_id.c_str(),
                    domain->getMac().c_str());
        }
        sendDbConfigUpdate(daoVec, VNM_DELETE);



        VMEventMessage *pMessage = new VMEventMessage();
        VmDomain dc(m_event->vm.vm.val, URLEncode::dbencode(m_event->vm.name), m_event->host.host.val, URLEncode::dbencode(m_event->host.name), m_event->datacenter.datacenter.val);
        pMessage->m_opcode=VNM_DELETE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;

    }

    void EventHandler::dvsRemove() {
        // delete the DVPGs, then delete the DVS
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        string dc_id = m_event->datacenter.datacenter.val;
        string dvs_id = m_event->dvs.dvs.val;

        DvpgDAO dao(vcenterDB);
        vector<Domain*> daoVec = dao.findAllBy(dvs_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPG DELETE message %s -%s .\n", dc_id.c_str(),
                    domain->getName().c_str());
        }
        sendDbConfigUpdate(daoVec, VNM_DELETE);



        DVSEventMessage *pMessage = new DVSEventMessage();
        DvsDomain dc(m_event->dvs.dvs.val, URLEncode::dbencode(m_event->dvs.name), m_event->net.network.val, URLEncode::dbencode(m_event->net.name), m_event->datacenter.datacenter.val);
        pMessage->m_opcode=VNM_DELETE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);
        delete pMessage;

        // need to clean up the vmnic row with dvs id
    }

    void EventHandler::hostRemove(string host_id, string vc_id, string dc_id) {
        // cascade delete must delete the following tables,
        // but since there is a bug in sqlite for that, we are manually deleting the following entries
        // delete all the pps created from this host
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        DistributionSender *ds = DistributionSender::getInstance();

        // delete VMNICS
        //added portgroup, vmkernel, console vnic, or vswitch


        updateVmnicsOnHostRemove(host_id, vc_id, dc_id);
        updateVnicsOnHostRemove(host_id, vc_id, dc_id);
        updateVmsOnHostRemove(host_id, vc_id, dc_id);
        updateSnicsOnHostRemove(host_id, vc_id, dc_id);
        updateVssOnHostRemove(host_id, vc_id, dc_id);
        updatePgsOnHostRemove(host_id, vc_id, dc_id);

        HostEventMessage *pMessage = new HostEventMessage();
        //HostDomain dc(m_event->host.host.val, URLEncode::dbencode(m_event->host.name), m_event->datacenter.datacenter.val);
        HostDomain dc(host_id, "", dc_id);
        pMessage->m_opcode=VNM_DELETE;
        pMessage->m_vcenter=vc_id;
        pMessage->m_domain=dc;
        ds->publishEvent(pMessage);

        delete pMessage;

    }


    void EventHandler::updateVmsOnHostRemove(string host_id, string vc_id, string dc_id) {
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        VmDAO vmDao(vcenterDB);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------ %s", host_id.c_str(), dc_id.c_str());

        vector<Domain*> daoVec = vmDao.findAllBy(host_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM DELETE message %s-%s\n", host_id.c_str(),
                    domain->getName().c_str());
        }

        sendDbConfigUpdate(daoVec, VNM_DELETE);

        DAO::closeDb();

    }

    void EventHandler::updateSnicsOnHostRemove(string host_id, string vc_id, string dc_id) {
        // delete special_nics
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        SpecialNicDAO dao(vcenterDB);
        //        vector<Domain*> daoVec = dao.findAllBy("host-1990", "datacenter-1341");
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------ %s", host_id.c_str(), dc_id.c_str());
        vector<Domain*> daoVec = dao.findAllBy(host_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "SNIC DELETE message %s -%s -%s.\n", host_id.c_str(),
                    domain->getDevice().c_str(),
                    domain->getMac().c_str());
        }
        sendDbConfigUpdate(daoVec, VNM_DELETE);

        DAO::closeDb();
    }

    void EventHandler::updateVssOnHostRemove(string host_id, string vc_id, string dc_id) {
        // delete vss
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());
        DistributionSender *ds = DistributionSender::getInstance();

        VsDomain vs("", host_id,
                "",dc_id); // host should be present
        VswitchEventMessage *vsMessage = new VswitchEventMessage();

        vsMessage->m_opcode=VNM_DELETE;
        vsMessage->m_vcenter=m_vcenter;
        vsMessage->m_domain=vs;
        VIC_PLUG_LOG(VIC_TRACE_INFO, "VSS DELETE message %s\n", host_id.c_str());

        ds->publishEvent(vsMessage);
        delete vsMessage;
        DAO::closeDb();
    }

    void EventHandler::updatePgsOnHostRemove(string host_id, string vc_id, string dc_id) {
        // delete pgs_hosts_vss
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        PgHostVsDAO dao(vcenterDB);

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------ %s", host_id.c_str(), dc_id.c_str());

        vector<Domain*> daoVec = dao.findAllBy(host_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "PGHOSTVSDAO DELETE message %s %d\n", host_id.c_str(), daoVec.size());
        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "pgs_hosts_vss DELETE message %s-%s-%s.\n",
                    host_id.c_str(),
                    domain->getPgNN().c_str(),
                    domain->getPgId().c_str());

        }
        sendDbConfigUpdate(daoVec, VNM_DELETE);

        DAO::closeDb();
    }

    void EventHandler::updateVnicsOnHostRemove(string host_id, string vc_id, string dc_id) {

        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        PgHostVsDAO dao(vcenterDB);

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------ %s", host_id.c_str(), dc_id.c_str());

        vector<Domain*> daoVec = dao.findAllBy(host_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "PGHOSTVSDAO GET message %s %d\n", host_id.c_str(), daoVec.size());
        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "pgs_hosts_vss GET message %s-%s-%s.\n",
                    host_id.c_str(),
                    domain->getPgNN().c_str(),
                    domain->getPgId().c_str());

            VnicDAO vnicDao(vcenterDB);
            vector<Domain*> vdaoVec = vnicDao.findAllBy(domain->getPgId(), dc_id);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNIC GET message %s %d\n", host_id.c_str(), vdaoVec.size());
            for (unsigned int j = 0 ; j < vdaoVec.size(); j++ ) {
                Domain *vdomain = vdaoVec[j];
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNIC DELETE message %s-%s\n", host_id.c_str(),
                        vdomain->getMac().c_str());
            }
            sendDbConfigUpdate(vdaoVec, VNM_DELETE);
        }


        SpecialNicDAO snicDao(vcenterDB);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s -SNIC DELETE----- %s", host_id.c_str(), dc_id.c_str());

        daoVec = snicDao.findAllBy(host_id, dc_id);

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "SNIC DELETE message %s-%s\n", host_id.c_str(),
                    domain->getMac().c_str());
        }

        sendDbConfigUpdate(daoVec, VNM_DELETE);

        SpecialPgDAO spgDao(vcenterDB);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s -SPG DELETE----- %s", host_id.c_str(), dc_id.c_str());

        daoVec = spgDao.findAllBy(host_id, dc_id);

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "SPG DELETE message %s-%s\n", host_id.c_str(),
                    domain->getMac().c_str());
        }

        sendDbConfigUpdate(daoVec, VNM_DELETE);

        DAO::closeDb();
    }

    void EventHandler::updateVmnicsOnHostRemove(string host_id, string vc_id, string dc_id) {

        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        VmnicDAO vmnicDao(vcenterDB);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------ %s", host_id.c_str(), dc_id.c_str());

        vector<Domain*> daoVec = vmnicDao.findAllBy(host_id, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VMNIC DELETE message %s-%s\n", host_id.c_str(),
                    domain->getMac().c_str());
        }

        sendDbConfigUpdate(daoVec, VNM_DELETE);

        DAO::closeDb();
    }

    // send and event message to local plugin
    void EventHandler::sendDbConfigUpdate(vector<Domain*> v, UI32 op) {
        vector<Domain*>::iterator it = v.begin();
        DistributionSender *ds = DistributionSender::getInstance();
        while (it != v.end()) {
            EventMessage* pMessage;
            (*it)->convertToEventMessage(&pMessage);
            pMessage->m_opcode = op;
            pMessage->m_vcenter = m_vcenter;
            ds->publishEvent(pMessage);
            delete pMessage;
            it++;
        }
    }

    void EventHandler::parseOneHost(ServiceInstance *serviceInstance, Host *host, string dc_id) {
        //        Host *host = static_cast<Host *>(*itAssetvector);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host ID %s ",
                host->id.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  HostName %s",
                host->hostName.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  DomainName %s",
                host->domainName.c_str());

        /**** DB INSERT ****/
        //DB : (*id = id,name= hostName+domainName)
        string host_name;
        host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);
        HostDomain hostDb(host->id, host_name, dc_id); //dc should be present
        HostEventMessage *pMessage = new HostEventMessage();

        pMessage->m_opcode=VNM_CREATE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=hostDb;
        DistributionSender *ds = DistributionSender::getInstance();
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  Created message %s",
                host->hostName.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "VMNICs %d",
                host->vmnicsList.size());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "VSS %d",
                host->vswitchList.size());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "VMs %d",
                host->vmList.size());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "VNICs %d",
                host->vnicsList.size());

        ds->publishEvent(pMessage);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Done creating host ");
        delete pMessage;
        return;

    }

    DVS* EventHandler::retrieveOneDVS(ServiceInstance *serviceInstance, string morVal) {
        AssetBuilder *builder = new DVSBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pautoBuilder(builder);
        Asset * asset;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DVSs %s for %s\n", morVal.c_str(), m_vcenter.c_str());
        int code = pautoBuilder->getAsset(morVal, &asset);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DVS Details for %s\n",
                    m_vcenter.c_str());
            return NULL;
        }

        return (static_cast<DVS *>(asset));


    }

    int EventHandler::retrieveQosMap(ServiceInstance *serviceInstance, map<string, string> &nrpToQosMap) {

        AssetBuilder *builder = new DVSBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pAutobuilder(builder);
        vector<Asset *> assetvector;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DVS for %s", m_vcenter.c_str());
        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {

            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DVS for %s", m_vcenter.c_str());
            return 0;

        }

        for (size_t i = 0; i < assetvector.size(); i++) {
            DVS *dvs = (DVS *) assetvector.at(i);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "dvs ID %s ",
                    dvs->id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s",
                    "---------------EVENT QOS----------");
            map<string, string>::iterator itnrpQosMap;
            for (itnrpQosMap = dvs->nrp_qosList.begin();
                    itnrpQosMap != dvs->nrp_qosList.end();
                    itnrpQosMap++)
            {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        " resourceKey %s = qosVal %s",
                        (*itnrpQosMap).first.c_str(),
                        (*itnrpQosMap).second.c_str());
                string nrp = (*itnrpQosMap).first;             // the key value (of type Key)
                string qos = (*itnrpQosMap).second;  
                nrpToQosMap[nrp] = qos;
            }

        }
        ListUtil<Asset>::clearPointersFromList(assetvector);

        return 1;

    }


    DVPG* EventHandler::retrieveOneDVPG(ServiceInstance *serviceInstance, string morVal) {
        AssetBuilder *builder = new DVPGBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pautoBuilder(builder);
        Asset * asset;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of DVPGs %s for %s\n", morVal.c_str(), m_vcenter.c_str());
        int code = pautoBuilder->getAsset(morVal, &asset);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve DVPG Details for %s\n",
                    m_vcenter.c_str());
            return NULL;
        }

        return (static_cast<DVPG *>(asset));


    }


    Host* EventHandler::retrieveAHost(ServiceInstance *serviceInstance, string morVal) {
        AssetBuilder *builder = new HostBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pautoBuilder(builder);
        StopWatch hostsw;
        Asset * asset;
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of Host %s for %s", morVal.c_str(), m_vcenter.c_str());
        hostsw.start();
        int code = pautoBuilder->getAsset(morVal, &asset);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,
                    "Failed to retrieve Host Details for %s",
                    m_vcenter.c_str());
            return NULL;
        }
        return static_cast<Host *>(asset);

    }

    Host* EventHandler::retrieveOneHost(ServiceInstance *serviceInstance, string morVal) {
        Host* host = retrieveAHost(serviceInstance, morVal);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this Host, Skipping Host ",m_event->host.host.val.c_str());
            return NULL;
        }

        int code;

        /* populating name and vlan of networks under host */
        vector<Network*>::iterator itnwList;
        AssetBuilder *networkBuilder = new NetworkBuilder(serviceInstance);
        auto_ptr<AssetBuilder> pNetworkAutoBuilder(networkBuilder);
        for (itnwList = host->nwList.begin(); itnwList != host->nwList.end(); itnwList++) {
            // retrieve the name of the networks under the host
            Asset* networkAsset;
            code = pNetworkAutoBuilder->getAsset((*itnwList)->id, &networkAsset);
            if (code != VIC_WSCLIENT_SUCCESS) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        "Failed to retrieve Network Details for %s",
                        m_vcenter.c_str());
                return NULL;
            }
            Network* network = static_cast<Network*>(networkAsset);
            (*itnwList)->name = network->name;

            // find this name in portGroupList and get the vlan
            vector<PortGroup*>::iterator itpgList;
            for (itpgList = host->portGroupList.begin(); itpgList != host->portGroupList.end(); itpgList++) {
                if (network->name == (*itpgList)->name) {
                    (*itnwList)->vlan = (*itpgList)->vlan;
                    (*itnwList)->vs = (*itpgList)->vs_nn;
                    break;
                }
            }

            delete network;
        }

        //prepare a temporary map of vmNics (key to object)
        map<string, VMNics *> vmNicsMap;
        vector<VMNics *>::iterator itvmnicsList;
        for (itvmnicsList = host->vmnicsList.begin();
                itvmnicsList < host->vmnicsList.end(); itvmnicsList++) {
            VMNics *vmnics = static_cast<VMNics *>(*itvmnicsList);
            vmNicsMap[vmnics->key] = vmnics;
        }


        /* populating vswitch id of the vmnics */
        vector<VSwitch *>::iterator itvswitchList;
        for (itvswitchList = host->vswitchList.begin();
                itvswitchList < host->vswitchList.end(); itvswitchList++) {
            VSwitch *vswitch = static_cast<VSwitch *>(*itvswitchList);

            vector<string>::iterator itpnicList;
            for (itpnicList = vswitch->pnicList.begin();
                    itpnicList < vswitch->pnicList.end(); itpnicList++) {
                string pnic = ((*itpnicList));
                map<string, VMNics *>::iterator itvmNicsMap =
                    vmNicsMap.find(pnic);
                VMNics *vmnics = (*itvmNicsMap).second;
                // when vmnic is on vswitch, dvs and dvs_nn are the same
                vmnics->dvs = vswitch->name;
                vmnics->dvs_nn = vswitch->name;
            }
        }

        return host;

    }

    void EventHandler::waitForUpdates(int t){
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "WaitingForUpdates: %d ", t);
        sleep(t);
    }

    void EventHandler::printEvent(){
        VIC_PLUG_LOG(VIC_TRACE_INFO,"\n----Event Received----%s ", m_event->fullFormattedMessage.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(type %s)", m_event->type.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO, "\n----Event Received----%s ", m_event->type.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(chain %s):(key %s):(created %s)", m_event->chainId.c_str(),
                m_event->key.c_str(), m_event->createdTime.c_str());

        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(dc_id %s):(dc_name %s):(host_id %s):(host_name %s)", 
                m_event->datacenter.datacenter.val.c_str(),
                m_event->datacenter.name.c_str(), m_event->host.host.val.c_str(), 
                m_event->host.name.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(dvs_id %s):(dvs_name %s):(pg_id %s):(pg_name %s)",
                m_event->dvs.dvs.val.c_str(), m_event->dvs.name.c_str(),
                m_event->net.network.val.c_str(), m_event->net.name.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(res_id %s):(res_name %s):(vm_id %s):(vm_name %s) ",
                m_event->computeResource.computeResource.val.c_str(), m_event->computeResource.name.c_str(),
                m_event->vm.vm.val.c_str(), m_event->vm.name.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"(msg %s)", m_event->fullFormattedMessage.c_str());
        Debug::VIC_PLUG_TRACE(VIC_TRACE_INFO,"%s", "------------------------\n");

    }
    string EventHandler::getTaskStatus(ServiceInstance *serviceInstance,ManagedObjectReference mor){
        vector<string> properties;


        vector<PropertyFilterSpec> specSet = PropertyCollector::createPropertyFilterSpec(
                mor, properties);
        //Get Property Collector
        PropertyCollector *propCollector = NULL;
        int code = serviceInstance->getPropertyCollector(&propCollector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to get Property Collector");
            return "";
        }
        auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);

        //Retrieve Properties
        vector<ObjectContent *> *objectContentList = NULL;
        code = propCollector->retrieveProperties(&specSet, &objectContentList);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","Unable to Fetch Details for %s", mor.val.c_str());
            return "";
        }
        string status="";
        for (size_t i = 0; i < objectContentList->size(); i++) {
            ObjectContent *objContent = objectContentList->at(i);

            vector<DynamicProperty *> *propSet = objContent->propset;
            for (size_t j = 0; j < propSet->size(); j++) {

                if (propSet->at(j)->objType.compare("TaskInfo") == 0) {
                    TaskInfo *info = (TaskInfo *) propSet->at(j)->obj;
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Task Info State %s\n", info->state.state.c_str());
                    VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Task Info Name %s\n", info->entityName.c_str());
                    status = info->state.state;
                }
            }

        }
        //Delete the Pointer Vectors implicitly
        ListUtil<ObjectContent>::clearListofPointers(objectContentList);
        return status;
    }

    vector <Asset*> EventHandler::retrieveHosts(ServiceInstance *serviceInstance) {
        HostBuilder *builder = new HostBuilder(serviceInstance, 1);
        auto_ptr<HostBuilder> pAutobuilder(builder);
        vector<Asset *> assetvector;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of hosts for %s\n", m_vcenter.c_str());

        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s","Failed to retrieve hosts");
            return assetvector;

        }
        return assetvector;

    }

    vector <Asset*> EventHandler::retrieveVMs(ServiceInstance *serviceInstance) {
        VMBuilder *builder = new VMBuilder(serviceInstance);
        auto_ptr<VMBuilder> pAutobuilder(builder);
        vector<Asset *> assetvector;

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "Starting Discovery of VMS for %s", m_vcenter.c_str());

        int code = pAutobuilder->getAsset(assetvector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            VIC_PLUG_LOG(VIC_TRACE_INFO, "%s","Failed to retrieve Vms ");
            return assetvector;

        }
        return assetvector;

    }

    class VM* EventHandler::retrieveOneVM(ServiceInstance *serviceInstance, string morVal) {

        VMBuilder *builder = new VMBuilder(serviceInstance);
        auto_ptr<VMBuilder> pAutobuilder(builder);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s", "Starting Discovery of VMS for sada");


        Asset * asset;
        int code = pAutobuilder->getAsset(morVal, &asset);

        if (code != VIC_WSCLIENT_SUCCESS) {

            VIC_PLUG_LOG(VIC_TRACE_ERROR, "%s %s","Failed to retrieve VmDetails for ",morVal.c_str());
            return NULL;

        }


        class VM *vm = NULL;
        vm = (class VM *) asset;
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM ID %s \n",
                vm->id.c_str());
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM  Name %s\n",
                vm->name.c_str());
        VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VM  isTemplate %d\n",
                vm->isTemplate);

        //Iterate through vnics
        vector<VNics *>::iterator itvnicsList;
        for (itvnicsList = vm->vnicsList.begin();
                itvnicsList < vm->vnicsList.end(); itvnicsList++) {
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s","************************************************\n");
            VNics *vnics = static_cast<VNics *>(*itvnicsList);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics NAME %s\n", vnics->mac.c_str());
            string ip = "";
            map<string, string>::iterator itipMacMap = vm->ipMacMap.find(
                    vnics->mac);
            if (itipMacMap != vm->ipMacMap.end()) {
                ip = (*itipMacMap).second;

            }

            string vcenterDB = FileUtils::getDBLocation(m_vcenter);

            DvpgDAO dao(vcenterDB);
            DvpgDomain dom =  dao.findBy(vnics->dvpg_id);
            string pgname;
            if (dom.getId() != vnics->dvpg_id) {
                PgDAO pdao(vcenterDB);
                PgDomain pdom =  pdao.findPg(vnics->dvpg_id);
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VPGDom %s-%s-%s-%s",
                        pdom.getId().c_str(),
                        pdom.getName().c_str(),
                        pdom.getVlan().c_str(),
                        pdom.getDcId().c_str()
                        );
                pgname = pdom.getName();
            }else {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG, "DVPGDom %s-%s-%s-%s",
                        dom.getId().c_str(),
                        dom.getName().c_str(),
                        dom.getVlan().c_str(),
                        dom.getDcId().c_str()
                        );
                pgname = dom.getName();
            }


            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics ipaddress %s\n",
                    ip.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"VNics mac %s\n",
                    vnics->mac.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_nn name1 %s\n",
                    pgname.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics dvpg_id %s\n",
                    vnics->dvpg_id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "VNics vm_id %s\n",
                    vnics->vm_id.c_str());
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", "************************************************\n");

            vnics->dvpg_nn  = pgname;

        }

        return (static_cast<class VM *>(asset));
    }
} /* namespace DcmNs */
