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
 *   Author : Pradeep Yelamanti			                                   *
 ***************************************************************************/

#ifndef VCSNODESETMODECHANGEPARAMETERSMESSAGE_H
#define VCSNODESETMODECHANGEPARAMETERSMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetModeChangeParametersMessage : public ManagementInterfaceMessage
{
    private :
        			void 	setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSetModeChangeParametersMessage ();
                    VcsNodeSetModeChangeParametersMessage (const UI32 &vcsId, const string &vcsMode, const UI32 &rbridgeId, const bool &rebootSw);
					
    virtual        ~VcsNodeSetModeChangeParametersMessage ();

    UI32            getVcsId            () const;
    void            setVcsId            (const UI32 &vcsId);

    string          getVcsMode          () const;
    void            setVcsMode          (const string &vcsMode);
					
    UI32            getRbridgeId        () const;
    void            setRbridgeId        (const UI32 &rbridgeId);

    bool            getRebootSw         () const;
    void            setRebootSw         (const bool &rebootSw);

    // Now the data members
					
	private :
                UI32   m_vcsId;
                string m_vcsMode;
                UI32   m_rbridgeId;
                bool   m_rebootSw;
    protected :
    public :
};

}

#endif // VCSNODESETMODECHANGEPARAMETERSMESSAGE_H


