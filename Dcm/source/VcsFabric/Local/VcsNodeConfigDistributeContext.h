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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef VCSNODECONFIGDISTRIBUTECONTEXT_H
#define VCSNODECONFIGDISTRIBUTECONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeConfigDistributeContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                    VcsNodeConfigDistributeContext  (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                    ~VcsNodeConfigDistributeContext  ();

        const   ResourceId          getVcsClusterType               () const;
                void                setVcsClusterType               (const ResourceId vcsClusterType);

        const   ResourceId          getContextFilteredConfigType    () const;
                void                setContextFilteredConfigType    (const DcmConfigFilterContextTypes filteredConfigContextType);

        const   string             &getContextFilteredConfigData    () const;
                void                setContextFilteredConfigData    (const string &filteredConfigContextData);         

        const   bool                getPreserveConfig               () const;
                void                setPreserveConfig               (const bool preserveConfig);

        const   bool                getForce                        () const;
                void                setForce                        (const bool force);

        const   bool                getIsDefaultConfigDetected      () const;
                void                setIsDefaultConfigDetected      (const bool isDefaultConfigDetected);

        const   LocationRole        getLocationRole                 () const;
                void                setLocationRole                 (const LocationRole locationRole);

    // Now the data members

    private :
                ResourceId                      m_vcsClusterType;
                DcmConfigFilterContextTypes     m_contextFilteredConfigType;
                string                          m_contextFilteredConfigData;
                bool                            m_preserveConfig;
                bool                            m_isDefaultConfigDetected;
                LocationRole                    m_locationRole;
                bool                            m_force;
    protected :
    public :
};

}

#endif // VCSNODECONFIGDISTRIBUTECONTEXT_H
