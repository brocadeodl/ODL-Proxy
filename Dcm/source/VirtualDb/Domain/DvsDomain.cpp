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
 * DataCenter.cpp
 *
 *  Created on: May 8, 2012
 *      Author: nfarhi
 */

#include "DvsDomain.h"

using namespace std;

namespace DcmNs {

    DvsDomain::DvsDomain() {

    }

    DvsDomain::DvsDomain(string id_val, string name_val, string pgroup_id_val, string pgroup_nn_val, string dc_id_val) {
        id = id_val;
        name = name_val;
        pgroup_id = pgroup_id_val;
        pgroup_nn = pgroup_nn_val;
        dc_id = dc_id_val;
    }


    DvsDomain::~DvsDomain() {
    }

    string DvsDomain::getId() {
        return id;
    }

    void DvsDomain::setId(string id_val) {
        id = id_val;
    }

    string DvsDomain::getName() {
        return name;
    }

    void DvsDomain::setName(string name_val) {
        name = name_val;
    }

    string DvsDomain::getPgroupId() {
        return pgroup_id;
    }

    void DvsDomain::setPgroupId(string pgroup_id_val) {
        pgroup_id = pgroup_id_val;
    }

    string DvsDomain::getPgroupNN() {
        return pgroup_nn;
    }

    void DvsDomain::setPgroupNN(string pgroup_nn_val) {
        pgroup_nn = pgroup_nn_val;
    }

    string DvsDomain::getDcId() {
        return dc_id;
    }

    void DvsDomain::setDcId(string dc_id_val) {
        dc_id = dc_id_val;
    }

    void DvsDomain::setupAttributesForSerialization ()
    {
    	Domain::setupAttributesForSerialization ();

        addSerializableAttribute (new AttributeString       (&id,       "id"));
        addSerializableAttribute (new AttributeString       (&name,     "name"));
        addSerializableAttribute (new AttributeString       (&pgroup_id,"pgroup_id"));
        addSerializableAttribute (new AttributeString       (&pgroup_nn,"pgroup_nn"));
        addSerializableAttribute (new AttributeString       (&dc_id,    "dc_id"));
    }
} /* namespace DcmNs */
