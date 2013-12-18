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

#include "TaskEventHandler.h"
#include <iostream>
#include "correlator/EventCorrelator.h"

namespace DcmNs {
    TaskEventHandler::TaskEventHandler(string vcenter, Event* event):EventHandler(vcenter, event) {

    }

    TaskEventHandler::~TaskEventHandler() {
    }

    void TaskEventHandler::handle(ServiceInstance *serviceInstance) {

        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "------Task Event Handler ----------- %s ", m_event->type.c_str());
        printEvent();
        string message = m_event->fullFormattedMessage;
        VIC_PLUG_LOG(VIC_TRACE_INFO,"Event fullFormattedMessage *%s*",
                message.c_str());


        correlateOperation_t op = NOOP;
        correlateObject_t obj_type = ALL;

        string host_mor = m_event->host.host.val;
        string dc_mor = m_event->datacenter.datacenter.val;
        if ((message == TASK_UPDATE_NET_CONF) || 
                (message == TASK_ADD_VS) ||
                (message == TASK_ADD_VNIC) ||
                (message == TASK_ADD_PG)) {
            //added portgroup, vmkernel, console vnic, or vswitch
            op = EVENT_INSERT;
        } else if (message == TASK_REMOVE_VS) {
            //remove vswitch
            op = EVENT_DELETE;
            obj_type = VSWITCH;
        } else if (message == TASK_REMOVE_VNIC) {
            //on remove vmkernel, this is the first message
            op = EVENT_DELETE;
            obj_type = VMKERNEL;
        } else if (message == TASK_REMOVE_PG) {
            //remove portgroup, vmkernel, console vnic 
            op = EVENT_DELETE;
            obj_type = PORTGROUP;
        } else if (message == TASK_REMOVE_VMK) {
            //on remove console vnic, this is the first message
            op = EVENT_DELETE;
            obj_type = CONSOLEVNIC;
        } else if (message == TASK_RECONF_PG) {
            //pg, vmkernel, console vnic vlan update 
            VIC_PLUG_LOG(VIC_TRACE_INFO,"reconfigure portgroup %s",
                    message.c_str());
            renamePortgroup(serviceInstance);
            return;
        } else if (message == TASK_REMOVE_DC) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Processing remove datacenter %s",
                    message.c_str());
            removeDatacenter(serviceInstance,(((TaskEvent *) m_event->obj)->info.task) , dc_mor);
            return;
        } else if (message == TASK_UPDATE_DNS_CONF) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Processing update dns conf  Event %s",
                    message.c_str());
            renameHost(serviceInstance, m_event->host.host, dc_mor);
            return;

        } else if ((message == TASK_REGISTER_VM) ||
                (message == TASK_RELOCATE_VM) ||
                (message == TASK_CLONE_VM)) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Task Event %s",
                    message.c_str());
            registerVM(serviceInstance);
            return;
        } else if ( message == TASK_UPDATE_RESPOOL_DVS ||
                message == TASK_IMPORT_CONF ) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Processing resource pool update on dvs%s",
                    message.c_str());
            qosUpdate(serviceInstance, m_event->dvs.dvs.val , dc_mor);
            return;
        } else if (message == TASK_ADD_HOST) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Task: Add standalone host %s",
                    message.c_str());
            registerHost(serviceInstance);
            return;
        } else if (message == TASK_MARK_VM_TEMPLATE) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"VM marked as template %s",
                    message.c_str());
            vmDelete();
            return;
        } else if (message == TASK_VM_RECONF) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"VM reconf treating it as vm rename %s",
                    message.c_str());
            int ret =  waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
            if (ret == 0) return; 

            vmRename(serviceInstance, host_mor, m_event->vm.vm.val, m_event->vm.name, dc_mor);
            return;
        } else {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Un supported TASK Event %s",
                    message.c_str());
            return;
        }

        int ret =  waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret == 0) return; 
        //sleep(30);

        Host* host = retrieveOneHost(serviceInstance, host_mor);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this HOST, Skipping HOST ",host_mor.c_str());
            return;
        }

        host->printAsset();
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        EventCorrelator ec(vcenterDB, host, dc_mor, m_vcenter);
        ec.findSendTaskUpdates(op, obj_type);

        delete host;
    }


    // if a task is not done in 120 sec we stop trying.
    int TaskEventHandler::waitTillTaskDone(ServiceInstance *serviceInstance, ManagedObjectReference mor) {
        if (mor.val == "") {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"Skipping Invalid Task ID (%s)",mor.val.c_str());
            return 0;
        }
        int retry = 40;
        while(retry >0 ) {
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Checking if task is done...  (%d):(%s)",
                    retry, mor.val.c_str());

            string status = getTaskStatus(serviceInstance, mor);
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"status ... %s (%d)",
                    status.c_str(), retry);

            if (status == "success") {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,"TASK Completion success ... %s ",
                        mor.val.c_str());
                return 1;
            }
            if (status == "error") {
                VIC_PLUG_LOG(VIC_TRACE_INFO,"TASK Completion failed ... %s ",
                        mor.val.c_str());
                return 0;
            }
            retry--;
            sleep(15);

        }
        VIC_PLUG_LOG(VIC_TRACE_INFO,"TASK Completion not completed ... %s ",
                mor.val.c_str());
        return 0;
    }
    void TaskEventHandler::qosUpdate(ServiceInstance *serviceInstance,string  dvs_mor, string dc_id) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret ==0)
            return;
        // get all dvpgs for this dvs including the qos.
        string vc_id = m_vcenter;
        string vcenterDB = FileUtils::getDBLocation(vc_id);

        DvpgDAO dao(vcenterDB);
        vector<Domain*> daoVec = dao.findAllBy(dvs_mor, dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "%s",
                " ----------invoking retrieveQosMap-----------");
        map<string, string> nrpToQosMap;
        retrieveQosMap(serviceInstance,nrpToQosMap);

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *dvpg = daoVec[i];
            string pg_id = dvpg->getId();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Processing PG %s %s.",pg_id.c_str(),dvpg->getNetworkResourcePoolKey().c_str());

            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "----------returned  retrieveQosMap----------- %s ",pg_id.c_str());
            map<string, string>::iterator itnrpQosMap;
            string qos="";
            for (itnrpQosMap = nrpToQosMap.find(dvpg->getNetworkResourcePoolKey());
                    itnrpQosMap != nrpToQosMap.end();
                    itnrpQosMap++) {
                VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                        " 1 NRP ID %s = QOS ID %s",
                        (*itnrpQosMap).first.c_str(),
                        (*itnrpQosMap).second.c_str());
                qos = (*itnrpQosMap).second;
            }

            VIC_PLUG_LOG(VIC_TRACE_DEBUG,
                    " ----------QOS  retrieveQosMap-----------old %s new %s ",dvpg->getQos().c_str(), qos.c_str());
            if (dvpg->getQos() != qos) {
                VIC_PLUG_LOG(VIC_TRACE_INFO,
                        " QOS  Updated to %s",qos.c_str());
                DVPGEventMessage *pMessage = new DVPGEventMessage();
                DvpgDomain dvpgDb(dvpg->getId(), URLEncode::dbencode(dvpg->getName()), dvpg->getVlan(), qos,
                        dvpg->getDvsId(), m_event->dvs.name,
                        m_event->datacenter.datacenter.val,
                        dvpg->getNetworkResourcePoolKey(),dvpg->getDVPGType()); // dc and dvs shouldbe present

                pMessage->m_opcode=VNM_RECONF;
                pMessage->m_vcenter=m_vcenter;
                pMessage->m_domain=dvpgDb;
                DistributionSender *ds = DistributionSender::getInstance();
                ds->publishEvent(pMessage);
                delete pMessage;
            }
        }
        // get qos map for this dvs
        // for each dvpg check if qos changed?
        // update it.


    }

    void TaskEventHandler::removeDatacenter(ServiceInstance *serviceInstance,ManagedObjectReference mor, string dc_id) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret ==0)
            return;
        // delete the datacenter and its members
        // get all hosts of this datacenter and delete each of them.
        string vc_id = m_vcenter;
        string vcenterDB = FileUtils::getDBLocation(vc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO,
                "%s ------opening DB----------- %s", vc_id.c_str(), vcenterDB.c_str());

        HostDAO dao(vcenterDB);
        //        vector<Domain*> dddaoVec = dao.findAllBy("host-1990", "datacenter-1341");
        vector<Domain*> daoVec = dao.findAllBy("", dc_id);
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            string host_id = domain->getHostId();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "Delete host %s",host_id.c_str() );
            hostRemove(host_id, m_vcenter, dc_id);
        }

        DatacenterEventMessage *pMessage = new DatacenterEventMessage();
        DataCenterDomain dc(dc_id, URLEncode::dbencode(m_event->datacenter.name));
        pMessage->m_opcode=VNM_DELETE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=dc;
        DistributionSender *ds = DistributionSender::getInstance();
        ds->publishEvent(pMessage);

        delete pMessage;
        DAO::closeDb();

    }

    void TaskEventHandler::registerHost(ServiceInstance *serviceInstance) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret ==0)
            return;
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        string dc_mor = m_event->datacenter.datacenter.val;

        vector<Domain*> old_v, new_v, added, deleted, updated;
        // find added, deleted, updated Domains
        vector <Asset*> assets = retrieveHosts(serviceInstance);
        Asset::printAssetVector(assets);
        EventCorrelator ec(vcenterDB,  dc_mor, m_vcenter);
        ec.findAddDeleteUpdates(HOST_DAO, assets, EVENT_INSERT, old_v, new_v, deleted, added, updated);

        for (unsigned int i = 0 ; i < added.size(); i++ ) {
            Domain *dom = added[i];
            VIC_PLUG_LOG(VIC_TRACE_DEBUG,"Added Host %s:%s",dom->getId().c_str(), dom->getName().c_str());
            if(isHostPresent( dom->getId(), m_vcenter, m_event->datacenter.datacenter.val)) {
                VIC_PLUG_LOG(VIC_TRACE_INFO, "Host already created skipping host create %s ", dom->getId().c_str());
                return;
            }

            hostCreate(serviceInstance, dom->getId(),m_vcenter, m_event->datacenter.datacenter.val );
        }
    }

    void TaskEventHandler::registerVM(ServiceInstance *serviceInstance) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret ==0)
            return;

        // get VMS from DB
        //VmDAO dao(vcenterDB);
        //vector<Domain*> daoVec = dao.findAll();
        //VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        string dc_mor = m_event->datacenter.datacenter.val;

        EventCorrelator ec(vcenterDB,  dc_mor, m_vcenter);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s","find asset updates");
        vector<Domain*> old_v, new_v, added, deleted, updated;
        // find added, deleted, updated Domains
        vector <Asset*> assets = retrieveVMs(serviceInstance);
        Asset::printAssetVector(assets);

        ec.findAddDeleteUpdates(VM_DAO, assets, EVENT_INSERT, old_v, new_v, deleted, added, updated);

        VIC_PLUG_LOG(VIC_TRACE_INFO,"Added VM :%d",added.size());
        for (unsigned int i = 0 ; i < added.size(); i++ ) {
            Domain *dom = added[i];
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Added VM %s:%s",dom->getId().c_str(), dom->getHostId().c_str());
            string host_name = getHostName(dom->getHostId());

            vmCreate(serviceInstance, dom->getId(),  dom->getHostId(),  host_name, m_vcenter, m_event->datacenter.datacenter.val);
        }
        ec.findAddDeleteUpdates(VM_DAO, assets, EVENT_UPDATE, old_v, new_v, deleted, added, updated);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"Updated VM :%d",updated.size());
        for (unsigned int i = 0 ; i < updated.size(); i++ ) {
            Domain *dom = updated[i];
            VIC_PLUG_LOG(VIC_TRACE_INFO,"Updated VM %s:%s:%s",dom->getId().c_str(), dom->getName().c_str(), dom->getHostId().c_str());

            vmReconf(serviceInstance, dom->getHostId(),  dom->getId(), dom->getName(), m_event->datacenter.datacenter.val);
        }

        ec.releaseSource(old_v);
        ec.releaseSource(new_v);

        //string host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);


        // find the new VM
        //do a vmcreate on it
    }

    string TaskEventHandler::getHostName(string host_id) {
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        HostDAO dao(vcenterDB);
        //        vector<Domain*> dddaoVec = dao.findAllBy("host-1990", "datacenter-1341");
        vector<Domain*> daoVec = dao.findAllBy(host_id, "");
        VIC_PLUG_LOG(VIC_TRACE_INFO, "find all returned %d",daoVec.size());

        for (unsigned int i = 0 ; i < daoVec.size(); i++ ) {
            Domain *domain = daoVec[i];
            string host_nn = domain->getName();
            string hostid = domain->getId();
            VIC_PLUG_LOG(VIC_TRACE_DEBUG, "get host %s:%s.",hostid.c_str(), host_nn.c_str() );
            return host_nn;
        }
        return "";
    }

    void TaskEventHandler::renamePortgroup(ServiceInstance *serviceInstance) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        sleep(10);
        if (ret ==0)
            return;
        string host_mor = m_event->host.host.val;
        string dc_mor = m_event->datacenter.datacenter.val;
        correlateOperation_t op = EVENT_DELETE;
        correlateObject_t obj_type = PORTGROUP;
        Host* host = retrieveOneHost(serviceInstance, host_mor);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this HOST, Skipping HOST ",host_mor.c_str());
            return;
        }

        host->printAsset();
        string vcenterDB = FileUtils::getDBLocation(m_vcenter);
        EventCorrelator ec(vcenterDB, host, dc_mor, m_vcenter);
        ec.findSendTaskUpdates(op, obj_type);

        op = EVENT_INSERT;
        ec.findSendTaskUpdates(op, obj_type);

        op = EVENT_UPDATE;
        ec.findSendTaskUpdates(op, obj_type);
        delete host;
    }

    void TaskEventHandler::renameHost(ServiceInstance *serviceInstance,ManagedObjectReference morVal, string dc_id) {
        int ret = waitTillTaskDone(serviceInstance,((TaskEvent *) m_event->obj)->info.task);
        if (ret ==0)
            return;
        Host* host = retrieveAHost(serviceInstance, morVal.val);
        if (host == NULL) {
            VIC_PLUG_LOG(VIC_TRACE_ERROR,"%s %s", "Error fetching this HOST, Skipping HOST ",morVal.val.c_str());
            return;
        }

        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host ID%s ",
                host->id.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  HostName %s",
                host->hostName.c_str());
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  DomainName %s",
                host->domainName.c_str());

        string host_name;
        host_name = URLEncode::dbencode(host->hostName + +"." + host->domainName);
        HostDomain hostDb(host->id, host_name, dc_id); //dc should be present
        HostEventMessage *pMessage = new HostEventMessage();

        pMessage->m_opcode=VNM_UPDATE;
        pMessage->m_vcenter=m_vcenter;
        pMessage->m_domain=hostDb;
        DistributionSender *ds = DistributionSender::getInstance();
        VIC_PLUG_LOG(VIC_TRACE_INFO, "Host  renamed message %s",
                host->hostName.c_str());

        ds->publishEvent(pMessage);

        delete pMessage;
    }
} /* namespace DcmNs */
