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

#include "VlanRangeForPoPhyIntfView.h"

namespace DcmNs {
    VlanRangeForPoPhyIntfView::VlanRangeForPoPhyIntfView(
            NsmLocalObjectManager *pNsmLocalObjectManager) :
            PrismElement(pNsmLocalObjectManager), PrismPersistableObject(
                    VlanRangeForPoPhyIntfView::getClassName()),
                    WaveManagedObject(pNsmLocalObjectManager),
                    DcmManagedObject(pNsmLocalObjectManager),
                    WaveLocalManagedObjectBase(this),
                    DcmLocalManagedObjectBase(this),
                    DcmLocalManagedObject(pNsmLocalObjectManager),
                    WaveManagedView(pNsmLocalObjectManager) {
    }

    VlanRangeForPoPhyIntfView::~VlanRangeForPoPhyIntfView() {

    }
    string VlanRangeForPoPhyIntfView::getClassName() {
        return ("VlanRangeForPoPhyIntfView");
    }
    void VlanRangeForPoPhyIntfView::setupAttributesForPersistence() {
        DcmLocalManagedObjectBase::setupAttributesForPersistence();
        addPersistableAttribute(
                new AttributeBool(&m_allowedVlanAll, ATTR_TRUNK_ALL));
        const UI32Range defaultData("");
        addPersistableAttribute(
                new AttributeUI32Range(&m_vlanRangeAdd, defaultData,
                        ATTR_ADD_RANGE));
        addPersistableAttribute(
                new AttributeUI32Range(&m_vlanRangeRemove, defaultData,
                        ATTR_REM_RANGE));
        addPersistableAttribute(
                new AttributeString(&m_ifName, ATTR_IF_NAME));
    }

    void VlanRangeForPoPhyIntfView::setupAttributesForCreate() {
        DcmLocalManagedObjectBase::setupAttributesForCreate();
        addPersistableAttributeForCreate(
                new AttributeBool(&m_allowedVlanAll, ATTR_TRUNK_ALL));
        addPersistableAttributeForCreate(
                new AttributeUI32Range(&m_vlanRangeAdd, ATTR_ADD_RANGE));
        addPersistableAttributeForCreate(
                new AttributeUI32Range(&m_vlanRangeRemove, ATTR_REM_RANGE));
        addPersistableAttributeForCreate(
                new AttributeString(&m_ifName, ATTR_IF_NAME));
    }

    string VlanRangeForPoPhyIntfView::getSqlForCreateView() {
        return
"CREATE OR REPLACE VIEW wavecurrent.VlanRangeForPoPhyIntfView AS "
"SELECT phy.ifname, isAllowedVlanAll,vlanRangeAdd,vlanRangeRemove, ownerwavenodeobjectidclassid, ownerwavenodeobjectidinstanceid, waveSlotLocalManagedObjectObjectIdclassid, waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.PhyIntfLocalManagedObject phy WHERE swMode = 2  "
"UNION "
"SELECT pophy.ifname, pophy.isAllowedVlanAll, pophy.vlanRangeAdd, pophy.vlanRangeRemove, phy.ownerwavenodeobjectidclassid, phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy WHERE pophy.swMode = 2 AND pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid "
"UNION  "
"SELECT phy.ifname, pVlanIsAllowedVlanAll,pVlanRangeAdd,pVlanRangeRemove, ownerwavenodeobjectidclassid, ownerwavenodeobjectidinstanceid, waveSlotLocalManagedObjectObjectIdclassid, waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.PhyIntfLocalManagedObject phy WHERE pVlanIsAllowedVlanAll = 't' OR pVlanRangeAdd != '' OR pVlanRangeRemove != '' "
"UNION "
"SELECT pophy.ifname, pophy.pVlanIsAllowedVlanAll, pophy.pVlanRangeAdd, pophy.pVlanRangeRemove, phy.ownerwavenodeobjectidclassid, phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy  WHERE (pophy.pVlanIsAllowedVlanAll = 't' OR pophy.pVlanRangeAdd != '' OR pophy.pVlanRangeRemove != '') AND pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid "
"UNION "
"SELECT phy.ifname, isAllowedVlanAll,rspan_trunkvlanrange,rspan_trunkvlanrangeremove, ownerwavenodeobjectidclassid, ownerwavenodeobjectidinstanceid, waveSlotLocalManagedObjectObjectIdclassid, waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.PhyIntfLocalManagedObject phy WHERE swMode = 2 AND rspan_trunkvlanrange != '' "
"UNION "
"SELECT phy.ifname, 'f', pvm.secvlanrange, '',  phy.ownerwavenodeobjectidclassid, phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.PVlanMappingManagedObject pvm, wavecurrent.phyintflocalmanagedobject phy WHERE pvm.ownermanagedobjectidclassid = phy.objectidclassid AND pvm.ownermanagedobjectidinstanceid = phy.objectidinstanceid "
"UNION "
"SELECT pophy.ifname, 'f', pvm.secvlanrange, '',  phy.ownerwavenodeobjectidclassid, phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.PVlanMappingManagedObject pvm, wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy WHERE pvm.ownermanagedobjectidclassid = phy.poidclassid AND pvm.ownermanagedobjectidinstanceid = phy.poidinstanceid AND phy.poidinstanceid = pophy.objectidinstanceid "
"UNION "
"select pophy.ifname, vpc.isAllowedVlanAll, vpc.vlanRangeAdd, vpc.vlanRangeRemove, phy.ownerwavenodeobjectidclassid, phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid FROM wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy, wavecurrent.amppprofiledomaincompositemanagedobject pd, wavecurrent.profiledomainmanagedobject pdmo, wavecurrent.profiledomaincompositemanagedobject pdc, wavecurrent.vlanprofilecfgmanagedobject vpc where pophy.isPortProfiled = 'f' and pophy.objectidinstanceid = pd.ownermanagedobjectidinstanceid and pd.portprofiledomainname = pdmo.portprofiledomainname and pdmo.objectidinstanceid = pdc.ownermanagedobjectidinstanceid and pdc.portprofilename = vpc.description and vpc.swMode=2 and pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid "
"UNION "
"select phy.ifname, vpc.isAllowedVlanAll, vpc.vlanRangeAdd, vpc.vlanRangeRemove, phy.ownerwavenodeobjectidclassid,phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid from  wavecurrent.phyintflocalmanagedobject phy, wavecurrent.amppprofiledomaincompositemanagedobject pd, wavecurrent.profiledomainmanagedobject pdmo, wavecurrent.profiledomaincompositemanagedobject pdc, wavecurrent.vlanprofilecfgmanagedobject vpc where phy.isPortProfiled = 'f' and phy.objectidinstanceid = pd.ownermanagedobjectidinstanceid and pd.portprofiledomainname = pdmo.portprofiledomainname and pdmo.objectidinstanceid = pdc.ownermanagedobjectidinstanceid and pdc.portprofilename = vpc.description and vpc.swMode=2 "
"UNION "
"select  pophy.ifname, vpc.isAllowedVlanAll, vpc.vlanRangeAdd, vpc.vlanRangeRemove, phy.ownerwavenodeobjectidclassid,phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid from  wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy, wavecurrent.amppprofilecompositemanagedobject pd, wavecurrent.profilemanagedobject ppmo,wavecurrent.vlanprofilecfgmanagedobject vpc where pophy.isPortProfiled = 'f' and pophy.objectidinstanceid = pd.ownermanagedobjectidinstanceid and pd.portprofilename = ppmo.profilename and ppmo.profilename = vpc.description and vpc.swMode=2 and pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid "
"UNION "
"select phy.ifname, vpc.isAllowedVlanAll, vpc.vlanRangeAdd, vpc.vlanRangeRemove, phy.ownerwavenodeobjectidclassid,phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid from  wavecurrent.phyintflocalmanagedobject phy, wavecurrent.amppprofilecompositemanagedobject pd, wavecurrent.profilemanagedobject ppmo,wavecurrent.vlanprofilecfgmanagedobject vpc where phy.isPortProfiled = 'f' and phy.objectidinstanceid = pd.ownermanagedobjectidinstanceid and pd.portprofilename = ppmo.profilename and ppmo.profilename = vpc.description and vpc.swMode=2 "
"UNION "
"select phy.ifname, vpmo.isallowedvlanall, vpmo.vlanrangeadd, vpmo.vlanrangeremove, phy.ownerwavenodeobjectidclassid,phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid from wavecurrent.vlanprofilecfgmanagedobject vpmo, wavecurrent.phyintflocalmanagedobject phy  where phy.isPortProfiled = 't' and vpmo.description in (select distinct pdc.portprofilename from wavecurrent.profiledomaincompositemanagedobject pdc, wavecurrent.profiledomainmanagedobject pdmo where pdc.ownermanagedobjectidclassid = pdmo.objectidclassid and pdc.ownermanagedobjectidinstanceid = pdmo.objectidinstanceid and pdmo.portprofiledomainname = 'default') and (exists (SELECT isPortProfiled FROM wavecurrent.phyintflocalmanagedobject WHERE isPortProfiled = 't') or exists (SELECT pophy.isPortProfiled FROM wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy WHERE pophy.isPortProfiled = 't' and pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid)) and vpmo.swmode = '2' "
"UNION "
"select pophy.ifname, vpmo.isallowedvlanall, vpmo.vlanrangeadd, vpmo.vlanrangeremove, phy.ownerwavenodeobjectidclassid,phy.ownerwavenodeobjectidinstanceid, phy.waveSlotLocalManagedObjectObjectIdclassid, phy.waveSlotLocalManagedObjectObjectIdinstanceid from wavecurrent.vlanprofilecfgmanagedobject vpmo, wavecurrent.phyintflocalmanagedobject phy, wavecurrent.pophyintfmanagedobject pophy  where (pophy.isPortProfiled = 't'  and pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid) and vpmo.description in (select distinct pdc.portprofilename from wavecurrent.profiledomaincompositemanagedobject pdc, wavecurrent.profiledomainmanagedobject pdmo where pdc.ownermanagedobjectidclassid = pdmo.objectidclassid and pdc.ownermanagedobjectidinstanceid = pdmo.objectidinstanceid and pdmo.portprofiledomainname = 'default') and (exists (SELECT isPortProfiled FROM wavecurrent.phyintflocalmanagedobject WHERE isPortProfiled = 't') or exists (SELECT pophy.isPortProfiled FROM wavecurrent.pophyintfmanagedobject pophy, wavecurrent.phyintflocalmanagedobject phy WHERE pophy.isPortProfiled = 't' and pophy.objectidclassid = phy.poidclassid AND pophy.objectidinstanceid = phy.poidinstanceid)) and vpmo.swmode = '2' "
"; "
;
    }

    UI32Range VlanRangeForPoPhyIntfView::getVlanRangeAdd() const
    {
        return m_vlanRangeAdd;
    }

    UI32Range VlanRangeForPoPhyIntfView::getVlanRangeRemove() const
    {
        return m_vlanRangeRemove;
    }

    bool VlanRangeForPoPhyIntfView::isAllowedVlanAll() const
    {
        return m_allowedVlanAll;
    }

    void VlanRangeForPoPhyIntfView::setAllowedVlanAll(bool allowedVlanAll)
    {
        m_allowedVlanAll = allowedVlanAll;
    }

    void VlanRangeForPoPhyIntfView::setVlanRangeAdd(UI32Range vlanRangeAdd)
    {
        m_vlanRangeAdd = vlanRangeAdd;
    }

    void VlanRangeForPoPhyIntfView::setVlanRangeRemove(UI32Range vlanRangeRemove)
    {
        m_vlanRangeRemove = vlanRangeRemove;
    }

    void VlanRangeForPoPhyIntfView::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string VlanRangeForPoPhyIntfView::getIfName() const
    {
        return m_ifName;
    }
}

