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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PHYSICALINTERFACEMESSAGES_H
#define PHYSICALINTERFACEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class PhysicalInterfaceCreateMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PhysicalInterfaceCreateMessage ();
                        PhysicalInterfaceCreateMessage (const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
        virtual        ~PhysicalInterfaceCreateMessage ();

                ObjectId  getInterfaceObjectId         () const;
                string  getName                        () const;
                UI32    getIfIndex                     () const;
                UI32    getMtu                         () const;
                string  getDescription                 () const;

    // Now the data members

    private :
        ObjectId m_interfaceObjectId;        
        string m_name;
        UI32   m_ifIndex;
        UI32   m_mtu;
        string m_description;
        

    protected :
    public :
};

class PhysicalInterfaceUpdateMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        PhysicalInterfaceUpdateMessage ();
                        PhysicalInterfaceUpdateMessage (const ObjectId &interfaceObjectId,const string &name, const UI32 &ifIndex, const UI32 &mtu, const string &description);
        virtual        ~PhysicalInterfaceUpdateMessage ();

                ObjectId  getInterfaceObjectId         () const;
                string  getName                        () const;
                UI32    getIfIndex                     () const;
                UI32    getMtu                         () const;
                UI32    getCmdCode                     () const;
                void    setCmdCode                     (const UI32 &mtu);
                string  getDescription                 () const;

    // Now the data members

    private :
        ObjectId m_interfaceObjectId;        
        string m_name;
        UI32   m_ifIndex;
        UI32   m_mtu;
        string m_description;
        UI32   m_cmdCode;
    protected :
    public :
};

}

#endif // PHYSICALINTERFACEMESSAGES_H
