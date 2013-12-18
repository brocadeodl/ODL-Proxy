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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VCSCLUSTERCONFIGURATION_H
#define VCSCLUSTERCONFIGURATION_H

#include "Framework/ObjectModel/SerializableObject.h"
#include "Framework/Types/Types.h"
#include <string>

namespace WaveNs
{

class VcsClusterConfiguration : virtual public SerializableObject
{
    private :
    protected :
        virtual void setupAttributesForSerialization                        ();
        virtual void setupAttributesForSerializationInAttributeOrderFormat  ();

    public :
                            VcsClusterConfiguration    ();
                           ~VcsClusterConfiguration    ();

        UI32                getVersion                 () const;
        void                setVersion                 (const UI32 &version);
        bool                getVcsMode                 () const;
        void                setVcsMode                 (const bool &vcsMode);
        UI32                getVcsId                   () const;
        void                setVcsId                   (const UI32 &vcsId);
        UI32                getRbridgeId               () const;
        void                setRbridgeId               (const UI32 &rbridgeId);
        UI32                getClusterType             () const;
        void                setClusterType             (const UI32 &clusterType);
        SI32                getClusterConf             () const;
        void                setClusterConf             (const SI32 &clusterConf);

        void                display                    () const;

    // now the data members

    private :
        UI32        m_version;
        bool        m_vcsMode;
        UI32        m_vcsId;
        UI32        m_rbridgeId;
        UI32        m_clusterType;
        SI32        m_clusterConf;

    protected :
    public :

    friend class PrismFrameworkObjectManagerHaSyncWorker;
};

}

#endif // VCSCLUSTERCONFIGURATION_H
