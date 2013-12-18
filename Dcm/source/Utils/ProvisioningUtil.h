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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Ganesh Sindigi
 *
 *   Declarations for provisioning utilities
 ***************************************************************************/
#ifndef PROVISIONING_UTIL_H
#define PROVISIONING_UTIL_H

#include <string>
#include <set>
#include "vcs.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Types/UI32Range.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {

/**
 * Utilities to deal with provisioned MOs.
 */

class ProvisioningUtil {
private:
    static PrismMutex provisioningMutex;
    static set<UI32> provisionedVlans;

public:

private:

public:

    /**
     * Returns true if the specified VLAN is provisioned.
     */
    static bool isVlanProvisioned(const UI32 vlanId);
    /**
     * Marks specified VLAN as provisioned.
     */
    static void addProvisionedVlan(const UI32 vlanId);
    /**
     * Marks specified VLAN as un-provisioned.
     */
    static void removeUnProvisionedVlan(const UI32 vlanId);

    /**
     * Marks specified VLAN as provisioned.
     */
    static void addProvisionedVlan(const UI32Range &vlanIdRange);
    /**
     * Marks specified VLAN as un-provisioned.
     */
    static void removeUnProvisionedVlan(const UI32Range &vlanIdRange);

    /** For debug purpose only */
    static void printProvisionedVlans();

    /** */
    static UI32 isGvlanEnabled();

    /**
     * Returns true if the specified managed object is provisioned on the
     * specified switch; otherwise false.
     * If switch is not specified, then its from the local switch.
     */
    static bool isMOProvisioned(const ObjectId &moObjectId,
            const LocationId locationId = 0);


    static bool isMOProvisioned (
            WaveManagedObjectSynchronousQueryContext* queryCtxt,
            const LocationId locationId = 0);

    /**
     * Fills in the given vector with the ObjectId of managed objects which
     * are provisioned on the switch specified by locationId.
     * If switch is not specified, then its from the local switch.
     */
    static ResourceId populateProvisionedMOIds(
            vector<ObjectId> &provisionedObjectIds,
            const string &moClassName = "",
            const LocationId locationId = 0);

    /**
     * Fills in the given vector with the WaveManagedObject pointers of managed
     * objects which are provisioned on the switch specified by locationId.
     *
     * If switch is not specified, then its from the local switch.
     */
    static ResourceId populateProvisionedMOs(
            vector<WaveManagedObject *> &provisionedMOs,
            const string &moClassName = "",
            const LocationId locationId = 0);

    /**
     *Fills the given vector with locationIds where the input managed object is 
     *provisioned.
     */
    static ResourceId populateProvisionedLocationIdsForMO (
            vector<LocationId> &locationIds,
            WaveManagedObjectSynchronousQueryContext* queryCtxt);

    /**
     *Returns range of vlan ids, which are provisioned on this node.
     */
    static UI32Range getProviosionedMORange();


};

}

#endif //PROVISIONING_UTIL_H

