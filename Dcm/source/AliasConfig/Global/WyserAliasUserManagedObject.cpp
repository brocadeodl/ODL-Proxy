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

#include "AliasConfig/Global/WyserAliasUserManagedObject.h"
#include "vcs.h"

#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include <string>

namespace DcmNs {

WyserAliasUserManagedObject::WyserAliasUserManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement            (pWaveObjectManager),
    PrismPersistableObject    (WyserAliasUserManagedObject::getClassName (), DcmManagedObject::getClassName ()),
    WaveManagedObject         (pWaveObjectManager),
    DcmManagedObject          (pWaveObjectManager),
    m_completeonspace         (false),
    m_ignoreleadingspace      (false),
    m_idletimeout             (600),
    m_paginate                (true),
    m_history                 (100),
    m_autowizard              (false),
    m_showdefaults            (false),
    m_prompt1                 ("\\H\\M# "),
    m_prompt2                 ("\\H\\M# ")      
{
}

WyserAliasUserManagedObject::~WyserAliasUserManagedObject ()
{
}

string WyserAliasUserManagedObject::getClassName ()
{
    return ("WyserAliasUserManagedObject");
}

void WyserAliasUserManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();
     
    addPersistableAttribute (new AttributeString (&m_username, "username"));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<WyserCliCommandAliasManagedObject>(&m_useralias,"useralias", getClassName (), getObjectId (), WyserCliCommandAliasManagedObject::getClassName ()));
    addPersistableAttribute (new AttributeString (&m_description, "description"));
    addPersistableAttribute (new AttributeBool (&m_completeonspace, "completeonspace"));
    addPersistableAttribute (new AttributeBool (&m_ignoreleadingspace, "ignoreleadingspace"));
    addPersistableAttribute (new AttributeUI64 (&m_idletimeout, "idletimeout"));
    addPersistableAttribute (new AttributeBool (&m_paginate, "paginate"));
    addPersistableAttribute (new AttributeUI64 (&m_history, "history"));
    addPersistableAttribute (new AttributeBool (&m_autowizard, "autowizard"));
    addPersistableAttribute (new AttributeBool (&m_showdefaults, "showdefaults"));
    addPersistableAttribute (new AttributeUI64 (&m_displaylevel, "displaylevel"));
    addPersistableAttribute (new AttributeString (&m_prompt1, "prompt1"));
    addPersistableAttribute (new AttributeString (&m_prompt2, "prompt2"));
}

void WyserAliasUserManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();
     
    addPersistableAttributeForCreate (new AttributeString (&m_username, "username"));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<WyserCliCommandAliasManagedObject> (&m_useralias,"useralias", getClassName (), getObjectId (), WyserCliCommandAliasManagedObject::getClassName ()));
    addPersistableAttributeForCreate (new AttributeString (&m_description, "description"));
    addPersistableAttributeForCreate (new AttributeBool (&m_completeonspace, "completeonspace"));
    addPersistableAttributeForCreate (new AttributeBool (&m_ignoreleadingspace, "ignoreleadingspace"));
    addPersistableAttributeForCreate (new AttributeUI64 (&m_idletimeout, "idletimeout"));    
    addPersistableAttributeForCreate (new AttributeBool (&m_paginate, "paginate"));
    addPersistableAttributeForCreate (new AttributeUI64 (&m_history, "history"));
    addPersistableAttributeForCreate (new AttributeBool (&m_autowizard, "autowizard"));
    addPersistableAttributeForCreate (new AttributeBool (&m_showdefaults, "showdefaults"));
    addPersistableAttributeForCreate (new AttributeUI64 (&m_displaylevel, "displaylevel"));
    addPersistableAttributeForCreate (new AttributeString (&m_prompt1, "prompt1"));
    addPersistableAttributeForCreate (new AttributeString (&m_prompt2, "prompt2"));

    vector<string > keyName;
    keyName.push_back ("username");
    setUserDefinedKeyCombination (keyName);
}

string WyserAliasUserManagedObject::getusername () const
{
    return (m_username);
}
 
void WyserAliasUserManagedObject::setusername (string username)
{
    m_username = username;
}

void  WyserAliasUserManagedObject::setuseralias(const vector<WaveManagedObjectPointer<WyserCliCommandAliasManagedObject> > &useralias)
{
     m_useralias  =  useralias;
}
 
vector<WaveManagedObjectPointer<WyserCliCommandAliasManagedObject> >  WyserAliasUserManagedObject::getuseralias() const
{
     return (m_useralias);
}


}
