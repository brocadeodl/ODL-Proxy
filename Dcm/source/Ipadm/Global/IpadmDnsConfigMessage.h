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
 *   Author : aviswana                                                     *
 **************************************************************************/

#ifndef IPADMDNSCONFIGMESSAGE_H
#define IPADMDNSCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpadmDnsConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpadmDnsConfigMessage ();
            IpadmDnsConfigMessage (const string &opr, const  UI32 &userWyserTag);
            virtual	~IpadmDnsConfigMessage ();
            void	setDomainName(const string &domain_name);
            string	getDomainName()  const;
            void setNameSrvIp(IpVxAddress &nameSrvIp);
	    IpVxAddress getNameSrvIp()  const;
            void	setOpr(const string &opr);
            string	getOpr()  const;
	    UI32	getWyserUserTag() const;
	    void	setWyserUserTag(const  UI32 &userWyserTag);
            void setData(const string &data);
            string getData()  const;
            void setIndex(const UI32 &index);
            UI32 getIndex()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_domain_name;
	    IpVxAddress m_nameSrvIp;
            string  m_opr;
	    UI32    m_userWyserTag;
	    string  m_data;
	    string  m_support;
	    UI32    m_index;
    };
}
#endif                                            //IPADMDNSCONFIGMESSAGE_H
