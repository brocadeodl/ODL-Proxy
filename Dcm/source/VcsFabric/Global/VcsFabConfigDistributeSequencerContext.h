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
 *@file VcsFabConfigDistributeSequencerContext.h
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file declares the
 * LinearSequencerContext.
 *
 * Author :     Brian Adaniya
 * Date :       07/13/2012
 */

#ifndef VCSFABCONFIGDISTRIBUTESEQUENCERCONTEXT_H
#define VCSFABCONFIGDISTRIBUTESEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabConfigDistributeSequencerContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                            VcsFabConfigDistributeSequencerContext  (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                            ~VcsFabConfigDistributeSequencerContext  ();

        const   vector<LocationId>         &getLocationIdVector                     () const;
                void                        setLocationIdVector                     (const vector<LocationId> &locationIds);

                bool                        getForce                                () const;
                void                        setForce                                (const bool force);

                ResourceId                  getVcsClusterType                       () const;
                void                        setVcsClusterType                       (ResourceId vcsClusterType);

                bool                        getPreserveConfig                       () const;
                void                        setPreserveConfig                       (const bool preserveConfig);

                LocationId                  getClusterPrimaryLocationId             () const;
                void                        setClusterPrimaryLocationId             (const LocationId clusterPrimaryLocationId); 

        const   string                     &getClusterGlobalOnlyConfigData          () const;             
                void                        setClusterGlobalOnlyConfigData          (const string &clusterGlobalOnlyConfigData);

                bool                        getIsDefaultConfigDetected              () const;
                void                        setIsDefaultConfigDetected              (const bool isDefaultConfigDetected);

                LocationRole                getLocationRole                         () const;
                void                        setLocationRole                         (const LocationRole locationRole);

    // Now the data members
    private :
                vector<LocationId>          m_locationIds;
                ResourceId                  m_vcsClusterType;
                bool                        m_preserveConfig;
                LocationId                  m_clusterPrimaryLocationId;
                bool                        m_force;
                string                      m_clusterGlobalOnlyConfigData;
                bool                        m_isDefaultConfigDetected;
                LocationRole                m_locationRole;

    protected :
    public :
};

}

#endif //VCSFABCONFIGDISTRIBUTESEQUENCERCONTEXT_H
