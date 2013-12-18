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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Qos/Local/QosPhyIntfConfigLocalManagedObject.h"
#include "Qos/Local/QosLocalObjectManager.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosPhyIntfConfigLocalManagedObject::QosPhyIntfConfigLocalManagedObject (QosLocalObjectManager *pQosLocalObjectManager)
        : PrismElement  (pQosLocalObjectManager),
        PrismPersistableObject (QosPhyIntfConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pQosLocalObjectManager),
        DcmManagedObject (pQosLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pQosLocalObjectManager)
    {
    }

    QosPhyIntfConfigLocalManagedObject::QosPhyIntfConfigLocalManagedObject (QosLocalObjectManager *pQosLocalObjectManager,const ObjectId &physicalInterface,const ObjectId &cosMutation,const ObjectId &cosTrafficClass,const ObjectId &ceeMap,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &flowControlPfcCosValue,const SI32 &defaultCosValue,const string &qosTrust)
        : PrismElement  (pQosLocalObjectManager),
        PrismPersistableObject (QosPhyIntfConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pQosLocalObjectManager),
        DcmManagedObject (pQosLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pQosLocalObjectManager),
        m_physicalInterface    (physicalInterface),
        m_cosMutation    (cosMutation),
        m_cosTrafficClass    (cosTrafficClass),
        m_ceeMap    (ceeMap),
        m_flowControlTxEnable    (flowControlTxEnable),
        m_flowControlRxEnable    (flowControlRxEnable),
        m_flowControlPfcCosValue    (flowControlPfcCosValue),
        m_defaultCosValue    (defaultCosValue),
        m_qosTrust    (qosTrust)
    {
    }

    QosPhyIntfConfigLocalManagedObject::~QosPhyIntfConfigLocalManagedObject ()
    {
    }

    string  QosPhyIntfConfigLocalManagedObject::getClassName()
    {
        return ("QosPhyIntfConfigLocalManagedObject");
    }

    void  QosPhyIntfConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_physicalInterface,"physicalInterface", PhyIntfLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cosMutation,"cosMutation", CosToCosMutationMapManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_cosTrafficClass,"cosTrafficClass", CosToTrafficClassMapManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_ceeMap,"ceeMap", CeeMapManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeSI32(&m_flowControlTxEnable,"flowControlTxEnable"));
        addPersistableAttribute (new AttributeSI32(&m_flowControlRxEnable,"flowControlRxEnable"));
        addPersistableAttribute (new AttributeSI32(&m_flowControlPfcCosValue,"flowControlPfcCosValue"));
        addPersistableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addPersistableAttribute (new AttributeString(&m_qosTrust,"qosTrust"));
    }

    void  QosPhyIntfConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_physicalInterface,"physicalInterface", PhyIntfLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosMutation,"cosMutation", CosToCosMutationMapManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_cosTrafficClass,"cosTrafficClass", CosToTrafficClassMapManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_ceeMap,"ceeMap", CeeMapManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_flowControlTxEnable,"flowControlTxEnable"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_flowControlRxEnable,"flowControlRxEnable"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_flowControlPfcCosValue,"flowControlPfcCosValue"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addPersistableAttributeForCreate  (new AttributeString(&m_qosTrust,"qosTrust"));
    }

    void  QosPhyIntfConfigLocalManagedObject::setPhysicalInterface(const ObjectId &physicalInterface)
    {
        m_physicalInterface  =  physicalInterface;
    }

    ObjectId  QosPhyIntfConfigLocalManagedObject::getPhysicalInterface() const
    {
        return (m_physicalInterface);
    }

    void  QosPhyIntfConfigLocalManagedObject::setCosMutation(const ObjectId &cosMutation)
    {
        m_cosMutation  =  cosMutation;
    }

    ObjectId  QosPhyIntfConfigLocalManagedObject::getCosMutation() const
    {
        return (m_cosMutation);
    }

    void  QosPhyIntfConfigLocalManagedObject::setCosTrafficClass(const ObjectId &cosTrafficClass)
    {
        m_cosTrafficClass  =  cosTrafficClass;
    }

    ObjectId  QosPhyIntfConfigLocalManagedObject::getCosTrafficClass() const
    {
        return (m_cosTrafficClass);
    }

    void  QosPhyIntfConfigLocalManagedObject::setCeeMap(const ObjectId &ceeMap)
    {
        m_ceeMap  =  ceeMap;
    }

    ObjectId  QosPhyIntfConfigLocalManagedObject::getCeeMap() const
    {
        return (m_ceeMap);
    }

    void  QosPhyIntfConfigLocalManagedObject::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  QosPhyIntfConfigLocalManagedObject::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  QosPhyIntfConfigLocalManagedObject::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  QosPhyIntfConfigLocalManagedObject::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  QosPhyIntfConfigLocalManagedObject::setFlowControlPfcCosValue(const SI32 &flowControlPfcCosValue)
    {
        m_flowControlPfcCosValue  =  flowControlPfcCosValue;
    }

    SI32  QosPhyIntfConfigLocalManagedObject::getFlowControlPfcCosValue() const
    {
        return (m_flowControlPfcCosValue);
    }

    void  QosPhyIntfConfigLocalManagedObject::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  QosPhyIntfConfigLocalManagedObject::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  QosPhyIntfConfigLocalManagedObject::setQosTrust(const string &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    string  QosPhyIntfConfigLocalManagedObject::getQosTrust() const
    {
        return (m_qosTrust);
    }

}
