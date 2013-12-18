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

#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    PfcFlowControlLocalManagedObject::PfcFlowControlLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PfcFlowControlLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_cos = 0;
		m_rx = 0;
		m_tx = 0;
    }

    PfcFlowControlLocalManagedObject::PfcFlowControlLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PfcFlowControlLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager)
    {
		m_cos = 0;
		m_rx = 0;
		m_tx = 0;
    }

    PfcFlowControlLocalManagedObject::PfcFlowControlLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI32 &tx,const UI32 &rx,const SI32 &cos)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (PfcFlowControlLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_tx    (tx),
        m_rx    (rx),
        m_cos    (cos)
    {
    }

    PfcFlowControlLocalManagedObject::PfcFlowControlLocalManagedObject (NsmGlobalObjectManager *pNsmGlobalObjectManager,const UI32 &tx,const UI32 &rx,const SI32 &cos)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (PfcFlowControlLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmGlobalObjectManager),
        m_tx    (tx),
        m_rx    (rx),
        m_cos    (cos)
    {
    }

    PfcFlowControlLocalManagedObject::~PfcFlowControlLocalManagedObject ()
    {
    }

    string  PfcFlowControlLocalManagedObject::getClassName()
    {
        return ("PfcFlowControlLocalManagedObject");
    }

    void  PfcFlowControlLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum(&m_tx,"tx", qos_pfc_flowcontrol_tx));
        addPersistableAttribute (new AttributeEnum(&m_rx,"rx", qos_pfc_flowcontrol_rx));
        addPersistableAttribute (new AttributeSI32(&m_cos,"cos"));
		setUserTagForAttribute("cos", qos_pfc_cos);
    }

    void  PfcFlowControlLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum(&m_tx,"tx", qos_pfc_flowcontrol_tx));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_rx,"rx", qos_pfc_flowcontrol_rx));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_cos,"cos"));

		vector<string > keyName;
		keyName.push_back ("cos");
		setUserDefinedKeyCombination (keyName);
    }

    void  PfcFlowControlLocalManagedObject::setTx(const UI32 &tx)
    {
        m_tx  =  tx;
    }

    UI32  PfcFlowControlLocalManagedObject::getTx() const
    {
        return (m_tx);
    }

    void  PfcFlowControlLocalManagedObject::setRx(const UI32 &rx)
    {
        m_rx  =  rx;
    }

    UI32  PfcFlowControlLocalManagedObject::getRx() const
    {
        return (m_rx);
    }

    void  PfcFlowControlLocalManagedObject::setCos(const SI32 &cos)
    {
        m_cos  =  cos;
    }

    SI32  PfcFlowControlLocalManagedObject::getCos() const
    {
        return (m_cos);
    }

}
