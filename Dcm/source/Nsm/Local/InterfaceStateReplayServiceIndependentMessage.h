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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mayank Maheshwari			                                   *
 ***************************************************************************/

#ifndef INTERFACESTATEREPLAYSERVICEINDEPENDENTMESSAGE_H
#define INTERFACESTATEREPLAYSERVICEINDEPENDENTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class InterfaceStateReplayServiceIndependentMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        			void 	setupAttributesForSerialization ();

    protected :
    public :
                    InterfaceStateReplayServiceIndependentMessage ();
                    InterfaceStateReplayServiceIndependentMessage (const UI32 &event, const UI32 &parameter);
					
    virtual        ~InterfaceStateReplayServiceIndependentMessage ();

                    void setEventId(const UI32 &event);
                    UI32 getEventId()  const;
                    void setParameter(const UI32 &parameter);
                    UI32 getParameter()  const;
                    void setIsPluginEnabled(bool isEnabled);
                    bool isPluginEnabled()  ;
					 // Now the data members
					
	private :
                    UI32     m_eventId;
                    UI32     m_parameter;
                    bool     m_isPluginEnabled;

    protected :
    public :
};

}

#endif // INTERFACESTATEREPLAYSERVICEINDEPENDENTMESSAGE_H


