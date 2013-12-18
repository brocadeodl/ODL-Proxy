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
 *   Author : hzhu                                                     *
 **************************************************************************/

#ifndef RTMCONFIGLOCALMANAGEDOBJECT_H
#define RTMCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpV4AddressUC.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeIpV4AddressUC.h"


using namespace WaveNs;

namespace DcmNs
{

    class RtmLocalObjectManager;
    class RtmConfigLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            RtmConfigLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager);
            RtmConfigLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager,const UI8 &nhProto,const bool &nhDefault,const UI32UC &nhRecur,const UI32UC &loadSharing,const IpV4AddressUC &routerID);
            virtual    ~RtmConfigLocalManagedObject ();
            static string getClassName();
            void setNhProto(const UI8 &nhProto);
            UI8 getNhProto()  const;
            void setNhDefault(const bool &nhDefault);
            bool getNhDefault()  const;
            void setNhRecur(const UI32 &nhRecur, bool userConfig = false);
            UI32UC getNhRecur()  const;
            void setLoadSharing(const UI32 &loadSharing, bool userConfig = false);
            UI32UC getLoadSharing()  const;
            void setRouterID(const IpV4Address &routerID, bool userConfig);
			void unsetRouterID();
            IpV4AddressUC getRouterID()  const;

// Now the data members

        private:
        protected:
        public:
            UI8  m_nhProto;
            bool  m_nhDefault;
            UI32UC  m_nhRecur;
            UI32UC  m_loadSharing;
            IpV4AddressUC  m_routerID;
    };
}
#endif                                            //RTMCONFIGLOCALMANAGEDOBJECT_H
