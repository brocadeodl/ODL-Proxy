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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFPERMITDENYREDISTRIBUTELOCALMANAGEDOBJECT_H
#define OSPFPERMITDENYREDISTRIBUTELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
    class OSPFPermitDenyRedistributeLocalMessage;
	class OSPFPermitDenyRedistributeMessage;

    class OSPFPermitDenyRedistributeLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFPermitDenyRedistributeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFPermitDenyRedistributeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const UI32 &index,const OSPFPermitDenyOption &permitDenyOption,const OSPFRouteOption &routeOption,const IpV4Address &subNet,const IpV4Address &mask, const UI32 &matchMetric, const UI32 &setMetric, const bool &crDummy);
            virtual    ~OSPFPermitDenyRedistributeLocalManagedObject ();
            static string getClassName();
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;
            void setPermitDenyOption(const OSPFPermitDenyOption &permitDenyOption);
            OSPFPermitDenyOption getPermitDenyOption()  const;
            void setRouteOption(const OSPFRouteOption &routeOption);
            OSPFRouteOption getRouteOption() const;
            void setSubNet(const IpV4Address &subNet, const bool &set);
            IpV4Address getSubNet()  const;
            void setMask(const IpV4Address &mask, const bool &set);
            IpV4Address getMask()  const;
            void setMatchMetric(const UI32 &matchMetric);
            UI32 getMatchMetric()  const;
			void setSetMetric(const UI32 &setMetric);
			UI32 getSetMetric() const;
            void setCrDummy(const bool &crDummy);
            bool getCrDummy() const;
			void updateFromMessage(OSPFPermitDenyRedistributeLocalMessage *pMsg);
			void updateClientMessageForReplay(OSPFPermitDenyRedistributeMessage *pMsg);


// Now the data members

        private:
        protected:
        public:
            UI32  m_index;
            OSPFPermitDenyOption m_permitDenyOption;
            OSPFRouteOption m_routeOption;
            IpV4AddressUC  m_subNet;
            IpV4AddressUC  m_mask;
            UI32  m_matchMetric;
			UI32  m_setMetric;
            bool m_crDummy; /* Dummy leaf for carriage return */
    };
}
#endif                                            //OSPFPERMITDENYREDISTRIBUTELOCALMANAGEDOBJECT_H
