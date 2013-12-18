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
 * Correlator.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: nfarhi
 */
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include "VirtualDb/DAO/DAO.h"
#include "correlator/Correlator.h"

using namespace std;

namespace DcmNs {
    // get DB info
    vector<Domain*> Correlator::getSource(daoType_t type, string db_file, string mor) {

        VIC_PLUG_LOG(VIC_TRACE_INFO,"DAO %s:%s", __FUNCTION__, db_file.c_str());
        DAO* dao = DaoFactory::getDao(type, db_file);
        vector<Domain*> v = dao->retrieveInfoForCorrelation(mor);
        sort(v.begin(), v.end(), Domain::compare);
        //printVectorOfDomain(v);
        DAO::closeDb();
        delete dao;
        return v;
    }

    void Correlator::releaseSource(vector<Domain*> v) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", __FUNCTION__);
        for (unsigned int i = 0; i < v.size(); i++ )
            delete v[i];
    }

    // get vcenter info
    vector<Domain*> Correlator::getSource(vector<Asset*> assets, string host_mor, string host_name, string dc_mor) {
        VIC_PLUG_LOG(VIC_TRACE_INFO,"ASSET %s host_mor %s host_name %s dc_mor %s", __FUNCTION__, host_mor.c_str(),host_name.c_str(),dc_mor.c_str());    
        vector<Domain*> v;
        for (unsigned int i = 0; i < assets.size(); i++)
            v.push_back(assets[i]->convertToDomain(host_mor, host_name, dc_mor));
        sort(v.begin(), v.end(), Domain::compare);
        //printVectorOfDomain(v);
        return v;
    }

    // find (a - b) based on Domain's primary key
    vector<Domain*> Correlator::diff(vector<Domain*> a, vector<Domain*> b) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", __FUNCTION__);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"------%s", "A");
        printVectorOfDomain(a);
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"------%s", "B");
        printVectorOfDomain(b);
        vector<Domain*> v;
        if (a.size() > 0) {
            set_difference(a.begin(), a.end(), b.begin(), b.end(),
                    inserter(v, v.begin()),
                    Domain::compare);
        }
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"------%s", "V");
        printVectorOfDomain(v);
        return v;
    }

    // find (a intersection b) based on Domain's primary key
    vector<Domain*> Correlator::intersect(vector<Domain*> a, vector<Domain*> b) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", __FUNCTION__);
        vector<Domain*> v;
        if (a.size() > 0 && b.size() > 0) {
            set_intersection(
                    a.begin(), a.end(), b.begin(), b.end(),
                    inserter(v, v.begin()),
                    Domain::compare);
        }
        //        printVectorOfDomain(v);
        return v;
    }

    // find updates on non-primary key field's of Domain
    void Correlator::findUpdatedConfig(vector<Domain*> a, vector<Domain*> b, vector<Domain*>& added, vector<Domain*>& deleted, vector<Domain*>& updated) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", __FUNCTION__);
        vector<Domain*> v = intersect(a, b);
        if(v.size() > 0) {
            vector<Domain*> w = intersect(b, a);
            vector<Domain*>::iterator it1 = v.begin();
            vector<Domain*>::iterator it2 = w.begin();
            while (it1 != v.end()) {
                bool added_flag = false, deleted_flag = false, updated_flag = false;
                (*it1)->isConfigUpdate(*it2, added_flag, deleted_flag, updated_flag);
                if (added_flag)
                    added.insert(added.begin(), (*it2));
                if (deleted_flag)
                    deleted.insert(deleted.begin(), (*it1));
                if (updated_flag)
                    updated.insert(updated.begin(), (*it2));
                it1++;
                it2++;
            }
        }
    }

    void Correlator::printVectorOfDomain(vector<Domain*> a) {
        Domain::printDomainVector(a);
    }

    void Correlator::printDomainChanges(vector<Domain*> added, vector<Domain*> deleted, vector<Domain*> updated) {
        VIC_PLUG_LOG(VIC_TRACE_DEBUG,"%s", __FUNCTION__);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s:%d","added:",added.size());
        printVectorOfDomain(added);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s:%d","deleted:",deleted.size());
        printVectorOfDomain(deleted);
        VIC_PLUG_LOG(VIC_TRACE_INFO,"%s:%d","updated:",updated.size());
        printVectorOfDomain(updated);
    }

} /* namespace DcmNs */
