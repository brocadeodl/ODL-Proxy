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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#ifndef VCSADDSWITCHMESSAGE
#define VCSADDSWITCHMESSAGE

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabricLocalSwitchInfoObject;

class VcsAddSwitchMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                            							VcsAddSwitchMessage				();
                                                        VcsAddSwitchMessage             (const UI32 senderId);

            void                                        setSenderId                     (const UI32 &senderId);
            UI32                                        getSenderId                     ();

            void                                        addToFailedDomainIdVector       (const UI32 &domainId);
            vector<UI32>                                getFailedDomainIdVector         () const;    

            void                                        setRemoteSwitchInfoObjects      (const vector<VcsFabricLocalSwitchInfoObject> &infoObjects);
            vector<VcsFabricLocalSwitchInfoObject>      getRemoteSwitchInfoObjects      () const;

    virtual                                            ~VcsAddSwitchMessage		();


    // Now the data members

    private :
        UI32                                    m_senderId;
		vector<UI32>                            m_failedDomainIds;
        vector<VcsFabricLocalSwitchInfoObject>  m_vcsFabricLocalSwitchInfoObjects;
    protected :
    public :
};

}

#endif // VCSADDSWITCHMESSAGE


