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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mayank Maheshwari                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

using namespace WaveNs;

namespace DcmNs
{
    class NsmLocalSlotOfflineMessage : public ManagementInterfaceMessage {
        private:
        protected:
            virtual void setupAttributesForSerialization() {
                ManagementInterfaceMessage::setupAttributesForSerialization ();
                addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
                addSerializableAttribute (new AttributeUI32(&m_slotId,"slotId"));
            }
        public:
            NsmLocalSlotOfflineMessage () : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (), NSM_SLOT_OFFLINE_MSG) {
                m_cmdCode = 0;
                m_slotId = 0;
            }

            virtual ~NsmLocalSlotOfflineMessage () {
            }

            void setCmdCode(const UI32 &cmdCode) {
                m_cmdCode = cmdCode;
            }

            UI32 getCmdCode()  const {
                return m_cmdCode;
            }

            void setSlotId(const UI32 &slotId) {
                m_slotId = slotId;
            }

            UI32 getSlotId() const {
                return m_slotId;
            }

        private:
            UI32 m_cmdCode;
            UI32 m_slotId;
    };
}

