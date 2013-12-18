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
 * PropertyCollector.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: rmadapur
 */

#include "PropertyCollector.h"

namespace DcmNs {

    PropertyCollector::PropertyCollector(vimStub *_stub,
            ManagedObjectReference *_mor) :
            ManagedObject(_stub, _mor) {
        // TODO Auto-generated constructor stub

    }

    PropertyCollector::~PropertyCollector() {
        // TODO Auto-generated destructor stub
    }

    int PropertyCollector::retrieveProperties(
            vector<PropertyFilterSpec> *specSet,
            vector<ObjectContent *> **r_mor) {
        return stub->retrieveProperties(this->mor, specSet, r_mor);
    }

    int PropertyCollector::retrieveProperties(ManagedObjectReference *source_mor,
            vector<string> properties, vector<ObjectContent *> **r_mor) {
        PropertySpec propertyspec;
        propertyspec.all = false;
        propertyspec.type = source_mor->type;

            vector<string>::iterator it;
            for (it = properties.begin(); it < properties.end(); it++)
                propertyspec.pathSet.push_back(*it);


        ObjectSpec objspec;
        objspec.obj = *source_mor;
        objspec.skip = false;

        PropertyFilterSpec propertyFilterSpec;
        propertyFilterSpec.propSet.push_back(propertyspec);
        propertyFilterSpec.objectSet.push_back(objspec);

        vector<PropertyFilterSpec> propertyFilterSpecVector;
        propertyFilterSpecVector.push_back(propertyFilterSpec);


        return stub->retrieveProperties(this->mor, &propertyFilterSpecVector, r_mor);
    }

   int PropertyCollector::retrieveProperties(vector<ManagedObjectReference> morObjList,string morType,
               vector<string> properties, vector<ObjectContent *> **r_mor){
           PropertySpec propertyspec;
           propertyspec.all = false;
           propertyspec.type = morType;

               vector<string>::iterator it;
        for (it = properties.begin(); it < properties.end(); it++)
            propertyspec.pathSet.push_back(*it);

           PropertyFilterSpec propertyFilterSpec;
           propertyFilterSpec.propSet.push_back(propertyspec);

           for (size_t i = 0; i < morObjList.size(); i++) {
            ObjectSpec objspec;
            objspec.obj = morObjList[i];
            objspec.skip = false;
            propertyFilterSpec.objectSet.push_back(objspec);
        }

           vector<PropertyFilterSpec> propertyFilterSpecVector;
           propertyFilterSpecVector.push_back(propertyFilterSpec);

           return stub->retrieveProperties(this->mor, &propertyFilterSpecVector, r_mor);
       }

    int PropertyCollector::createFilter(PropertyFilterSpec *spec,
            bool partialUpdates, ManagedObjectReference **r_mor) {
        return stub->createFilter(this->mor, spec, partialUpdates, r_mor);
    }
    int PropertyCollector::waitForUpdates(string version, UpdateSet **r_mor) {
        return stub->waitForUpdates(this->mor, version, r_mor);
    }
    int PropertyCollector::cancelWaitForUpdates() {

        return stub->cancelWaitForUpdates(this->mor);
    }
    int PropertyCollector::destroyPropertyFilter(
            ManagedObjectReference *_this) {
        return stub->destroyPropertyFilter(_this);
    }
    vector <PropertyFilterSpec> PropertyCollector:: createPropertyFilterSpec(ManagedObjectReference ref, vector<string> &props) {
            ObjectSpec objspec;
            objspec.obj =ref;
            objspec.skip = false;
            PropertySpec propertyspec;
            propertyspec.type = ref.type;
            propertyspec.pathSet.push_back("info");
            PropertyFilterSpec propertyFilterSpec;
            propertyFilterSpec.propSet.push_back(propertyspec);
            propertyFilterSpec.objectSet.push_back(objspec);
            vector<PropertyFilterSpec> specSet;
            specSet.push_back(propertyFilterSpec);
            return specSet;
     }

} /* namespace DcmNs */
