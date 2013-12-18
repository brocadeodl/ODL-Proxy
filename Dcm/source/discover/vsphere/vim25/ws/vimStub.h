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
 * vimStub.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rmadapur
 */

#ifndef VIMSTUB_H_
#define VIMSTUB_H_
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ws/Argument.h"

#include "discover/vsphere/vim25/WSObject.h"
#include "discover/vsphere/vim25/PropertyFilterSpec.h"
#include "discover/vsphere/vim25/EventFilterSpec.h"
#include "discover/vsphere/vim25/UserSession.h"
#include "discover/vsphere/vim25/UpdateSet.h"
#include "discover/vsphere/WSClient.h"
#include "discover/vsphere/vim25/ObjectContent.h"
#include "discover/vsphere/vim25/Event.h"

namespace DcmNs {
    /**
     *  Stub for all vCenter API's
     */
    class vimStub {
        public:
            /**
             * Creates a stub to access vCenter API's
             * @param url - http/https URL of vCenter
             */
            vimStub(string url,int connectionTimeout);
            virtual ~vimStub();

            /**
             * vSphere API to login to vCenter
             *
             * @param _this pointer to SessionManager ManagedObjectReference(This is
             * retrieved using the retrieveServiceContent method)
             * @see retrieveServiceContent
             * @param userName  user Name to login
             * @param password  password to login
             * @param userSession  returns the pointer to the retrieved UserSession ManagedObectReference.
             * CAller should call a delete on userSession to deallocate the memory
             */
            int login(ManagedObjectReference *_this, string userName,
                    string password, UserSession **userSession);

            /**
             * Logout the the current session
             * @param _this pointer to SessionManager ManagedObjectReference(This is
             * retrieved using the retrieveServiceContent method)
             */
            int logout(ManagedObjectReference *_this);


            /**
             * vSphere API to retrieveServiceContent
             * @param _this pointer to ServiceInstance ManagedObjectReference
             * @param r_serviceContent returns the pointer to the retrieved ServiceContent ManagedObectReference
             * CAller should call a delete on r_serviceContent to deallocate the memory
             */
            int retrieveServiceContent(ManagedObjectReference *_this,
                    ServiceContent **r_serviceContent);

            /**
             * vSphere API to create containerView
             * @param _this pointer to ViewManager ManagedObjectReference
             * @param container A reference to an instance of a Folder, Datacenter,
             * ComputeResource, ResourcePool, or HostSystem object.
             * @param type An optional list of managed entity types. The server associates only objects of the
             * specified type(s) with the view. If you specify an empty array, the server uses all types.
             * @param recursive Whether to include only the immediate children of the container instance,
             * or to include additional objects by following paths beyond the immediate children.
             * When recursive is false, the list of objects contains only immediate children.
             * When recursive is true, the server populates the list by following references beyond the immediate children
             *  (using a child's references, and then references in the resulting objects, and so on).
             * @param r_mor returns the pointer to the created containerView
             */
            int createContainerView(ManagedObjectReference *_this,
                    ManagedObjectReference *container, vector<string> *type,
                    bool recursive, ManagedObjectReference **r_mor);



            int createCollectorForEvents(ManagedObjectReference *_this,
                        EventFilterSpec *filter,
                        ManagedObjectReference **r_mor);

            int createFilter(ManagedObjectReference *_this,
                        PropertyFilterSpec *spec, bool partialUpdates,
                        ManagedObjectReference **r_mor);

            /**
             * Destroy the Property Filter
             * @param _this pointer to PropertyFilter
             */
            int destroyPropertyFilter(ManagedObjectReference *_this);

            int waitForUpdates(ManagedObjectReference *_this, string version,
                    UpdateSet **r_mor);

            int cancelWaitForUpdates(ManagedObjectReference *_this);

            /**
             * vSphere API to create retrieveProperties
             * @param _this pointer to PropertyCollector ManagedObjectReference
             * @param specSet specifications of Properties to retrieved
             * @param objectContentList returns the pointer to the retrieved ObjectContent List
             */
            int retrieveProperties(ManagedObjectReference *_this,
                    vector<PropertyFilterSpec> *specSet,
                    vector<ObjectContent *> **objectContentList);


            int destroyCollector(ManagedObjectReference *_this);
            int resetCollector(ManagedObjectReference *_this);
            int rewindCollector(ManagedObjectReference *_this);
            int setCollectorPageSize(ManagedObjectReference *_this,int maxCount);

            int readNextEvents(ManagedObjectReference *_this,int maxCount,
                            vector<Event *> **r_mor);

            int readPreviousEvents(ManagedObjectReference *_this,int maxCount,
                                vector<Event *> **r_mor);



            Fault* getLastFault(){
                return wsc->lastFault;
            }

        private:
            WSClient *wsc;
    };

} /* namespace DcmNs */
#endif /* VIMSTUB_H_ */
