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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu			                                           *
 ***************************************************************************/

#ifndef VCSNODESETCLUSTERCONFIGURATIONMESSAGE_H  
#define VCSNODESETCLUSTERCONFIGURATIONMESSAGE_H

#include "VcsFabric/Local/VcsNodeClusterConfigurationChangeMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetClusterConfigurationMessage : public VcsNodeClusterConfigurationChangeMessage
{
    private :
    void            setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSetClusterConfigurationMessage           ();
                    VcsNodeSetClusterConfigurationMessage           (const UI32 &vcsId, const UI32 &vcsMode, const UI32 &rbridgeId, const ResourceId &clusterType);
    void            setCommitTransactionCompleted                   (const bool& completed);
    const bool      getCommitTransactionCompleted                   () const;
					
    virtual        ~VcsNodeSetClusterConfigurationMessage           ();

    // Now the data members
					
	private :
        bool        m_isCommitTransactionCompleted;

    protected :
    public :
};

}

#endif // VCSNODESETCLUSTERCONFIGURATIONMESSAGE_H

