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

/***************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Created on: Oct 5, 2012                                               *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/

#ifndef VCENTERDIFF_H_
#define VCENTERDIFF_H_

#include <string>
#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "correlator/Correlator.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Utils/DceLinearSeqContext.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"

#include "PortProfileObject.h"

using namespace std;

namespace DcmNs {

class VcenterDiff: public WaveObjectManagerToolKit, public Correlator
  {

  		protected:

        private:
		protected:
            VcenterDiff();
			

		public:
			static void getPPDiff(string vCenterName, vector<PortProfileObject*>& vector_added, vector<PortProfileObject*>& vector_deleted, vector<PortProfileObject*>& vector_updated, bool isReconcile); 
			static vector<PortProfileObject*> diff(vector<PortProfileObject*> a, vector<PortProfileObject*> b);
            static vector<PortProfileObject*> intersect(vector<PortProfileObject*> a, vector<PortProfileObject*> b);
			static void findPortProfileConfigMismatch(vector<PortProfileObject*> pp_vector_db, vector<PortProfileObject*> pp_vector_sw, vector<PortProfileObject*>& vector_updated);
			static std::multimap<string, string> createMapOfProfileToMac(string, vector<Domain*>);
			static vector<Domain*> getSource(daoType_t type, string db_file, string mor);
			static void reconcile(string vcenter,bool useDB=true);
			static void configureVlans (string vcenter, vector<PortProfileObject*> added_vector, vector<PortProfileObject*> updated_vector);
			static bool compareByName(PortProfileObject* a, PortProfileObject* b);
			static void releaseSource(vector<PortProfileObject*> v); 
            static void deleteLegacyPortProfiles();  
            static void removeDuplicates(vector<PortProfileObject*>& a);
            static string getVlanFromPPObject(PortProfileObject* a);
	};
}

#endif /* VCENTERDIFF_H_ */
