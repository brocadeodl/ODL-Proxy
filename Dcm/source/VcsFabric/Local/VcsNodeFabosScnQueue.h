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

#ifndef VCSNODEFABOSSCNQUEUEOBJECT_H
#define VCSNODEFABOSSCNQUEUEOBJECT_H

#include "sysmod/syscomm.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
#include "VcsNodeFabosInterfaceObjectManager.h"
#include <deque>

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeFabosScnQueueObject
{
    private :
    protected :

    public :
				VcsNodeFabosScnQueueObject ();
				~VcsNodeFabosScnQueueObject ();

        UI32    getDomainId               () const;
		UI32	getPrincipalDomainId	() const;
		UI32	getScnObjectId () const;
		UI32	getScnType		     () const;
		
		void	setPrincipalDomainId (const UI32 &principalDomainId);
        void    setDomainId               (const UI32 &domainId);
		void	setScnType               (const UI32 &scnType);
		void	setScnObjectId (const UI32 &scnObjectId);

    // Now the data members

    private :
    	UI32   m_scnObjectId;
        UI32   m_domainId;
		UI32   m_principalDomainId;
		UI32   m_scnType;

    protected :
    public :
		friend class VcsNodeFabosInterfaceObjectManager;
};
}

#endif // VCSNODEFABOSSCNQUEUEOBJECT_H


