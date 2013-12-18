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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#ifndef LLDP_FCOE_CLIENT_FSBMESSAGE_H
#define LLDP_FCOE_CLIENT_FSBMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{   

    class LldpFcoeClientFsbMessage : public DcmManagementInterfaceMessage
    {   
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LldpFcoeClientFsbMessage ();
            LldpFcoeClientFsbMessage (const bool &fsb_enable);
            virtual    ~LldpFcoeClientFsbMessage ();
            bool getFsbMode()  const;
            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure) {
                return ;
            }

// Now the data members

        private:
        protected:
        public:
            bool  m_fsb_enable;
    };
}
#endif                                            //FCOE_CLIENT_FSBMESSAGE_H
