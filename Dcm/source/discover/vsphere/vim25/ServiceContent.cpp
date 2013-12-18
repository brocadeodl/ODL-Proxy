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
 * ServiceContent.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: rmadapur
 */

#include "ServiceContent.h"
#include "discover/vsphere/vim25/ManagedObjectReference.h"

namespace DcmNs {

    ServiceContent::ServiceContent() {
        // TODO Auto-generated constructor stub

    }

    ServiceContent::~ServiceContent() {
        // TODO Auto-generated destructor stub
    }

    AboutInfo ServiceContent::getAbout() const {
        return about;
    }

    ManagedObjectReference ServiceContent::getAccountManager() const {
        return accountManager;
    }

    ManagedObjectReference ServiceContent::getAlarmManager() const {
        return alarmManager;
    }

    ManagedObjectReference ServiceContent::getAuthorizationManager() const {
        return authorizationManager;
    }

    ManagedObjectReference ServiceContent::getClusterProfileManager() const {
        return clusterProfileManager;
    }

    ManagedObjectReference ServiceContent::getComplianceManager() const {
        return complianceManager;
    }

    ManagedObjectReference ServiceContent::getCustomFieldsManager() const {
        return customFieldsManager;
    }

    ManagedObjectReference ServiceContent::getCustomizationSpecManager() const {
        return customizationSpecManager;
    }

    ManagedObjectReference ServiceContent::getDiagnosticManager() const {
        return diagnosticManager;
    }

    ManagedObjectReference ServiceContent::getDvSwitchManager() const {
        return dvSwitchManager;
    }

    ManagedObjectReference ServiceContent::getEventManager() const {
        return eventManager;
    }

    ManagedObjectReference ServiceContent::getExtensionManager() const {
        return extensionManager;
    }

    ManagedObjectReference ServiceContent::getFileManager() const {
        return fileManager;
    }

    ManagedObjectReference ServiceContent::getGuestOperationsManager() const {
        return guestOperationsManager;
    }

    ManagedObjectReference ServiceContent::getHostProfileManager() const {
        return hostProfileManager;
    }

    ManagedObjectReference ServiceContent::getIpPoolManager() const {
        return ipPoolManager;
    }

    ManagedObjectReference ServiceContent::getLicenseManager() const {
        return licenseManager;
    }

    ManagedObjectReference ServiceContent::getLocalizationManager() const {
        return localizationManager;
    }

    ManagedObjectReference ServiceContent::getOvfManager() const {
        return ovfManager;
    }

    ManagedObjectReference ServiceContent::getPerfManager() const {
        return perfManager;
    }

    ManagedObjectReference ServiceContent::getPropertyCollector() const {
        return propertyCollector;
    }

    ManagedObjectReference ServiceContent::getRootFolder() const {
        return rootFolder;
    }

    ManagedObjectReference ServiceContent::getScheduledTaskManager() const {
        return scheduledTaskManager;
    }

    ManagedObjectReference ServiceContent::getSearchIndex() const {
        return searchIndex;
    }

    ManagedObjectReference ServiceContent::getSessionManager() const {
        return sessionManager;
    }

    ManagedObjectReference ServiceContent::getSetting() const {
        return setting;
    }

    ManagedObjectReference ServiceContent::getSnmpSystem() const {
        return snmpSystem;
    }

    ManagedObjectReference ServiceContent::getStorageResourceManager() const {
        return storageResourceManager;
    }

    ManagedObjectReference ServiceContent::getTaskManager() const {
        return taskManager;
    }

    ManagedObjectReference ServiceContent::getUserDirectory() const {
        return userDirectory;
    }

    ManagedObjectReference ServiceContent::getViewManager() const {
        return viewManager;
    }

    ManagedObjectReference ServiceContent::getVirtualDiskManager() const {
        return virtualDiskManager;
    }

    ManagedObjectReference ServiceContent::getVirtualizationManager() const {
        return virtualizationManager;
    }

    ManagedObjectReference ServiceContent::getVmCompatibilityChecker() const {
        return vmCompatibilityChecker;
    }

    ManagedObjectReference ServiceContent::getVmProvisioningChecker() const {
        return vmProvisioningChecker;
    }

    void ServiceContent::setAbout(AboutInfo _about) {
        this->about = _about;
    }

    void ServiceContent::setAccountManager(
            ManagedObjectReference _accountManager) {
        this->accountManager = _accountManager;
    }

    void ServiceContent::setAlarmManager(ManagedObjectReference _alarmManager) {
        this->alarmManager = _alarmManager;
    }

    void ServiceContent::setAuthorizationManager(
            ManagedObjectReference _authorizationManager) {
        this->authorizationManager = _authorizationManager;
    }

    void ServiceContent::setClusterProfileManager(
            ManagedObjectReference _clusterProfileManager) {
        this->clusterProfileManager = _clusterProfileManager;
    }

    void ServiceContent::setComplianceManager(
            ManagedObjectReference _complianceManager) {
        this->complianceManager = _complianceManager;
    }

    void ServiceContent::setCustomFieldsManager(
            ManagedObjectReference _customFieldsManager) {
        this->customFieldsManager = _customFieldsManager;
    }

    void ServiceContent::setCustomizationSpecManager(
            ManagedObjectReference _customizationSpecManager) {
        this->customizationSpecManager = _customizationSpecManager;
    }

    void ServiceContent::setDiagnosticManager(
            ManagedObjectReference _diagnosticManager) {
        this->diagnosticManager = _diagnosticManager;
    }

    void ServiceContent::setDvSwitchManager(
            ManagedObjectReference _dvSwitchManager) {
        this->dvSwitchManager = _dvSwitchManager;
    }

    void ServiceContent::setEventManager(ManagedObjectReference _eventManager) {
        this->eventManager = _eventManager;
    }

    void ServiceContent::setExtensionManager(
            ManagedObjectReference _extensionManager) {
        this->extensionManager = _extensionManager;
    }

    void ServiceContent::setFileManager(ManagedObjectReference _fileManager) {
        this->fileManager = _fileManager;
    }

    void ServiceContent::setGuestOperationsManager(
            ManagedObjectReference _guestOperationsManager) {
        this->guestOperationsManager = _guestOperationsManager;
    }

    void ServiceContent::setHostProfileManager(
            ManagedObjectReference _hostProfileManager) {
        this->hostProfileManager = _hostProfileManager;
    }

    void ServiceContent::setIpPoolManager(
            ManagedObjectReference _ipPoolManager) {
        this->ipPoolManager = _ipPoolManager;
    }

    void ServiceContent::setLicenseManager(
            ManagedObjectReference _licenseManager) {
        this->licenseManager = _licenseManager;
    }

    void ServiceContent::setLocalizationManager(
            ManagedObjectReference _localizationManager) {
        this->localizationManager = _localizationManager;
    }

    void ServiceContent::setOvfManager(ManagedObjectReference _ovfManager) {
        this->ovfManager = _ovfManager;
    }

    void ServiceContent::setPerfManager(ManagedObjectReference _perfManager) {
        this->perfManager = _perfManager;
    }

    void ServiceContent::setPropertyCollector(
            ManagedObjectReference _propertyCollector) {
        this->propertyCollector = _propertyCollector;
    }

    void ServiceContent::setRootFolder(ManagedObjectReference _rootFolder) {
        this->rootFolder = _rootFolder;
    }

    void ServiceContent::setScheduledTaskManager(
            ManagedObjectReference _scheduledTaskManager) {
        this->scheduledTaskManager = _scheduledTaskManager;
    }

    void ServiceContent::setSearchIndex(ManagedObjectReference _searchIndex) {
        this->searchIndex = _searchIndex;
    }

    void ServiceContent::setSessionManager(
            ManagedObjectReference _sessionManager) {
        this->sessionManager = _sessionManager;
    }

    void ServiceContent::setSetting(ManagedObjectReference _setting) {
        this->setting = _setting;
    }

    void ServiceContent::setSnmpSystem(ManagedObjectReference _snmpSystem) {
        this->snmpSystem = _snmpSystem;
    }

    void ServiceContent::setStorageResourceManager(
            ManagedObjectReference _storageResourceManager) {
        this->storageResourceManager = _storageResourceManager;
    }

    void ServiceContent::setTaskManager(ManagedObjectReference _taskManager) {
        this->taskManager = _taskManager;
    }

    void ServiceContent::setUserDirectory(
            ManagedObjectReference _userDirectory) {
        this->userDirectory = _userDirectory;
    }

    void ServiceContent::setViewManager(ManagedObjectReference _viewManager) {
        this->viewManager = _viewManager;
    }

    void ServiceContent::setVirtualDiskManager(
            ManagedObjectReference _virtualDiskManager) {
        this->virtualDiskManager = _virtualDiskManager;
    }

    void ServiceContent::setVirtualizationManager(
            ManagedObjectReference _virtualizationManager) {
        this->virtualizationManager = _virtualizationManager;
    }

    void ServiceContent::setVmCompatibilityChecker(
            ManagedObjectReference _vmCompatibilityChecker) {
        this->vmCompatibilityChecker = _vmCompatibilityChecker;
    }

    void ServiceContent::setVmProvisioningChecker(
            ManagedObjectReference _vmProvisioningChecker) {
        this->vmProvisioningChecker = _vmProvisioningChecker;
    }

    void ServiceContent::deSerialize(xmlNodePtr node) {
        xmlNodePtr childNode = node->children;
        while (childNode != NULL) {
            WSObject::extractObject(childNode, "rootFolder", &this->rootFolder);
            WSObject::extractObject(childNode, "propertyCollector",
                    &this->propertyCollector);
            WSObject::extractObject(childNode, "viewManager",
                    &this->viewManager);
            WSObject::extractObject(childNode, "sessionManager",
                    &this->sessionManager);
            WSObject::extractObject(childNode, "eventManager",
                               &this->eventManager);

            WSObject::extractObject(childNode, "about",
                                           &this->about);

            //More can be done if required
            childNode = childNode->next;
        }
    }

} /* namespace DcmNs */
