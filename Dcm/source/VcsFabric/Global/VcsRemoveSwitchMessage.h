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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VCSREMOVESWITCHMESSAGE_H
#define VCSREMOVESWITCHMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsRemoveSwitchMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsRemoveSwitchMessage          ();
                    VcsRemoveSwitchMessage          (const vector<UI32> &switchMappedId);
					vector<UI32> getMappedId        ();
					void setMappedId                (const vector<UI32> &switchMappedId);
                    
                    void setIsDisconnected          (bool isDisconnected);
                    bool getIsDisconnected          ();
                    

    virtual        ~VcsRemoveSwitchMessage ();


    // Now the data members

    private :
       vector<UI32> m_switchMappedId;
       bool         m_isDisconnected; 

    protected :
    public :
};

}

#endif // VCSREMOVESWITCHMESSAGE_H


