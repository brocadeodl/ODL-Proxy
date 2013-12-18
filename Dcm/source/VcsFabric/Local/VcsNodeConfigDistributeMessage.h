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
 *   Author : Vipool Prajapati, Brian Adaniya                              *
 ***************************************************************************/

#ifndef VCSNODECONFIGDISTRIBUTEMESSAGE_H
#define VCSNODECONFIGDISTRIBUTEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeConfigDistributeMessage : public PrismMessage
{
    private :
                void            setupAttributesForSerialization         ();

    protected :
    public :
                                VcsNodeConfigDistributeMessage          ();
                                VcsNodeConfigDistributeMessage          (const ResourceId& vcsClusterType);
        virtual                ~VcsNodeConfigDistributeMessage          ();

                ResourceId      getConfigDistributeOperation            () const;
                void            setConfigDistributeOperation            (ResourceId configDistributeOperation);

                void            setVcsClusterType                       (const ResourceId& vcsClusterType);
                ResourceId      getVcsClusterType                       () const;

                bool            getPreserveConfig                       () const;
                void            setPreserveConfig                       (bool preserveConfig);

        const   string         &getClusterGlobalOnlyConfigData          () const;
                void            setClusterGlobalOnlyConfigData          (const string &clusterGlobalOnlyConfigData);

                LocationId      getClusterPrimaryLocationId             () const;
                void            setClusterPrimaryLocationId             (LocationId clusterPrimaryLocationId);

                bool            getForce                                () const;
                void            setForce                                (bool force);

                bool            getIsDefaultConfigDetected              () const;
                void            setIsDefaultConfigDetected              (bool isDefaultConfigDetected);

        const   LocationRole    getLocationRole                         () const;
                void            setLocationRole                         (const LocationRole locationRole);
    
    // Now the data members

    private :
                ResourceId      m_configDistributeOperation;
                ResourceId      m_vcsClusterType;
                bool            m_preserveConfig;
                string          m_clusterGlobalOnlyConfigData;
                LocationId      m_clusterPrimaryLocationId;
                bool            m_force;
                bool            m_isDefaultConfigDetected;
                LocationRole    m_locationRole;

    protected :
    public :
};

}

#endif // VCSNODECONFIGDISTRIBUTEMESSAGE_H
