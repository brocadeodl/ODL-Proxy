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

 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Brunda Rajagopala
 **************************************************************************/

#ifndef ELDLOCALCLEARMESSSAGE_H
#define ELDLOCALCLEARMESSSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class EldLocalClearMessage: public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
                        EldLocalClearMessage ();
                        EldLocalClearMessage (const UI32 &cmdcode, const UI32 &rbridgeId, const bool&isDistributionReq, const string &ifname, const UI32 &ifType);
                        EldLocalClearMessage(EldLocalClearMessage *pMsg);
                        virtual ~EldLocalClearMessage ();
                        void setCmdCode(const UI32 &cmdcode);
                        UI32 getCmdCode() const;
                        bool getisDistributionRequired() const;
                        void setisDistributionRequired(const bool f);
                        void setRbridgeId(const UI32& rbridgeId);
                        UI32 getRbridgeId()const;
                        vector<UI32> getLocationIds(void) const { return m_locs ;}
                        void setLocationIds(vector<UI32> locations) { m_locs = locations;}

                        void setIfName(const string &ifName);
                        string getIfName() const;
                        void setIfType(const UI32 &ifName);
                        UI32 getIfType() const;

        private:
        protected:
        public:
            UI32 m_cmdcode;
            bool m_is_distribution_required;
            UI32 m_rbridgeId;
            vector<UI32>  m_locs;
            string  m_ifName;
            UI32    m_ifType;

    };
}
#endif  //ELDCLEARMESSSAGE_H
