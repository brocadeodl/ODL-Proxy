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

/****************************************************************************
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Pritee Kadu         					                        *
 ***************************************************************************/

#ifndef OMSPECIFICSCHEMACHANGEINFOFORUPGRADE_H
#define OMSPECIFICSCHEMACHANGEINFOFORUPGRADE_H

#include "Framework/ObjectRelationalMapping/ManagedObjectSchemaInfoRepository.h"
#include <vector>
#include <string>


namespace WaveNs
{

class OMSpecificSchemaChangeInfoForUpgrade 
{
	
	private:
	public:
                                OMSpecificSchemaChangeInfoForUpgrade ();
                               ~OMSpecificSchemaChangeInfoForUpgrade ();

                
                string                                              getObjectManagerName            () const;
                void                                                setObjectManagerName            (const string& fieldNamesStrings);
                vector<string>                                      getAddedManagedObjectNames      () const;
                void                                                setAddedManagedObjectNames      (const vector<string>& fieldNamesTypes);
                vector<string>                                      getRemovedManagedObjectNames    () const;
                void                                                setRemovedManagedObjectNames    (const vector<string>& fieldNamesTypes);
                map<string, ModifiedManagedObjectSchemaDifference*>& getModifiedManagedObjectInfo   ();
                void                                                setModifiedManagedObjectInfo    (const map<string, ModifiedManagedObjectSchemaDifference*> &modifiedMoInfo);
                vector<string>                                      getModifiedManagedObjectNames   () const;
                ModifiedManagedObjectSchemaDifference*              getDifferenceSchemaInfoForMo    (const string& managedObjectName) const;
        

	protected:

    // Data Members    
    private:
                string                                              m_name;
                vector<string>                                      m_addedManagedObjectNames;
                vector<string>                                      m_removedManagedObjectNames;
                map<string, ModifiedManagedObjectSchemaDifference*> m_modifiedManagedObjectSchemaInfo;

	public:
	protected:

};

}

#endif
