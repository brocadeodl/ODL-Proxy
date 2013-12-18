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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skbalasu                                                     *
 **************************************************************************/

#ifndef NSMGLOBALCREATEPOINTFMESSAGE_H
#define NSMGLOBALCREATEPOINTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    class PoIntfManagedObject;
    class NsmGlobalCreatePoIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalCreatePoIntfMessage ();
            NsmGlobalCreatePoIntfMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc,const UI32 &poMode,const UI32 &poType, const UI32 &poMinLinks, const LineSpeed &poAllowedSpeed, const UI32 &poLoadBalanceMode, const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId);
            virtual    ~NsmGlobalCreatePoIntfMessage ();
            void setPoId(const string &poId);
            string getPoId()  const;
			void setLocationId(const LocationId &locationId);
			LocationId getLocationId() const;
			void setSendToLocal(const bool &send);
			bool getSendToLocal() const; 
            void setPoName(const string &poName);
            string getPoName()  const;
            void setPoIfIndex(const UI32 &poIfIndex);
            UI32 getPoIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setPoMode(const UI32 &poMode);
            UI32 getPoMode()  const;
            void setPoType(const UI32 &poType);
            UI32 getPoType()  const;
            void setPoMinLinks(const UI32 &poMinLinks);
            UI32 getPoMinLinks()  const;
			void setPoAllowedSpeed(const LineSpeed &poAllowedSpeed);
			LineSpeed getPoAllowedSpeed() const;
			void setPoLoadBalanceMode(const UI32 &poLoadBalanceMode);
            UI32 getPoLoadBalanceMode()  const;
            void setPoLacpSystemId(const string &poLacpSystemId);
            string getPoLacpSystemId()  const;
            void setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId);
            UI32 getPoLacpSystemIdDomainId()  const;
            PoIntfManagedObject *getPoRef();
            void setPoRef(PoIntfManagedObject *refPoIntfManagedObject);

// Now the data members

        private:
        protected:
        public:
            string  m_poId;
			LocationId m_locationId;
			bool m_send;
            string  m_poName;
            UI32  m_poIfIndex;
            string  m_desc;
            UI32  m_poMode;
            UI32  m_poType;
            UI32  m_poMinLinks;
			LineSpeed m_poAllowedSpeed;
            UI32  m_poLoadBalanceMode;
            string  m_poLacpSystemId;
            UI32  m_poLacpSystemIdDomainId;
            PoIntfManagedObject *m_refPoIntfManagedObject;
    };
}
#endif                                            //NSMGLOBALCREATEPOINTFMESSAGE_H
