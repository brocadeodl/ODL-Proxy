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
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#ifndef VCSFABRICGLOBALMESSAGES
#define VCSFABRICGLOBALMESSAGES

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabConfigSnapshotMessage : public PrismMessage
{
    private :
                void        setupAttributesForSerialization     ();
                
    protected :
    public :
                            VcsFabConfigSnapshotMessage         (const UI32 &opcode);
                           ~VcsFabConfigSnapshotMessage         ();
                           
                void        setSnapshotId                       ( const string& snapshotId );
                string      getSnapshotId                       () const;
                
                void        setRbridgeId                        ( const UI32& rbridgeId );
                UI32        getRbridgeId                        () const;
    
                void        setSnapshotCreatedTime              ( const DateTime &snapshotCreatedTime );
                DateTime    getSnapshotCreatedTime              () const;

                void        setLocalConfigData                  ( const string &localConfigData );     
                string      getLocalConfigData                  () const;

    // Now the data members
    
    private :
                string      m_snapshotId;
                UI32        m_rbridgeId;
                DateTime    m_snapshotCreatedTime;
                string      m_localConfigData;
        
    protected :
    public :
};
}

#endif // VCSFABRICGLOBALMESSAGES

