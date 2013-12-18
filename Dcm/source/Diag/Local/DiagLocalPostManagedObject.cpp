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
 *   Author : sshaik                                                     *
 **************************************************************************/

#include "Diag/Local/DiagLocalPostManagedObject.h"
#include "Diag/Local/DiagLocalObjectManager.h"
#include "Diag/Global/DiagTypes.h"

#include "vcs.h"
//#include "brocade-diag.h"

namespace DcmNs
{

    DiagLocalPostManagedObject::DiagLocalPostManagedObject (DiagLocalObjectManager *pDiagLocalObjectManager)
        : PrismElement  (pDiagLocalObjectManager),
        PrismPersistableObject (DiagLocalPostManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pDiagLocalObjectManager),
        DcmManagedObject (pDiagLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pDiagLocalObjectManager)
    {
    }
	
	DiagLocalPostManagedObject::DiagLocalPostManagedObject (DiagLocalObjectManager *pDiagLocalObjectManager,const UI32 &rbridgeId,const bool &enabled)
        : PrismElement  (pDiagLocalObjectManager),
        PrismPersistableObject (DiagLocalPostManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pDiagLocalObjectManager),
        DcmManagedObject (pDiagLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pDiagLocalObjectManager),
        m_rbridgeId    (rbridgeId),
        m_enabled    (enabled)
    {
    }


    DiagLocalPostManagedObject::~DiagLocalPostManagedObject ()
    {
    }

    string  DiagLocalPostManagedObject::getClassName()
    {
        return ("DiagLocalPostManagedObject");
    }

	void  DiagLocalPostManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId", diagnostics_rbridge_id));
        addPersistableAttribute (new AttributeBool(&m_enabled,"enable", diagnostics_enable));
    }

    void  DiagLocalPostManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_rbridgeId,"rbridgeId", diagnostics_rbridge_id));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enabled,"enable", diagnostics_enable));
        vector<string> keyName;
        keyName.push_back ("rbridgeId");
        setUserDefinedKeyCombination (keyName);
    }

    void  DiagLocalPostManagedObject::setrbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;

    }

    UI32  DiagLocalPostManagedObject::getrbridgeId() const
    {
         return (m_rbridgeId);
    }

    void  DiagLocalPostManagedObject::setDiagPost(const bool &enabled)
    {
        m_enabled  =  enabled;
    }

    bool  DiagLocalPostManagedObject::getDiagPost() const
    {
        return (m_enabled);
    }

}
