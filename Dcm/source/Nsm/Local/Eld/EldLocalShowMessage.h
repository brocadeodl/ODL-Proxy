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
 *   Author : Amitayu Das	                                               *
 **************************************************************************/

#ifndef ELDLOCALSHOWMESSAGE_H 
#define ELDLOCALSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class EldLocalShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            EldLocalShowMessage ();
            EldLocalShowMessage (const string &ifName, const UI32 &poNum, const UI32 &cmdCode, bool isGwRequest);
            EldLocalShowMessage(EldLocalShowMessage *pMsg);
            virtual ~EldLocalShowMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setPoNum (const UI32 &poNum);
            UI32 getPoNum ()  const;            
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode () const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;            
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
            void copyAllBuffers (const PrismMessage &prismMessage);
            void transferOutputBuffer(EldLocalShowMessage *&pMsg);
            UI32  getVlanId() const;
            void  setVlanId(const UI32 &vlanId);
            void  setRbridgeId(const UI32 &rbridgeId);
            UI32  getRbridgeId() const;
            UI32  getMappedId() const;
            void  setMappedId(const UI32 &mappedId);
            bool getisDistributionRequired() const { return m_is_distribution_required;}
            void setisDistributionRequired(const bool f) { m_is_distribution_required = f;}
            vector<UI32> getLocationIds(void) const { return m_locs ;}
            void setLocationIds(vector<UI32> locations) { m_locs = locations;}


        private:
        protected:
        public:
            string  m_ifName;
            UI32    m_poNum;
            UI32    m_cmdCode;
            bool    m_isGwRequest;
            UI32    m_mappedId;
            UI32    m_rbridgeId;
            UI32    m_vlanId;
            bool    m_is_distribution_required;
            vector<UI32>  m_locs;

    };
}
#endif  //ELDLOCALSHOWMESSAGE_H
