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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"

#include "Nsm/Local/Igmp/IgmpIntfConfManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

IgmpIntfConfManagedObject::IgmpIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
    : PrismElement  (pNsmLocalObjectManager),
    PrismPersistableObject (IgmpIntfConfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pNsmLocalObjectManager),
    DcmManagedObject (pNsmLocalObjectManager),
    WaveLocalManagedObjectBase(this),
    DcmLocalManagedObjectBase(this),
    DcmLocalManagedObject(pNsmLocalObjectManager)
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
    m_immediateLeave = false;
    m_lmqi = 1000;
    m_qmrt = 10;
    m_queryInterval = 125;
}

IgmpIntfConfManagedObject::IgmpIntfConfManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval)
    : PrismElement  (pNsmLocalObjectManager),
    PrismPersistableObject (IgmpIntfConfManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pNsmLocalObjectManager),
    DcmManagedObject (pNsmLocalObjectManager),
    WaveLocalManagedObjectBase(this),
    DcmLocalManagedObjectBase(this),
    DcmLocalManagedObject(pNsmLocalObjectManager),
    m_lmqi    (lmqi),
    m_qmrt    (qmrt),
    m_immediateLeave    (immediateLeave),
    m_queryInterval    (queryInterval)
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

IgmpIntfConfManagedObject::~IgmpIntfConfManagedObject ()
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

string  IgmpIntfConfManagedObject::getClassName()
{
    return ("IgmpIntfConfManagedObject");
}

void  IgmpIntfConfManagedObject::setupAttributesForPersistence()
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
    DcmLocalManagedObject::setupAttributesForPersistence ();
    addPersistableAttribute (new AttributeUI32UC(&m_lmqi, true, "lmqi"));
    addPersistableAttribute (new AttributeUI32UC(&m_qmrt, true, "qmrt"));
    addPersistableAttribute (new AttributeBoolUC(&m_immediateLeave, true, "immediateLeave"));
    addPersistableAttribute (new AttributeUI32UC(&m_queryInterval, true, "queryInterval"));
   
    setUserTagForAttribute("lmqi", igmp_last_member_query_interval);
    setUserTagForAttribute("qmrt", igmp_query_max_response_time);
    setUserTagForAttribute("immediateLeave", igmp_immediate_leave);
    setUserTagForAttribute("queryInterval", igmp_query_interval);
}

void  IgmpIntfConfManagedObject::setupAttributesForCreate()
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
    DcmLocalManagedObject::setupAttributesForCreate ();
    addPersistableAttributeForCreate  (new AttributeUI32UC(&m_lmqi, true, "lmqi"));
    addPersistableAttributeForCreate  (new AttributeUI32UC(&m_qmrt, true, "qmrt"));
    addPersistableAttributeForCreate  (new AttributeBoolUC(&m_immediateLeave, true, "immediateLeave"));
    addPersistableAttributeForCreate  (new AttributeUI32UC(&m_queryInterval, true, "queryInterval"));
}

void  IgmpIntfConfManagedObject::setLmqi(const UI32 &lmqi, bool usrCfg)
{
    trace(TRACE_LEVEL_INFO, __FUNCTION__);
    m_lmqi.setIsUserConfigured(usrCfg);
    m_lmqi.setUI32Value(lmqi);
}

UI32  IgmpIntfConfManagedObject::getLmqi() const
{
    return (m_lmqi.getUI32Value());
}

void  IgmpIntfConfManagedObject::setQmrt(const UI32 &qmrt, bool usrCfg)
{
    m_qmrt.setIsUserConfigured(usrCfg);
    m_qmrt.setUI32Value(qmrt);
}

UI32  IgmpIntfConfManagedObject::getQmrt() const
{
    return (m_qmrt.getUI32Value());
}

void  IgmpIntfConfManagedObject::setImmediateLeave(const bool &immediateLeave, bool usrCfg)
{
    m_immediateLeave.setIsUserConfigured(usrCfg);
    m_immediateLeave.setBoolValue(immediateLeave);
}

bool  IgmpIntfConfManagedObject::getImmediateLeave() const
{
    return (m_immediateLeave.getBoolValue());
}

void  IgmpIntfConfManagedObject::setQueryInterval(const UI32 &queryInterval, bool usrCfg)
{
    m_queryInterval.setIsUserConfigured(usrCfg);
    m_queryInterval.setUI32Value(queryInterval);
}

UI32  IgmpIntfConfManagedObject::getQueryInterval() const
{
    return (m_queryInterval.getUI32Value());
}

bool  IgmpIntfConfManagedObject::hasNonDefaultConfig() const
{
  if (m_lmqi.getIsUserConfigured() || m_qmrt.getIsUserConfigured() 
      || m_immediateLeave.getIsUserConfigured() || m_queryInterval.getIsUserConfigured())
      return true;
  return false;
}

}
