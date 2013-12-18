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

#ifndef NSM_UTILS_H 
#define NSM_UTILS_H 

#undef max
#undef min
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Utils/DceLinearSeqContext.h"
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

#include "APPM/Global/ProfileManagedObject.h"
#include "APPM/Global/VlanProfileCfgManagedObject.h"

typedef enum  {
	SET_LOCATION_ID_API_SUCCESS = 0,
	SET_LOCATION_ID_API_ERROR,
	SWITCH_NOT_IN_VCS_MODE,
	PORT_NOT_PART_OF_ANY_INTERFACE
} NsmEnum1;		

#define	SSM_NSM_ERR_APPM_PROFILED_PORT					-34
#define NSM_API_SET_SUCCESS                               0
#define NSM_API_SET_ERR_NO_SUCH_INTERFACE	        	-38
#define NSM_ERR_PORT_PROFILE_PORT_NOT_ALLOWED           -83
#define NSM_API_ERR_INVALID_MAC							-904
#define NSM_API_ERR_MCAST_MAC							-920
#define NSM_API_ERR_BCAST_MAC							-921
#define NSM_API_ERR_RULE_ID_EXIST						-922
#define NSM_API_ERR_SAME_RULE							-923
#define NSM_API_ERR_RULE_ADD_FAILED						-924
#define NSM_API_ERR_GRP_UNCONFIG						-925
#define NSM_API_ERR_IF_INVLD_MODE						-927
#define NSM_API_ERR_IF_UNBOUND							-928
#define NSM_API_ERR_VLAN_UNCONFIG						-929
#define NSM_API_ERR_GRP_DEACTIVATE	        			-931
#define NSM_API_ERR_NO_RULE                      		-939
#define NSM_API_ERR_NO_GROUP                      		-940
#define NSM_API_VLAN_CLASS_ERR_FCOE                     -957
#define NSM_API_ERR_IF_RULE_OVERLAP                     -959
#define NSM_API_VLAN_CLASS_ERR_TRUNKPORT                -982
#define NSM_API_ERR_IF_GROUP_OVERLAP				    -960
#define NSM_API_ERR_GRP_ACTIVATE                        -930

#define NSM_API_ERR_IF_ARP_AGEING_TIMEOUT_CONFIG         -967
#define NSM_API_ERR_PORTCHANNEL_DOT1X_ENABLE            -986
#define NSM_API_ERR_PORTCHANNEL_MTU_CONFIG              -987
#define NSM_API_ERR_PORTCHANNEL_IP_MTU_CONFIG           -988
#define NSM_API_ERR_PORTCHANNEL_MAX_LAGS                -989
#define NSM_API_ERR_PORTCHANNEL_MAX_LINKS_STD           -990
#define NSM_API_ERR_PORTCHANNEL_MAX_LINKS_BRCD          -991
#define NSM_API_ERR_PORTCHANNEL_MIRROR                  -992
#define NSM_API_ERR_PORTCHANNEL_MAX_LAGS_BRCD           -993
#define NSM_API_ERR_PORTCHANNEL_ALREADY_STATIC          -994
#define NSM_API_ERR_PORTCHANNEL_ALREADY_DYNAMIC         -995
#define NSM_API_ERR_PORTCHANNEL_ALREADY_STD             -996
#define NSM_API_ERR_PORTCHANNEL_ALREADY_BRCD            -997
#define NSM_API_ISL_INTERFACE_TYPE                      -999
#define NSM_API_ERR_FPMA_MAC				-1000
#define NSM_PLUGABLE_INTERFACE_ERROR        -1001

/* Error returns for API calls */
#define NSM_LACP_ERROR_BASE               -2000
#define NSM_LACP_ERROR_ADMIN_KEY_MISMATCH (NSM_LACP_ERROR_BASE + 1)
#define NSM_LACP_ERROR_TRUNK_TYPE_MISMATCH (NSM_LACP_ERROR_BASE + 2)
#define NSM_LACP_ERROR_CHIP_NUM_MISMATCH   (NSM_LACP_ERROR_BASE + 3)
#define NSM_LACP_ERROR_CONFIGURED (NSM_LACP_ERROR_BASE + 4)
#define NSM_LACP_ERROR_MAX_LAGS_REACHED (NSM_LACP_ERROR_BASE + 5)
#define NSM_LACP_ERROR_SPEED_MISMATCH (NSM_LACP_ERROR_BASE + 6)



#define HAL_IF_CAP_EDGE     0x01
#define HAL_IF_CAP_ISL      0x02


#define SWITCHING_NOT_ENABLED					        -18000
#define BRIDGE_NOT_VLAN_AWARE					        -18001
#define NSM_APPM_PROFILED_PORT						    -18002

typedef enum NSM_MESSAGE_OPCODE_ENUM
{
    NSM_MSG_OP_NONE = 0,
    NSM_MSG_OP_CREATE,
    NSM_MSG_OP_UPDATE,
    NSM_MSG_OP_DELETE,

    NSM_MSG_OP_NUMBER
} NSM_MESSAGE_OPCODE;

typedef enum NSM_MESSAGE_CMD_ENUM
{
   NSM_MSG_CMD_NONE = 0,
   NSM_MSG_CMD_SWITCHPORT,
   NSM_MSG_CMD_ID,
   NSM_MSG_CMD_INTF_SHUTDOWN,
   NSM_MSG_CMD_STATUS,
   NSM_MSG_CMD_IP_ADDRESS,
   NSM_MSG_CMD_ALL_IPADDR,
   NSM_MSG_CMD_NODE_VLAN,
   NSM_MSG_CMD_NODE_PO,
   NSM_MSG_CMD_VLAN_IP_ADDRESS,
   NSM_MSG_CMD_PO_IP_ADDRESS,
   NSM_MSG_CMD_MTU,
   NSM_MSG_CMD_PROXY_ARP,
   NSM_MSG_CMD_ARP_AGING_TIMEOUT,
   NSM_MSG_CMD_INTF_STATUS,
   NSM_MSG_CMD_INTF_LOOPBACK,
   NSM_MSG_CMD_IP_DIRECTED_BROADCAST,
   NSM_MSG_CMD_L3READY_MO,

   NSM_MSG_CMD_NUMBER
} NSM_MESSAGE_CMD;

#define NSM_PLUG_ASSERT(exp) \
	        prismAssert ((exp), __FILE__, __LINE__);

#define NSM_PLUG_ASSERT_PTR(exp) \
	        prismAssert ((exp) != NULL, __FILE__, __LINE__);

#define NSM_PLUG_DBG_TRACE() \
	    {\
			      char nsm_cpw_debug_buffer[512]; \
			      snprintf(nsm_cpw_debug_buffer, 512,\
					                 "%s:%s:%d: Entered Function", __FILE__, __FUNCTION__, __LINE__);\
			      trace(TRACE_LEVEL_DEBUG, __FUNCTION__);\
			    }

#define NSM_PLUG_DBG(msg) \
	    { \
			        char nsm_cpw_debug_buffer[512]; \
			        snprintf(nsm_cpw_debug_buffer, 512,\
						                 "%s:%s:%d:%s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_DEBUG, string(nsm_cpw_debug_buffer)); \
			    }

#define NSM_PLUG_DBG_ERR(msg) \
	    { \
			        char nsm_cpw_debug_buffer[512]; \
			        snprintf(nsm_cpw_debug_buffer, 512,\
						                 "ERROR: %s:%s:%d:%s", __FILE__, __FUNCTION__, __LINE__, msg); \
			        trace(TRACE_LEVEL_ERROR, string(nsm_cpw_debug_buffer)); \
			    }

#define NSM_PLUG_DBG_ERR_S(msg) \
	    { \
			        char nsm_cpw_debug_buffer[512]; \
			        snprintf(nsm_cpw_debug_buffer, 512,\
						                 "ERROR: %s:%s:%d", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_ERROR, string(nsm_cpw_debug_buffer) + msg ); \
			    }


#define NSM_PLUG_DBG_S(msg) \
	    { \
			        char nsm_cpw_debug_buffer[512]; \
			        snprintf(nsm_cpw_debug_buffer, 512,\
						                 "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
			        trace(TRACE_LEVEL_DEBUG, string(nsm_cpw_debug_buffer) + msg); \
	    }

#define NSM_PLUG_INFO_S(msg) \
	    { \
			        char nsm_cpw_debug_buffer[512]; \
			        snprintf(nsm_cpw_debug_buffer, 512,\
						                 "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
			        WaveNs::trace(TRACE_LEVEL_INFO, string(nsm_cpw_debug_buffer) + msg); \
	    }

#define DCM_PLUG_TRACE(_tag_, _trace_lvl_, _msg_)                                           \
        {                                                                                   \
                    char _debug_buffer[512];                                                \
                    snprintf(_debug_buffer, 512, "[%s] %s:%s:%d: ",                         \
                        _tag_, __FILE__, __FUNCTION__, __LINE__);                           \
                    trace(_trace_lvl_, string(_debug_buffer) + _msg_);                      \
        }

#define APPM_PLUG_TRACE(_trace_lvl_, _msg_) DCM_PLUG_TRACE("APPM", _trace_lvl_, _msg_)

#define APPM_PLUG_TRACE_FUNC_ENTRY(_trace_lvl_) APPM_PLUG_TRACE(_trace_lvl_, "FN Enter")

#define APPM_PLUG_TRACE_FUNC_EXIT(_trace_lvl_) APPM_PLUG_TRACE(_trace_lvl_, "FN Exit")

using namespace WaveNs;
using namespace std;

namespace DcmNs
{
class PhyIntfLocalManagedObject;
class VlanIntfManagedObject;
class PoIntfManagedObject;
class DceContext;
class DceLinearSeqContext;

template<class T>
string getString(const T & t);
string StringToLower(string strToConvert);

typedef struct VlanInformation_ {
    UI32Range provisionedVlans;
    UI32Range unProvisionedVlans;
} VlanInformation;

class NsmUtils: public WaveObjectManagerToolKit
{
    private:
        NsmUtils();
    public:
    static const UI32 MAX_DOT1Q_VLAN = 4092;
    static const UI32 DEFAULT_DOT1Q_VLAN = 1;
    static ResourceId getThreeTupleIfName (const string& ifName, 
                                           string& threeTupleIfName,
                                           const SI32 mappedId);
    static void getTwoTupleIfName (const string& ifName, string& twoTupleIfName);
    static SI32 getMappedIdFromIfName (const string& threeTupleIfName);
    static bool isValidTwoTuple (const string& ifName);
    static bool isValidThreeTuple (const string& ifName);
	static void ClusterGetFirstFailureLocation(WaveSendToClusterContext  *ctx_p , LocationId &locationid);
	static ResourceId mapBackEndErrorsToDcmdErrors(int err);
    static UI32 getLocalLocationId ();
    static SI32 getRbridgeId (string ifName);
    static string getTeThreeTuple (UI32  mappedId, const string& ifName);
	static string getMeIfName(UI32  mappedId, const string& ifName);
	static bool isPoCmdSendToLocalPlugin(const string& PoId);
	static bool isPoCmdSendToLocalPlugin(UI32& PoId);
	static ResourceId isPoIntfHasMember(const ObjectId &PoObjectId);
	static ResourceId isPoIntfHasMemberByLocation(const ObjectId &PoObjectId,const LocationId &locationId);
	static bool isLocationIdExists(vector<LocationId> &locs ,const  LocationId &locationid);
	static void insert_LocationId(vector<LocationId> &locs ,const  LocationId &locationid);
	static ResourceId getPoMemberLocationIds(const UI32 &PoId, vector<LocationId> &locationIds, bool includeDisconnectedNodes = true);
	static ResourceId getPoMemberLocationIds(const string  &PoName,  vector<LocationId>& locationIds, bool includeDisconnectedNodes = true);
	static ResourceId getPoTypeAndRbridge(const UI32 &poId, UI32 &poType, UI32 &rBridge);
    static ResourceId getPoMemberIfindexList(const UI32 &PoId, vector<UI64> &ifx_list);
    static ResourceId getPoMemberIfindexList(const string  &PoName,  vector<UI64>& ifx_list);
	static int getPoNumOfMemberPorts(const string  &PoName);
	static int getPoNumOfMemberPortsByLocationId(const string &PoName, const LocationId &locId);
	static int getPoNumOfRBridges(const string  &PoName, vector<SI32> &memberRbridges);
	static UI32 isPoConfigured(const SI32 &poId);  
    static ResourceId GetPhyMoByPoOid(ObjectId &poIntf_oid,vector<PhyIntfLocalManagedObject *> &vpPhyIntfLocalManagedObject);
	static int setLocationIdsForPoSubModeCmds(string PoId, WaveSendToClusterContext *ctx_p, bool includeDisconnectedNodes = false);
	static int setLocationIdsForPoSubModeCmds(UI32 PoId, WaveSendToClusterContext *ctx_p, bool includeDisconnectedNodes = false);
    static ResourceId wyserEaConfigurationErrorCode(WaveNs::PrismMessage* msg_p, ResourceId status);
    static VlanIntfManagedObject * getVlanIntfObjectByObjectId (const ObjectId &objectId);
    static string getVlanIntfNameByObjectId(const ObjectId &objectId);
    static PoIntfManagedObject * getPoIntfObjectByObjectId (const ObjectId &objectId);
    static string getPoIntfNameByObjectId(const ObjectId &objectId);
	static LocationId getLocationId(string ifName);
	static int32_t getSlotId(string ifName);
    static int32_t getPortId(string ifName);

    static bool isIntfL2 (const InterfaceType ifType, const string &ifName);
    static bool isIntfPortProfiled(const InterfaceType ifType, const string &ifName);
    static bool isIntfShut(UI32 ifType, const string &ifName);

	static ResourceId getIntfPortProfiled(const  string &ifName, UI32 type);

	static InterfaceType getIfType(const ObjectId &ifOid);
    static string getIfShortName (InterfaceType ifType, const string &ifName);

	static ResourceId isAllUI32RangeVlanConfigured (UI32Range vlanRange, vector<ObjectId> *vlanOids = NULL, PrismLinearSequencerContext *prismCntx = NULL,
		PrismSynchronousLinearSequencerContext *prismSyncCntx = NULL);
    static ResourceId isAllUI32RangeVlanConfigured (UI32Range vlanRange, vector<ObjectId>  &vlanid);
    static bool isNativeVlanPresentInList (UI32Range &vlanAddrange, PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL, PoIntfManagedObject *pPoIntfManagedObject = NULL);
   static bool isNativeVlanPresentInList (UI32Range &vlanAddrange, UI32 nativeVlanId, PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL, PoIntfManagedObject *pPoIntfManagedObject = NULL); 
    static bool isVlanClassifierGroupIdAddedToPoInterface(UI32 groupId, string ifName, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static bool isVlanClassifierRuleIdExists(UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static bool isVlanClassifierGroupRuleEntryExists(UI32 groupId, UI32 ruleId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static bool isVlanClassifierGroupIdExists(UI32 groupId, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
	static bool isVlanClassifierGroupIdAddedToPhyInterface(UI32 groupId, string ifName, bool isMarkedForDeletion);

    static ResourceId getVlanIntfObjectIdByVlanIfName(UI32 vlanId, ObjectId &vlanOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static ResourceId getObjectIdByVlanClassifierRuleId(int ruleId, ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static ResourceId getPoIntfObjectIdByPoIfName(string poName, ObjectId &poIntfOid,  bool isMarkedForDeletion= false, PrismLinearSequencerContext *seq_ctx_p = NULL);
    static ResourceId getPoMoByPoId (UI32 PoId, PoIntfManagedObject* &ppPoIntfManagedObject);
	static ResourceId getPhyMoByPhyName(const string &phyName, PhyIntfLocalManagedObject* &pPhyIntfLocalManagedObject);
	static UI32 getVlanClassifierRuleByObjectId(const ObjectId &ruleOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
	static UI32 getVlanIdByObjectId(const ObjectId &vlanOid, bool isMarkedForDeletion, PrismLinearSequencerContext *seq_ctx_p = NULL);
	static bool isPhyPortInISLMode(const string ifName, bool isMarkForDeletion ,PrismSynchronousLinearSequencerContext *seq_ctx_p = NULL);
    static ResourceId checkIsISLLink(const string &ifName, bool &isISL);
    static int32_t getFcoeVlan();
	/* APPM */
	static bool isProfileSpeficFcoeApplied(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject);
	static bool isProfileSpecificFcoeApplied(string ifName);
	static bool isFcoeConfiguredOnAnyProfiledPort();
	static void setLocalLocationIdForPhase1Message(WaveSendToClusterContext *ctx_p);
	static void setLocationIdFromIfNameForPhase1Message(WaveSendToClusterContext *ctx_p, string ifName);
	static ResourceId getVlanIntfObjectIdByVlanIfName (UI32 vlanId, ObjectId &vlanOid, bool isMarkedForDeletion, PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	static vector<WaveManagedObject *>  * queryPoMembersForLocationId (const ObjectId &poIntf_oid,const LocationId &locationId);
	static int queryPoEdgeMembersCountForLocationId (const ObjectId &poIntf_oid,const LocationId &locationId);
	static ResourceId isPoIntfHasMemberByLocationId (const ObjectId &poIntf_oid, const LocationId &locationId);
	static ResourceId getIntfMode(const  string &ifName, UI32 type);

	static int convertThreeTuppleToTwoTupple(const string ifName,string &twoTuppleIfName);

	static InterfaceType getInterfaceType(UI32 ifcapability); 
	static bool isValidInterface(string ifName, InterfaceType ifType, LocationId loc = 0);
    static bool verifyPORange (char *argument);
	static bool checkPhyPortFcoeQosCompatibility(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject, ResourceId &status);
	static bool checkLagFcoeQosCompatibility(PoIntfManagedObject *pPoIntfManagedObject, ResourceId &status);

    static bool isFcoeConfiguredForAnyLagPort(const UI32 &loc_id);
    static bool isFcoeConfiguredForAnyPhysicalPort(const UI32 &loc_id);

    static bool isFcoeConfiguredForEthPort(const UI32 &loc_id);
    static bool isFcoeConfiguredForLagPort(const UI32 &po_id);
	static bool isAnyPoMemberFcoeConfigured(const UI32 &PoId);
	static bool isAnyPoMemberFcoeConfigured(const string  &PoName);
    static bool isFsbEnabled();

    static void SlotGetPhyIntfMOs(vector<PhyIntfLocalManagedObject*>& mos,
        int32_t slotId = 0, LocationId locId = 0);
    static void GetPoIntfMOs(vector<PoIntfManagedObject*>& mos);
    static InterfaceType getIntfTypeFromName(const string &name);
    static ResourceId NsmBackendErrorMapToDcmStatus(int err);
    static ResourceId ArpBackendErrorMapToDcmStatus(int err);
 
    static UI32 getClientOpCode(UI32 cmd, UI32 op);

    static void setAllCompositionfalse(WaveManagedObjectSynchronousQueryContext& syncQueryCtxt);
    static void loadOnlySelectedField(WaveManagedObjectSynchronousQueryContext& syncQueryCtxt, const char *field, bool isSelectedFieldOid = false);

    static bool isVlanPartOfActiveProfile(UI32 vlanId, bool &isChangeVlanList);
    static UI64 getPhyPortIfindexByName(const string ifName);

    static bool isRemoteSpanVlan (UI32 vlanId);

    static vector<WaveManagedObject *>* queryVlanMo(vector<UI32> &vids, const string &selectField = "");
    static vector<WaveManagedObject *>* queryVlanMo(vector<UI32> &vids, vector<string> &selectFields);
	static ResourceId isAnyPortAMemberofVlan(UI32 vlanId, DceContext *ctxt = NULL);
	static bool isVlanPartOfRspanSession(UI32 vlanId);
	static bool isAclAppliedOnVlanInterfaceMember(const ObjectId &vlanObjectId, const UI32 &vlanId, const ssmAcl_binding_direction &direction);
	static bool isIgmpSnoopingConfiguredOnVlan(UI32 vlanId);
	static ResourceId isAllUI32RangeRspanVlanConfigured (UI32Range vlanRange);
	static bool isInterfaceInService(const string ifName);
	static UI32Range getRspanVlanUI32Range(PrismSynchronousLinearSequencerContext *pContext);
	static void getPoMemberIfindexListForLocalNode(const string  &PoName, vector<UI64> &ifx_list);
	static ResourceId doesVlanExist(UI32 vlanId);

    static UI32 getVlanClassificationMOCount(UI32 vlanId, UI32 ctagId, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
    static bool isGvlanIdPresentInRange(UI32Range &vlanRange);
    static ResourceId populateVlanInformation(VlanInformation &vlanInformation,
            const SI32 slotId = INVALID_SLOT_ID, const string &ifname = "");
	static bool isTrunkAllPresent(const SI32 slotId = INVALID_SLOT_ID,
	        const string &ifname = "");
    static bool isGVlanConfigPresent();
	static UI32 getVlanClassificationMOCount(UI32Range ctagRange, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
	static UI32 getVlanClassificationMOCount(const string &vlanClassificationMO, const UI32 &vlanId, const string &mac, const UI32 &macGroupId, const UI32 &ctagId, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
//	static UI32 getVlanClassificationMOCount(vector<string> &vlanClassificationMOVector, const UI32 &vlanId, const string &mac, const UI32 &macGroupId, const UI32 &ctagId, const ObjectId &pophyObjectId = ObjectId::NullObjectId);
};



}

#endif  //NSM_UTILS_H

