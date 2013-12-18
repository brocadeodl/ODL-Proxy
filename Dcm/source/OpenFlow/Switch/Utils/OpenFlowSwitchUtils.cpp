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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Utils/OpenFlowSwitchUtils.h"
#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

using namespace WaveNs;

namespace OpenFlowNs
{

OpenFlowLogicalSwitchManagedObject *OpenFlowSwitchUtils::getOpenFlowLogicalSwitchManagedObjectByName (const string &openFlowLogicalSwitchManagedObjectName)
{
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (OpenFlowLogicalSwitchManagedObject::getClassName ());

    synchronousQueryContext.addAndAttribute (new AttributeString (openFlowLogicalSwitchManagedObjectName, "openFlowLogicalSwitchName"));

    vector<WaveManagedObject *> *pQueryResults = WaveObjectManagerToolKit::querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pQueryResults, __FILE__, __LINE__);

    UI32 numberOfQueryResults = pQueryResults->size ();

    prismAssert (1 >= numberOfQueryResults, __FILE__, __LINE__);

    OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject = NULL;

    if (1 == numberOfQueryResults)
    {
        pOpenFlowLogicalSwitchManagedObject = dynamic_cast<OpenFlowLogicalSwitchManagedObject *> ((*pQueryResults)[0]);

        prismAssert (NULL != pOpenFlowLogicalSwitchManagedObject, __FILE__, __LINE__);

        delete pQueryResults;
    }

    return (pOpenFlowLogicalSwitchManagedObject);
}

}
