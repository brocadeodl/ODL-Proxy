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

#include "OSPF/Local/OSPFPermitRedistributeLocalManagedObject.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFPermitRedistributeLocalManagedObject::OSPFPermitRedistributeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFPermitRedistributeLocalManagedObject::getClassName (), OSPFPermitDenyRedistributeLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
		DcmLocalManagedObject (pOSPFLocalObjectManager),
		OSPFPermitDenyRedistributeLocalManagedObject (pOSPFLocalObjectManager)
    {
    }

    OSPFPermitRedistributeLocalManagedObject::~OSPFPermitRedistributeLocalManagedObject ()
    {
    }

    string  OSPFPermitRedistributeLocalManagedObject::getClassName()
    {
        return ("OSPFPermitRedistributeLocalManagedObject");
    }

    void  OSPFPermitRedistributeLocalManagedObject::setupAttributesForCreate()
    {
    	//To fix the DB Conversion failure
    }
}
