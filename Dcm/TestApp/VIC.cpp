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

//============================================================================
// Name        : VIC.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "discover/vsphere/WSClient.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <string>
#include <stdexcept>

#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ws/vimStub.h"

#include "discover/vsphere/vim25/ObjectSpec.h"
#include "discover/vsphere/vim25/TraversalSpec.h"
#include "discover/vsphere/vim25/PropertySpec.h"
#include "discover/vsphere/vim25/PropertyFilterSpec.h"
#include "discover/vsphere/vim25/HostVirtualNic.h"
#include "discover/vsphere/vim25/PhysicalNic.h"
#include "discover/vsphere/vim25/HostPortGroup.h"
#include "discover/vsphere/vim25/HostProxySwitch.h"
#include "discover/vsphere/vim25/HostVirtualSwitch.h"
#include "discover/vsphere/vim25/DistributedVirtualSwitchHostMember.h"
#include "discover/vsphere/vim25/VMwareDVSPortSetting.h"
#include "discover/vsphere/vim25/VirtualMachinePowerState.h"
#include "discover/vsphere/vim25/VirtualDevice.h"
#include "discover/vsphere/vim25/Tag.h"
#include "discover/vsphere/vim25/EventFilterSpec.h"
#include "discover/vsphere/vim25/mo/ServiceInstance.h"

#include "discover/vsphere/vim25/mo/ViewManager.h"
#include <map>


using namespace std;
using namespace DcmNs;

PropertyFilterSpec createEventFilterSpec(
        ManagedObjectReference *eventHistoryCollector) {

    PropertySpec propertyspec;
    propertyspec.all = false;
    propertyspec.type = eventHistoryCollector->type;
    propertyspec.pathSet.push_back("latestPage");

    ObjectSpec objspec;
    objspec.obj = *eventHistoryCollector;
    objspec.skip = false;

    PropertyFilterSpec propertyFilterSpec;
    propertyFilterSpec.propSet.push_back(propertyspec);
    propertyFilterSpec.objectSet.push_back(objspec);

    return propertyFilterSpec;
}

void checkAndPrintError(int vcRes, Fault *fault) {
    if (vcRes == VIC_WSCLIENT_CURL_FAULT_RESPONSE_ERROR) {
        printf("Fault Error String %s\n", fault->faultstring.c_str());
    }
}

void queryEvents(ServiceInstance *serviceInstance) {

    {

        EventFilterSpec eventFilterSpec;
        eventFilterSpec.type.push_back("DatacenterCreatedEvent");
        //eventFilterSpec.type.push_back("DatacenterRenamedEvent");
        //eventFilterSpec.type.push_back("HostConnectedEvent");
        //eventFilterSpec.type.push_back("HostAddedEvent");
        //eventFilterSpec.type.push_back("HostRemovedEvent");
        eventFilterSpec.type.push_back("VmCreatedEvent");
        //eventFilterSpec.type.push_back("VmPoweredOnEvent");
        //eventFilterSpec.type.push_back("VmPoweredOffEvent");
        eventFilterSpec.type.push_back("VmRenamedEvent");
        //eventFilterSpec.type.push_back("VmReconfiguredEvent");
        //eventFilterSpec.type.push_back("VmBeingHotMigratedEvent");
        //eventFilterSpec.type.push_back("VmBeingMigratedEvent");
        //eventFilterSpec.type.push_back("VmRemovedEvent");
        //eventFilterSpec.type.push_back("DvsCreatedEvent");
        //eventFilterSpec.type.push_back("DVPortgroupCreatedEvent");
        //eventFilterSpec.type.push_back("DVPortgroupDestroyedEvent");
        //eventFilterSpec.type.push_back("DVPortgroupReconfiguredEvent");
        //eventFilterSpec.type.push_back("DVPortgroupRenamedEvent");
        //eventFilterSpec.type.push_back("DvsDestroyedEvent");
        //eventFilterSpec.type.push_back("DvsReconfiguredEvent");
        // eventFilterSpec.type.push_back("DvsRenamedEvent");

        //eventFilterSpec.type.push_back("VmClonedEvent");
        eventFilterSpec.time.beginTime = "2009-01-03T04:53:27.696999Z";
        eventFilterSpec.time.endTime = "2012-06-25T04:53:27.696999Z";

        EventManager *eventManager = NULL;
        serviceInstance->getEventManager(&eventManager);
        auto_ptr<EventManager> pautoEventManager(eventManager);

        // ManagedObjectReference *eventHistoryCollector = NULL;

        EventHistoryCollector *eventHistoryCollector = NULL;

        int vcRes = eventManager->createCollectorForEvents(&eventFilterSpec,
                &eventHistoryCollector);

        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            printf("Failed to create Event History Collector\n");
            return;
        }
        auto_ptr<EventHistoryCollector> pautoEventHistoryCollector(
                eventHistoryCollector);

        vector<Event *> *eventList;
        while (true) {
            eventHistoryCollector->readNextEvents(10, &eventList);
            if (eventList->size() == 0) {
                break;
            }
            for (size_t m = 0; m < eventList->size(); m++) {
                Event *event = eventList->at(m);
                printf("Event Type %s\n", event->type.c_str());
                printf("Event chain Id %s\n", event->chainId.c_str());
                printf("Event key %s\n", event->key.c_str());
                printf("Event userName %s\n", event->userName.c_str());
                printf("Event createdTime  %s\n", event->createdTime.c_str());
                printf("Event fullFormattedMessage  %s\n",
                        event->fullFormattedMessage.c_str());

                printf("Event DatacEnter ID %s\n",
                        event->datacenter.datacenter.val.c_str());
                printf("Event DatacEnter Name %s\n",
                        event->datacenter.name.c_str());

                printf("Event host ID %s\n", event->host.host.val.c_str());
                printf("Event host Name %s\n", event->host.name.c_str());
                printf("Event dvs ID %s\n", event->dvs.dvs.val.c_str());
                printf("Event dvs Name %s\n", event->dvs.name.c_str());
                printf("Event netwrork ID %s\n",
                        event->net.network.val.c_str());
                printf("Event netwrork Name %s\n", event->net.name.c_str());

                printf("Event computeResource ID %s\n",
                        event->computeResource.computeResource.val.c_str());
                printf("Event computeResource Name %s\n",
                        event->computeResource.name.c_str());

                printf("Event VM ID %s\n", event->vm.vm.val.c_str());
                printf("Event VM Name %s\n", event->vm.name.c_str());
//                VmRenamedEvent
                if (event->type.compare("VmRenamedEvent") == 0) {

//                	VmRenamedEvent *vmr = (VmRenamedEvent *) event->obj;
                    printf(
                            "VM Renamed \n");



                }
                if (event->type.compare("DatacenterRenamedEvent") == 0) {
                    printf(
                            "Event DatacEnter Old Name %s\n",
                            ((DatacenterRenamedEvent *) event->obj)->oldName.c_str());
                    printf(
                            "Event DatacEnter New Name %s\n",
                            ((DatacenterRenamedEvent *) event->obj)->newName.c_str());
                }
                if (event->type.compare("VmPoweredOffEvent") == 0) {
                    printf("EventVM Template  %d\n",
                            ((VmPoweredOffEvent *) event->obj)->_template);
                }
                if (event->type.compare("VmRenamedEvent") == 0) {
                    printf("Event VM Old Name %s\n",
                            ((VmRenamedEvent *) event->obj)->oldName.c_str());
                    printf("Event VM New Name %s\n",
                            ((VmRenamedEvent *) event->obj)->newName.c_str());
                }
                if (event->type.compare("VmBeingHotMigratedEvent") == 0) {

                    printf(
                            "Event host ID %s\n",
                            ((VmBeingHotMigratedEvent *) event->obj)->destHost.host.val.c_str());
                    printf(
                            "Event host Name %s\n",
                            ((VmBeingHotMigratedEvent *) event->obj)->destHost.name.c_str());
                }
                if (event->type.compare("VmBeingMigratedEvent") == 0) {

                    printf(
                            "Event host ID %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destHost.host.val.c_str());
                    printf(
                            "Event host Name %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destHost.name.c_str());
                    printf(
                            "Event destDatacenter ID %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destDatacenter.datacenter.val.c_str());
                    printf(
                            "Event destDatacenter Name %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destDatacenter.name.c_str());
                    printf(
                            "Event destDatastore ID %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destDatastore.datastore.val.c_str());
                    printf(
                            "Event destDatastore Name %s\n",
                            ((VmBeingMigratedEvent *) event->obj)->destDatastore.name.c_str());
                }
                if (event->type.compare("DvsCreatedEvent") == 0) {

                    printf(
                            "Event parent ID %s\n",
                            ((DvsCreatedEvent *) event->obj)->parent.folder.val.c_str());
                    printf(
                            "Event parent Name %s\n",
                            ((DvsCreatedEvent *) event->obj)->parent.name.c_str());
                }
                if (event->type.compare("DVPortgroupRenamedEvent") == 0) {
                    printf(
                            "Event dvpg Old Name %s\n",
                            ((DVPortgroupRenamedEvent *) event->obj)->oldName.c_str());
                    printf(
                            "Event dvpg DVPortgroupRenamedEvent Name %s\n",
                            ((DVPortgroupRenamedEvent *) event->obj)->newName.c_str());
                }
                if (event->type.compare("DvsRenamedEvent") == 0) {
                    printf("Event DVS Old Name %s\n",
                            ((DvsRenamedEvent *) event->obj)->oldName.c_str());
                    printf("Event DVS  Name %s\n",
                            ((DvsRenamedEvent *) event->obj)->newName.c_str());
                }
            }
            ListUtil<Event>::clearListofPointers(eventList);
        }

    }

}
void getTaskStatus(ServiceInstance *serviceInstance,ManagedObjectReference mor){
                  vector<string> properties;


    vector<PropertyFilterSpec> specSet = PropertyCollector::createPropertyFilterSpec(
             mor, properties);
    //Get Property Collector
    PropertyCollector *propCollector = NULL;
    int code = serviceInstance->getPropertyCollector(&propCollector);
    if (code != VIC_WSCLIENT_SUCCESS) {
        printf("Unable to get Property Collector");
        return;
    }
    auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);

    //Retrieve Properties
    vector<ObjectContent *> *objectContentList = NULL;
    code = propCollector->retrieveProperties(&specSet, &objectContentList);
    if (code != VIC_WSCLIENT_SUCCESS) {
        printf("Unable to Fetch Details for %s", mor.val.c_str());
        return;
    }
    for (size_t i = 0; i < objectContentList->size(); i++) {
        ObjectContent *objContent = objectContentList->at(i);

        vector<DynamicProperty *> *propSet = objContent->propset;
        for (size_t j = 0; j < propSet->size(); j++) {

            if (propSet->at(j)->objType.compare("TaskInfo") == 0) {
                TaskInfo *info = (TaskInfo *) propSet->at(j)->obj;
                printf("Task Info State %s\n", info->state.state.c_str());
                printf("Task Info Name %s\n", info->entityName.c_str());
            }
        }

    }
    //Delete the Pointer Vectors implicitly
    ListUtil<ObjectContent>::clearListofPointers(objectContentList);

}
void fetchEvents(ServiceInstance *serviceInstance) {
    {

        EventFilterSpec eventFilterSpec;
        eventFilterSpec.type.push_back("DatacenterRenamedEvent");
        eventFilterSpec.type.push_back("HostConnectedEvent");
        eventFilterSpec.type.push_back("HostAddedEvent");
        eventFilterSpec.type.push_back("HostRemovedEvent");
        eventFilterSpec.type.push_back("VmCreatedEvent");
        eventFilterSpec.type.push_back("VmPoweredOnEvent");
        eventFilterSpec.type.push_back("VmPoweredOffEvent");
        eventFilterSpec.type.push_back("VmRenamedEvent");
        eventFilterSpec.type.push_back("VmReconfiguredEvent");
        eventFilterSpec.type.push_back("VmBeingHotMigratedEvent");
        eventFilterSpec.type.push_back("VmBeingMigratedEvent");
        eventFilterSpec.type.push_back("VmRemovedEvent");
        eventFilterSpec.type.push_back("DvsCreatedEvent");
        eventFilterSpec.type.push_back("DVPortgroupCreatedEvent");
        eventFilterSpec.type.push_back("DVPortgroupDestroyedEvent");
        eventFilterSpec.type.push_back("DVPortgroupReconfiguredEvent");
        eventFilterSpec.type.push_back("DVPortgroupRenamedEvent");
        eventFilterSpec.type.push_back("DvsDestroyedEvent");
        eventFilterSpec.type.push_back("DvsReconfiguredEvent");
         eventFilterSpec.type.push_back("DvsRenamedEvent");

        eventFilterSpec.type.push_back("VmClonedEvent"); 
        eventFilterSpec.type.push_back("TaskEvent");

        //eventFilterSpec.time.beginTime = "2012-06-03T04:53:27.696999Z";
        //eventFilterSpec.time.endTime = "2012-06-25T04:53:27.696999Z";

        EventManager *eventManager = NULL;
        serviceInstance->getEventManager(&eventManager);
        auto_ptr<EventManager> pautoEventManager(eventManager);

        ManagedObjectReference *eventHistoryCollector = NULL;

        int vcRes = eventManager->createCollectorForEvents(&eventFilterSpec,
                &eventHistoryCollector);

        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            printf("Failed to create Event History Collector\n");
            return;
        }
        auto_ptr<ManagedObjectReference> pautoEventHistoryCollector(
                eventHistoryCollector);

        PropertyFilterSpec propertyFilterSpec = createEventFilterSpec(
                eventHistoryCollector);

        //Get Property Collector
        PropertyCollector *propCollector = NULL;
        int code = serviceInstance->getPropertyCollector(&propCollector);
        if (code != VIC_WSCLIENT_SUCCESS) {
            printf("Unable to get Property Collector");
            return;
        }
        auto_ptr<PropertyCollector> pautoPropertyCollector(propCollector);

        ManagedObjectReference *propFilter = NULL;
        vcRes = propCollector->createFilter(&propertyFilterSpec, true,
                &propFilter);
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            printf("Failed to create Event Filter \n");
            return;
        }
        auto_ptr<ManagedObjectReference> pautoPropFilter(propFilter);
        string version = "";
        while (1) {
            UpdateSet *updateSet = NULL;
            //
            vcRes = propCollector->waitForUpdates(version, &updateSet);
            if (vcRes != VIC_WSCLIENT_SUCCESS) {
                checkAndPrintError(vcRes, serviceInstance->getLastFault());
                printf("Failed to execute waitForupdates\n");
                return;
            }
            auto_ptr<UpdateSet> pautoUpdateSet(updateSet);
            version = updateSet->version;
            printf("UpdateSet version %s\n", updateSet->version.c_str());

            for (size_t i = 0; i < updateSet->filterSet->size(); i++) {
                PropertyFilterUpdate * propertyFilterUpdate =
                        updateSet->filterSet->at(i);
                ManagedObjectReference filter = propertyFilterUpdate->filter;
                printf("Filter type %s\n", filter.type.c_str());
                printf("Filter val %s\n", filter.val.c_str());
                vector<ObjectUpdate *> *objectSet =
                        propertyFilterUpdate->objectSet;

                for (size_t s = 0; s < objectSet->size(); s++) {
                    ObjectUpdate * objectUpdate = objectSet->at(s);
                    printf("ObjectUpdateKind %s\n",
                            objectUpdate->kind.val.c_str());
                    ManagedObjectReference _obj = objectUpdate->obj;
                    printf("ObjectUpdate obj type %s\n", _obj.type.c_str());
                    printf("ObjectUpdate obj val %s\n", _obj.val.c_str());
                    vector<PropertyChange *> * changeSet =
                            objectUpdate->changeSet;

                    for (size_t k = 0; k < changeSet->size(); k++) {
                        PropertyChange *propertyChange = changeSet->at(k);
                        printf("PropertyChange Name %s\n",
                                propertyChange->name.c_str());
                        printf("PropertyChange valType %s\n",
                                propertyChange->valType.c_str());
                        printf("PropertyChange op val %s\n",
                                propertyChange->op.val.c_str());

                        if ((propertyChange->valType.compare("ArrayOfEvent")
                                == 0)
                                || (propertyChange->valType.find("Event")
                                        != std::string::npos)) {
                            vector<Event *> *eventList =
                                    (vector<Event *> *) propertyChange->val;
                            for (size_t m = 0; m < eventList->size(); m++) {
                                Event *event = eventList->at(m);
                                printf("Event Type %s\n", event->type.c_str());
                                printf("Event chain Id %s\n", event->chainId.c_str());
                                printf("Event key %s\n", event->key.c_str());
                                printf("Event userName %s\n", event->userName.c_str());
                                printf("Event createdTime  %s\n", event->createdTime.c_str());
                                printf("Event fullFormattedMessage  %s\n",
                                        event->fullFormattedMessage.c_str());

                                printf("Event DatacEnter ID %s\n",
                                        event->datacenter.datacenter.val.c_str());
                                printf("Event DatacEnter Name %s\n",
                                        event->datacenter.name.c_str());

                                printf("Event host ID %s\n", event->host.host.val.c_str());
                                printf("Event host Name %s\n", event->host.name.c_str());
                                printf("Event dvs ID %s\n", event->dvs.dvs.val.c_str());
                                printf("Event dvs Name %s\n", event->dvs.name.c_str());
                                printf("Event netwrork ID %s\n",
                                        event->net.network.val.c_str());
                                printf("Event netwrork Name %s\n", event->net.name.c_str());

                                printf("Event computeResource ID %s\n",
                                        event->computeResource.computeResource.val.c_str());
                                printf("Event computeResource Name %s\n",
                                        event->computeResource.name.c_str());

                                printf("Event VM ID %s\n", event->vm.vm.val.c_str());
                                printf("Event VM Name %s\n", event->vm.name.c_str());
                                if (event->type.compare(
                                        "DatacenterRenamedEvent") == 0) {
                                    printf(
                                            "Event DatacEnter Old Name %s\n",
                                            ((DatacenterRenamedEvent *) event->obj)->oldName.c_str());
                                    printf(
                                            "Event DatacEnter New Name %s\n",
                                            ((DatacenterRenamedEvent *) event->obj)->newName.c_str());
                                }else  if (event->type.compare(
                                        "TaskEvent") == 0) {
                                    printf(
                                            "Event Task id %s\n",
                                            ((TaskEvent *) event->obj)->info.task.val.c_str());
                                    printf(
                                                "Event Task id %s\n",
                                                                                ((TaskEvent *) event->obj)->info.state.state.c_str());

                                    getTaskStatus(serviceInstance,((TaskEvent *) event->obj)->info.task);

                                }

                            }
                        }

                    }
                }
            }
            sleep(2);
        }

        //Cancel Wait for Updates
        vcRes = propCollector->cancelWaitForUpdates();
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            printf("Failed to Cancel Wait For Updates\n");
            return;
        }
        //Destroy Property Filter
        vcRes = propCollector->destroyPropertyFilter(propFilter);
        if (vcRes != VIC_WSCLIENT_SUCCESS) {
            checkAndPrintError(vcRes, serviceInstance->getLastFault());
            printf("Failed to Destroy Property Filter\n");
            return;
        }

    }
}
int main(int argc, char *argv[]) {
//updateVmnicsOnHostRemove();
    //if (argc < 4) {
        //printf("usage: VIC <url> <user-id> <password>\n");
//        return 0;
    //}

    //vCenterSourceCollector col(argv[1],argv[2],argv[3],VNM_FULL_DISCOVERY);
    //col.run();
    //ServiceInstance ins("https://10.20.51.82");
    ServiceInstance ins("https://10.24.84.150");
    ins.login("administrator", "pass");

    /*DVPGBuilder dvpgbuilder(&ins);
     vector<Asset *> dvpgassetvector;
     dvpgbuilder.getAsset(dvpgassetvector);

     DVSBuilder builder(&ins);
     vector<Asset *> assetvector;
     builder.getAsset(assetvector);

     map<string, string> nrpToQosMap;
     for (size_t i = 0; i < assetvector.size(); i++) {
     DVS *dvs = (DVS *) assetvector.at(i);
     dvs->nrp_qosList;
     nrpToQosMap.insert(dvs->nrp_qosList.begin(), dvs->nrp_qosList.end());
     }

     map<string, string>::iterator it;

     for (it = nrpToQosMap.begin(); it != nrpToQosMap.end(); it++) {
     printf("Key %s = Value %s \n", (*it).first.c_str(),
     (*it).second.c_str());
     }



     for (size_t i = 0; i < dvpgassetvector.size(); i++) {
     DVPG *dvpg = (DVPG *) dvpgassetvector.at(i);

     map<string, string>::iterator itnrpToQosMap =
     nrpToQosMap.find( dvpg->networkResourcePoolKey);
     if(itnrpToQosMap!=nrpToQosMap.end()){
     string val  = (*itnrpToQosMap).second;
     printf(" DVPG %s , QOs  %s \n",dvpg->name.c_str(),val.c_str());
     }
     }
     */

    //queryEvents(&ins);
    fetchEvents(&ins);
//    Host* host = retrieveOneHost(&ins, "host-1112");
 //   parseOneHost(host, "host-1112");

    return 0;

}
