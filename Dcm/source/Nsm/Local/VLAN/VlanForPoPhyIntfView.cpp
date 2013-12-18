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

#include <vector>

#include "VlanForPoPhyIntfView.h"

namespace DcmNs {
    VlanForPoPhyIntfView::VlanForPoPhyIntfView(NsmLocalObjectManager *pNsmLocalObjectManager)
       :    PrismElement           (pNsmLocalObjectManager),
            PrismPersistableObject(VlanForPoPhyIntfView::getClassName()),
            WaveManagedObject          (pNsmLocalObjectManager),
            DcmManagedObject           (pNsmLocalObjectManager),
            WaveLocalManagedObjectBase (this),
            DcmLocalManagedObjectBase  (this),
            DcmLocalManagedObject (pNsmLocalObjectManager),
            WaveManagedView(pNsmLocalObjectManager)
            {
    }

    VlanForPoPhyIntfView::~VlanForPoPhyIntfView() {

    }
    string VlanForPoPhyIntfView::getClassName() {
        return ("VlanForPoPhyIntfView");
    }
    void VlanForPoPhyIntfView::setupAttributesForPersistence() {
        addPersistableAttribute (new AttributeString(&_ifName,"ifName"));
        addPersistableAttribute (new AttributeUI32(&_vlanBegin,"vlan_begin"));
        addPersistableAttribute (new AttributeUI32(&_vlanEnd,"vlan_end"));
        addPersistableAttribute (new AttributeObjectId (&_poPhyObjectId, "poPhyObjectId"));
    }

    void VlanForPoPhyIntfView::setupAttributesForCreate() {
        addPersistableAttributeForCreate(new AttributeString(&_ifName,"ifName"));
        addPersistableAttributeForCreate(new AttributeUI32(&_vlanBegin,"vlan_begin"));
        addPersistableAttributeForCreate(new AttributeUI32(&_vlanEnd,"vlan_end"));
        addPersistableAttributeForCreate (new AttributeObjectId (&_poPhyObjectId, "poPhyObjectId"));
    }

    void VlanForPoPhyIntfView::setupOrm() {
        PrismPersistableObject::setupOrm();
    }

    UI32 VlanForPoPhyIntfView::getVlanBegin() const {
        return _vlanBegin;
    }

    UI32 VlanForPoPhyIntfView::getVlanEnd() const {
        return _vlanEnd;
    }

    string VlanForPoPhyIntfView::getIfName() const {
        return _ifName;
    }

    ObjectId VlanForPoPhyIntfView::getPoPhyObjectId() const {
        return _poPhyObjectId;
    }

    string VlanForPoPhyIntfView::getSqlForCreateView() {
        return
"CREATE OR REPLACE VIEW wavecurrent.VlanForPoPhyIntfView AS "
"SELECT ppv.ifname, vlan_begin, vlan_end, pophy.objectidclassid AS poPhyObjectIdclassid, pophy.objectidinstanceid AS poPhyObjectIdinstanceid FROM (SELECT ifname, regexp_replace(range, '-.*$', '')::int AS vlan_begin, regexp_replace(range, '^.*-', '')::int AS vlan_end FROM (SELECT DISTINCT ifname, regexp_split_to_table(vlanrangeadd, ',') AS range FROM wavecurrent.vlanrangeforpophyintfview where vlanrangeadd != '' ) AS vlanrange "
"UNION "
"SELECT ifname, vlanid AS vlan_begin, vlanid AS vlan_end FROM wavecurrent.vlanidforpophyintfview ) AS ppv, wavecurrent.pophyintfmanagedobject pophy WHERE pophy.ifname = ppv.ifname "
"; "
;
    }
}
