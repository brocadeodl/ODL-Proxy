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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh			                                   *
 ***************************************************************************/

#ifndef VCSREJOINSWITCHMESSAGE
#define VCSREJOINSWITCHMESSAGE

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsRejoinSwitchMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsRejoinSwitchMessage			();
                    VcsRejoinSwitchMessage			(const vector<UI32> &domainId);
					VcsRejoinSwitchMessage			(const string &ipAddress, const UI32 portId);

		vector<UI32> getDomainIds					();
		void		setDomainIds					(const vector<UI32> &domainId);
		UI32		getPortId						();
		void		setPortId						(const UI32 &portId);
		string		getIpAddress					();
		void		setIpAddress					(const string &ipAddress);



    virtual        ~VcsRejoinSwitchMessage	();


    // Now the data members

    private :
        vector<UI32>	m_domainIds;
		string			m_ipAddress;
		UI32			m_portId;

    protected :
    public :
};

}

#endif // VCSREJOINSWITCHMESSAGE


