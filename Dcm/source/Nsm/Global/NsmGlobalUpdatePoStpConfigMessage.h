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
 *   Author : nsong                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEPOSTPCONFIGMESSAGE_H
#define NSMGLOBALUPDATEPOSTPCONFIGMESSAGE_H

#include "Nsm/Common/NsmPortStpConfigMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdatePoStpConfigMessage : public NsmPortStpConfigMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdatePoStpConfigMessage ();
            NsmGlobalUpdatePoStpConfigMessage (const string &ifName,const UI16 &priority,const UI32 &helloTime,const UI32 &cost,const UI32 &cmdCode);
            virtual    ~NsmGlobalUpdatePoStpConfigMessage ();
            void setIfName(const string &ifName);
            void setHelloTime(const UI32 &helloTime);
            UI32 getHelloTime()  const;
            void setBpduGuardEnable(const bool &bpduGuardEnable);
            bool getBpduGuardEnable()  const;
            UI8	 getBpduDropDirection() const;
            void setBpduDropDirection(const UI8 direction);

	    void  printMsg();

// Now the data members

        private:
        protected:
        public:
            UI32  m_bpduGuardEnable;
            UI32  m_helloTime;
            UI8	  m_bpduDropDirection;
    };
}
#endif                                            //NSMGLOBALUPDATEPOSTPCONFIGMESSAGE_H
