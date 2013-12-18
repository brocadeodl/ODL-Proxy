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
 * Event.h
 *
 *  Created on: Apr 9, 2012
 *      Author: rmadapur
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "discover/vsphere/vim25/WSObject.h"
#include <string>
#include <vector>
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ListUtil.h"
#include <map>

#define DATACENTER_RENAMED_EVENT "DatacenterRenamedEvent"
#define DATACENTER_CREATED_EVENT "DatacenterCreatedEvent"
using namespace std;

namespace DcmNs {

    //EventArguments
    class EntityEventArgument: public WSObject {
        public:
            string name;
            EntityEventArgument();
            virtual ~EntityEventArgument();
            void deSerialize(xmlNodePtr node);

    };

    class FolderEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference folder;

            void deSerialize(xmlNodePtr node);

    };
    class NetworkEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference network;

            void deSerialize(xmlNodePtr node);

    };
    class DvsEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference dvs;

            void deSerialize(xmlNodePtr node);

    };

    class HostEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference host;
            HostEventArgument();
            virtual ~HostEventArgument();
            void deSerialize(xmlNodePtr node);

    };

    class DataCenterEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference datacenter;
            DataCenterEventArgument();
            virtual ~DataCenterEventArgument();
            void deSerialize(xmlNodePtr node);

    };
    class DatastoreEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference datastore;

            void deSerialize(xmlNodePtr node);

    };

    class ComputeResourceEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference computeResource;
            ComputeResourceEventArgument();
            virtual ~ComputeResourceEventArgument();
            void deSerialize(xmlNodePtr node);

    };
    class VmEventArgument: public EntityEventArgument {
        public:
            ManagedObjectReference vm;
            VmEventArgument();
            virtual ~VmEventArgument();
            void deSerialize(xmlNodePtr node);

    };

    //Host Events
    class HostConnectedEvent: public WSObject {
        public:

            HostConnectedEvent() {

            }
            virtual ~HostConnectedEvent() {

            }
            void deSerialize(xmlNodePtr node) {

            }

    };
    class HostAddedEvent: public WSObject {
        public:

            HostAddedEvent() {

            }
            virtual ~HostAddedEvent() {

            }
            void deSerialize(xmlNodePtr node) {

            }

    };
    class HostRemovedEvent: public WSObject {
        public:

            HostRemovedEvent() {

            }
            virtual ~HostRemovedEvent() {

            }
            void deSerialize(xmlNodePtr node) {

            }

    };

    //Vm Events

    class VmCreatedEvent: public WSObject {
        public:

            VmCreatedEvent() {

            }
            virtual ~VmCreatedEvent() {

            }
            void deSerialize(xmlNodePtr node) {

            }

    };
    class VmEvent: public WSObject {
        public:
            bool _template;
            void deSerialize(xmlNodePtr node);
    };
    class VmPoweredOnEvent: public VmEvent {

    };
    class VmPoweredOffEvent: public VmEvent {

    };
    class VmRemovedEvent: public VmEvent {

    };
    class VmRenamedEvent: public VmEvent {
        public:
            string newName;
            string oldName;
            void deSerialize(xmlNodePtr node);
    };
    class VmReconfiguredEvent: public VmEvent {

    };
    class VmBeingHotMigratedEvent: public VmEvent {
        public:
            HostEventArgument destHost;
            void deSerialize(xmlNodePtr node);

    };
    class VmBeingMigratedEvent: public VmEvent {
        public:
            HostEventArgument destHost;
            DatastoreEventArgument destDatastore;
            DataCenterEventArgument destDatacenter;

            void deSerialize(xmlNodePtr node);

    };

    //DataCenter Events
    class DatacenterCreatedEvent: public WSObject {
        public:

            DatacenterCreatedEvent();
            virtual ~DatacenterCreatedEvent();
            void deSerialize(xmlNodePtr node);

    };

    class DatacenterRenamedEvent: public WSObject {
        public:
            string newName;
            string oldName;
            DatacenterRenamedEvent();
            virtual ~DatacenterRenamedEvent();
            void deSerialize(xmlNodePtr node);

    };

    //DVS Events
    class DvsCreatedEvent: public WSObject {
        public:
            FolderEventArgument parent;
            void deSerialize(xmlNodePtr node);

    };
    class DvsDestroyedEvent: public WSObject {

    };
    class DvsReconfiguredEvent: public WSObject {

    };
    class DvsRenamedEvent: public WSObject {
        public:
            string newName;
            string oldName;

            void deSerialize(xmlNodePtr node);

    };

    //DVPortGroup

    class DVPortgroupCreatedEvent: public WSObject {

    };

    class DVPortgroupDestroyedEvent: public WSObject {

    };
    class DVPortgroupReconfiguredEvent: public WSObject {

    };

    class DVPortgroupRenamedEvent: public WSObject {
        public:
            string newName;
            string oldName;

            void deSerialize(xmlNodePtr node);

    };
    class TaskInfoState: public WSObject {
        public:
            string state;

            void deSerialize(xmlNodePtr node);

    };
    class TaskInfo: public WSObject {
        public:
            TaskInfoState state;
            ManagedObjectReference task;
            string entityName;

            void deSerialize(xmlNodePtr node);

    };

    class TaskEvent: public WSObject {
        public:
            TaskInfo info;


            void deSerialize(xmlNodePtr node);

    };

    class Event: public WSObject {
        public:

            string key;
            string chainId;
            string createdTime;
            string userName;
            string type;
            string fullFormattedMessage;
            DataCenterEventArgument datacenter;
            HostEventArgument host;
            ComputeResourceEventArgument computeResource;
            DvsEventArgument dvs;
            VmEventArgument vm;
            NetworkEventArgument net;
            void *obj;

            //  map<string, WSObject> propMap;

            Event();
            virtual ~Event();
            void deSerialize(xmlNodePtr node);
    }
    ;

} /* namespace DcmNs */
#endif /* EVENT_H_ */
