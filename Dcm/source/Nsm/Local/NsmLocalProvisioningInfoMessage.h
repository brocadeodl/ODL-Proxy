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

/**********************************************************
 *  Copyright (C) 2011 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Dec 20, 2011
 *      Author: statpatt
 *
 *
 **********************************************************/
#ifndef  NSMLOCALPROVISIONINGINFOMESSAGE_H_
#define  NSMLOCALPROVISIONINGINFOMESSAGE_H_

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Types/UI32Range.h"

using namespace WaveNs;

namespace DcmNs {

    class NsmLocalProvisioningInfoMessage: public PrismMessage {

        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalProvisioningInfoMessage();
            virtual ~NsmLocalProvisioningInfoMessage();

            void setProvisionedMORange(const UI32Range provisionedMORange);
            UI32Range getProvisionedMORange() const;

            bool getIsGwRequest() const;
            void setIsGwRequest(bool isGwRequest);

            void copyAllBuffers (const PrismMessage &prismMessage);

        private:
        protected:
        public:
            UI32Range m_provisionedMORange;
            bool m_isGwRequest;
    };

}

#endif /* NSMLOCALPROVISIONINGINFOMESSAGE_H_ */
