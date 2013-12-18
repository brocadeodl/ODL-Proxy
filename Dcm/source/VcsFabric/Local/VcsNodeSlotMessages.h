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

#ifndef VCSNODESLOTMESSAGES_H
#define VCSNODESLOTMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "sys/fabos/fabctrl.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeClearLinecardMessage : public PrismMessage
{
    public :
                    VcsNodeClearLinecardMessage ();
					VcsNodeClearLinecardMessage (const UI32 &slotNumber);
                    VcsNodeClearLinecardMessage (const UI32 &slotNumber, const UI32 &hwType);
					VcsNodeClearLinecardMessage (const PrismServiceId &serviceId,const UI32 &operationCode, const UI32 &slotNumber, const UI32 &hwType);
    virtual        ~VcsNodeClearLinecardMessage ();
	
	UI32 			getSlotNumber ();
	void 			setSlotNumber (const UI32 &slotNumber);
	UI32 			getHwType ();
	void 			setHwType (const UI32 &hwType);
	ResourceId      getReturnStatus ();
	void            setReturnStatus (const ResourceId &returnStatus);
	
	private:
	void 			setupAttributesForSerialization ();

    // Now the data members

    private :
	UI32 			m_slotNumber;
	UI32 			m_hwType;
    ResourceId      m_returnStatus;
	
    protected :
};

class VcsNodeSlotSetLinecardModeMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSlotSetLinecardModeMessage ();
					VcsNodeSlotSetLinecardModeMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId);
                    VcsNodeSlotSetLinecardModeMessage (const UI32 &slotNumber, const UI32 & rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType = SYS_SLOT_LC);
					VcsNodeSlotSetLinecardModeMessage (const PrismServiceId &serviceId, const UI32 &operationCode, const UI32 &slotNumber, const UI32 &rbridgeId, const UI32 &hwId, const UI32 &linecardMode, const UI32 &hwType);
    virtual        ~VcsNodeSlotSetLinecardModeMessage ();
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

class VcsNodeSlotStateQueryMessage : public PrismMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                    VcsNodeSlotStateQueryMessage ();
                    VcsNodeSlotStateQueryMessage (const UI32 &slotNumber);
                    VcsNodeSlotStateQueryMessage (const UI32 &slotNumber, const UI32 &hwType);
    virtual        ~VcsNodeSlotStateQueryMessage ();
                    UI32 getSlotNumber ();
                    void setSlotNumber (const UI32 &slotNumber);
					UI32 getHwType ();
					void setHwType (const UI32 &hwType);
                    UI32 getSlotState ();
                    void setSlotState (const UI32 &slotState);
                    bool getIsEnabled ();
    private:


    // Now the data members


    private :
        UI32 m_slotNumber;
        UI32 m_hwType;
        UI32 m_slotState;

    protected :
    public :
};

class VcsNodeCreateDummySlotMessage : public PrismMessage
{
    public :
                    VcsNodeCreateDummySlotMessage (const UI32 &slotNumber);
    virtual        ~VcsNodeCreateDummySlotMessage ();

    UI32            getSlotNumber ();
    void            setSlotNumber (const UI32 &slotNumber);

    private:
    void            setupAttributesForSerialization ();

    // Now the data members

    private :
    UI32            m_slotNumber;

    protected :
};

}

#endif // VCSNODESLOTMESSAGES_H


