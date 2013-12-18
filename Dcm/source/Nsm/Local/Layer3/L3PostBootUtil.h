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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Ankit Jindal                                                 *
 **************************************************************************/

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Fcoe/Common/FcoeLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "WyserEaGateway/GenericClientMessage.h"

#include "Nsm/Local/NsmTypes.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"

#include "ClientInterface/Ssm/SsmIpAclIntfAssocBulkMessage.h"

using namespace WaveNs;
using namespace std;


namespace DcmNs
{
    class PhyIntfLocalManagedObject;
    class NsmLocalIntfLoopbackWorker;
    class NsmL3Util;
    class NsmLocalL3IpWorker;
    class NsmLocalIntfLoopbackWorker;
    class NsmIpConfigManagedObject;
    class NsmIpAddrConfigManagedObject;
    class NsmIntfLoopbackManagedObject;

class L3PostBootUtil: public WaveObjectManagerToolKit
{

  private:
	static void ConfigReplayIpAclIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static void ConfigReplayPBRPhyIntfIslToEdge(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static void ConfigReplayIpAclGlobalIntf(PoIntfManagedObject *pPoIntfManagedObject);

  protected:
	static void triggerPostBootAfterIslToEdge(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);

  public:
	//Loopback
	static void ConfigReplayIntfLoopbackIpConfig (LocationId locId);
	static void ConfigReplayIntfLoopbackConfig (NsmIntfLoopbackManagedObject *mo);

	//IP
	static void ConfigReplayVlanIpConfig (LocationId locId);
	static void ConfigReplayPoIpConfig (LocationId locId);
	static void ConfigReplayVeConfig (NsmIntfSviManagedObject *pIntfSviManagedObject);
	static void ConfigReplayNodePoConfig (UI32 ifId, string ifName);
    static void ConfigReplayVrfBind(const string ifName, const UI32 ifType, ObjectId vrfId);
	static void configReplayPhyIntfIpConfig (const PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static GenericClientMessage_ * ConfigReplayPBRPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static void ConfigReplayIpAddr ( string ifName, UI32 ifType, const NsmIpAddrConfigManagedObject *pIpAddrConfigMo);
	static void ConfigReplayIpConfig ( string ifName, UI32 ifType, const NsmIpConfigManagedObject *pIpConfigMo);
	static SsmIpAclIntfAssocBulkMessage *ConfigReplayIpAclIntfAssoc(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static const string getPolicyNameByObjId(const ObjectId objId);
	static SsmIpAclIntfAssocBulkMessage *ConfigReplayIpAclGlobalIntfAssoc(PoIntfManagedObject *pPoIntfManagedObject);
	static void configReplayIntfArpConfig(const NsmIpConfigManagedObject *pIpConfigMo, string ifName, UI32 ifType);
	friend class IntfConfLocalWorker;

};

}





