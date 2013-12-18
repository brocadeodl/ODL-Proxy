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
 *   Author : Cindy Hou     			                                   *
 ***************************************************************************/

#ifndef CLUSTERLOCALSLOTREMOVEALLMESSAGE_H
#define CLUSTERLOCALSLOTREMOVEALLMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"

using namespace WaveNs;

namespace WaveNs
{

class ClusterLocalSlotRemoveAllMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    ClusterLocalSlotRemoveAllMessage ();
                    ClusterLocalSlotRemoveAllMessage (const LocationId &locationId);
                    ClusterLocalSlotRemoveAllMessage (const LocationId &locationId, const UI32 &hwType);
    virtual        ~ClusterLocalSlotRemoveAllMessage ();
					LocationId getLocationId ();
					void setLocationId (const LocationId &locationId);
                    UI32 getHwType ();
                    void setHwType (const UI32 &hwType);

	private:


    // Now the data members


    private :
		LocationId m_locationId;
        UI32 m_hwType;

    protected :
    public :
};

}

#endif // CLUSTERLOCALSLOTREMOVEALLMESSAGE_H


