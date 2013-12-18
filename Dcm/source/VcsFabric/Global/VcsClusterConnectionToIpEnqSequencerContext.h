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
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/

#ifndef VCSCLUSTERCONNECTIONTOIPENQSEQUENCERCONTEXT_H
#define VCSCLUSTERCONNECTIONTOIPENQSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"

#include <map>

using namespace WaveNs;

namespace DcmNs
{

class VcsClusterConnectionToIpEnqSequencerContext : public PrismLinearSequencerContext
{
    private:
    protected:
    public:
                            VcsClusterConnectionToIpEnqSequencerContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual            ~VcsClusterConnectionToIpEnqSequencerContext  ();

                void        addLocationToMap (const LocationId& lid, const UI32 rbridgeId);
                UI32       getRbridgeIdForLocation (LocationId lid);
	void		addPingResponseToMap (const UI32 rbridgeId, IpAddressPingResponseParameters& pingResponse);
	IpAddressPingResponseParameters&	   getPingResponseForRbridgeId (const UI32 rbridgeId);
	map<UI32, IpAddressPingResponseParameters>&  getPingResponseForAllRbridgeId ();


    // Now the data members
    private:
		bool        m_vcsMode;
		map<LocationId, UI32>m_locationToRbridgeidMap;
		map<UI32, IpAddressPingResponseParameters>m_pingResponseInfoMap;


    protected:
    public:
};

}

#endif // VCSCLUSTERCONNECTIONTOIPENQSEQUENCERCONTEXT_H


