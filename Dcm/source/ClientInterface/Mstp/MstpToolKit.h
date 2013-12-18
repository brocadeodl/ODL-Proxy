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

#ifndef MSTP_TOOL_KIT__H
#define MSTP_TOOL_KIT__H

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/MacAddress.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmResourceIds.h"

#define MSTP_PLUG_ASSERT   	    NSM_PLUG_ASSERT
#define MSTP_PLUG_ASSERT_PTR  	NSM_PLUG_ASSERT_PTR
#define MSTP_PLUG_DBG_TRACE     NSM_PLUG_DBG_TRACE
#define MSTP_PLUG_DBG        	NSM_PLUG_DBG  
#define MSTP_PLUG_DBG_ERR    	NSM_PLUG_DBG_ERR  
#define MSTP_PLUG_DBG_ERR_S    	NSM_PLUG_DBG_ERR_S  
#define MSTP_PLUG_DBG_S      	NSM_PLUG_DBG_S  
#define MSTP_PLUG_INFO_S     	NSM_PLUG_INFO_S  


#define XSTP_SYSID_LSB_UIDNAME  "br_mac_suffix"

#define LOCATION_FILTER_NONE         0
#define LOCATION_FILTER_CONNECTED    1
#define LOCATION_FILTER_DISCONNECTED 2


using namespace WaveNs;
using namespace std;

namespace DcmNs
{

template<class T>
string toString(const T & t)
{
    stringstream ss;
    ss<<t;
    return ss.str();
}

class XstpBridgeConfigManagedObject;
class VlanIntfManagedObject;
class MstpInstanceConfigManagedObject;
class DcmManagementInterfaceMessage;
class DceContext;



class MstpToolKit : public WaveObjectManagerToolKit 
{
	private: 
		MstpToolKit();
	public:
        static ResourceId getBridgeMode(int &mode, DceContext *pSequencerCtx = NULL);
		static ResourceId getBridgeConfigMO(XstpBridgeConfigManagedObject *&pMo, int &mode, DceContext *pSequencerCtx = NULL);
        static ResourceId getBridgeClassName(string &o_brClassName, DceContext *pSequencerCtx = NULL);

        static ResourceId isSpanningTreeConfigured (bool &o_configured);
        static ResourceId isSpanningTreeEnabled (bool &o_enabled);
        static ResourceId isSpanningTreeEnabledOnIntf (bool &o_enabled, InterfaceType ifType, const string &ifName);
        static ResourceId isSpanningTreeEnabledOnIntf (bool &o_enabled, const ObjectId &intfOid);

        /**
         * Query if bpdu drop or tagged bpdu tunnelling is configured on any interface.. The filterType can be specified to limit
         * the search to connected or disconnected nodes in the cluster..
         */
        static ResourceId validateBpduTunnellingParamsNotConfigured (UI32 filterType = LOCATION_FILTER_NONE);

		static ResourceId checkMstpInstancePresent (int instanceId);

        static MstpInstanceConfigManagedObject *getMstpInstanceByVlanOid(const ObjectId &vlanOid, const string &selectField = "");
        static MstpInstanceConfigManagedObject *getMstpInstanceByVlanOid(const ObjectId &vlanOid, vector<string> &selectFields);

		static ResourceId verifyBridgePriority(UI32 priority);
		static ResourceId verifyPortPriority(UI32 priority);
		static ResourceId verifyFwdDlyMaxAgeHelloTime(UI32 forward_delay, UI32 max_age, UI32 hello_time);

        /**
         * Validate vlan for vlan specific xSTP configs
         */
        static ResourceId validateVlanForXstpConfig(UI32 vid, DceContext *pSequencerContext);
        static ResourceId validateVlanForXstpConfig(const UI32Range &vidRange, DceContext *pSequencerContext);

		static ResourceId mapBackEndErrorsToDcmdErrors(int err);

        /**
         * Send message to backend agent and map the return code to ResourceId..
         */
        static ResourceId sendToBackend (const char *name, DcmManagementInterfaceMessage *pMessage);

		/**
		 * Validate if given portfast/edgeport config is valid for current
		 * bridge mode..
		 */
		static bool isAllowPortfastEdgeport(const string &keypath);

        static ResourceId allocateVcsBridgeMac(MacAddress &mac);
        static ResourceId freeVcsbridgeMac();

        static string toPortIdName (InterfaceType ifType, const string &ifName);
        static ResourceId allocatePortId(UI32 &uidOut, InterfaceType ifType, const string &ifName, bool ignoreNoSpt = true, DceContext *pSequencerCtx = NULL);
		static ResourceId allocatePortId(UI32 &uidOut, int brMode, InterfaceType ifType, const string &ifName);
		static ResourceId freePortId(InterfaceType ifType, const string &ifName, bool ignoreNoSpt = true, DceContext *pSequencerCtx = NULL);
		static ResourceId freePortId(int brMode, InterfaceType ifType, const string &ifName);

        static void logBridgeMacAllocError (ResourceId err, const char *context);
        static void logPortIdAllocError (ResourceId err, const char *context, InterfaceType ifType, const string &ifName);

		static MacAddress getVcsBridgeMac(UI8 lastByte);

		static bool isVcsEnabled();
        static vector<LocationId> getDisconnectedLocations ();

        static int getModeForClassName(const string &className);
        static string getClassNameForMode(const int brMode);

        static vector<UI32> getVlanIdsForVlanObjectIds (const vector<ObjectId> &oids, DceContext *pContext = NULL);
        static bool isXstpConfigPresent(UI32 vlanId);
        static ResourceId isMstpEnabled(bool &isEnabled);
        static ResourceId checkSTPEnabledGvlanExists(UI32 &stpEnabledGvlanCount);

};


}

#endif	// MSTP_TOOL_KIT__H
