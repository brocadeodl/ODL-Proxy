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
 *   View definition to hold different vlan-ranges for physical interfaces
 ***************************************************************************/
#ifndef VLAN_RANGE_FOR_PHY_INTF_H
#define VLAN_RANGE_FOR_PHY_INTF_H

#include <string>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveManagedView.h"
#include "DcmObjectModel/DcmLocalManagedObjectBase.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"

#define ATTR_TRUNK_ALL  "isAllowedVlanAll"
#define ATTR_ADD_RANGE  "vlanRangeAdd"
#define ATTR_REM_RANGE  "vlanRangeRemove"
#define ATTR_IF_NAME    "ifName"

using namespace WaveNs;

namespace DcmNs {
    // Forward references

    class VlanRangeForPoPhyIntfView: public DcmLocalManagedObject, public WaveManagedView
    {
    protected:
        void setupAttributesForPersistence();
        void setupAttributesForCreate();
    public:
        VlanRangeForPoPhyIntfView(NsmLocalObjectManager *pNsmLocalObjectManager);
        virtual ~VlanRangeForPoPhyIntfView();
        static string getClassName();
        virtual string getSqlForCreateView();
        UI32Range getVlanRangeAdd() const;
        UI32Range getVlanRangeRemove() const;
        bool isAllowedVlanAll() const;
        void setAllowedVlanAll(bool allowedVlanAll);
        void setVlanRangeAdd(UI32Range vlanRangeAdd);
        void setVlanRangeRemove(UI32Range vlanRangeRemove);
        string getIfName() const;
        void setIfName(const string &ifName);

        // Now the data members
    private:
        bool m_allowedVlanAll;
        UI32Range m_vlanRangeAdd;
        UI32Range m_vlanRangeRemove;
        string m_ifName;
    protected:
    public:
    };

}
#endif // VLAN_RANGE_FOR_PHY_INTF_H
