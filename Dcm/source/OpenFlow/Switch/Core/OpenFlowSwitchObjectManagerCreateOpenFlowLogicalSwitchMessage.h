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

#ifndef OPENFLOWSWITCHOBJECTMANAGERCREATEOPENFLOWLOGICALSWITCHMESSAGE_H
#define OPENFLOWSWITCHOBJECTMANAGERCREATEOPENFLOWLOGICALSWITCHMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/Uri.h"

using namespace WaveNs;

namespace OpenFlowNs
{

class OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage ();
        virtual        ~OpenFlowSwitchObjectManagerCreateOpenFlowLogicalSwitchMessage ();

                string  getOpenFlowLogicalSwitchName                                  () const;
                void    setOpenFlowLogicalSwitchName                                  (const string &openFlowLogicalSwitchName);

                Uri     getOpenFlowLogicalSwitchUri                                   () const;
                void    setOpenFlowLogicalSwitchUri                                   (const Uri &openFlowLogicalSwitchUri);


    // Now the Data Members

    private :
        string m_openFlowLogicalSwitchName;
        Uri    m_openFlowLogicalSwitchUri;

    protected :
    public :
};

}

#endif // OPENFLOWSWITCHOBJECTMANAGERCREATEOPENFLOWLOGICALSWITCHMESSAGE_H
