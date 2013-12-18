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

#ifndef OSPFAREARANGELOCALMANAGEDOBJECT_H
#define OSPFAREARANGELOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
	class OSPFAreaRangeMessage;

    class OSPFAreaRangeLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFAreaRangeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFAreaRangeLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &rangeAddress,const IpV4Address &rangeMask,const OSPFAreaRangeEffect &rangeAdvertise,const UI32 &rangeCost);
            virtual    ~OSPFAreaRangeLocalManagedObject ();
            static string getClassName();
            void setRangeAddress(const IpV4Address &rangeAddress);
            IpV4Address getRangeAddress()  const;
            void setRangeMask(const IpV4Address &rangeMask);
            IpV4Address getRangeMask()  const;
            void setRangeAdvertise(const OSPFAreaRangeEffect &rangeAdvertise);
            OSPFAreaRangeEffect getRangeAdvertise()  const;
            void setRangeCost(const UI32 &rangeCost);
            UI32 getRangeCost()  const;
			void updateClientMessageForReplay(OSPFAreaRangeMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
            IpV4Address  m_rangeAddress;
            IpV4Address  m_rangeMask;
            OSPFAreaRangeEffect  m_rangeAdvertise;
			UI32         m_rangeCost;
    };
}
#endif                                            //OSPFAREARANGELOCALMANAGEDOBJECT_H
