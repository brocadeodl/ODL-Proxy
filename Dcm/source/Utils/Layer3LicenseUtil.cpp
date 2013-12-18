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

/********************************************************** 
 * 	Copyright (C) 2011 Brocade Communications Systems, Inc.
 * 	All rights reserved.
 * 
 *  Created on: Nov 17, 2011
 *      Author: Mayank Maheshwari
 *
 *	Common utilities related to Layer 3 Licensing
 **********************************************************/

#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Utils/Layer3LicenseUtil.h"
#include "fabos/license.h"
#include "VRRP/Local/VRRPGlobalConfigManagedObject.h"
#include "DcmCStatus.h"
#include "OSPF/Local/OSPFConfigLocalManagedObject.h"
#include "Ssm/Global/IpAclPolicyManagedObject.h"
#include "Utils/Capabilities.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Pim/Local/PimRtrCfgLocalManagedObject.h"
#include "Bgp/Local/BgpLocalBasicConfigManagedObject.h"

namespace DcmNs {
    Layer3LicenseUtil :: Layer3LicenseUtil() {
    }
    Layer3LicenseUtil :: ~ Layer3LicenseUtil() {
    }

	ResourceId Layer3LicenseUtil :: isLayer3Configured(bool &isL3Configured) {
        ResourceId resId = WAVE_MESSAGE_SUCCESS;
        bool isFeatureConfigured = false;

        resId = isVrrpConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("VRRP is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isOspfConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("OSPF is configured"));
            isL3Configured = true;
            return resId;
        }

		resId = isBgpConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("BGP is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isPimConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("PIM is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isRtmConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("RTM is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isArpConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("Arp is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isL3AclConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("L3 Acl is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isIpPolicyConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("IP Policy is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isIgmpConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("IGMP is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isL3NsmConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("L3 NSM is configured"));
            isL3Configured = true;
            return resId;
        }

        resId = isL3FibConfigured(isFeatureConfigured);
        if ((WAVE_MESSAGE_SUCCESS == resId) && isFeatureConfigured) {
            trace(TRACE_LEVEL_DEBUG, string("L3 FIB is configured"));
            isL3Configured = true;
            return resId;
        }

        return resId;
    }

	ResourceId Layer3LicenseUtil :: isOspfConfigured(bool &isConfigured) {

       	trace(TRACE_LEVEL_DEBUG, "Layer3LicenseUtil::isOspfConfigured : Getting OPSF Configuration status");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 rowCount = 0;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFConfigLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeBool(true, "status"));
        syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        status = querySynchronouslyForCount(&syncQueryCtxt, rowCount);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, string ("Layer3LicenseUtil::isOspfConfigured : querySynchronouslyForCount for OPSFConfigLocalMO failed!"));
            return status;
        }

        isConfigured = (rowCount > 0) ? true : false;
        trace(TRACE_LEVEL_INFO, string("Layer3LicenseUtil::isOspfConfigured : ") + (isConfigured ? string("OSPF is configured") : string("OSPF is NOT configured")));

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isBgpConfigured(bool &isConfigured) {

        trace(TRACE_LEVEL_DEBUG, string("Layer3LicenseUtil::isBGPConfigured"));
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 rowCount = 0;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(BgpLocalBasicConfigManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        status = querySynchronouslyForCount(&syncQueryCtxt, rowCount);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, string ("Layer3LicenseUtil::isBGPConfigured: querySynchronouslyForCount failed!"));
            return status;
        }

        isConfigured = (rowCount > 0) ? true : false;
        trace(TRACE_LEVEL_INFO, string("Layer3LicenseUtil::isBgpConfigured : ") + (isConfigured ? string("BGP is configured") : string("BGP is NOT configured")));

        return (WAVE_MESSAGE_SUCCESS);
    }


	ResourceId Layer3LicenseUtil :: isPimConfigured(bool &isConfigured) {
       trace(TRACE_LEVEL_DEBUG, "Layer3LicenseUtil::isPimConfigured : Getting PIM Configuration status");
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 rowCount = 0;

        WaveManagedObjectSynchronousQueryContext
            syncQueryCtxt(PimRtrCfgLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeBool(true, "presenceFlag"));
		syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        status = querySynchronouslyForCount(&syncQueryCtxt, rowCount);


        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, string ("Layer3LicenseUtil::isPimConfigured : querySynchronouslyForCount for PimRtrCfgLocalManagedObject failed!"));
            return status;
        }

        isConfigured = (rowCount > 0) ? true : false;
        trace(TRACE_LEVEL_INFO, string("Layer3LicenseUtil::isPimConfigured : ") + (isConfigured ? string("PIM is configured") : string("PIM is NOT configured")));

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isVrrpConfigured(bool &isConfigured) {
        trace(TRACE_LEVEL_INFO, string("Getting VRRP/VRRPE configuration status"));
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 rowCount = 0;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(VRRPGlobalConfigManagedObject::getClassName());

		syncQueryCtxt.createConditionSet("VRRP_CONFIG_CHECK");
		syncQueryCtxt.addAttributeToConditionSet("VRRP_CONFIG_CHECK", new AttributeBool(true, "vrrp_enable"),
										 WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
		syncQueryCtxt.addAttributeToConditionSet("VRRP_CONFIG_CHECK", new AttributeBool(true, "vrrpe_enable"),
										 WAVE_ATTRIBUTE_CONCATENATION_OPERATOR_OR, WAVE_ATTRIBUTE_CONDITION_OPERATOR_EQUAL);
		syncQueryCtxt.combineConditionSets("VRRP_CONFIG_CHECK");
        syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());

        status = querySynchronouslyForCount(&syncQueryCtxt, rowCount);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for enable status of VRRPGlobalConfMO failed"));
            return status;
        }

        isConfigured = (rowCount > 0) ? true : false;
        trace(TRACE_LEVEL_DEBUG, isConfigured ? string("VRRP/VRRPE is configured") : string("VRRP/VRRPE NOT configured"));

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isRtmConfigured(bool &isConfigured) {
		/*
		 * Add feature specific code here to determine whether feature is
		 * configured or not. Accordingly set value of bool isConfigured.
		 * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
		 * return error message resource-Id.
		 */

		return (WAVE_MESSAGE_SUCCESS);
	}

	ResourceId Layer3LicenseUtil :: isArpConfigured(bool &isConfigured) {
        /*
         * Add feature specific code here to determine whether feature is
         * configured or not. Accordingly set value of bool isConfigured.
         * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
         * return error message resource-Id.
         */

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isL3AclConfigured(bool &isConfigured) {
        /*
         * Add feature specific code here to determine whether feature is
         * configured or not. Accordingly set value of bool isConfigured.
         * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
         * return error message resource-Id.
         */
		// commenting out the below code, as L3ACL is not supported for the license check.
		/*
		  ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 Totalipv4Acls = 0;
		  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IpAclPolicyManagedObject::getClassName());
		  status = querySynchronouslyForCount(&syncQueryCtxt, Totalipv4Acls);

        if (WAVE_MESSAGE_SUCCESS != status) {
            trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for enable status of Ipv4AclPolicyMO failed"));
            return status;
        }

		  isConfigured = (Totalipv4Acls > 0) ? true : false;			 
		  trace(TRACE_LEVEL_DEBUG, isConfigured ? string("Ipv4 ACL is configured") : string("Ipv4 ACL NOT configured"));
		  */
		  return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isIpPolicyConfigured(bool &isConfigured) {

	  	trace(TRACE_LEVEL_DEBUG, "Layer3LicenseUtil::isIpPolicyConfigured : Getting RouteMap/Prefix-list Configuration status");
		ResourceId status1 = WAVE_MESSAGE_SUCCESS;
		ResourceId status2 = WAVE_MESSAGE_SUCCESS;
		UI32 rowCountRouteMap = 0, rowCountPrefixList = 0;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(RouteMapLocalManagedObject::getClassName());
		syncQueryCtxt1.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
		status1 = querySynchronouslyForCount(&syncQueryCtxt1, rowCountRouteMap);

        if (WAVE_MESSAGE_SUCCESS != status1) {
            trace (TRACE_LEVEL_ERROR, string ("Layer3LicenseUtil::isIpPolicyConfigured : querySynchronouslyForCount for RouteMapLocalManagedObject failed!"));
            return status1;
        }

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(IpPrefixLocalManagedObject::getClassName());
		syncQueryCtxt2.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
		status2 = querySynchronouslyForCount(&syncQueryCtxt2, rowCountPrefixList);

		if (WAVE_MESSAGE_SUCCESS != status2) {
			trace (TRACE_LEVEL_ERROR, string ("Layer3LicenseUtil::isIpPolicyConfigured : querySynchronouslyForCount for IpPrefixLocalManagedObject failed!"));
			return status2;
		}

		if((rowCountRouteMap > 0) || (rowCountPrefixList > 0))
		  	isConfigured = true;
		else
		  	isConfigured = false;
		//isConfigured = ((rowCountRouteMap > 0) || (rowCountPrefixList > 0))? true : false;
		trace(TRACE_LEVEL_INFO, string("Layer3LicenseUtil::isIpPolicyConfigured : ") + (isConfigured ? string("IPPOLICY is configured") : string("IPPOLICY is NOT configured")));

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isIgmpConfigured(bool &isConfigured) {
        /*
         * Add feature specific code here to determine whether feature is
         * configured or not. Accordingly set value of bool isConfigured.
         * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
         * return error message resource-Id.
         */

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isL3NsmConfigured(bool &isConfigured) {
        /*
         * Add feature specific code here to determine whether feature is
         * configured or not. Accordingly set value of bool isConfigured.
         * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
         * return error message resource-Id.
         */

        return (WAVE_MESSAGE_SUCCESS);
    }

	ResourceId Layer3LicenseUtil :: isL3FibConfigured(bool &isConfigured) {
        /*
         * Add feature specific code here to determine whether feature is
         * configured or not. Accordingly set value of bool isConfigured.
         * If there are no errors, then return WAVE_MESSAGE_SUCCESS else
         * return error message resource-Id.
         */

        return (WAVE_MESSAGE_SUCCESS);
    }

    ResourceId Layer3LicenseUtil :: checkLayer3License() {
        if (! isLayer3Supported()) {
            return WRC_LAYER_3_NOT_SUPPORTED_SA_MODE;
        }

        if (licenseCheck(LAYER_3_LICENSE) == 0) {
            return WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    bool Layer3LicenseUtil :: isLayer3Supported() {
        if (!(Capabilities :: isCapabilityPresent(L3_SUPPORT_SA_MODE)) &&
            (WAVE_PERSISTENCE_CLUSTER_DISABLED == DcmToolKit::isVcsEnabled())) {
            return false;
        }

        return true;
    }
}
