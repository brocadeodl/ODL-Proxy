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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh			                                   *
 ***************************************************************************/

#ifndef CLUSTERLOCALSLOTREMOVEMESSAGE_H
#define CLUSTERLOCALSLOTREMOVEMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Cluster/Local/ClusterLocalTypes.h"

using namespace WaveNs;

namespace WaveNs
{

class ClusterLocalSlotRemoveMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    ClusterLocalSlotRemoveMessage ();
					ClusterLocalSlotRemoveMessage (const UI32 &slotNumber);
                    ClusterLocalSlotRemoveMessage (const UI32 &slotNumber, const UI32 &hwType);
                    ClusterLocalSlotRemoveMessage (const UI32 &slotNumber, const UI32 &hwType, const LocationId &locationId);
    virtual        ~ClusterLocalSlotRemoveMessage ();
					UI32 getSlotNumber ();
					void setSlotNumber (const UI32 &slotNumber);
					UI32 getHwType ();
					void setHwType (const UI32 &hwType);
					LocationId getLocationId ();
					void setLocationId (const LocationId &locationId);
	private:


    // Now the data members


    private :
		UI32 m_slotNumber;
		UI32 m_hwType;
		LocationId m_locationId;

    protected :
    public :
};

}

#endif // CLUSTERLOCALSLOTREMOVEMESSAGE_H


