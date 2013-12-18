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

/**
 *@file VcsConfigSnapshotObject.h 
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file declares the
 *              Config snapshot Object used to store
 *              both meta-data and configuration 
 *         
 *              
 * Author :     Anil ChannaveeraSetty 
 *  
 */

#ifndef VCSCONFIGSNAPSHOTOBJECT_H
#define VCSCONFIGSNAPSHOTOBJECT_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/DateTime.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsConfigSnapshotObject : virtual public SerializableObject
{
    private :
    protected :
        virtual     void            setupAttributesForSerialization         ();

    public :
                                    VcsConfigSnapshotObject                 ();
                                   ~VcsConfigSnapshotObject                 ();

                    ResourceId      saveConfiguration                       (const string &configurationFileName);
                    ResourceId      loadConfiguration                       (const string &configurationFileName);     
                    ResourceId      validateConfiguration                   ();

                    void            setSnapshotId                           ( const string& snapshotId );
                    string          getSnapshotId                           () const;

                    void            setRbridgeId                            (const UI32& rbridgeId );
                    UI32            getRbridgeId                            () const;

                    void            setSWBDModel                            ( const SI32 &SWBDModel);
                    SI32            getSWBDModel                            () const;

                    void            setSnapshotCreatedTime                  ( const DateTime &snapshotCreatedTime );
                    DateTime        getSnapshotCreatedTime                  () const;

                    void            setRestoreOnDisconnectedNode            (const bool &restoreOnDisconnectedNode);
                    bool            getRestoreOnDisconnectedNode            () const;

                    void            setConfigData                           (const string &configData);
                    string          getConfigData                           () const;

                    void            setMetadataOnly                         ( const bool &metadataOnlyObject);
                    bool            getMetadataOnly                         () const;

    // now the data members

    private :
                    UI32            m_rbridgeId;
                    string          m_snapshotId;
                    SI32            m_SWBDModel;
                    DateTime        m_snapshotCreatedTime;
                    bool            m_restoreOnDisconnectedNode;
                    string          m_configData;
                    bool            m_metadataOnlyObject;

    protected :
    public :

};

}

#endif  //VCSCONFIGSNAPSHOTOBJECT_H
