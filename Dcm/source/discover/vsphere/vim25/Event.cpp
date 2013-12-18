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
 * Event.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: rmadapur
 */

#define SUB_EVENT_DELETE(EVTTYPE)  if (type.compare(#EVTTYPE) == 0) { \
                delete ((EVTTYPE *) obj);\
            }
#define SUB_EVENT_CREATE(EVTTYPE) if (type.compare(#EVTTYPE) == 0) { \
                if (obj == NULL) { \
                    obj = new EVTTYPE(); \
                }\
                ((EVTTYPE *) obj)->deSerialize(childNode); \
            }

#include "Event.h"

namespace DcmNs {

    Event::Event() :
            obj(NULL) {

    }

    Event::~Event() {

        //printf("Event Destructor\n");
        if (obj != NULL) {
            SUB_EVENT_DELETE(DatacenterRenamedEvent)
            else SUB_EVENT_DELETE(DatacenterCreatedEvent)
            else SUB_EVENT_DELETE(HostConnectedEvent)
            else SUB_EVENT_DELETE(HostAddedEvent)
            else SUB_EVENT_DELETE(HostRemovedEvent)
            else SUB_EVENT_DELETE(VmCreatedEvent)
            else SUB_EVENT_DELETE(VmPoweredOnEvent)
            else SUB_EVENT_DELETE(VmPoweredOffEvent)
            else SUB_EVENT_DELETE(VmRenamedEvent)
            else SUB_EVENT_DELETE(VmReconfiguredEvent)
            else SUB_EVENT_DELETE(VmBeingHotMigratedEvent)
            else SUB_EVENT_DELETE(VmBeingMigratedEvent)
            else SUB_EVENT_DELETE(VmRemovedEvent)
            else SUB_EVENT_DELETE(DvsCreatedEvent)
            else SUB_EVENT_DELETE(DvsDestroyedEvent)
            else SUB_EVENT_DELETE(DvsReconfiguredEvent)
            else SUB_EVENT_DELETE(DvsRenamedEvent)
            else SUB_EVENT_DELETE(DVPortgroupCreatedEvent)
            else SUB_EVENT_DELETE(DVPortgroupDestroyedEvent)
            else SUB_EVENT_DELETE(DVPortgroupReconfiguredEvent)
            else SUB_EVENT_DELETE(DVPortgroupRenamedEvent)
            else SUB_EVENT_DELETE(TaskEvent)

        }
    }
    void Event::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            XMLUtil::extractAttributeString(node->properties, "type",
                    this->type);

            XML_EXTRACTSTRING(childNode, "key", this->key);
            XML_EXTRACTSTRING(childNode, "chainId", this->chainId);
            XML_EXTRACTSTRING(childNode, "createdTime", this->createdTime);
            XML_EXTRACTSTRING(childNode, "userName", this->userName);
            XML_EXTRACTSTRING(childNode, "fullFormattedMessage",
                    this->fullFormattedMessage);
            WSObject::extractObject(childNode, "datacenter", &this->datacenter);
            WSObject::extractObject(childNode, "host", &this->host);
            WSObject::extractObject(childNode, "computeResource",
                    &this->computeResource);
            WSObject::extractObject(childNode, "vm", &this->vm);
            WSObject::extractObject(childNode, "dvs", &this->dvs);
            WSObject::extractObject(childNode, "net", &this->net);

            SUB_EVENT_CREATE(DatacenterCreatedEvent)
            else SUB_EVENT_CREATE(DatacenterRenamedEvent)
            else SUB_EVENT_CREATE(HostConnectedEvent)
            else SUB_EVENT_CREATE(HostAddedEvent)
            else SUB_EVENT_CREATE(HostRemovedEvent)
            else SUB_EVENT_CREATE(VmCreatedEvent)
            else SUB_EVENT_CREATE(VmPoweredOnEvent)
            else SUB_EVENT_CREATE(VmPoweredOffEvent)
            else SUB_EVENT_CREATE(VmRenamedEvent)
            else SUB_EVENT_CREATE(VmReconfiguredEvent)
            else SUB_EVENT_CREATE(VmBeingHotMigratedEvent)
            else SUB_EVENT_CREATE(VmBeingMigratedEvent)
            else SUB_EVENT_CREATE(VmRemovedEvent)
            else SUB_EVENT_CREATE(DvsCreatedEvent)
            else SUB_EVENT_CREATE(DvsDestroyedEvent)
            else SUB_EVENT_CREATE(DvsReconfiguredEvent)
            else SUB_EVENT_CREATE(DvsRenamedEvent)
            else SUB_EVENT_CREATE(DVPortgroupCreatedEvent)
            else SUB_EVENT_CREATE(DVPortgroupDestroyedEvent)
            else SUB_EVENT_CREATE(DVPortgroupReconfiguredEvent)
            else SUB_EVENT_CREATE(DVPortgroupRenamedEvent)
            else SUB_EVENT_CREATE(TaskEvent)

            childNode = childNode->next;
        }
    }
    DataCenterEventArgument::DataCenterEventArgument() {

    }

    DataCenterEventArgument::~DataCenterEventArgument() {
        // TODO Auto-generated destructor stub
    }
    void DataCenterEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "datacenter", &this->datacenter);
            childNode = childNode->next;
        }
    }

    void DatastoreEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "datastore", &this->datastore);
            childNode = childNode->next;
        }
    }
    void FolderEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "folder", &this->folder);
            childNode = childNode->next;
        }
    }
    void NetworkEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "network", &this->network);
            childNode = childNode->next;
        }
    }
    void DvsEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "dvs", &this->dvs);
            childNode = childNode->next;
        }
    }

    HostEventArgument::HostEventArgument() {

    }

    HostEventArgument::~HostEventArgument() {
        // TODO Auto-generated destructor stub
    }
    void HostEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "host", &this->host);
            childNode = childNode->next;
        }
    }
    ComputeResourceEventArgument::ComputeResourceEventArgument() {

    }

    ComputeResourceEventArgument::~ComputeResourceEventArgument() {
        // TODO Auto-generated destructor stub
    }
    void ComputeResourceEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "computeResource",
                    &this->computeResource);
            childNode = childNode->next;
        }
    }

    VmEventArgument::VmEventArgument() {

    }

    VmEventArgument::~VmEventArgument() {
        // TODO Auto-generated destructor stub
    }
    void VmEventArgument::deSerialize(xmlNodePtr node) {
        //Calling Super Deserialize
        EntityEventArgument::deSerialize(node);
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            WSObject::extractObject(childNode, "vm", &this->vm);
            childNode = childNode->next;
        }
    }

    EntityEventArgument::EntityEventArgument() {

    }

    EntityEventArgument::~EntityEventArgument() {
        // TODO Auto-generated destructor stub
    }
    void EntityEventArgument::deSerialize(xmlNodePtr node) {

        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {

            XML_EXTRACTSTRING(childNode, "name", this->name);
            childNode = childNode->next;
        }
    }

    DatacenterRenamedEvent::DatacenterRenamedEvent() {

    }

    DatacenterRenamedEvent::~DatacenterRenamedEvent() {
        // TODO Auto-generated destructor stub
    }
    void DatacenterRenamedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            XML_EXTRACTSTRING(node, "newName", this->newName);
            XML_EXTRACTSTRING(node, "oldName", this->oldName);
            node = node->next;
        }
    }

    DatacenterCreatedEvent::DatacenterCreatedEvent() {

    }

    DatacenterCreatedEvent::~DatacenterCreatedEvent() {
        // TODO Auto-generated destructor stub
    }
    void DatacenterCreatedEvent::deSerialize(xmlNodePtr node) {

    }

    void VmEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            XML_EXTRACTBOOLEAN(node, "template", this->_template);

            node = node->next;
        }
    }

    void VmRenamedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            XML_EXTRACTSTRING(node, "newName", this->newName);
            XML_EXTRACTSTRING(node, "oldName", this->oldName);

            node = node->next;
        }
    }
    void VmBeingHotMigratedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            WSObject::extractObject(node, "destHost", &this->destHost);

            node = node->next;
        }
    }
    void VmBeingMigratedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            WSObject::extractObject(node, "destHost", &this->destHost);
            WSObject::extractObject(node, "destDatacenter",
                    &this->destDatacenter);
            WSObject::extractObject(node, "destDatastore",
                    &this->destDatastore);

            node = node->next;
        }
    }

    void DvsCreatedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            WSObject::extractObject(node, "parent", &this->parent);

            node = node->next;
        }
    }
    void DVPortgroupRenamedEvent::deSerialize(xmlNodePtr node) {

        while (node != NULL) {

            XML_EXTRACTSTRING(node, "newName", this->newName);
            XML_EXTRACTSTRING(node, "oldName", this->oldName);
            node = node->next;
        }
    }
    void DvsRenamedEvent::deSerialize(xmlNodePtr node) {

            while (node != NULL) {

                XML_EXTRACTSTRING(node, "newName", this->newName);
                XML_EXTRACTSTRING(node, "oldName", this->oldName);
                node = node->next;
            }
        }
    void TaskInfoState::deSerialize(xmlNodePtr node) {

                    while (node != NULL) {

                        XML_EXTRACTSTRING(node, "state", this->state);
                        node = node->next;
                    }
                }


    void TaskInfo::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
               while (childNode != NULL) {


                    WSObject::extractObject(childNode, "state",
                                        &this->state);
                    WSObject::extractObject(childNode, "task",
                                                                        &this->task);
                    XML_EXTRACTSTRING(childNode, "entityName", this->entityName);
                    childNode = childNode->next;
                }
            }

    void TaskEvent::deSerialize(xmlNodePtr node) {

            while (node != NULL) {

                WSObject::extractObject(node, "info",
                                    &this->info);


                node = node->next;
            }
        }


} /* namespace DcmNs */
