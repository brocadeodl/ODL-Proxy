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
 *   Author : chenj                                                        *
 **************************************************************************/

#ifndef IPADMLOCALDNSMESSAGE_H
#define IPADMLOCALDNSMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "IpadmTypes.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Attributes/AttributeIpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

//////////////////////////////////////
//  IpadmLocalDnsCfgMessage definition    //
//////////////////////////////////////

class IpadmLocalDnsCfgMessage : public PrismMessage
{
    private:
        void setupAttributesForSerialization();
        protected:
        public:
            IpadmLocalDnsCfgMessage ();
            IpadmLocalDnsCfgMessage (const ObjectId &LoggingOId);
            IpadmLocalDnsCfgMessage (const ObjectId &LoggingOId, UI32 &userWyserTag);
            IpadmLocalDnsCfgMessage (const ObjectId &LoggingOId, string &domain_name, UI32 &userWyserTag);
	    ~IpadmLocalDnsCfgMessage();
	    void setData(const string &Data);
	    string getData() const;
	    void setWyserUserTag(const UI32 &userWyserTag);
	    UI32 getWyserUserTag() const;
	    void setNameSrvIp(const IpVxAddress &nameSrvIp);
	    IpVxAddress getNameSrvIp()  const;
	    string getDomainName() const;
	    void setDomainName (const string &domain_name);
            void setMappedId(const UI32 &mappedId);
            UI32 getMappedId() const;
            string getOpr() const;
            void setOpr (const string &opr);

// Now the data members

        private:
            ObjectId m_ObjectId;
            UI32    m_mappedId;
            UI32    m_userWyserTag;
            string  m_data;
            string  m_domain_name;
            IpVxAddress m_nameSrvIp;
	    string  m_opr;

        protected:
        public:
};
}

#endif
