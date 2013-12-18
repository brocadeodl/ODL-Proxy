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
 * DataCenterDomain.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "DataCenterDomain.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {

    DataCenterDomain::DataCenterDomain() : Domain (){
    }

    DataCenterDomain::DataCenterDomain(string id_val, string name_val): Domain() {
        id = id_val;
        name = name_val;
    }


    DataCenterDomain::~DataCenterDomain() {
    }

    string DataCenterDomain::getId() {
        return id;
    }
    
    void DataCenterDomain::setId(string id_val) {
        id = id_val;
    }

    string DataCenterDomain::getName() {
        return name;
    }

    void DataCenterDomain::setName(string name_val) {
        name = name_val;
    }

    void DataCenterDomain::setupAttributesForSerialization ()
    {
        Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,   "id"));
        addSerializableAttribute (new AttributeString       (&name, "name"));
    }

} /* namespace DcmNs */
