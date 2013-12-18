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
 * ServiceInstance.h
 *
 *  Created on: Apr 12, 2012
 *      Author: rmadapur
 */

#ifndef SERVICEINSTANCE_H_
#define SERVICEINSTANCE_H_
#include "discover/vsphere/vim25/mo/ManagedObject.h"
#include "discover/vsphere/vim25/mo/PropertyCollector.h"
#include "discover/vsphere/vim25/mo/EventManager.h"
#include "discover/vsphere/vim25/mo/ViewManager.h"
#include "discover/vsphere/vim25/ServiceContent.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include "discover/vsphere/vim25/ws/vimStub.h"

namespace DcmNs {
    /**
     * Managed Object that provides the first level of access to the vSphere
     *
     *
     *  ServiceInstance *serviceInstance = new ServiceInstance(
     *       "https://10.24.84.150/");
     *  serviceInstance->login("administrator", "pass");
     *
     *
     *  This Managed Object holds the session attribute in userSession
     *
     *  After the operation the user can call logout
     *       *
     *  serviceInstance->logout();
     *
     *
     */

    class ServiceInstance: public ManagedObject {
        private:
            UserSession *userSession;
            string url;

            ManagedObjectReference SERVICE_INSTANCE_MOR;
            int retrieveServiceContent();
        public:

            ServiceContent *serviceContent;

            /**
             *  @param url - set the vcenter URL
             */
            ServiceInstance(string url,int connectionTimeout =DEFAULT_CONNECTION_TIMEOUT);

            /**
             * Login to the vCenter
             * @param userName - userName to login
             * @param password - password to login
             */
            int login(string userName, string password);

            /**
             * Logout from the vCenter
             */
            int logout();

            /**
             * Obtain a reference to ServiceConent.
             * @see ServiceContent
             */
            int getServiceContent(ServiceContent **r_serviceContent);
            /**
             * Obtain a reference to PropertyCollector
             *@see PropertyCollector
             */
            int getPropertyCollector(PropertyCollector **propCollector);
            /**
             * Obtain a reference to ViewManager
             *@see ViewManager
             */
            int getViewManager(ViewManager **viewmanager);
            /**
             * Obtain a reference to EventManager
             *@see EventManager
             */
            int getEventManager(EventManager **eventManager);
            /**
             * Obtain a reference to Fault
             *@see Fault
             */
            Fault* getLastFault();

            virtual ~ServiceInstance();
    };

} /* namespace DcmNs */
#endif /* SERVICEINSTANCE_H_ */
