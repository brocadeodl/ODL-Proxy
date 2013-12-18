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

#ifndef VCSNODESLOTMANAGEMENTINTERFACEMESSAGES_H
#define VCSNODESLOTMANAGEMENTINTERFACEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "sys/fabos/fabctrl.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSlotRemoveMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSlotRemoveMessage ();
					VcsNodeSlotRemoveMessage (const UI32 &slotNumber, const UI32 & rbridgeId);
                    VcsNodeSlotRemoveMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwType);
					VcsNodeSlotRemoveMessage (const string &objectManagerName, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwType);
    virtual        ~VcsNodeSlotRemoveMessage ();
					UI32 getSlotNumber ();
					void setSlotNumber (const UI32 &slotNumber);
					UI32 getRbridgeId ();
					void setRbridgeId (const UI32 &rbridgeId);
					UI32 getHwType ();
					void setHwType (const UI32 &hwType);
					ResourceId getReturnStatus ();
					void setReturnStatus (const ResourceId &returnStatus);
	private:


    // Now the data members


    private :
		UI32       m_slotNumber;
		UI32       m_rbridgeId;
		UI32       m_hwType;
        ResourceId m_returnStatus;

    protected :
    public :
};

class VcsNodeSlotTypeQueryMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSlotTypeQueryMessage ();
                    VcsNodeSlotTypeQueryMessage (const UI32 &slotNumber);
                    VcsNodeSlotTypeQueryMessage (const UI32 &slotNumber, const UI32 &hwType);
    virtual        ~VcsNodeSlotTypeQueryMessage ();
                    UI32 getSlotNumber ();
                    void setSlotNumber (const UI32 &slotNumber);
                    UI32 getHwType ();
                    void setHwType (const UI32 &hwType);
                    UI32 getSlotType ();
                    void setSlotType (const UI32 &slotType);
                    UI32 getLinecardMode ();
                    void setLinecardMode (const UI32 &linecardMode);
    private:


    // Now the data members


    private :
        UI32 m_slotNumber;
        UI32 m_hwType;
        UI32 m_slotType;
        UI32 m_linecardMode;

    protected :
    public :
};

class VcsNodeSlotConfigMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSlotConfigMessage ();
					VcsNodeSlotConfigMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId);
                    VcsNodeSlotConfigMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType = SYS_SLOT_LC);
					VcsNodeSlotConfigMessage (const string &objectManagerName, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType);
    virtual        ~VcsNodeSlotConfigMessage ();
					UI32 getSlotNumber ();
					void setSlotNumber (const UI32 &slotNumber);
					UI32 getRbridgeId ();
					void setRbridgeId (const UI32 &rbridgeId);
					UI32 getHwId ();
					void setHwId (const UI32 &hwId);
					UI32 getLinecardMode ();
					void setLinecardMode (const UI32 &linecardMode);
					UI32 getHwType ();
					void setHwType (const UI32 &hwType);
					ResourceId getReturnStatus ();
					void setReturnStatus (const ResourceId &returnStatus);
	private:


    // Now the data members


    private :
		UI32       m_slotNumber;
		UI32       m_rbridgeId;
		UI32       m_hwId;
		UI32       m_linecardMode;
		UI32       m_hwType;
        ResourceId m_returnStatus;

    protected :
    public :
};

}

#endif // VCSNODESLOTMANAGEMENTINTERFACEMESSAGES_H


