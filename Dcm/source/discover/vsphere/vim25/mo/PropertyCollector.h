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
 * PropertyCollector.h
 *
 *  Created on: Apr 13, 2012
 *      Author: rmadapur
 */

#ifndef PROPERTYCOLLECTOR_H_
#define PROPERTYCOLLECTOR_H_
#include "discover/vsphere/vim25/mo/ManagedObject.h"
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ws/vimStub.h"
namespace DcmNs {

    /**
     *The PropertyCollector service interface provides a way to monitor and retrieve
     *The information about managed objects, such as whether a virtual machine is powered
     * on or whether a host in a cluster is offline.The PropertyCollector uses one or
     * more filters to determine the scope of collection and it has methods to retrieve data
     *. A filter uses a set of data objects that specify the following information
     */
    class PropertyCollector :public ManagedObject{
        public:

            PropertyCollector(vimStub *_stub,ManagedObjectReference *mor);
            virtual ~PropertyCollector();

            /**
             * Retrieve Properties as per the PropertyFilterSpec
             *              */
            int retrieveProperties(
                       vector<PropertyFilterSpec> *specSet,
                       vector<ObjectContent *> **r_mor);
            int retrieveProperties(
                        ManagedObjectReference *mor,
                        vector<string> properties,
                        vector<ObjectContent *> **r_mor);
            int retrieveProperties(vector<ManagedObjectReference> morObjList,string morType,
                          vector<string> properties, vector<ObjectContent *> **r_mor);
            int createFilter(
                        PropertyFilterSpec *spec, bool partialUpdates,
                        ManagedObjectReference **r_mor);
            int waitForUpdates(string version,
                            UpdateSet **r_mor);
            int cancelWaitForUpdates();
            int destroyPropertyFilter(ManagedObjectReference *_this);
            static vector <PropertyFilterSpec>  createPropertyFilterSpec(ManagedObjectReference ref, vector<string> &props);
    };

} /* namespace DcmNs */
#endif /* PROPERTYCOLLECTOR_H_ */
