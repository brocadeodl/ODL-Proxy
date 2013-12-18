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
*   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : cgangwar                                                     *
***************************************************************************/

#include "AliasConfig/Global/WyserCliCommandAliasManagedObject.h"
#include "vcs.h"


namespace DcmNs {

WyserCliCommandAliasManagedObject::WyserCliCommandAliasManagedObject (WaveObjectManager *pWaveObjectManager)
    :PrismElement              (pWaveObjectManager),
     PrismPersistableObject    (WyserCliCommandAliasManagedObject::getClassName (), DcmManagedObject::getClassName ()),
     WaveManagedObject         (pWaveObjectManager),
     DcmManagedObject          (pWaveObjectManager)
{
}

WyserCliCommandAliasManagedObject::~WyserCliCommandAliasManagedObject ()
{
}

string WyserCliCommandAliasManagedObject::getClassName ()
{
    return ("WyserCliCommandAliasManagedObject");
}

void WyserCliCommandAliasManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
         
    addPersistableAttribute (new AttributeString (&m_aliasname, "aliasname"));
    addPersistableAttribute (new AttributeString (&m_aliasexpansion, "aliasexpansion"));
}


void WyserCliCommandAliasManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
         
    addPersistableAttributeForCreate (new AttributeString (&m_aliasname, "aliasname"));
    addPersistableAttributeForCreate (new AttributeString (&m_aliasexpansion, "aliasexpansion"));

    vector<string > keyName;
    keyName.push_back ("aliasname");
    setUserDefinedKeyCombination (keyName);
}



string WyserCliCommandAliasManagedObject::getaliasname () const
{
    return (m_aliasname);
}
 
void WyserCliCommandAliasManagedObject::setaliasname (string aliasname)
{
    m_aliasname = aliasname;
}


string  WyserCliCommandAliasManagedObject::getaliasexpansion () const
{
    return (m_aliasexpansion);
}
 
void WyserCliCommandAliasManagedObject::setaliasexpansion (string aliasexpansion)
{
    m_aliasexpansion = aliasexpansion;
}

WyserEaCliCommandGlobalAliasManagedObject::WyserEaCliCommandGlobalAliasManagedObject (WaveObjectManager *pWaveObjectManager)
    :PrismElement              (pWaveObjectManager),
     PrismPersistableObject    (WyserEaCliCommandGlobalAliasManagedObject::getClassName (), DcmManagedObject::getClassName ()),
     WaveManagedObject         (pWaveObjectManager),
     DcmManagedObject          (pWaveObjectManager)
{
}
 
WyserEaCliCommandGlobalAliasManagedObject::~WyserEaCliCommandGlobalAliasManagedObject ()
{
}
 
string WyserEaCliCommandGlobalAliasManagedObject::getClassName ()
{
    return ("WyserEaCliCommandGlobalAliasManagedObject");
}
 
void WyserEaCliCommandGlobalAliasManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
         
    addPersistableAttribute (new AttributeString (&m_aliasname, "aliasname"));
    addPersistableAttribute (new AttributeString (&m_aliasexpansion, "aliasexpansion"));
}
 
 
void WyserEaCliCommandGlobalAliasManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
         
    addPersistableAttributeForCreate (new AttributeString (&m_aliasname, "aliasname"));
    addPersistableAttributeForCreate (new AttributeString (&m_aliasexpansion, "aliasexpansion"));
 
    vector<string > keyName;
    keyName.push_back ("aliasname");
    setUserDefinedKeyCombination (keyName);
}
 
 
 
string WyserEaCliCommandGlobalAliasManagedObject::getaliasname () const
{
    return (m_aliasname);
}
 
void WyserEaCliCommandGlobalAliasManagedObject::setaliasname (string aliasname)
{
    m_aliasname = aliasname;
}
 
 
string  WyserEaCliCommandGlobalAliasManagedObject::getaliasexpansion () const
{
    return (m_aliasexpansion);
}

void WyserEaCliCommandGlobalAliasManagedObject::setaliasexpansion (string aliasexpansion)
{
    m_aliasexpansion = aliasexpansion;
}

}
