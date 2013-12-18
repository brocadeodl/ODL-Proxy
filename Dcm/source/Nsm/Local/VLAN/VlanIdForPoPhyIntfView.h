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
 *
 *   View definition to hold vlan-ids for physical interfaces
 ***************************************************************************/
#ifndef VLAN_ID_FOR_PHY_INTF_H
#define VLAN_ID_FOR_PHY_INTF_H

#include <string>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveManagedView.h"
#include "DcmObjectModel/DcmLocalManagedObjectBase.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"

#define ATTR_IF_NAME    "ifName"
#define ATTR_VLAN_ID    "vlanId"

using namespace WaveNs;

namespace DcmNs {
    // Forward references

    class VlanIdForPoPhyIntfView: virtual public DcmLocalManagedObject, virtual public WaveManagedView {
    protected:
        void setupAttributesForPersistence();
        void setupAttributesForCreate();
    public:
        VlanIdForPoPhyIntfView(NsmLocalObjectManager *pNsmLocalObjectManager);
        virtual ~VlanIdForPoPhyIntfView();

        static string getClassName();
        virtual string getSqlForCreateView();

        void setVlanId(const UI32 &vlanId);
        UI32 getVlanId() const;
        void setIfName(const string &ifName);
        string getIfName() const;
        // Now the data members
    private:
        UI32 _vlanId;
        string _ifName;
    protected:
    public:
    };

}
#endif // VLAN_ID_FOR_PHY_INTF_H
