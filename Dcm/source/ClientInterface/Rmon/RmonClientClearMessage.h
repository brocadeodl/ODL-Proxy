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
 *   Author : Venkat                                                       *
 **************************************************************************/

#ifndef RMONCLIENTCLEARMESSAGE_H
#define RMONCLIENTCLEARMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClientClearMessage : public DcmManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClientClearMessage ();
            RmonClientClearMessage (const SI32 &statsId, const SI32 &cmdCode);
            virtual    ~RmonClientClearMessage ();
            void setStatsId(const SI32 &statsId);
            SI32 getStatsId()  const;
            void setCmdCode(const SI32 &cmdCode);
            SI32 getCmdCode()  const;

			virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            SI32  m_statsId;
            SI32  m_cmdCode;
    };
}
#endif                                            //RMONCLIENTCLEARMESSAGE_H
