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
 *   Author : dchakrab                                                     *
 **************************************************************************/

#ifndef RMONCLEARMESSAGE_H
#define RMONCLEARMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClearMessage : public PrismMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClearMessage ();
            RmonClearMessage (const SI32 &statsId, const SI32 &cmdCode, const bool isGwRequest);
            virtual    ~RmonClearMessage ();
            void setStatsId(const SI32 &statsId);
            SI32 getStatsId()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;
            void setIsGwRequest (bool isGwRequest);
            bool getIsGwRequest ()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_statsIndex;
			SI32  m_cmdCode;
			bool m_isGwRequest;
    };
}
#endif                                            //RMONCLEARMESSAGE_H
