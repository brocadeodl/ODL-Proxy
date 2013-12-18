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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#ifndef VCSCLUSTERMANAGEDOBJECT_H
#define VCSCLUSTERMANAGEDOBJECT_H

#include <time.h>
#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Types/Uuid.h"
#include "Framework/Attributes/AttributeUUID.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabObjectManager;

class VcsClusterManagedObject : public DcmManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                          VcsClusterManagedObject (VcsFabObjectManager *pVcsFabObjectManager);
                          VcsClusterManagedObject (VcsFabObjectManager *pVcsFabObjectManager, const UI32 &vcsClusterId);
        virtual          ~VcsClusterManagedObject ();
        static            string getClassName              ();
		void              addSwitchObjectId                (const ObjectId &switchObjectId);
		UI32              removeSwitchObjectId             (const ObjectId switchObjectId);
        vector<ObjectId >  getSwitchObjectIds             () const;

 		UI32              getVcsClusterId                  ();
        void              setVcsClusterId                  (const UI32 vcsClusterId);
		string            getClusterName                   ();
		void              setClusterName                   (const string clusterName);

		Uuid              getClusterGUID				   ();
		void			  setClusterGUID				   (const Uuid &clusterGUID);
		void			  clearClusterGUID				   ();
	
		ObjectId          getPrincipalSwitchObjectId       ();
		void			  setPrincipalSwitchObjectId       (const ObjectId switchObjectId);
		UI32              getPrincipalSwitchDomainId       ();
		void              setPrincipalSwitchDomainId       (UI32 mappedId);
		UI32              getSwitchCount                   ();	
		bool                isObjectIdInClusterMO			(const ObjectId switchObjectId);
        string              getClusterFormationTime        ();

    private:
        void                setClusterFormationTime         ();

    // Now the data members

    private :
        vector<ObjectId> m_switchObjectIds;
		UI32             m_vcsClusterId;
		string           m_licenseString;
		ObjectId         m_principalSwitchObjectId;
		UI32             m_principalSwitchDomainId;
		string           m_clusterFormationTimeStamp;
		Uuid	         m_clusterGUID;

	protected :
    public :
};
}

#endif // VCSCLUSTERMANAGEDOBJECT_H
