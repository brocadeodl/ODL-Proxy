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

#ifndef INTERFACECONTAINERINLISTLOCALMANAGEDOBJECT_H
#define INTERFACECONTAINERINLISTLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Interface/ValInterfaceTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceContainerInListLocalManagedObject : public DcmLocalManagedObject
{
    private :
        virtual void setupAttributesForPersistence ();
        virtual void setupAttributesForCreate      ();

    protected :
    public :
                        InterfaceContainerInListLocalManagedObject      (WaveObjectManager *pWaveObjectManager);
        virtual        ~InterfaceContainerInListLocalManagedObject      ();
        virtual UI32    getCase                                         (const UI32 &userChoiceWyserTag);

        static  string  getClassName                     ();

                IpV4Address    getIp () const;
                void           setIp (const IpV4Address &ipAddress);

                SI32           getMtu32 () const;
                void           setMtu32 (const SI32 &mtu32);

                WorldWideName  getWwn () const;
                void           setWwn (const WorldWideName &worldWideName);
                
                UI32           getConfigTypeChoice () const;
                void           setConfigTypeChoice (const UI32 &configTypeChoice);
                
                SI32           getFcFabricId () const;
                void           setFcFabricId (const SI32 &fcFabricId);
                
                SI32           getEthIpAddress () const;
                void           setEthIpAddress (const SI32 &ethIpAddress);

    // Now the Data Members
    private :
        IpV4Address   m_ip;
        SI32          m_mtu32;
        WorldWideName m_wwn;
        UI32          m_configTypeChoice;
        SI32          m_fcFabricId;
        SI32          m_ethIpAddress;


    protected :
    public :
};

}

#endif // INTERFACECONTAINERINLISTLOCALMANAGEDOBJECT_H
