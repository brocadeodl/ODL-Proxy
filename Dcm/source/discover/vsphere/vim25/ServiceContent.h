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
 * ServiceContent.h
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#ifndef SERVICECONTENT_H_
#define SERVICECONTENT_H_

#include "discover/vsphere/vim25/AboutInfo.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"
#include <libxml/tree.h>
#include <libxml/xpath.h>
namespace DcmNs {
    /**
     * The ServiceContent data object defines properties for the ServiceInstance managed
     * object. The ServiceInstance itself does not have directly-accessible properties
     * because reading the properties of a managed object requires the use of a property
     * collector, and the property collector itself is a property of the ServiceInstance.
     * For this reason, use the method RetrieveServiceContent to retrieve the ServiceContent
     *  object.
     *
     *  Only the following fields are populated rootFolder,propertyCollector,viewManager
     *  sessionManager,eventManager
     */
    class ServiceContent :WSObject{
        public:
            ServiceContent();
            virtual ~ServiceContent();
            AboutInfo getAbout() const;
            ManagedObjectReference getAccountManager() const;
            ManagedObjectReference getAlarmManager() const;
            ManagedObjectReference getAuthorizationManager() const;
            ManagedObjectReference getClusterProfileManager() const;
            ManagedObjectReference getComplianceManager() const;
            ManagedObjectReference getCustomFieldsManager() const;
            ManagedObjectReference getCustomizationSpecManager() const;
            ManagedObjectReference getDiagnosticManager() const;
            ManagedObjectReference getDvSwitchManager() const;
            ManagedObjectReference getEventManager() const;
            ManagedObjectReference getExtensionManager() const;
            ManagedObjectReference getFileManager() const;
            ManagedObjectReference getGuestOperationsManager() const;
            ManagedObjectReference getHostProfileManager() const;
            ManagedObjectReference getIpPoolManager() const;
            ManagedObjectReference getLicenseManager() const;
            ManagedObjectReference getLocalizationManager() const;
            ManagedObjectReference getOvfManager() const;
            ManagedObjectReference getPerfManager() const;
            ManagedObjectReference getPropertyCollector() const;
            ManagedObjectReference getRootFolder() const;
            ManagedObjectReference getScheduledTaskManager() const;
            ManagedObjectReference getSearchIndex() const;
            ManagedObjectReference getSessionManager() const;
            ManagedObjectReference getSetting() const;
            ManagedObjectReference getSnmpSystem() const;
            ManagedObjectReference getStorageResourceManager() const;
            ManagedObjectReference getTaskManager() const;
            ManagedObjectReference getUserDirectory() const;
            ManagedObjectReference getViewManager() const;
            ManagedObjectReference getVirtualDiskManager() const;
            ManagedObjectReference getVirtualizationManager() const;
            ManagedObjectReference getVmCompatibilityChecker() const;
            ManagedObjectReference getVmProvisioningChecker() const;
            void setAbout(AboutInfo about);
            void setAccountManager(ManagedObjectReference accountManager);
            void setAlarmManager(ManagedObjectReference alarmManager);
            void setAuthorizationManager(
                    ManagedObjectReference authorizationManager);
            void setClusterProfileManager(
                    ManagedObjectReference clusterProfileManager);
            void setComplianceManager(ManagedObjectReference complianceManager);
            void setCustomFieldsManager(
                    ManagedObjectReference customFieldsManager);
            void setCustomizationSpecManager(
                    ManagedObjectReference customizationSpecManager);
            void setDiagnosticManager(ManagedObjectReference diagnosticManager);
            void setDvSwitchManager(ManagedObjectReference dvSwitchManager);
            void setEventManager(ManagedObjectReference eventManager);
            void setExtensionManager(ManagedObjectReference extensionManager);
            void setFileManager(ManagedObjectReference fileManager);
            void setGuestOperationsManager(
                    ManagedObjectReference guestOperationsManager);
            void setHostProfileManager(
                    ManagedObjectReference hostProfileManager);
            void setIpPoolManager(ManagedObjectReference ipPoolManager);
            void setLicenseManager(ManagedObjectReference licenseManager);
            void setLocalizationManager(
                    ManagedObjectReference localizationManager);
            void setOvfManager(ManagedObjectReference ovfManager);
            void setPerfManager(ManagedObjectReference perfManager);
            void setPropertyCollector(ManagedObjectReference propertyCollector);
            void setRootFolder(ManagedObjectReference rootFolder);
            void setScheduledTaskManager(
                    ManagedObjectReference scheduledTaskManager);
            void setSearchIndex(ManagedObjectReference searchIndex);
            void setSessionManager(ManagedObjectReference sessionManager);
            void setSetting(ManagedObjectReference setting);
            void setSnmpSystem(ManagedObjectReference snmpSystem);
            void setStorageResourceManager(
                    ManagedObjectReference storageResourceManager);
            void setTaskManager(ManagedObjectReference taskManager);
            void setUserDirectory(ManagedObjectReference userDirectory);
            void setViewManager(ManagedObjectReference viewManager);
            void setVirtualDiskManager(
                    ManagedObjectReference virtualDiskManager);
            void setVirtualizationManager(
                    ManagedObjectReference virtualizationManager);
            void setVmCompatibilityChecker(
                    ManagedObjectReference vmCompatibilityChecker);
            void setVmProvisioningChecker(
                    ManagedObjectReference vmProvisioningChecker);
            ManagedObjectReference rootFolder;
            ManagedObjectReference propertyCollector;
            ManagedObjectReference viewManager;
            AboutInfo about;
            ManagedObjectReference setting;
            ManagedObjectReference userDirectory;
            ManagedObjectReference sessionManager;
            ManagedObjectReference authorizationManager;
            ManagedObjectReference perfManager;
            ManagedObjectReference scheduledTaskManager;
            ManagedObjectReference alarmManager;
            ManagedObjectReference eventManager;
            ManagedObjectReference taskManager;
            ManagedObjectReference extensionManager;
            ManagedObjectReference customizationSpecManager;
            ManagedObjectReference customFieldsManager;
            ManagedObjectReference accountManager;
            ManagedObjectReference diagnosticManager;
            ManagedObjectReference licenseManager;
            ManagedObjectReference searchIndex;
            ManagedObjectReference fileManager;
            ManagedObjectReference virtualDiskManager;
            ManagedObjectReference virtualizationManager;
            ManagedObjectReference snmpSystem;
            ManagedObjectReference vmProvisioningChecker;
            ManagedObjectReference vmCompatibilityChecker;
            ManagedObjectReference ovfManager;
            ManagedObjectReference ipPoolManager;
            ManagedObjectReference dvSwitchManager;
            ManagedObjectReference hostProfileManager;
            ManagedObjectReference clusterProfileManager;
            ManagedObjectReference complianceManager;
            ManagedObjectReference localizationManager;
            ManagedObjectReference storageResourceManager;
            ManagedObjectReference guestOperationsManager;

            void deSerialize(xmlNodePtr node);

    };

} /* namespace DcmNs */
#endif /* SERVICECONTENT_H_ */
