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
 * EventCorrelator.h
 *
 *  Created on: August 29, 2012
 *      Author: nfarhi
 */

#ifndef EVENTCORRELATOR_H_
#define EVENTCORRELATOR_H_

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include "discover/connector/Host.h"
#include "discover/connector/VM.h"
#include "VirtualDb/Domain/DomainInclude.h"
#include "VirtualDb/DAO/DAOInclude.h"
#include "correlator/Correlator.h"
#include "discover/events/EventDefines.h"


using namespace std;

namespace DcmNs {

    typedef enum correlateOperation_ {
        NOOP = 0,
        EVENT_INSERT,
        EVENT_UPDATE,
        EVENT_DELETE
    } correlateOperation_t;

    typedef enum correlateObject_ {
        ALL = 0,
        VSWITCH,
        VMKERNEL,
        PORTGROUP,
        CONSOLEVNIC,
        VM,
        VMNIC
    } correlateObject_t;

    class EventCorrelator : public Correlator {
        public:
            EventCorrelator();
            EventCorrelator(std::string, Host*, std::string, std::string);
            EventCorrelator(std::string, std::string, std::string);
            void findSendTaskUpdates(correlateOperation_t, correlateObject_t);
			void correlateVMReconf(Host*, class VM*);
			string getHostName();
            void findAssetUpdates(vector <Asset *>, correlateOperation_t, daoType_t);
            void findAddDeleteUpdates(daoType_t, vector<Asset*>, correlateOperation_t, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&, vector<Domain*>&);
            vector<Asset *>  extractSpecialPortGroups();
        private:
            void sendDbConfigUpdate(vector<Domain*>, UI32);
            std::string dbfile_r;
            Host* 	host;
            class VM*		vm;
            std::string dc_mor;
            std::string vcenter;
    };

} /* namespace DcmNs */
#endif /* EVENTCORRELATOR_H_ */
