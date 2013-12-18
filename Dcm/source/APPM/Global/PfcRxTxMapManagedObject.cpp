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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/PfcRxTxMapManagedObject.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"

namespace DcmNs
{

    PfcRxTxMapManagedObject::PfcRxTxMapManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (PfcRxTxMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager)
    {
    }

    PfcRxTxMapManagedObject::PfcRxTxMapManagedObject (APPMGlobalObjectManager *pAPPMGlobalObjectManager,const SI32 &pfcValue,const UI32 &txValue,const UI32 &rxValue)
        : PrismElement  (pAPPMGlobalObjectManager),
        PrismPersistableObject (PfcRxTxMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pAPPMGlobalObjectManager),
        DcmManagedObject (pAPPMGlobalObjectManager),
        m_pfcValue    (pfcValue),
        m_txValue    (txValue),
        m_rxValue    (rxValue)
    {
    }

    PfcRxTxMapManagedObject::~PfcRxTxMapManagedObject ()
    {
    }

    string  PfcRxTxMapManagedObject::getClassName()
    {
        return ("PfcRxTxMapManagedObject");
    }

    void  PfcRxTxMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeSI32(&m_pfcValue,"pfcValue", appm_pfc_cos));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_txValue,"txValue", appm_pfc_tx));
        addPersistableAttribute (new AttributeEnum((UI32*)&m_rxValue,"rxValue", appm_pfc_rx));
    }

    void  PfcRxTxMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeSI32(&m_pfcValue,"pfcValue", appm_pfc_cos));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_txValue,"txValue", appm_pfc_tx));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_rxValue,"rxValue", appm_pfc_rx));
        vector<string > keyName;
        keyName.push_back ("pfcValue");
        setUserDefinedKeyCombination (keyName);
    }

    void  PfcRxTxMapManagedObject::setPfcValue(const SI32 &pfcValue)
    {
        m_pfcValue  =  pfcValue;
    }

    SI32  PfcRxTxMapManagedObject::getPfcValue() const
    {
        return (m_pfcValue);
    }

    void  PfcRxTxMapManagedObject::setTxValue(const UI32 &txValue)
    {
        m_txValue  =  txValue;
    }

    UI32  PfcRxTxMapManagedObject::getTxValue() const
    {
        return (m_txValue);
    }

    void  PfcRxTxMapManagedObject::setRxValue(const UI32 &rxValue)
    {
        m_rxValue  =  rxValue;
    }

    UI32  PfcRxTxMapManagedObject::getRxValue() const
    {
        return (m_rxValue);
    }

}
