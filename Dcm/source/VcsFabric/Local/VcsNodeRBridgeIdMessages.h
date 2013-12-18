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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef VCSNODERBRIDGEIDMESSAGES_H
#define VCSNODERBRIDGEIDMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetRBridgeIdMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        //PhysicalInterfaceCreateMessage ();
                        VcsNodeSetRBridgeIdMessage();
                        VcsNodeSetRBridgeIdMessage (const UI32 &rbridgeId);
        virtual        ~VcsNodeSetRBridgeIdMessage ();

                UI32    getRBridgeId                     () const;

    // Now the data members

    private :
        UI32 m_rbridgeId;

    protected :
    public :
};

class VcsNodeGetRBridgeIdMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetRBridgeIdMessage ();
        virtual        ~VcsNodeGetRBridgeIdMessage ();

                UI32    getRBridgeId               () const;
                void    setRBridgeId               (const UI32 &rbridgeId);

    // Now the data members

    private :
        UI32 m_rbridgeId;

    protected :
    public :
};

class VcsNodeGetRBridgeIdFromSwitchWWNMessage : public ManagementInterfaceMessage
{
    private :
                void            setupAttributesForSerialization         ();

    protected :
    public :
                                VcsNodeGetRBridgeIdFromSwitchWWNMessage ();
        virtual                ~VcsNodeGetRBridgeIdFromSwitchWWNMessage ();

                UI32            getRBridgeId                            () const;
                void            setRBridgeId                            (const UI32 rBridgeId);
                WorldWideName   getSwitchWWN                            () const;
                void            setSwitchWWN                            (const WorldWideName &switchWWN);
        
    // Now the data members
    private :
                UI32            m_rBridgeId;
                WorldWideName   m_switchWWN;
    
    protected :
    public :
};

}

#endif // VCSNODERBRIDGEIDMESSAGES_H
