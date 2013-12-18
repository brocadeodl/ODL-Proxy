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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Interface/Composition/InterfaceCompositionLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionChildLocalManagedObject.h"
#include "Interface/NDeepComposition/InterfaceNDeepCompositionGChildLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectLoadOperationalDataContext.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"

#include "interface_with_composition.h"

namespace DcmNs
{

InterfaceCompositionLocalManagedObject::InterfaceCompositionLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceCompositionLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceCompositionLocalManagedObject::~InterfaceCompositionLocalManagedObject ()
{
}

string InterfaceCompositionLocalManagedObject::getClassName ()
{
    return ("InterfaceCompositionLocalManagedObject");
}

void InterfaceCompositionLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeManagedObjectVectorComposition<InterfaceCompositionChildLocalManagedObject> (&m_children, "children", getClassName (), getObjectId (), InterfaceCompositionChildLocalManagedObject::getClassName ()));

    setUserTagForAttribute ("name", interface_composition_name);
}

void InterfaceCompositionLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<InterfaceCompositionChildLocalManagedObject> (&m_children, "children", getClassName (), getObjectId (), InterfaceCompositionChildLocalManagedObject::getClassName ()));

    vector<string > keyName;
    keyName.push_back ("name");
    setUserDefinedKeyCombination (keyName);
}

void InterfaceCompositionLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}


vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > InterfaceCompositionLocalManagedObject::getChildren () const
{
    return (m_children);
}

void InterfaceCompositionLocalManagedObject::setChildren (vector<WaveManagedObjectPointer<InterfaceCompositionChildLocalManagedObject> > children)
{
    m_children = children;
}

vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > InterfaceCompositionLocalManagedObject::getNDeepChildren () const
{
    return (m_nDeepchildren);
}

void InterfaceCompositionLocalManagedObject::setNDeepChildren (vector<WaveManagedObjectPointer<InterfaceNDeepCompositionChildLocalManagedObject> > children)
{
    m_nDeepchildren = children;
}

////////////////////////////////////////////////////////////////////////////////////
/// Composition Child
///////////////////////////////////////////////////////////////////

InterfaceCompositionChildLocalManagedObject::InterfaceCompositionChildLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceCompositionChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager)
{
}

InterfaceCompositionChildLocalManagedObject::InterfaceCompositionChildLocalManagedObject (WaveObjectManager *pWaveObjectManager, SI32 &id, bool allow)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (InterfaceCompositionChildLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this),
      DcmLocalManagedObject      (pWaveObjectManager),
      m_rule_id                  (id),
      m_allow                    (allow)
{
}

InterfaceCompositionChildLocalManagedObject::~InterfaceCompositionChildLocalManagedObject ()
{
}

string InterfaceCompositionChildLocalManagedObject::getClassName ()
{
    return ("InterfaceCompositionChildLocalManagedObject");
}

void InterfaceCompositionChildLocalManagedObject::setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeSI32          (&m_rule_id,          "rule_id",         interface_composition_rule_id));
    addPersistableAttribute (new AttributeBool          (&m_allow,            "allow",           interface_composition_isAllowed));
	addPersistableAttribute (new AttributeUI32          (&m_operationalRule , "operationalRule", interface_composition_operationalRule,  true));
	addPersistableAttribute (new AttributeUI32          (&m_operationalRule1 , "operationalRule1", interface_composition_operationalRule1,  true));
}

void InterfaceCompositionChildLocalManagedObject::setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeSI32          (&m_rule_id,           "rule_id",           interface_composition_rule_id));
    addPersistableAttributeForCreate (new AttributeBool          (&m_allow,           "allow",           interface_composition_isAllowed));
	addPersistableAttributeForCreate (new AttributeUI32          (&m_operationalRule , "operationalRule", interface_composition_operationalRule,  true));
	addPersistableAttributeForCreate (new AttributeUI32          (&m_operationalRule1 , "operationalRule1", interface_composition_operationalRule1,  true));

    vector<string > keyName;
    keyName.push_back ("rule_id");
    setUserDefinedKeyCombination (keyName);
}

void InterfaceCompositionChildLocalManagedObject::loadOperationalData (WaveManagedObjectLoadOperationalDataContext *pWaveManagedObjectLoadOperationalDataContext)
{
    trace (TRACE_LEVEL_DEVEL, "InterfaceCompositionChildLocalManagedObject::loadOperationalData: Entering ...");
    
    vector<string> operationalFields         = pWaveManagedObjectLoadOperationalDataContext->getOperationalDataFields ();
    UI32           numberOfOperationalFields = operationalFields.size ();
    UI8         i               = 0;
    static UI32 operationalRule = 0;

    if (0 == numberOfOperationalFields)
    {
        // Fill All Operational Fields
        setOperationalRule (operationalRule++);
        setOperationalRule1 (operationalRule * getId());
    }    
    else 
    {
        for (i = 0; i < numberOfOperationalFields; i++)
        {
            if (!operationalFields[i].compare ("operationalRule")) 
			{
                setOperationalRule (operationalRule++);
			}
			else if (!operationalFields[i].compare ("operationalRule1")) 
			{
                operationalRule++;
                setOperationalRule1 (operationalRule * getId());
			}
        }
    }
    pWaveManagedObjectLoadOperationalDataContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveManagedObjectLoadOperationalDataContext->callback ();
}

SI32 InterfaceCompositionChildLocalManagedObject::getId () const
{
    return (m_rule_id);
}

void InterfaceCompositionChildLocalManagedObject::setId (const SI32 &rule_id)
{
    m_rule_id = rule_id;
}

bool InterfaceCompositionChildLocalManagedObject::getIsAllowed () const
{
    return (m_allow);
}

void InterfaceCompositionChildLocalManagedObject::setIsAllowed (const bool &allow)
{
    m_allow = allow;
}

UI32 InterfaceCompositionChildLocalManagedObject::getOperationalRule () const
{
    return (m_operationalRule);
}

void InterfaceCompositionChildLocalManagedObject::setOperationalRule (const UI32 &operationalRule) 
{
    m_operationalRule = operationalRule;
}

UI32 InterfaceCompositionChildLocalManagedObject::getOperationalRule1 () const
{
    return (m_operationalRule1);
}

void InterfaceCompositionChildLocalManagedObject::setOperationalRule1 (const UI32 &operationalRule) 
{
    m_operationalRule1 = operationalRule;
}

}
