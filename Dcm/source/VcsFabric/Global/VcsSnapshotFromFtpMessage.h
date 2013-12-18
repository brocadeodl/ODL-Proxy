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
*   Author : Vcs Infra Team                                               *
***************************************************************************/

#ifndef VCSSNAPSHOTFTPMESSAGES
#define VCSSNAPSHOTFTPMESSAGES

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsCopySnapshotFromFtpMessage : public ManagementInterfaceMessage
{
     private :
                void        setupAttributesForSerialization     ();
        
    protected :
    public :
                            VcsCopySnapshotFromFtpMessage       ();
                            VcsCopySnapshotFromFtpMessage       (const string &snapshotId, const UI32 &rbridgeId, const string &localFilename, const string &globalFilename);
        virtual            ~VcsCopySnapshotFromFtpMessage       ();

                void        setSnapshotId                       (const string& snapshotId);
                string      getSnapshotId                       () const;
                
                void        setRbridgeId                        (const UI32& rbridgeId);
                UI32        getRbridgeId                        () const;
    
                void        setLocalConfigFilename              (const string &localFilename);
                string      getLocalConfigFilename              () const;

                void        setGlobalConfigFilename             (const string &globalFilename);
                string      getGlobalConfigFilename             () const;

                void        setSnapshotCreatedTime              (const DateTime &snapshotCreatedTime);
                DateTime    getSnapshotCreatedTime              () const;

     // Now the data members
                
    private :   
                string      m_snapshotId;
                UI32        m_rbridgeId;
                string      m_localFilename;
                string      m_globalFilename;
                DateTime    m_snapshotCreatedTime;              
                
    protected :
    public :
};  
    
}               
                
#endif // VCSSNAPSHOTFTPMESSAGES
