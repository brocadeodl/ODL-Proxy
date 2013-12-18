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
 * DiscoveryCorrelator.h
 *
 *  Created on: May 24, 2012
 *      Author: nfarhi
 */

#ifndef DISCOVERYCORRELATOR_H_
#define DISCOVERYCORRELATOR_H_

#include <iostream>
#include <string>
#include <string.h>
#include "correlator/SwitchAdapter.h"
#include "correlator/Correlator.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include "DcmCore/DcmToolKit.h"
#include "discover/utils/URLEncode.h"
#include "discover/vsphere/Debug.h"
#include "correlator/VcenterUtils.h"
#include "VirtualDb/DAO/DAO.h"

#define FULL_DISCOVERY 1
#define MINI_DISCOVERY 2

using namespace std;

namespace DcmNs {

    class DiscoveryCorrelator: public Correlator {
        public:
            DiscoveryCorrelator();
            DiscoveryCorrelator(std::string,std::string, std::string, bool _isNodeRejoin=false);
            virtual ~DiscoveryCorrelator();
            int run(int type);
            int deleteVCenter(UI32 vcCount);

        protected:
        private:
            void findPgUpdates();
            void findDvpgUpdates();
            void findSpecialPgUpdates();
            void findVnicMacAssociationUpdates();
            void findSpecialNicMacAssociationUpdates();
            void findAddDeleteUpdates(daoType_t, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&, bool);
            void configurePortProfiles(vector<Domain*>, vector<Domain*>, vector<Domain*>);
            void configureVlans (vector<Domain*>, vector<Domain*>);
            void configureMacAssociations(vector<Domain*>, vector<Domain*>, vector<Domain*>);
			std::string getVlanUI32Range(std::string);
            void addListofProfiles(vector<Domain*> );
            void removeListofProfiles(vector<Domain*> );
            void updateListofProfiles(vector<Domain*>);
			UI32 populatePortProfileInVnicAndSpecialNicTable (string dc_id, string pgName);
            void removeAllProfiles(daoType_t type);
            void addOrDeleteListOfMacAssociations(vector<Domain*>, bool);
            std::multimap<string, string> createMapOfProfileToMac(vector<Domain*>);
            bool isUpLink(Domain *dom);
        
            std::string old_dbfile_r;
            std::string new_dbfile_r;
			std::string vcenter_r;
            bool isNodeRejoin;
            SwitchAdapter *switchAdapter;

    };

} /* namespace DcmNs */
#endif /* DISCOVERYCORRELATOR_H_ */
