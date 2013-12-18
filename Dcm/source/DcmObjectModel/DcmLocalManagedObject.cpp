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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "DcmObjectModel/DcmLocalManagedObject.h"

namespace DcmNs
{

DcmLocalManagedObject::DcmLocalManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement               (pWaveObjectManager),
      PrismPersistableObject     (DcmLocalManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject          (pWaveObjectManager),
      DcmManagedObject           (pWaveObjectManager),
      WaveLocalManagedObjectBase (this),
      DcmLocalManagedObjectBase  (this)
{
}

DcmLocalManagedObject::~DcmLocalManagedObject ()
{
}

void DcmLocalManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    DcmLocalManagedObjectBase::setupAttributesForPersistence ();
}

void DcmLocalManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    DcmLocalManagedObjectBase::setupAttributesForCreate ();
}

string DcmLocalManagedObject::getClassName ()
{
    return ("DcmLocalManagedObject");
}

}
